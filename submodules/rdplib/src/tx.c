// Copyright (c) 2026 solar
// SPDX-License-Identifier: MIT

#if defined(_MSC_VER) && (defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL))
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "tx.h"

#include <stddef.h>
#ifdef RDPLIB_DEBUG
#include <assert.h>
#endif
#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
#include <stdio.h>
#endif
#ifdef RDPLIB_SOURCE_FAITHFUL
#include <stdlib.h>
#endif
#include <string.h>

#if defined(__linux__) && !defined(RDPLIB_SOURCE_FAITHFUL)
#include <errno.h>
#endif

#include "bandwidth.h"
#include "bitops.h"
#ifdef RDPLIB_DEBUG
#include "dpf.h"
#endif
#include "fast.h"
#ifdef RDPLIB_SOURCE_FAITHFUL
#include "log.h"
#endif
#include "msg_outgoing.h"
#if defined(RDPLIB_DEBUG) || !defined(RDPLIB_SOURCE_FAITHFUL)
#include "protocol_limits.h"
#endif
#include "rdplib_platform.h"
#ifndef RDPLIB_SOURCE_FAITHFUL
#include "rdplib_random.h"
#endif
#include "rdp.h"
#include "rdpstat.h"
#include "rx.h"
#include "serial.h"
#include "timeout.h"
#include "trace.h"
#include "txq.h"
#include "uevent.h"
#include "usend.h"
#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
#include "ustrerror.h"
#endif
#ifndef RDPLIB_SOURCE_FAITHFUL
#include "rdplib_wire.h"
#endif
#include "utime.h"

#ifdef RDPLIB_DEBUG
// Names retained so the recovered assertions produce the May object strings.
typedef int16_t int16;
typedef int32_t int32;
#define has_msgid(msg_outgoing) ((msg_outgoing)->options & RDP_FLAG_MSGID)
#define RDP_HEADER_OPTION_MASKOFFSET RDP_FLAG_MASKOFFSET
#define URESULT_OK 0u
#define UERROR_TRY_AGAIN 5u
#define UERROR_SYSTEM 1u
#endif

void tx_init(connection_t *c, rdp_t *rdp, struct sockaddr *remote_addr)
{
#ifndef RDPLIB_SOURCE_FAITHFUL
    uint16_t initial_message_id;
#endif

#ifdef RDPLIB_SOURCE_FAITHFUL
    srand(time_get_ms());
#else
    initial_message_id = (uint16_t)rdplib_random_next();
#endif

    memcpy(&c->tx_remote_addr, remote_addr, sizeof(c->tx_remote_addr));
    memset(&c->trace_remote_addr, 0, sizeof(c->trace_remote_addr));

    switch (remote_addr->sa_family)
    {
    case RDP_TRANSMIT_ADDRESS_IPV4:
        c->tx_socket = rdp->udp_socket;
        memcpy(&c->trace_remote_addr, remote_addr, sizeof(c->trace_remote_addr));
        c->trace_remote_addr.sin_port = htons((uint16_t)(ntohs(c->trace_remote_addr.sin_port) | 0x8000));
        break;

    case RDP_TRANSMIT_ADDRESS_IPX:
        c->tx_socket = rdp->ipx_socket;
        break;

    case RDP_TRANSMIT_ADDRESS_SERIAL:
        c->tx_socket = -1;
        break;

    default:
#ifdef RDPLIB_DEBUG
        assert(!"wtf?");
#endif
        break;
    }

    c->trace_socket = rdp->trace_socket;
#ifdef RDPLIB_SOURCE_FAITHFUL
    c->tx_next_msgid = (uint16_t)rand();
#else
    c->tx_next_msgid = initial_message_id;
#endif
    c->tx_acked_thru = (uint16_t)(c->tx_next_msgid - 1u);
    c->tx_next_seqnum = 0;
    c->tx_next_fragid = 0;
    c->tx_time_last_guaranteed_send = time_get_ms();
    bitarray_clear(&c->tx_outstanding_packet_mask);
    txq_init(&c->tx_outstanding_packets);
    txq_init(&c->tx_virgin_packets);
    txq_init(&c->tx_delayed_packets);
    bandwidth_init(&c->tx_bandwidth);
    c->tx_send_buffer_size = 8000;
    memset(c->tx_guaranteed_stream_seqnum, 0, sizeof(c->tx_guaranteed_stream_seqnum));
    c->tx_time_since_bandwidth_change = time_get_ms();
    c->tx_modem = 0;
    timeout_init(&c->tx_rt_tracker, 500, 1);
    c->tx_syn_sent = 0;
    c->tx_syn_acked = 0;
    c->tx_syn_msgid = c->tx_next_msgid;
    c->tx_fin_sent = 0;
    c->tx_fin_acked = 0;
    c->tx_fin_msgid = 0;
    c->tx_connected = 1;
    c->tx_stopped = 0;
    c->tx_disconnect_reason = 0;
    c->tx_enqueued_disconnect_msg = 0;
    c->tx_max_message_age = 10000;
    c->tx_max_service_outage = 10000;
    c->tx_delayed_ack = 0;
    c->tx_ack_time = 0;
    c->tx_last_rt_time = 0;
    c->tx_all_acked_event = NULL;
    c->tx_all_acked = NULL;
    c->trace_udp_ttl = rdp->udp_socket_ttl;
}

uint32_t tx_create(connection_t *c)
{
    uint32_t result;

    result = 0;
    txq_create(&c->tx_outstanding_packets);
    txq_create(&c->tx_virgin_packets);
    txq_create(&c->tx_delayed_packets);
    return result;
}

void tx_destroy(connection_t *c)
{
    tx_flush_output_buffers(c);

    if (c->tx_all_acked)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x4000u, "all_acked == FALSE (connection deleted)\n");
#endif
        *c->tx_all_acked = 0;
        c->tx_all_acked = NULL;
    }

    if (c->tx_all_acked_event)
    {
        uevent_signal(c->tx_all_acked_event);
        c->tx_all_acked_event = NULL;
    }
}

void tx_flush_output_buffers(connection_t *c)
{
    msg_outgoing_t *outgoing;

    while ((outgoing = txq_remove_head(&c->tx_delayed_packets)) != NULL)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x2000u, "[0x%08x] delayed message deleted\n", outgoing);
#endif
        fast_free(outgoing);
    }
    while ((outgoing = txq_remove_head(&c->tx_virgin_packets)) != NULL)
    {
        if ((outgoing->options & RDP_FLAG_MSGID) != 0)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x2000u, "[0x%08x] guaranteed virgin message deleted\n", outgoing);
#endif
        }
        else
        {
#ifdef RDPLIB_DEBUG
            dpf(0x2000u, "[0x%08x] virgin message deleted\n", outgoing);
#endif
        }
        fast_free(outgoing);
    }
    while ((outgoing = txq_remove_head(&c->tx_outstanding_packets)) != NULL)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x2000u, "[0x%08x] outstanding message deleted\n", outgoing);
#endif
        fast_free(outgoing);
    }
}

void tx_handle_ack(connection_t *c, uint16_t msgid)
{
    msg_outgoing_t *msg_outgoing;
    uint32_t rt_time;

    msg_outgoing = txq_remove_msgid(&c->tx_outstanding_packets, msgid);
    ++g_rdp_stat->messages_acked;
    ++c->stat.messages_acked;
#ifdef RDPLIB_DEBUG
    assert(( msg_outgoing != NULL ) || !c->tx_connected || c->tx_stopped);
#endif

    if (msg_outgoing)
    {
        rt_time = time_get_ms() - msg_outgoing->time_first_sent;

        ++g_rdp_stat->packets_updated_rtt_attempts;
        ++c->stat.packets_updated_rtt_attempts;

        if (msg_outgoing->attempts == 1)
        {
            ++g_rdp_stat->packets_updated_rtt;
            ++c->stat.packets_updated_rtt;
            timeout_add_sample(&c->tx_rt_tracker, rt_time, msg_outgoing->attempts);
            c->tx_last_rt_time = (uint16_t)rt_time;
        }

        fast_free(msg_outgoing);
    }

    // The first newly acknowledged reliable message establishes the SYN transition; debug also verifies its ID.
    if (c->tx_syn_sent && !c->tx_syn_acked)
    {
#ifdef RDPLIB_DEBUG
        assert(msgid == c->tx_syn_msgid);
        dpf(0x4000u, "[0x%08x] SYN acked\n", c);
#endif
        c->tx_syn_acked = 1;
        ++g_rdp_stat->connections_established;
        c->tx_max_message_age = 120000;
        c->tx_max_service_outage = 30000;
    }

    if (c->tx_fin_sent && !c->tx_fin_acked && msgid == c->tx_fin_msgid)
    {
        c->tx_fin_acked = 1;
#ifdef RDPLIB_DEBUG
        dpf(0x4000u, "[0x%08x] FIN acked\n", c);
#endif
    }

    // tx_record_ack_arrival calls this before publishing an advancing ACKTHRU
    // base. Preserve that order: FIN recognition and waiter completion can be
    // separated across calls.
    if (c->tx_fin_sent && c->tx_acked_thru == c->tx_fin_msgid)
    {
        if (c->tx_all_acked)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x4000u, "all_acked == TRUE (all messages acknowledged) (c->tx_tx_handle_ack)\n");
#endif
            *c->tx_all_acked = 1;
            c->tx_all_acked = NULL;
        }

        if (c->tx_all_acked_event)
        {
            uevent_signal(c->tx_all_acked_event);
            c->tx_all_acked_event = NULL;
        }
    }
}

#ifndef RDPLIB_SOURCE_FAITHFUL
static int rdplib_tx_sent_messages_contains(const connection_t *c, uint16_t message_id)
{
    const rdp_link_t *link = c->tx_outstanding_packets.list.head;

    while (link)
    {
        const msg_outgoing_t *message = (const msg_outgoing_t *)link->item;

        if (message->msgid == message_id)
        {
            return 1;
        }
        link = link->next;
    }
    return 0;
}

static int rdplib_tx_acknowledged_id_is_retired_or_sent(connection_t *c, uint16_t message_id)
{
    int32_t offset = (int16_t)(message_id - c->tx_acked_thru);

    if (offset <= 0)
    {
        return 1;
    }
    if ((uint32_t)offset > RDP_BITARRAY_BITS)
    {
        return 0;
    }

    // A clear history bit was already retired.  A newly claimed bit must name a record in the sent queue.
    if (!getbit(c->tx_outstanding_packet_mask.bits, (uint32_t)offset - 1u))
    {
        return 1;
    }
    return rdplib_tx_sent_messages_contains(c, message_id);
}

static int rdplib_tx_ack_claims_are_retired_or_sent(connection_t *c, rdp_header_t *header)
{
    uint8_t *mask;
    uint16_t base_message_id;
    uint32_t mask_bits;
    uint32_t index;

    if ((header->options & (RDP_FLAG_ACKTHRU | RDP_FLAG_MASKOFFSET)) == 0)
    {
        return 1;
    }

    base_message_id = rdplib_load_network_u16((const uint8_t *)header->ack);
    mask = (uint8_t *)(header->ack + 1);
    mask_bits = ((header->options & RDP_FLAG_ACK_MASK_LENGTH) >> 4) * 8u;

    if ((header->options & RDP_FLAG_ACKTHRU) != 0)
    {
        int32_t advance = (int16_t)(base_message_id - c->tx_acked_thru);

        for (index = 0; advance > 0 && index < (uint32_t)advance; ++index)
        {
            if (!rdplib_tx_acknowledged_id_is_retired_or_sent(c, (uint16_t)(c->tx_acked_thru + index + 1u)))
            {
                return 0;
            }
        }
    }
    else if (!rdplib_tx_acknowledged_id_is_retired_or_sent(c, base_message_id))
    {
        return 0;
    }

    for (index = 0; index < mask_bits; ++index)
    {
        if (getbit(mask, index) && !rdplib_tx_acknowledged_id_is_retired_or_sent(c, (uint16_t)(base_message_id + index + 1u)))
        {
            return 0;
        }
    }
    return 1;
}
#endif

uint32_t tx_validate_ack_arrival(connection_t *c, rdp_header_t *header, uint32_t *ack_size)
{
    uint16_t *ack;
    uint32_t validation;
    uint16_t highbit;
    uint16_t msgid_hi;
    uint16_t options;

    validation = RDP_RX_ACCEPT;
    ack = header->ack;
    options = header->options;
    *ack_size = 0;
    if ((options & RDP_FLAG_ACKTHRU) && (options & RDP_FLAG_MASKOFFSET))
    {
#ifdef RDPLIB_SOURCE_FAITHFUL
        char addr[64];

        validation = RDP_RX_ABORT;
        format_sockaddr(addr, &c->tx_remote_addr);
        // This historical format contains seven trailing %s conversions but supplies only 6 flag strings.
        discard_log_append("%s both ack bits set: %04x ( %s%s%s%s%s%s%s)\n", addr, options, options & RDP_FLAG_SYN ? "SYN " : "", options & RDP_FLAG_FIN ? "FIN " : "",
                           options & RDP_FLAG_MSGID ? "MSGID " : "", options & RDP_FLAG_FRAGMENT ? "FRAG " : "", options & RDP_FLAG_SYSTEM ? "SYS " : "",
                           options & RDP_FLAG_MULTI ? "MULTI " : "");
#else
        validation = RDP_RX_ABORT;
#endif
        ++g_rdp_stat->discarded_bad_ack_header;
        ++c->stat.discarded_bad_ack_header;
    }
    else if (options & (RDP_FLAG_ACKTHRU | RDP_FLAG_MASKOFFSET))
    {
        uint16_t ack_mask_size;
        uint16_t bit;

#ifdef RDPLIB_SOURCE_FAITHFUL
        msgid_hi = ntohs(*ack++);
#else
        msgid_hi = rdplib_load_network_u16((const uint8_t *)ack);
        ++ack;
#endif
        *ack_size = 2;
        ack_mask_size = (uint16_t)((options & RDP_FLAG_ACK_MASK_LENGTH) >> 4);
        if (ack_mask_size)
        {
            *ack_size += ack_mask_size;
            ack_mask_size = (uint16_t)(ack_mask_size * 8u);
            highbit = ack_mask_size;
            for (bit = (uint16_t)(ack_mask_size - 8u); bit < ack_mask_size; ++bit)
            {
                if (getbit((uint8_t *)ack, bit))
                {
                    highbit = bit;
                }
            }

            if (highbit == ack_mask_size)
            {
#ifdef RDPLIB_SOURCE_FAITHFUL
                char addr[64];

                validation = RDP_RX_ABORT;
                format_sockaddr(addr, &c->tx_remote_addr);
                discard_log_append("%s last byte in ack mask is empty: %x\n", addr, *((uint8_t *)ack + ack_mask_size / 8u - 1u));
#else
                validation = RDP_RX_ABORT;
#endif
                ++g_rdp_stat->discarded_bad_ackmask;
                ++c->stat.discarded_bad_ackmask;
                return validation;
            }
            msgid_hi = (uint16_t)(msgid_hi + highbit + 1u);
        }

        if ((uint32_t)(int16_t)(msgid_hi - c->tx_next_msgid) < (uint32_t)(int16_t)-4096)
        {
#ifdef RDPLIB_SOURCE_FAITHFUL
            char addr[64];

            format_sockaddr(addr, &c->tx_remote_addr);
            discard_log_append("%s ack for ancient/unsent: %i=%u-%u\n", addr, (int16_t)(msgid_hi - c->tx_next_msgid), msgid_hi, c->tx_next_msgid);
#endif
            validation = RDP_RX_DISCARD;
            if ((int16_t)(msgid_hi - c->tx_next_msgid) < 0)
            {
                ++g_rdp_stat->discarded_old_ack;
                ++c->stat.discarded_old_ack;
            }
            else
            {
                ++g_rdp_stat->acks_for_unsent_messages;
                ++c->stat.acks_for_unsent_messages;
            }
        }
    }
    else if (options & RDP_FLAG_ACK_MASK_LENGTH)
    {
        validation = RDP_RX_ABORT;
        ++g_rdp_stat->discarded_mask_wo_ack;
        ++c->stat.discarded_mask_wo_ack;
    }

#ifndef RDPLIB_SOURCE_FAITHFUL
    if (validation == RDP_RX_ACCEPT && !rdplib_tx_ack_claims_are_retired_or_sent(c, header))
    {
        ++g_rdp_stat->acks_for_unsent_messages;
        ++c->stat.acks_for_unsent_messages;
        validation = RDP_RX_DISCARD;
    }
#endif

    return validation;
}

void tx_record_ack_arrival(connection_t *c, rdp_header_t *header)
{
    uint16_t ack_mask_size;
    uint16_t bit;
    uint16_t *ack;
#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
    char ackmask_str[249];
    char *index;
#endif
    uint16_t maskoffset;
    uint16_t shift_size;
    uint16_t ackoffset;
    uint16_t new_msgids;
    uint16_t ack_size;
    uint16_t options;

    ack = header->ack;
    options = header->options;
    new_msgids = 0;
    ack_size = 2;
    maskoffset = 0;
#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
    index = ackmask_str;
#endif

    if (options & RDP_FLAG_ACKTHRU)
    {
        uint16_t ackthru;

#ifdef RDPLIB_SOURCE_FAITHFUL
        ackthru = ntohs(*ack++);
#else
        ackthru = rdplib_load_network_u16((const uint8_t *)ack);
        ++ack;
#endif
#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
        index += sprintf(index, "ackthru [%u", ackthru);
#endif
        shift_size = 0;
        while ((int16_t)(ackthru - c->tx_acked_thru - shift_size) > 0)
        {
            if (bitarray_getbit(&c->tx_outstanding_packet_mask, shift_size))
            {
                tx_handle_ack(c, (uint16_t)(c->tx_acked_thru + shift_size + 1u));
                ++new_msgids;
            }
            ++shift_size;
        }

        if (shift_size)
        {
#ifdef RDPLIB_DEBUG
            uint16_t bits_to_move;

            bits_to_move = (uint16_t)(c->tx_next_msgid - c->tx_acked_thru);
            assert(( (int16)bits_to_move ) >= 0);
#endif
            bitarray_left_shift(&c->tx_outstanding_packet_mask, shift_size);
            c->tx_acked_thru = ackthru;

            while (txq_peek_head(&c->tx_delayed_packets) && tx_outgoing_msg_in_outstanding_range(c, txq_peek_head(&c->tx_delayed_packets)))
            {
                tx_enqueue_outgoing(c, txq_remove_head(&c->tx_delayed_packets));
            }
        }

        maskoffset = (uint16_t)(ackthru - c->tx_acked_thru);
#ifdef RDPLIB_DEBUG
        assert(0 == ( options & RDP_HEADER_OPTION_MASKOFFSET ));
#endif
    }
    else if (options & RDP_FLAG_MASKOFFSET)
    {
        uint16_t header_option_maskoffset;

#ifdef RDPLIB_SOURCE_FAITHFUL
        header_option_maskoffset = ntohs(*ack++);
#else
        header_option_maskoffset = rdplib_load_network_u16((const uint8_t *)ack);
        ++ack;
#endif
        maskoffset = (uint16_t)(header_option_maskoffset - c->tx_acked_thru);
        if ((int16_t)maskoffset > 0 && bitarray_clearbit(&c->tx_outstanding_packet_mask, (uint16_t)(maskoffset - 1u)))
        {
            tx_handle_ack(c, header_option_maskoffset);
            ++new_msgids;
        }
#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
        index += sprintf(index, "maskoffset [%u", header_option_maskoffset);
#endif
    }

    ack_size = (uint16_t)(ack_size + ((options & RDP_FLAG_ACK_MASK_LENGTH) >> 4));
    ack_mask_size = (uint16_t)(8u * ((options & RDP_FLAG_ACK_MASK_LENGTH) >> 4));

    if (header->options & (RDP_FLAG_ACKTHRU | RDP_FLAG_MASKOFFSET))
    {
        if (header->data_size)
        {
            ++g_rdp_stat->ack_and_data_packets_rx;
            ++c->stat.ack_and_data_packets_rx;
        }
        else
        {
            ++g_rdp_stat->ack_only_packets_rx;
            ++c->stat.ack_only_packets_rx;
        }
    }

#if defined(RDPLIB_SOURCE_FAITHFUL) && !defined(_WIN32)
    if (ack_mask_size)
    {
        *index++ = ':';
        for (bit = 0; bit < ack_mask_size; ++bit)
        {
            *index++ = (char)(getbit((uint8_t *)ack, bit) + '0');
        }
    }
    *index++ = ']';
#if defined(__powerpc__) || defined(__ppc__) || defined(_M_PPC)
    *index++ = '\r';
#else
    // PowerPC Mac uses CR; Intel Mac uses LF.
    *index++ = '\n';
#endif
    *index = '\0';
#endif

    ackoffset = 0;
    if (maskoffset & 0x8000)
    {
        ackoffset = (uint16_t)-(int16_t)maskoffset;
        maskoffset = 0;
        ack_mask_size = (uint16_t)(ack_mask_size - ackoffset);
    }

    // A wrapped negative mask length performs no iterations.
    for (bit = 0; (int32_t)bit < (int16_t)ack_mask_size; ++bit)
    {
        if (getbit((uint8_t *)ack, (uint16_t)(bit + ackoffset)) && bitarray_clearbit(&c->tx_outstanding_packet_mask, (uint16_t)(bit + maskoffset)))
        {
            tx_handle_ack(c, (uint16_t)(bit + c->tx_acked_thru + maskoffset + 1u));
            ++new_msgids;
        }
    }

    if (!new_msgids)
    {
        ++g_rdp_stat->duplicate_acks;
        g_rdp_stat->bytes_in_duplicate_acks += ack_size;
        ++c->stat.duplicate_acks;
        c->stat.bytes_in_duplicate_acks += ack_size;
    }
}

uint32_t tx_send_ready(connection_t *c)
{
    int ready;

    ready = bandwidth_get_queue_size(&c->tx_bandwidth) < (c->tx_bandwidth.bandwidth >> 3);
    if (c->tx_remote_addr.sa_family == RDP_TRANSMIT_ADDRESS_SERIAL)
    {
        ready = ready && rdp_serial_tx_ready(c->cn_rdp);
    }
    return ready;
}

void tx_send_virgin(connection_t *c, msg_outgoing_t *msg_virgin)
{
    uint32_t timesent;

    timesent = time_get_ms();
    msg_outgoing_set_time_first_sent(msg_virgin, timesent);
    tx_send_packet(c, msg_outgoing_get_data(msg_virgin), msg_virgin->size, msg_virgin->options);
#ifdef RDPLIB_DEBUG
    {
        uint32_t queueing_delay;

        queueing_delay = timesent - msg_virgin->enqueue_time;
        if (c->stat.tqd_samples)
        {
            if (queueing_delay < c->stat.tqd_min)
            {
                c->stat.tqd_min = queueing_delay;
            }
            if (queueing_delay > c->stat.tqd_max)
            {
                c->stat.tqd_max = queueing_delay;
            }
            c->stat.tqd_sum += queueing_delay;
            c->stat.tqd_bytes += msg_virgin->size;
        }
        else
        {
            c->stat.tqd_sum = queueing_delay;
            c->stat.tqd_max = queueing_delay;
            c->stat.tqd_min = queueing_delay;
        }
        ++c->stat.tqd_samples;

        if (timesent - c->stat.tqd_last_interval > 10000u)
        {
            char temp[128];

            c->stat.tqd_last_interval = timesent;
            sprintf(temp, "[0x%08x] %u tx queueing delay min:%u max:%u avg:%u (%u bytes)\n", (uint32_t)(uintptr_t)c, c->stat.tqd_samples, c->stat.tqd_min, c->stat.tqd_max,
                    c->stat.tqd_sum / c->stat.tqd_samples, c->stat.tqd_bytes);
            dpf(0x80u, "%s", temp);
            c->stat.tqd_bytes = 0;
            c->stat.tqd_sum = 0;
            c->stat.tqd_max = 0;
            c->stat.tqd_min = 0;
            c->stat.tqd_samples = 0;
        }
    }
#endif

    if (msg_virgin->options & RDP_FLAG_MSGID)
    {
        ++g_rdp_stat->guaranteed_packets_tx;
        g_rdp_stat->guaranteed_bytes_tx += msg_virgin->size;
        ++c->stat.guaranteed_packets_tx;
        c->stat.guaranteed_bytes_tx += msg_virgin->size;
        txq_add_tail(&c->tx_outstanding_packets, msg_virgin);
    }
    else
    {
        ++g_rdp_stat->best_effort_packets_tx;
        g_rdp_stat->best_effort_bytes_tx += msg_virgin->size;
        ++c->stat.best_effort_packets_tx;
        c->stat.best_effort_bytes_tx += msg_virgin->size;
        fast_free(msg_virgin);
    }
}

uint16_t tx_reserve_msgid(connection_t *c)
{
    uint16_t prev_bit;
    uint16_t msgid;
    uint16_t bit_index_of_msgid;

    bit_index_of_msgid = (uint16_t)(c->tx_next_msgid - c->tx_acked_thru - 1u);
#ifdef RDPLIB_DEBUG
    assert(bit_index_of_msgid < RDP_MAX_OUTSTANDING_IDS);
#endif
    prev_bit = bitarray_setbit(&c->tx_outstanding_packet_mask, bit_index_of_msgid);
#ifdef RDPLIB_DEBUG
    assert(prev_bit == 0);
#endif
    (void)prev_bit;
    c->tx_time_last_guaranteed_send = time_get_ms();
    msgid = c->tx_next_msgid++;
    return msgid;
}

uint32_t tx_outgoing_msg_in_outstanding_range(connection_t *c, msg_outgoing_t *msg_outgoing)
{
#ifdef RDPLIB_DEBUG
    assert(has_msgid(msg_outgoing));
#endif
    return (int16_t)(msg_outgoing->msgid - c->tx_acked_thru - 120u) < 0;
}

void tx_enqueue_outgoing(connection_t *c, msg_outgoing_t *msg_outgoing)
{
    if (!(msg_outgoing->options & RDP_FLAG_MSGID) || c->tx_syn_sent)
    {
        if (!(msg_outgoing->options & RDP_FLAG_MSGID) || tx_outgoing_msg_in_outstanding_range(c, msg_outgoing))
        {
            if (c->tx_syn_acked && tx_send_ready(c))
            {
#ifdef RDPLIB_DEBUG
                dpf(1u, "immediate tx [%u]\n", msg_outgoing->msgid);
#endif
                if (txq_peek_head(&c->tx_virgin_packets))
                {
                    txq_add_tail(&c->tx_virgin_packets, msg_outgoing);
                    msg_outgoing = txq_remove_head(&c->tx_virgin_packets);
                }
                tx_send_virgin(c, msg_outgoing);
            }
            else
            {
                txq_add_tail(&c->tx_virgin_packets, msg_outgoing);
            }
        }
        else
        {
#ifdef RDPLIB_DEBUG
            dpf(4u, "outstanding message limit reached: enqueue\n");
#endif
            txq_add_tail(&c->tx_delayed_packets, msg_outgoing);
        }
    }
    else
    {
#ifdef RDPLIB_DEBUG
        dpf(0x4000u, "[0x%08x] SYN sent (%u)\n", c, c->tx_syn_msgid);
        assert(c->tx_syn_msgid == msg_outgoing->msgid);
#endif
        msg_outgoing_set_syn_bit(msg_outgoing);
        c->tx_syn_sent = 1;
        tx_send_virgin(c, msg_outgoing);
    }
}

uint32_t tx_send_fin(connection_t *c)
{
    int32_t result;

    result = 0;
    if (!c->tx_fin_sent && c->tx_connected && !c->tx_stopped)
    {
        msg_outgoing_t *msg_outgoing;

#ifndef RDPLIB_SOURCE_FAITHFUL
        if ((uint16_t)(c->tx_next_msgid - c->tx_acked_thru - 1u) >= RDP_BITARRAY_BITS)
        {
            return RDP_CONNECTION_SEND_HISTORY_FULL;
        }
#endif
        msg_outgoing = (msg_outgoing_t *)fast_malloc((uint32_t)sizeof(msg_outgoing_t) + sizeof(uint16_t));
        if (!msg_outgoing)
        {
            return RDP_CONNECTION_SEND_ALLOCATION_FAILED;
        }

        c->tx_fin_msgid = tx_reserve_msgid(c);
        msg_outgoing->options = RDP_FLAG_FIN | RDP_FLAG_MSGID;
        msg_outgoing->msgid = c->tx_fin_msgid;
        msg_outgoing_init(msg_outgoing);
#ifdef RDPLIB_DEBUG
        dpf(0x4000u, "[0x%08x] FIN sent\n", c);
#endif
        tx_enqueue_outgoing(c, msg_outgoing);
        c->tx_fin_sent = 1;
    }
    return result;
}

uint32_t tx_send_alive(connection_t *c)
{
    int32_t result;

    result = 0;
    if (!c->tx_fin_sent && c->tx_connected && !c->tx_stopped)
    {
        msg_outgoing_t *msg_outgoing;

#ifndef RDPLIB_SOURCE_FAITHFUL
        // Keep the final history position available for FIN.
        if ((uint16_t)(c->tx_next_msgid - c->tx_acked_thru - 1u) >= RDP_BITARRAY_BITS - 1u)
        {
            return RDP_CONNECTION_SEND_HISTORY_FULL;
        }
#endif
        msg_outgoing = (msg_outgoing_t *)fast_malloc((uint32_t)sizeof(msg_outgoing_t) + sizeof(uint16_t));
        if (!msg_outgoing)
        {
            return RDP_CONNECTION_SEND_ALLOCATION_FAILED;
        }

        msg_outgoing->options = RDP_FLAG_SYSTEM | RDP_FLAG_MSGID;
        msg_outgoing->msgid = tx_reserve_msgid(c);
        msg_outgoing_init(msg_outgoing);
        tx_enqueue_outgoing(c, msg_outgoing);
    }
    return result;
}

uint32_t trace_send(connection_t *c)
{
    uint32_t ttl;
    uint8_t index;
    trace_probe_t *tp;
    int32_t result;

#ifdef RDPLIB_DEBUG
    assert(c->trace_socket != -1);
#endif
    tp = &c->trace_probes[c->trace_next_index];
#ifdef RDPLIB_DEBUG
    assert(tp->ttl == 0);
#endif
    tp->time_sent = time_get_ms();
    tp->ttl = (uint8_t)c->trace_next_ttl;
    ttl = tp->ttl;
    index = (uint8_t)c->trace_next_index;

    result = rdplib_platform_socket_set_option(c->trace_socket, 0, 7, &ttl, sizeof(ttl));
#ifdef RDPLIB_DEBUG
    assert(result == 0);
#endif
    if (result)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x10000000u, "setsockopt: %s\n", net_strerror(rdplib_platform_last_socket_error()));
#elif defined(RDPLIB_SOURCE_FAITHFUL)
        (void)net_strerror(rdplib_platform_last_socket_error());
#endif
        c->trace_pass = 3;
    }
    else
    {
        result = rdplib_platform_send_datagram(c->trace_socket, &index, sizeof(index), (const uint8_t *)&c->trace_remote_addr);
#ifdef RDPLIB_DEBUG
        assert(result == sizeof(index));
#endif
        if (result == sizeof(index))
        {
            if (++c->trace_next_ttl > c->trace_max_ttl)
            {
                ++c->trace_pass;
                c->trace_next_ttl = 1;
            }
            ++c->trace_next_index;
            c->trace_en_route = 1;
            c->trace_time = time_get_ms();
        }
        else
        {
#ifdef RDPLIB_DEBUG
            dpf(0x10000000u, "sendto: %s\n", net_strerror(rdplib_platform_last_socket_error()));
#elif defined(RDPLIB_SOURCE_FAITHFUL)
            (void)net_strerror(rdplib_platform_last_socket_error());
#endif
            result = -1;
            memset(tp, 0, sizeof(*tp));
        }

        result = rdplib_platform_socket_set_option(c->trace_socket, 0, 7, &c->trace_udp_ttl, sizeof(c->trace_udp_ttl));
#ifdef RDPLIB_DEBUG
        assert(result == 0);
#endif
        if (result)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x10000000u, "setsockopt: %s\n", net_strerror(rdplib_platform_last_socket_error()));
#elif defined(RDPLIB_SOURCE_FAITHFUL)
            (void)net_strerror(rdplib_platform_last_socket_error());
#endif
            c->trace_pass = 3;
        }
#ifdef RDPLIB_DEBUG
        dpf(0x10000000u, "[%u] PROBE %u SENT ttl==%u\n", tp->time_sent, c->trace_next_index, tp->ttl);
#endif
    }
    return result;
}

uint32_t connection_send(connection_t *to, const char *data, uint32_t size, uint32_t stream, uint32_t flags)
{
    iov_t iov;

    iov.data = (void *)data;
    iov.size = size;
    return connection_sendv(to, &iov, 1, stream, flags);
}

uint32_t connection_sendv(connection_t *c, iov_t *iov, uint32_t iov_len, uint32_t stream, uint32_t flags)
{
    uint32_t flag_mask;
    uint32_t i;
    uint16_t last_msgid;
    uint16_t outstanding_ids;
    uint32_t size;
    uint32_t result;
#ifndef RDPLIB_SOURCE_FAITHFUL
    uint16_t checked_fragments;
#endif

    result = RDP_CONNECTION_SEND_OK;
    if (!c)
    {
        return RDP_CONNECTION_SEND_INVALID_ARGUMENT;
    }
#ifndef RDPLIB_SOURCE_FAITHFUL
    if (!iov && iov_len != 0)
    {
        return RDP_CONNECTION_SEND_INVALID_ARGUMENT;
    }
#endif

    size = 0;
    for (i = 0; i < iov_len; ++i)
    {
#ifndef RDPLIB_SOURCE_FAITHFUL
        if ((!iov[i].data && iov[i].size != 0) || iov[i].size > UINT32_MAX - size)
        {
            return RDP_CONNECTION_SEND_INVALID_ARGUMENT;
        }
#endif
        size += iov[i].size;
    }

    if (size > 51200u || (!(flags & RDP_SEND_RELIABLE) && size > 512u))
    {
#ifdef RDPLIB_DEBUG
        dpf(0x400000u, "RDP_SEND_ERROR_TOO_BIG %s, %u bytes\n", flags == 1 ? "guaranteed" : "best effort", size);
#endif
        return RDP_SEND_ERROR_TOO_BIG;
    }
    flag_mask = ~UINT32_C(0xff);
    if (stream & flag_mask)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x400000u, "UERROR_PARAMETER: stream == %u\n", stream);
#endif
        return RDP_CONNECTION_SEND_INVALID_ARGUMENT;
    }
    flag_mask = ~UINT32_C(1);
    if (flags & flag_mask)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x400000u, "UERROR_PARAMETER: flags == %u\n", flags);
#endif
        return RDP_CONNECTION_SEND_INVALID_ARGUMENT;
    }
#ifndef RDPLIB_SOURCE_FAITHFUL
    if (stream >= STREAMS_PER_CONNECTION)
    {
        return RDP_CONNECTION_SEND_INVALID_ARGUMENT;
    }
    checked_fragments = (flags & RDP_SEND_RELIABLE) ? (uint16_t)((size + 511u) >> 9) : 0;
#endif

    rdp_lock_connection(c);
    if (!c->tx_connected)
    {
        result = RDP_CONNECTION_SEND_NOT_CONNECTED;
        goto exit;
    }
    if (c->tx_stopped)
    {
        result = RDP_CONNECTION_SEND_PEER_STOPPED;
        goto exit;
    }
    if (c->tx_fin_sent)
    {
        result = RDP_CONNECTION_SEND_FIN_SENT;
        goto exit;
    }

    last_msgid = (uint16_t)(c->tx_next_msgid - 1u);
    outstanding_ids = (uint16_t)(last_msgid - c->tx_acked_thru);
#ifndef RDPLIB_SOURCE_FAITHFUL
    if ((uint32_t)outstanding_ids + 1u >= RDP_BITARRAY_BITS || ((flags & RDP_SEND_RELIABLE) && (uint32_t)outstanding_ids + checked_fragments >= RDP_BITARRAY_BITS))
    {
        result = RDP_CONNECTION_SEND_HISTORY_FULL;
        goto exit;
    }
    if (txq_get_queue_size(&c->tx_outstanding_packets) > UINT32_MAX - txq_get_queue_size(&c->tx_virgin_packets) ||
        txq_get_queue_size(&c->tx_outstanding_packets) + txq_get_queue_size(&c->tx_virgin_packets) > UINT32_MAX - txq_get_queue_size(&c->tx_delayed_packets))
    {
        result = RDP_CONNECTION_SEND_INVALID_ARGUMENT;
        goto exit;
    }
#else
    if ((uint32_t)outstanding_ids + 1u >= RDP_BITARRAY_BITS)
    {
        result = RDP_CONNECTION_SEND_HISTORY_FULL;
        goto exit;
    }
#endif
    if (tx_get_queue_size(c) > c->tx_send_buffer_size)
    {
        result = RDP_CONNECTION_SEND_BUFFER_FULL;
        goto exit;
    }

    if (flags & RDP_SEND_RELIABLE)
    {
        uint32_t iov_index;
        uint16_t fragments;
        uint16_t frag;
        msg_outgoing_t *msg_outgoing;
        uint32_t remaining_bytes;
        uint32_t iov_pos;
        uint16_t options;

#ifdef RDPLIB_SOURCE_FAITHFUL
        fragments = (uint16_t)((size + 511u) >> 9);
#else
        fragments = checked_fragments;
#endif
#ifdef RDPLIB_DEBUG
        assert(fragments <= RDP_FRAGMENT_COUNT_MAX);
#endif
        options = RDP_FLAG_MSGID;
        remaining_bytes = size;
#ifndef RDPLIB_SOURCE_FAITHFUL
        msg_outgoing_t *fragment_messages[RDP_FRAGMENT_COUNT_MAX] = {0};

        for (frag = 0; frag < fragments; ++frag)
        {
            uint32_t checked_frag_size;
            uint32_t checked_msg_outgoing_size;

            checked_frag_size = remaining_bytes >= 512u ? 512u : remaining_bytes;
            checked_msg_outgoing_size = (uint32_t)sizeof(msg_outgoing_t) + 10u + checked_frag_size;

            fragment_messages[frag] = (msg_outgoing_t *)fast_malloc(checked_msg_outgoing_size);
            if (!fragment_messages[frag])
            {
                uint16_t release_index;

#ifdef RDPLIB_DEBUG
                dpf(UINT32_MAX, "malloc failed %s:%u\n", "C:\\scratch\\Network\\rdp\\tx.c", 1111u);
#endif
                for (release_index = 0; release_index < frag; ++release_index)
                {
                    fast_free(fragment_messages[release_index]);
                }
                result = RDP_CONNECTION_SEND_ALLOCATION_FAILED;
                goto exit;
            }
            remaining_bytes -= checked_frag_size;
        }
        remaining_bytes = size;
#endif
        if (fragments > 1)
        {
            options = RDP_FLAG_MSGID | RDP_FLAG_FRAGMENT;
        }
        iov_index = 0;
        iov_pos = 0;
        for (frag = 0; frag < fragments; ++frag)
        {
            uint32_t frag_size;
            uint32_t msg_outgoing_size;

            frag_size = remaining_bytes >= 512u ? 512u : remaining_bytes;
            msg_outgoing_size = (uint32_t)sizeof(msg_outgoing_t) + 10u + frag_size;

#ifdef RDPLIB_DEBUG
            assert(msg_outgoing_size <= 570);
#endif
            (void)msg_outgoing_size;
#ifndef RDPLIB_SOURCE_FAITHFUL
            msg_outgoing = fragment_messages[frag];
#else
            msg_outgoing = (msg_outgoing_t *)fast_malloc(msg_outgoing_size);
            if (!msg_outgoing)
            {
#ifdef RDPLIB_DEBUG
                dpf(UINT32_MAX, "malloc failed %s:%u\n", "C:\\scratch\\Network\\rdp\\tx.c", 1111u);
#endif
                result = RDP_CONNECTION_SEND_ALLOCATION_FAILED;
                goto exit;
            }
#endif
            options &= (uint16_t)~RDP_FLAG_SEQUENCED;
            if (stream && !frag)
            {
                options |= RDP_FLAG_SEQUENCED;
#ifdef RDPLIB_DEBUG
                dpf(0x1000u, "stream [%u] seqnum [%u] sent on msgid (%u)\n", stream, c->tx_guaranteed_stream_seqnum[stream], c->tx_next_msgid);
#endif
            }
            if (options & RDP_FLAG_FRAGMENT)
            {
#ifdef RDPLIB_DEBUG
                dpf(0x800u, "sending fragment [%u:%u/%u] (%u)\n", c->tx_next_fragid, frag, fragments, c->tx_next_msgid);
#endif
            }
            else
            {
#ifdef RDPLIB_DEBUG
                dpf(0x800u, "sending message (%u)\n", c->tx_next_msgid);
#endif
            }

            msg_outgoing->options = options;
            msg_outgoing->msgid = tx_reserve_msgid(c);
            msg_outgoing->fragid = c->tx_next_fragid;
            msg_outgoing->frag_number = frag;
            msg_outgoing->frag_total = fragments;
            msg_outgoing->stream = (uint8_t)stream;
            msg_outgoing->stream_seqnum = c->tx_guaranteed_stream_seqnum[stream];
            msg_outgoing_init(msg_outgoing);

            {
                uint32_t bytes_remaining_in_fragment;

                bytes_remaining_in_fragment = frag_size;
                while (bytes_remaining_in_fragment)
                {
                    uint32_t copy_size;
                    uint32_t bytes_remaining_in_vector_element;

#ifndef RDPLIB_SOURCE_FAITHFUL
                    // Zero length vectors are valid in checked builds; skip them before forming a pointer from data.
                    while (iov_index < iov_len && iov[iov_index].size == 0)
                    {
                        ++iov_index;
                    }
#endif
                    bytes_remaining_in_vector_element = iov[iov_index].size - iov_pos;
                    copy_size = bytes_remaining_in_vector_element >= bytes_remaining_in_fragment ? bytes_remaining_in_fragment : bytes_remaining_in_vector_element;

                    msg_outgoing_append(msg_outgoing, (const uint8_t *)iov[iov_index].data + iov_pos, copy_size);
                    iov_pos += copy_size;
                    bytes_remaining_in_fragment -= copy_size;
                    if (iov_pos == iov[iov_index].size)
                    {
                        iov_pos = 0;
                        ++iov_index;
                    }
                }
            }

            if (options & RDP_FLAG_SEQUENCED)
            {
                ++c->tx_guaranteed_stream_seqnum[stream];
            }
            tx_enqueue_outgoing(c, msg_outgoing);
            remaining_bytes -= frag_size;
        }

#ifndef RDPLIB_SOURCE_FAITHFUL
        while (iov_index < iov_len && iov[iov_index].size == 0)
        {
            ++iov_index;
        }
#endif
        if (fragments > 1)
        {
            ++c->tx_next_fragid;
        }
#ifdef RDPLIB_DEBUG
        assert(iov_index == iov_len);
        assert(remaining_bytes == 0);
#endif
    }
    else
    {
        msg_outgoing_t *msg_outgoing;
        uint32_t msg_outgoing_size;
        uint16_t options;

        options = stream ? RDP_FLAG_SEQUENCED : 0;
        msg_outgoing_size = (uint32_t)sizeof(msg_outgoing_t) + 4u + size;
#ifdef RDPLIB_DEBUG
        assert(msg_outgoing_size <= 570);
#endif
        msg_outgoing = (msg_outgoing_t *)fast_malloc(msg_outgoing_size);
#ifndef RDPLIB_SOURCE_FAITHFUL
        if (!msg_outgoing)
        {
            result = RDP_CONNECTION_SEND_ALLOCATION_FAILED;
            goto exit;
        }
#endif
        msg_outgoing->options = options;
        msg_outgoing->stream = (uint8_t)stream;
        msg_outgoing_init(msg_outgoing);

        for (i = 0; i < iov_len; ++i)
        {
#ifndef RDPLIB_SOURCE_FAITHFUL
            if (iov[i].size == 0)
            {
                continue;
            }
#endif
            msg_outgoing_append(msg_outgoing, iov[i].data, iov[i].size);
        }
        tx_enqueue_outgoing(c, msg_outgoing);
    }

    rdp_resort(c, 1);

exit:
    rdp_unlock(c);
    return result;
}

void tx_get_event_time(connection_t *c, timeout_data *timeout_data)
{
    msg_outgoing_t *msg_outstanding;
    msg_outgoing_t *msg_virgin;

    msg_virgin = txq_peek_head(&c->tx_virgin_packets);
    msg_outstanding = txq_peek_head(&c->tx_outstanding_packets);
#ifdef RDPLIB_DEBUG
    dpf(0x40u, "tx queues: 0x%08x 0x%08x 0x%08x %01d\n", txq_peek_head(&c->tx_delayed_packets), msg_virgin, msg_outstanding, c->tx_delayed_ack);
#endif
    if (msg_virgin || msg_outstanding || c->tx_delayed_ack)
    {
#ifndef RDPLIB_SOURCE_FAITHFUL
        // An unreliable message cannot establish SYN. Leave it queued until the first reliable message establishes this direction.
        if (msg_virgin && !c->tx_syn_acked && !msg_outstanding && !c->tx_delayed_ack)
        {
            timeout_data->infinite = 1;
            timeout_data->time = 0;
            return;
        }
#endif
        timeout_data->infinite = 0;
        timeout_data->time = bandwidth_get_time_empty(&c->tx_bandwidth);
        if (c->tx_remote_addr.sa_family == RDP_TRANSMIT_ADDRESS_SERIAL)
        {
            timeout_data->time = rdp_serial_get_time_empty(c->cn_rdp);
        }

        if (!msg_virgin || !c->tx_syn_acked)
        {
            uint32_t lower_timeout;

            lower_timeout = c->tx_ack_time;
            if (msg_outstanding)
            {
                uint32_t timeout_outstanding;

                timeout_outstanding = msg_outstanding->time_last_sent + timeout_get_timeout(&c->tx_rt_tracker);
                if (!c->tx_delayed_ack || (int32_t)(c->tx_ack_time - timeout_outstanding) > 0)
                {
                    lower_timeout = timeout_outstanding;
                }
            }
            if ((int32_t)(lower_timeout - timeout_data->time) > 0)
            {
                timeout_data->time = lower_timeout;
            }
        }
    }
    else
    {
#ifdef RDPLIB_DEBUG
        assert(!c->tx_syn_sent || c->tx_syn_acked || !c->tx_connected || c->tx_stopped);
#endif
        timeout_data->infinite = 1;
        timeout_data->time = 0;
    }
}

uint32_t tx_send_ready_virgins(connection_t *c)
{
    uint32_t age;
    msg_outgoing_t *msg_outgoing;
    uint32_t result;
    msg_outgoing_t *msg_virgin;

    age = 0;
    result = 0;
    msg_virgin = txq_peek_head(&c->tx_virgin_packets);
    msg_outgoing = txq_peek_head(&c->tx_outstanding_packets);
    if (msg_outgoing)
    {
        age = time_get_ms() - msg_outgoing->time_last_sent;
    }

    if (msg_virgin)
    {
        if (c->tx_remote_addr.sa_family == RDP_TRANSMIT_ADDRESS_SERIAL)
        {
            result = age <= 10000u;
        }
        else
        {
            result = age <= timeout_get_ancient(&c->tx_rt_tracker);
        }
    }
    return result;
}

void tx_abort_connection(connection_t *c, uint32_t disconnect_reason)
{
#ifdef RDPLIB_DEBUG
    dpf(0x2000u, "[0x%08x] aborting connection (%08x)\n", c, disconnect_reason);
#endif
    tx_flush_output_buffers(c);
    rx_flush_input_buffers(c);

#ifndef RDPLIB_SOURCE_FAITHFUL
    if (!c->tx_connected)
    {
        // Keep the first abort's cause, counters, and notification state.
        goto complete_waiters;
    }
#endif

    if (!c->tx_syn_acked)
    {
        ++g_rdp_stat->embryonic_connections_dropped;
    }

    c->tx_connected = 0;
    c->tx_stopped = 1;
    c->tx_disconnect_reason = disconnect_reason;

    if (disconnect_reason == RDP_DISCONNECT_REASON_PROTOCOL_ERROR)
    {
        ++g_rdp_stat->connections_dropped_hacker;
    }
#ifdef RDPLIB_SOURCE_FAITHFUL
    else if (c->rx_icmp_received && (int32_t)(time_get_ms() - c->rx_icmp_time) < 10000)
    {
        c->tx_disconnect_reason = RDP_DISCONNECT_REASON_ICMP;
    }
#endif

    if (disconnect_reason > RDP_DISCONNECT_REASON_UNACKNOWLEDGED_MESSAGE)
    {
        if (disconnect_reason == RDP_DISCONNECT_REASON_CONNECTION_INACTIVITY)
        {
            ++g_rdp_stat->connections_dropped_no_response;
        }
    }
    else
    {
        switch (disconnect_reason)
        {
        case RDP_DISCONNECT_REASON_UNACKNOWLEDGED_MESSAGE:
            ++g_rdp_stat->connections_dropped_msg_age;
            break;

        case RDP_DISCONNECT_REASON_PEER_RESET:
            ++g_rdp_stat->connections_dropped_rst;
            break;

        case RDP_DISCONNECT_REASON_ICMP:
            switch (c->rx_icmp_type)
            {
            case 3:
                if (c->rx_icmp_code < 16)
                {
                    ++g_rdp_stat->connections_dropped_unreachable[c->rx_icmp_code];
                }
                else
                {
                    ++g_rdp_stat->connections_dropped_icmp_unknown;
                }
                break;

            case 4:
                ++g_rdp_stat->connections_dropped_source_quench;
                break;

            case 11:
                if (c->rx_icmp_code < 2)
                {
                    ++g_rdp_stat->connections_dropped_ttl_expired[c->rx_icmp_code];
                }
                else
                {
                    ++g_rdp_stat->connections_dropped_icmp_unknown;
                }
                break;

            case 12:
                if (c->rx_icmp_code < 2)
                {
                    ++g_rdp_stat->connections_dropped_parameter_problem[c->rx_icmp_code];
                }
                else
                {
                    ++g_rdp_stat->connections_dropped_icmp_unknown;
                }
                break;
            }
            break;
        }
    }

#ifndef RDPLIB_SOURCE_FAITHFUL
    c->tx_enqueued_disconnect_msg = 0;

complete_waiters:
#endif
    if (c->tx_all_acked)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x4000u, "all_acked == FALSE (not connected) (c->tx_abort_connection)\n");
#endif
        *c->tx_all_acked = 0;
        c->tx_all_acked = NULL;
    }

    if (c->tx_all_acked_event)
    {
        uevent_signal(c->tx_all_acked_event);
        c->tx_all_acked_event = NULL;
    }

#ifdef RDPLIB_SOURCE_FAITHFUL
    c->tx_enqueued_disconnect_msg = 0;
#endif
}

void tx_received_stopped(connection_t *c)
{
    if (c->tx_all_acked)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x4000u, "all_acked == TRUE (stopped by peer) (c->tx_received_stopped)\n");
#endif
        *c->tx_all_acked = 1;
        c->tx_all_acked = NULL;
    }

    if (c->tx_all_acked_event)
    {
        uevent_signal(c->tx_all_acked_event);
        c->tx_all_acked_event = NULL;
    }

    tx_flush_output_buffers(c);
    c->tx_stopped = 1;
}

void tx_set_delayed_ack(connection_t *c)
{
    if (!c->tx_delayed_ack)
    {
        c->tx_delayed_ack = 1;
        c->tx_ack_time = time_get_ms() + 50u;
    }
}

void tx_tx(connection_t *c)
{
    uint32_t ready_time;
    uint32_t current_time;
    msg_outgoing_t *msg_outstanding;
    msg_outgoing_t *msg_virgin;

    msg_virgin = txq_peek_head(&c->tx_virgin_packets);
    msg_outstanding = txq_peek_head(&c->tx_outstanding_packets);
    current_time = time_get_ms();
    ready_time = bandwidth_get_time_empty(&c->tx_bandwidth);
    if (c->tx_remote_addr.sa_family == RDP_TRANSMIT_ADDRESS_SERIAL)
    {
        ready_time = rdp_serial_get_time_empty(c->cn_rdp);
    }

    ++g_rdp_stat->shipping_tx;
    if ((int32_t)(current_time - ready_time + 10u) < 0)
    {
        ++g_rdp_stat->shipping_tx_not_ready;
    }
    else if (c->tx_syn_acked && tx_send_ready_virgins(c))
    {
        msg_virgin = txq_remove_head(&c->tx_virgin_packets);

        if (msg_virgin->options)
        {
#ifdef RDPLIB_DEBUG
            dpf(1u, "delayed tx [%u]\n", msg_virgin->msgid);
#endif
        }
        else
        {
#ifdef RDPLIB_DEBUG
            dpf(1u, "delayed tx\n");
#endif
        }
        tx_send_virgin(c, msg_virgin);
    }
    else if (msg_outstanding && (int32_t)(msg_outstanding->time_last_sent + timeout_get_timeout(&c->tx_rt_tracker) - 10u - current_time) <= 0)
    {
        uint32_t time_since_arrival;
        uint32_t msg_age;

        time_since_arrival = current_time - c->rx_time_last_arrival;
        msg_age = current_time - msg_outstanding->time_first_sent;

        if (msg_age >= c->tx_max_message_age / 2u || (msg_age >= c->tx_max_service_outage / 2u && time_since_arrival >= c->tx_max_service_outage))
        {
#ifdef RDPLIB_DEBUG
            assert(c->tx_connected);
#endif
            if ((c->cn_flags & RDP_CONNECTION_FEATURE_TRACEROUTE) && !trace_start(c))
            {
#ifdef RDPLIB_DEBUG
                dpf(0x10000000u, "starting trace on faulty connection\n");
#endif
            }
        }

        if (msg_age < c->tx_max_message_age && (msg_age < c->tx_max_service_outage || time_since_arrival < c->tx_max_service_outage))
        {
            msg_outstanding = txq_remove_head(&c->tx_outstanding_packets);
            msg_outgoing_set_time_last_sent(msg_outstanding, current_time);
            if ((int16_t)(msg_outstanding->msgid - c->tx_acked_thru - 120u) >= 0)
            {
#ifdef RDPLIB_DEBUG
                assert(0);
#endif
            }
            else
            {
                tx_send_packet(c, msg_outgoing_get_data(msg_outstanding), msg_outstanding->size, msg_outstanding->options);
                ++g_rdp_stat->guaranteed_packets_retx;
                g_rdp_stat->guaranteed_bytes_retx += msg_outstanding->size;
                ++c->stat.guaranteed_packets_retx;
                c->stat.guaranteed_bytes_retx += msg_outstanding->size;
            }
            txq_add_tail(&c->tx_outstanding_packets, msg_outstanding);
        }
        else
        {
#ifdef RDPLIB_DEBUG
            assert(c->tx_connected);
#endif
            if (c->tx_connected)
            {
                if (msg_age < c->tx_max_message_age)
                {
                    tx_abort_connection(c, RDP_DISCONNECT_REASON_CONNECTION_INACTIVITY);
                }
                else
                {
                    tx_abort_connection(c, RDP_DISCONNECT_REASON_UNACKNOWLEDGED_MESSAGE);
                }
            }
#ifdef RDPLIB_DEBUG
            dpf(0x2000u, "connection lost: msg_age==%u time_since_arrival==%u\n", msg_age, time_since_arrival);
#endif
        }
    }
    else if (c->tx_delayed_ack)
    {
        tx_send_packet(c, NULL, 0, 0);
    }
    else
    {
        ++g_rdp_stat->shipping_tx_useless;
    }
}

uint32_t tx_send_packet(connection_t *c, char *data, uint32_t size, uint16_t options_in_data)
{
    iov_t iov[2];
    uint32_t header_size;
    uint32_t ack_size;
    uint16_t header[12];
    uint32_t result;
#if defined(__linux__) && !defined(RDPLIB_SOURCE_FAITHFUL)
    uint32_t short_backoff = 0;
#endif
    uint16_t *options;
#ifndef RDPLIB_SOURCE_FAITHFUL
    uint32_t send_ack_separately = 0;
    uint16_t options_without_ack;
    uint16_t saved_unreported_message_count = c->rx_msgid_count;
    uint16_t saved_unreported_min_message_id = c->rx_msgid_lo;
    uint16_t saved_unreported_max_message_id = c->rx_msgid_hi;
    uint32_t saved_delayed_ack_pending = c->tx_delayed_ack;
    uint32_t saved_delayed_ack_deadline_ms = c->tx_ack_time;
#endif

    options = header;
#ifdef RDPLIB_DEBUG
    assert((int32)(rdp_serial_get_time_empty( c->cn_rdp ) - time_get_ms()) < 20);
#endif
    header_size = 2;
    *options = options_in_data;
    if (c->tx_connected)
    {
        if (c->cn_closed)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x4000u, "[0x%08x] connection closed, setting stop bit\n", c);
#endif
            *options |= RDP_FLAG_STOP;
        }
    }
    else
    {
#ifdef RDPLIB_DEBUG
        dpf(0x4000u, "[0x%08x] connection broken, setting reset bit\n", c);
#endif
        *options |= RDP_FLAG_RESET;
    }

#ifndef RDPLIB_SOURCE_FAITHFUL
    options_without_ack = *options;
#endif
    header[header_size / 2u] = htons(c->tx_next_seqnum);
    header_size += 2;
    ack_size = rx_append_ack(c, &header[header_size / 2u], options);
    header_size += ack_size;
#ifdef RDPLIB_DEBUG
    assert(ack_size || !c->tx_delayed_ack);
#endif
    c->tx_delayed_ack = 0;

#ifndef RDPLIB_SOURCE_FAITHFUL
    if (data && ack_size)
    {
        uint32_t framed_size = header_size + size;

        if (c->tx_remote_addr.sa_family != RDP_TRANSMIT_ADDRESS_SERIAL)
        {
            framed_size = rdplib_usend_framed_size(framed_size, c->cn_rdp->encrypt, c->cn_rdp->crc);
        }

        if (framed_size > RDP_LEGACY_DATAGRAM_BYTES)
        {
            // Keep each wire datagram within an original peer's receive
            // capacity. The successful data send is followed by an ACK-only
            // packet carrying the complete acknowledgement report.
            c->rx_msgid_count = saved_unreported_message_count;
            c->rx_msgid_lo = saved_unreported_min_message_id;
            c->rx_msgid_hi = saved_unreported_max_message_id;
            c->tx_delayed_ack = saved_delayed_ack_pending;
            c->tx_ack_time = saved_delayed_ack_deadline_ms;

            *options = options_without_ack;
            header_size = RDP_WIRE_HEADER_BASE_BYTES;
            ack_size = 0;
            send_ack_separately = 1;
        }
    }
#endif

    if (ack_size)
    {
        if (data)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x10000u, "ack appended to data packet (optimized)\n");
#endif
            ++g_rdp_stat->ack_and_data_packets_tx;
            ++c->stat.ack_and_data_packets_tx;
        }
        else
        {
#ifdef RDPLIB_DEBUG
            dpf(0x10000u, "ack appended to empty packet (timeout)\n");
#endif
            ++g_rdp_stat->ack_only_packets_tx;
            ++c->stat.ack_only_packets_tx;
        }
    }

    iov[0].data = header;
    iov[0].size = header_size;
    iov[1].data = data;
    iov[1].size = size;
    *options = htons(*options);

    if (c->tx_remote_addr.sa_family == RDP_TRANSMIT_ADDRESS_SERIAL)
    {
#ifdef RDPLIB_SOURCE_FAITHFUL
        result = serial_send(&c->cn_rdp->serial, iov, 2u, (sockaddr_com *)&c->tx_remote_addr);
#else
        result = serial_send(&c->cn_rdp->serial, iov, data ? 2u : 1u, (sockaddr_com *)&c->tx_remote_addr);
#endif
#ifdef RDPLIB_DEBUG
        assert(result == URESULT_OK || result == UERROR_TRY_AGAIN || result == UERROR_SYSTEM);
#endif
    }
    else
    {
#ifdef RDPLIB_SOURCE_FAITHFUL
        result = usend(c->tx_socket, iov, 2u, &c->tx_remote_addr, c->cn_rdp->encrypt, c->cn_rdp->crc);
#else
        result = rdplib_usend(c, c->tx_socket, iov, data ? 2u : 1u, &c->tx_remote_addr, c->cn_rdp->encrypt, c->cn_rdp->crc);
#if defined(__linux__)
        if (result == 5)
        {
            // Capture the failed send's errno before logging or clock calls.
            int error = errno;
            short_backoff = error != EAGAIN && error != EWOULDBLOCK && error != ENOBUFS && error != ENOMEM;
        }
#endif
#endif
#ifdef RDPLIB_DEBUG
        dpf(0x40000u, "sendto TIME: %u SEQNUM: %u SIZE: %u\n", time_get_ms(), c->tx_next_seqnum, size + header_size);
        assert(result == URESULT_OK || result == UERROR_TRY_AGAIN);
#elif defined(RDPLIB_SOURCE_FAITHFUL)
        (void)time_get_ms();
#endif
    }

    ++g_rdp_stat->sendto_calls;
    if (result)
    {
        if (g_rdp_stat->last_sendto_failed)
        {
            ++g_rdp_stat->sendto_consecutive_failures;
        }
        g_rdp_stat->last_sendto_failed = 1;
        ++g_rdp_stat->sendto_failures;
    }
    else
    {
        g_rdp_stat->last_sendto_failed = 0;
    }

    if (result == 1)
    {
        tx_abort_connection(c, RDP_DISCONNECT_REASON_SEND_ERROR);
    }
    else if (result == 5)
    {
        uint32_t rate = bandwidth_get_send_speed(&c->tx_bandwidth);
        uint32_t backoff_bytes = rate / 10u;

#if defined(__linux__) && !defined(RDPLIB_SOURCE_FAITHFUL)
        if (short_backoff)
        {
            // Keep retry pacing for unattributed errors modest: about 20 ms.
            // Round up so a low configured rate still yields a nonzero charge.
            backoff_bytes = rate / 50u + (rate % 50u != 0u);
        }
#endif
        bandwidth_enqueue_bytes(&c->tx_bandwidth, backoff_bytes);
#ifndef RDPLIB_SOURCE_FAITHFUL
        c->rx_msgid_count = saved_unreported_message_count;
        c->rx_msgid_lo = saved_unreported_min_message_id;
        c->rx_msgid_hi = saved_unreported_max_message_id;
        c->tx_delayed_ack = saved_delayed_ack_pending;
        c->tx_ack_time = saved_delayed_ack_deadline_ms;
#endif
    }
    else if (!result)
    {
        bandwidth_enqueue_bytes(&c->tx_bandwidth, header_size + size + 28u);
        ++c->tx_next_seqnum;
#ifndef RDPLIB_SOURCE_FAITHFUL
        if (send_ack_separately)
        {
            // The data has already been submitted. A retryable ACK send
            // restores the pending report through the normal maintained path.
            (void)tx_send_packet(c, NULL, 0, 0);
        }
#endif
    }

    return result;
}

uint32_t trace_start(connection_t *c)
{
    rdp_t *rdp;
    uint32_t probe_record_size;
    uint32_t flag_mask;
    uint32_t result;

    result = 0;
    if (!c)
    {
        return 6;
    }
    rdp = c->cn_rdp;
    if (rdp->trace_socket == -1)
    {
        return 8;
    }

    rdp_lock_connection(c);
    flag_mask = RDP_CONNECTION_FEATURE_TRACEROUTE;
    if ((c->cn_flags & flag_mask) && time_get_ms() - c->trace_start < c->tx_max_service_outage)
    {
        result = 9;
    }
    else if (c->trace_socket != -1)
    {
        if (c->trace_probes)
        {
            if (c->rx_connect_trace)
            {
                rdplib_platform_free(c->trace_probes);
                c->trace_probes = NULL;
            }
            else
            {
                probe_record_size = (uint32_t)(sizeof(trace_probe_t) * c->trace_next_index);
                c->rx_connect_trace = (trace_probe_t *)rdplib_platform_malloc(probe_record_size);
                if (!c->rx_connect_trace)
                {
                    result = 2;
                    goto exit;
                }
                memcpy(c->rx_connect_trace, c->trace_probes, probe_record_size);
                c->rx_connect_count = c->trace_next_index;
                c->rx_connect_clock = c->trace_clock;
            }
        }

        probe_record_size = (uint32_t)(sizeof(trace_probe_t) * 90);
        if (c->trace_probes || (c->trace_probes = (trace_probe_t *)rdplib_platform_malloc(probe_record_size)) != NULL)
        {
            memset(c->trace_probes, 0, probe_record_size);
            c->trace_en_route = 0;
            c->trace_start = time_get_ms();
            c->trace_time = c->trace_start;
            c->trace_clock = rdplib_platform_wall_time_seconds();
            c->trace_next_ttl = 1;
            c->trace_max_ttl = 30;
            c->trace_pass = 0;
            c->trace_next_index = 0;
            c->cn_flags |= flag_mask;
        }
        else
        {
            result = 2;
        }
    }
    else
    {
        result = 8;
    }

exit:
    rdp_unlock(c);
    return result;
}

uint32_t connection_set_max_data_rate(connection_t *c, uint32_t bytes_per_second)
{
    uint32_t prev_bandwidth;

    prev_bandwidth = c->tx_bandwidth.bandwidth;
    c->tx_bandwidth.bandwidth = bytes_per_second;
    return prev_bandwidth;
}

uint32_t tx_get_stall_time(connection_t *c)
{
    uint32_t stall;
    uint32_t tx_time;

    stall = 0;
    if (txq_peek_head(&c->tx_outstanding_packets))
    {
        stall = time_get_ms() - txq_get_oldest_time_sent(&c->tx_outstanding_packets);
    }
    tx_time = timeout_get_timeout(&c->tx_rt_tracker);

    if (stall <= tx_time)
    {
        return 0;
    }
    return stall - tx_time;
}
