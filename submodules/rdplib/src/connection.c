// Copyright (c) 2026 solar
// SPDX-License-Identifier: MIT

#include "connection.h"

#ifdef RDPLIB_DEBUG
#include <assert.h>
#endif
#include <stddef.h>
#include <string.h>

#ifdef RDPLIB_DEBUG
#include "dpf.h"
#endif
#ifdef RDPLIB_SOURCE_FAITHFUL
#include "log.h"
#endif
#include "rdp.h"
#ifndef RDPLIB_SOURCE_FAITHFUL
#include "protocol_limits.h"
#include "rdplib_wire.h"
#endif
#include "rdpstat.h"
#include "rx.h"
#include "rxq.h"
#include "trace.h"
#include "tx.h"
#include "usemaphore.h"
#include "utime.h"

void connection_init(connection_t *c, rdp_t *rdp, struct sockaddr *remote_addr, uint32_t flags)
{
    c->cn_rdp = rdp;
    tx_init(c, rdp, remote_addr);
    rx_init(c);

    c->cn_closed = 0;
    c->cn_delete_time = 0;
    c->cn_accepted = 0;
    c->cn_abort = 0;
    c->cn_app_ptr[0] = NULL;
    c->cn_app_ptr[1] = NULL;
    c->cn_app_ptr[2] = NULL;

    umutex_create(&c->cn_lock);
    c->cn_addr_map_link.item = c;
    c->cn_addr_map_link.key.p = &c->tx_remote_addr;
    c->cn_event_queue_link.item = c;
    c->cn_event_queue_link.key.p = &c->cn_event_time;
    c->cn_event_time.infinite = 1;
    c->cn_event_time.time = 0;
    c->cn_event_type = CONNECTION_EVENT_NONE;
    c->cn_flags = flags;

#ifndef RDPLIB_SOURCE_FAITHFUL
    c->rdplib_keepalive_interval_ms = RDPLIB_DEFAULT_KEEPALIVE_INTERVAL_MS;
    c->rdplib_packet_drop_callback = NULL;
    c->rdplib_packet_drop_context = NULL;
#endif
}

uint32_t connection_create(connection_t *c)
{
    uint32_t result;

    result = tx_create(c);
    if (result == 0)
    {
        return rx_create(c);
    }
    return result;
}

void connection_destroy(connection_t *c)
{
    tx_destroy(c);
    rx_destroy(c);
    umutex_destroy(&c->cn_lock);
}

void connection_recalc_event_timeout(connection_t *c, timeout_data *uevent_time)
{
    uevent_time->infinite = 1;
    uevent_time->time = 0;
#ifdef RDPLIB_DEBUG
    assert(umutex_owner( &c->cn_lock ));
#endif
    c->cn_event_type = CONNECTION_EVENT_NONE;

    if (!c->tx_connected || c->tx_stopped)
    {
        if (c->tx_connected && c->tx_stopped && c->tx_delayed_ack)
        {
            c->cn_event_type = CONNECTION_EVENT_TX;
            uevent_time->infinite = 0;
#ifdef RDPLIB_SOURCE_FAITHFUL
            uevent_time->time = c->tx_ack_time;
#else
            // We may still owe ACKs to a stopped peer; respect retry pacing.
            tx_get_event_time(c, uevent_time);
#endif
        }
    }
    else
    {
        tx_get_event_time(c, uevent_time);
        if (!uevent_time->infinite)
        {
            c->cn_event_type = CONNECTION_EVENT_TX;
        }
    }

    if (c->cn_flags & RDP_CONNECTION_FEATURE_KEEPALIVE)
    {
        if (c->tx_syn_sent && !c->tx_fin_sent && c->tx_connected && !c->tx_stopped
#ifndef RDPLIB_SOURCE_FAITHFUL
            // prevents a keepalive from consuming the final available history slot
            && (uint16_t)(c->tx_next_msgid - c->tx_acked_thru - 1u) < RDP_BITARRAY_BITS - 1u
#endif
        )
        {
            uint32_t alive_time;
            int32_t delta;

#ifdef RDPLIB_SOURCE_FAITHFUL
            alive_time = c->tx_time_last_guaranteed_send + 10000u;
#else
            alive_time = c->tx_time_last_guaranteed_send + c->rdplib_keepalive_interval_ms;
#endif
            delta = (int32_t)(alive_time - uevent_time->time);
            if (uevent_time->infinite || delta < 0)
            {
#ifdef RDPLIB_DEBUG
                dpf(0x40u, "CONNECTION_EVENT_ALIVE: overriding other events with alive event (%d) { %u, %u }\n", delta, uevent_time->infinite, uevent_time->time);
#endif
                c->cn_event_type = CONNECTION_EVENT_ALIVE;
                uevent_time->infinite = 0;
                uevent_time->time = alive_time;
            }
        }
    }

    if ((c->cn_flags & RDP_CONNECTION_FEATURE_TRACEROUTE) && c->trace_pass < 3)
    {
        int32_t delta;
        uint32_t trace_time_next_send;

#ifdef RDPLIB_DEBUG
        assert(c->trace_next_ttl <= c->trace_max_ttl);
#endif
        if (c->trace_en_route)
        {
            trace_time_next_send = c->trace_time + timeout_get_timeout(&c->tx_rt_tracker);
        }
        else
        {
            trace_time_next_send = c->trace_time;
        }

        delta = (int32_t)(trace_time_next_send - uevent_time->time);
        if (uevent_time->infinite || delta < 0)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x40u, "CONNECTION_EVENT_TRACE: overriding other events with trace event (%d) { %u, %u }\n", delta, uevent_time->infinite, uevent_time->time);
#endif
            c->cn_event_type = CONNECTION_EVENT_TRACE;
            uevent_time->infinite = 0;
            uevent_time->time = trace_time_next_send;
        }
    }

    if (c->cn_closed)
    {
        int32_t delta;

        delta = (int32_t)(c->cn_delete_time - uevent_time->time);
        if (uevent_time->infinite || delta < 0)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x40u, "CONNECTION_EVENT_DELETE: overriding other events with delete event (%d) { %u, %u }\n", delta, uevent_time->infinite, uevent_time->time);
#endif
            c->cn_event_type = CONNECTION_EVENT_DELETE;
            uevent_time->infinite = 0;
            uevent_time->time = c->cn_delete_time;
        }
    }
}

#ifndef RDPLIB_SOURCE_FAITHFUL
static uint32_t rdplib_connection_abort_short_datagram(connection_t *c)
{
    ++g_rdp_stat->discarded_too_short;
    ++c->stat.discarded_too_short;
    if (c->tx_connected)
    {
        tx_abort_connection(c, RDP_DISCONNECT_REASON_PROTOCOL_ERROR);
    }
    return RDP_RX_ABORT;
}

static uint32_t rdplib_connection_abort_invalid_fragment(connection_t *c)
{
    ++g_rdp_stat->discarded_bad_fragment;
    ++c->stat.discarded_bad_fragment;
    if (c->tx_connected)
    {
        tx_abort_connection(c, RDP_DISCONNECT_REASON_PROTOCOL_ERROR);
    }
    return RDP_RX_ABORT;
}

static uint32_t rdplib_connection_preflight_arrival(connection_t *c, const uint8_t *packet, uint16_t size, rdp_header_t *header)
{
    uint32_t required_size;
    uint32_t fragment_offset = 0;
    uint16_t options;
    uint16_t ack_options;

    if (!c || !packet || !header)
    {
        return RDP_RX_ABORT;
    }
    if (size < sizeof(uint16_t))
    {
        return rdplib_connection_abort_short_datagram(c);
    }

    options = rdplib_load_network_u16(packet);
    if ((((options & (RDP_FLAG_SYN | RDP_FLAG_FIN | RDP_FLAG_FRAGMENT)) == 0) || (options & RDP_FLAG_MSGID) != 0) && (options & RDP_FLAG_MULTI) == 0)
    {
        required_size = 4;
        ack_options = options & (RDP_FLAG_ACKTHRU | RDP_FLAG_MASKOFFSET);
        if (ack_options == RDP_FLAG_ACKTHRU || ack_options == RDP_FLAG_MASKOFFSET)
        {
            required_size += 2u + ((options & RDP_FLAG_ACK_MASK_LENGTH) >> 4);
        }
        if (options & RDP_FLAG_MSGID)
        {
            required_size += 2;
        }
        if (options & RDP_FLAG_FRAGMENT)
        {
            fragment_offset = required_size;
            required_size += 6;
        }
        if (options & RDP_FLAG_SEQUENCED)
        {
            ++required_size;
            if (options & RDP_FLAG_MSGID)
            {
                ++required_size;
            }
        }

        if (required_size > size)
        {
            return rdplib_connection_abort_short_datagram(c);
        }
        if (fragment_offset)
        {
            uint32_t data_size = size - required_size;

            if (rdplib_load_network_u16(packet + fragment_offset + 4u) > RDP_FRAGMENT_COUNT_MAX || data_size < 1u ||
                data_size > RDP_FRAGMENT_PAYLOAD_BYTES)
            {
                return rdplib_connection_abort_invalid_fragment(c);
            }
        }
    }
    return RDP_RX_ACCEPT;
}
#endif

uint32_t connection_parse_and_validate_arrival(connection_t *c, uint16_t *packet, uint16_t size, rdp_header_t *header)
{
    uint16_t *src;
    uint32_t acksize;
    uint32_t validation;

#ifndef RDPLIB_SOURCE_FAITHFUL
    validation = rdplib_connection_preflight_arrival(c, (const uint8_t *)packet, size, header);
    if (validation != RDP_RX_ACCEPT)
    {
        return validation;
    }
#endif

    validation = RDP_RX_ACCEPT;
#ifdef RDPLIB_DEBUG
    assert(umutex_owner( &c->cn_lock ));
#endif
    src = packet;
#ifdef RDPLIB_SOURCE_FAITHFUL
    header->options = ntohs(*src++);
#else
    header->options = rdplib_load_network_u16((const uint8_t *)src);
    ++src;
#endif
    if (((header->options & (RDP_FLAG_SYN | RDP_FLAG_FIN)) && !(header->options & RDP_FLAG_MSGID)) ||
        ((header->options & RDP_FLAG_FRAGMENT) && !(header->options & RDP_FLAG_MSGID)) || (header->options & RDP_FLAG_MULTI))
    {
#ifdef RDPLIB_SOURCE_FAITHFUL
        char addr[64];
#endif

        validation = RDP_RX_ABORT;
#ifdef RDPLIB_SOURCE_FAITHFUL
        format_sockaddr(addr, &c->tx_remote_addr);
#endif
        ++g_rdp_stat->discarded_bad_options;
        ++c->stat.discarded_bad_options;
    }
    else
    {
#ifdef RDPLIB_SOURCE_FAITHFUL
        header->seqnum = ntohs(*src++);
#else
        header->seqnum = rdplib_load_network_u16((const uint8_t *)src);
        ++src;
#endif
        validation = rx_validate_seqnum_arrival(c, header->seqnum);
        if (validation == RDP_RX_ACCEPT)
        {
            header->ack = src;
            validation = tx_validate_ack_arrival(c, header, &acksize);
            if (validation == RDP_RX_ACCEPT)
            {
                src = (uint16_t *)((uint8_t *)src + acksize);
                if (header->options & RDP_FLAG_MSGID)
                {
                    memcpy(&header->msgid, src++, sizeof(header->msgid));
                    header->msgid = ntohs(header->msgid);
                    validation = rx_validate_msgid_arrival(c, header);
                }

                if (validation == RDP_RX_ACCEPT)
                {
                    header->fragid = 0;
                    header->frag_number = 0;
                    header->frag_total = 1;
                    if (header->options & RDP_FLAG_FRAGMENT)
                    {
                        memcpy(&header->fragid, src++, sizeof(header->fragid));
                        header->fragid = ntohs(header->fragid);
                        memcpy(&header->frag_number, src++, sizeof(header->frag_number));
                        header->frag_number = ntohs(header->frag_number);
                        memcpy(&header->frag_total, src++, sizeof(header->frag_total));
                        header->frag_total = ntohs(header->frag_total);
                    }

                    if (header->options & RDP_FLAG_SEQUENCED)
                    {
                        uint8_t *small_src;

                        small_src = (uint8_t *)src;
                        header->stream = *small_src++;
                        if (header->options & RDP_FLAG_MSGID)
                        {
                            header->stream_seqnum = *small_src++;
                        }
                        src = (uint16_t *)small_src;
                        validation = rx_validate_stream_arrival(c, header);
                    }

                    if (validation == RDP_RX_ACCEPT)
                    {
                        header->header_size = (uint16_t)((uint8_t *)src - (uint8_t *)packet);
                        if (size >= header->header_size)
                        {
                            header->data_size = size - header->header_size;
                            if (header->options & RDP_FLAG_FRAGMENT)
                            {
                                validation = rx_validate_fragment_arrival(c, header);
                            }
                        }
                        else
                        {
#ifdef RDPLIB_SOURCE_FAITHFUL
                            char addr[64];
#endif

                            validation = RDP_RX_ABORT;
#ifdef RDPLIB_SOURCE_FAITHFUL
                            format_sockaddr(addr, &c->tx_remote_addr);
                            discard_log_append("%s received short packet %u < %u\n", addr, size, header->header_size);
#endif
                            ++g_rdp_stat->discarded_too_short;
                            ++c->stat.discarded_too_short;
                        }
                    }
                }
            }
        }
    }

    if (validation == RDP_RX_ABORT && c->tx_connected)
    {
        tx_abort_connection(c, RDP_DISCONNECT_REASON_PROTOCOL_ERROR);
    }
    return validation;
}

void connection_record_arrival(connection_t *c, rdp_header_t *header, uint32_t *duplicate)
{
    *duplicate = 0;
#ifdef RDPLIB_DEBUG
    assert(umutex_owner( &c->cn_lock ));
#endif
    rx_record_packet_arrival(c);

    if ((header->options & RDP_FLAG_RESET) && c->tx_connected)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x4000u, "[0x%08x] received reset bit\n", (uint32_t)(uintptr_t)c);
#endif
        tx_abort_connection(c, RDP_DISCONNECT_REASON_PEER_RESET);
    }
    else
    {
        if ((header->options & RDP_FLAG_STOP) && !c->tx_stopped)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x4000u, "[0x%08x] received stop bit\n", (uint32_t)(uintptr_t)c);
#endif
            tx_received_stopped(c);
        }

        rx_record_seqnum_arrival(c, header);
        tx_record_ack_arrival(c, header);
        if (header->options & RDP_FLAG_MSGID)
        {
            *duplicate = rx_record_msgid_arrival(c, header->msgid);
            if (*duplicate)
            {
#ifdef RDPLIB_DEBUG
                dpf(0x10u, "duplicate data received (%u)\n", header->msgid);
#endif
                ++g_rdp_stat->duplicate_packets_rx;
                g_rdp_stat->duplicate_bytes_rx += header->data_size;
                ++c->stat.duplicate_packets_rx;
                c->stat.duplicate_bytes_rx += header->data_size;
            }
            else
            {
                ++g_rdp_stat->guaranteed_packets_rx;
                g_rdp_stat->guaranteed_bytes_rx += header->data_size;
                ++c->stat.guaranteed_packets_rx;
                c->stat.guaranteed_bytes_rx += header->data_size;
            }
        }
        else if (header->data_size)
        {
            ++g_rdp_stat->best_effort_packets_rx;
            g_rdp_stat->best_effort_bytes_rx += header->data_size;
            ++c->stat.best_effort_packets_rx;
            c->stat.best_effort_bytes_rx += header->data_size;
        }

        g_rdp_stat->header_bytes_rx += header->header_size;
        c->stat.header_bytes_rx += header->header_size;
        if (header->options & RDP_FLAG_MSGID)
        {
            if (header->options & RDP_FLAG_FIN)
            {
#ifdef RDPLIB_DEBUG
                dpf(0x4000u, "setting delayed ack for fin\n");
#endif
            }
            tx_set_delayed_ack(c);
        }
    }
}

void connection_handle_icmp(connection_t *c, uint8_t type, uint8_t code, uint8_t trace_reply, uint8_t probe_index, struct sockaddr_in *from)
{
    uint32_t current_time;
    uint32_t ignored;

#ifndef RDPLIB_SOURCE_FAITHFUL
    // Preserve the original disconnect details; trace replies are separate.
    if (!trace_reply && !c->tx_connected)
    {
        return;
    }
#endif

    ignored = 0;
    current_time = time_get_ms();
    if (trace_reply)
    {
        if (probe_index < c->trace_next_index && c->trace_probes)
        {
            trace_probe_t *tp;

            tp = &c->trace_probes[probe_index];
            if (type == 3 && code == 3 && c->trace_max_ttl > tp->ttl)
            {
                c->trace_max_ttl = tp->ttl;
                if (c->trace_next_ttl > c->trace_max_ttl)
                {
                    ++c->trace_pass;
                    c->trace_next_ttl = 1;
                }
#ifdef RDPLIB_DEBUG
                dpf(0x10000000u, "TRACE: setting max ttl: %u\n", c->trace_max_ttl);
#endif
            }

            tp->reply_time = current_time - tp->time_sent;
#ifdef RDPLIB_SOURCE_FAITHFUL
            tp->icmp_from = from->sin_addr;
#else
            memcpy(&tp->icmp_from, (const uint8_t *)from + offsetof(struct sockaddr_in, sin_addr), sizeof(tp->icmp_from));
#endif
            tp->icmp_type = type;
            tp->icmp_code = code;
            c->trace_en_route = 0;
            c->trace_time = current_time;
        }
    }
    else
    {
        switch (type)
        {
        case 3:
            if (code < 16)
            {
                ++g_rdp_stat->icmp_unreachable[code];
                ++c->stat.icmp_unreachable[code];
            }
            else
            {
                ++g_rdp_stat->icmp_unknown;
                ++c->stat.icmp_unknown;
            }
            break;

        case 4:
            ++g_rdp_stat->icmp_source_quench;
            ++c->stat.icmp_source_quench;
            break;

        case 11:
            if (code < 2)
            {
                ++g_rdp_stat->icmp_ttl_expired[code];
                ++c->stat.icmp_ttl_expired[code];
            }
            else
            {
                ++g_rdp_stat->icmp_unknown;
                ++c->stat.icmp_unknown;
            }
            break;

        case 12:
            if (code < 2)
            {
                ++g_rdp_stat->icmp_parameter_problem[code];
                ++c->stat.icmp_parameter_problem[code];
            }
            else
            {
                ++g_rdp_stat->icmp_unknown;
                ++c->stat.icmp_unknown;
            }
            break;

        default:
            ignored = 1;
            break;
        }

        if (!ignored)
        {
            ++c->rx_icmp_received;
            c->rx_icmp_type = type;
            c->rx_icmp_code = code;
            c->rx_icmp_time = time_get_ms();
#ifdef RDPLIB_SOURCE_FAITHFUL
            c->rx_icmp_from.s_addr = from->sin_addr.s_addr;
#else
            memcpy(&c->rx_icmp_from.s_addr, (const uint8_t *)from + 4, sizeof(c->rx_icmp_from.s_addr));
#endif
            if (type == 3 && code == 3)
            {
                tx_abort_connection(c, RDP_DISCONNECT_REASON_ICMP);
            }
        }
    }
}

void connection_event_process(connection_t *c, uint32_t max_time, timeout_data *next_time)
{
#ifdef RDPLIB_DEBUG
    assert(umutex_owner( &c->cn_lock ));
#endif
    for (;;)
    {
        switch (c->cn_event_type)
        {
        case CONNECTION_EVENT_DELETE:
#ifdef RDPLIB_DEBUG
            dpf(0x2000u, "[0x%08x] processing CONNECTION_EVENT_DELETE\n", (uint32_t)(uintptr_t)c);
#endif
            return;

        case CONNECTION_EVENT_TX:
            tx_tx(c);
            break;

        case CONNECTION_EVENT_ALIVE:
            tx_send_alive(c);
            break;

        case CONNECTION_EVENT_TRACE:
            trace_send(c);
            break;

        default:
#ifdef RDPLIB_DEBUG
            assert(NULL != "Unknown connection event\n");
#endif
            break;
        }

        connection_recalc_event_timeout(c, next_time);
        if (next_time->infinite || (int32_t)(max_time - next_time->time) < 0)
        {
            return;
        }
    }
}

uint32_t connection_close(connection_t *c, uint32_t linger_time, uint32_t *all_acked, uevent_t *all_acked_event)
{
    rdp_t *rdp;
    uint32_t wait_to_signal;
    uint32_t messages_in_queue;

    if (!c)
    {
        return RDP_CONNECTION_SEND_INVALID_ARGUMENT;
    }

    rdp_lock_connection(c);
    rdp = c->cn_rdp;

    umutex_lock(&rdp->message_rxq_mutex);
    messages_in_queue = rxq_peek_head(&rdp->message_rxq) != NULL;
    rxq_flush_all_messages(&rdp->message_rxq, c);
    if (messages_in_queue && !rxq_peek_head(&rdp->message_rxq))
    {
        usemaphore_decrement(&rdp->receive_semaphore, 0);
    }
    umutex_unlock(&rdp->message_rxq_mutex);

    umutex_lock(&rdp->external_rxq_mutex);
    rxq_flush_all_messages(&rdp->external_rxq, c);
    umutex_unlock(&rdp->external_rxq_mutex);

    wait_to_signal = 1;
    if ((c->tx_connected && c->tx_stopped) || (c->tx_fin_acked && c->tx_fin_msgid == c->tx_acked_thru))
    {
        if (all_acked)
        {
            if (c->tx_fin_acked && c->tx_fin_msgid == c->tx_acked_thru)
            {
#ifdef RDPLIB_DEBUG
                dpf(0x4000u, "all_acked == TRUE (all messages acknowledged) (no block)\n");
#endif
            }
            else if (c->tx_stopped)
            {
#ifdef RDPLIB_DEBUG
                dpf(0x4000u, "all_acked == TRUE (stopped by peer) (no block)\n");
#endif
            }
            *all_acked = 1;
        }
        wait_to_signal = 0;
    }
    else if (!c->tx_connected)
    {
        if (all_acked)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x4000u, "all_acked == FALSE (not connected) (no block)\n");
#endif
            *all_acked = 0;
        }
        wait_to_signal = 0;
    }

    if (linger_time)
    {
        uevent_t *event;
        uint32_t *status;

        event = NULL;
        status = NULL;
        if (wait_to_signal)
        {
            event = all_acked_event;
            status = all_acked;
        }

#ifdef RDPLIB_DEBUG
        dpf(0x2000u, "[0x%08x] connection_close\n", (uint32_t)(uintptr_t)c);
#endif
        rx_flush_input_buffers(c);
        c->cn_closed = 1;
        c->cn_delete_time = linger_time + time_get_ms();
        c->tx_all_acked_event = event;
        c->tx_all_acked = status;
        tx_send_fin(c);
        rdp_resort(c, 1);
    }
    else
    {
        if (wait_to_signal)
        {
            if (all_acked)
            {
#ifdef RDPLIB_DEBUG
                dpf(0x4000u, "all_acked == FALSE (no linger)\n");
#endif
                *all_acked = 0;
            }
            wait_to_signal = 0;
        }
#ifndef RDPLIB_SOURCE_FAITHFUL
        // Prevent a lookup that already holds a temporary reference from resuming protocol work.
        c->cn_abort = 1;
#endif
        rdp_connection_mark_for_delete(rdp, c);
    }

    if (!wait_to_signal && all_acked_event)
    {
        uevent_signal(all_acked_event);
    }
    rdp_unlock(c);
    return RDP_CONNECTION_SEND_OK;
}

uint32_t connection_linger_expired(connection_t *c)
{
    int32_t time_until_delete;

    time_until_delete = (int32_t)(c->cn_delete_time - time_get_ms());
    return c->cn_closed && time_until_delete <= 10;
}

// unused, retained for historical interest
#ifdef RDP_DEAD_CODE
uint32_t connection_keepalive(connection_t *c, uint32_t on)
{
    uint32_t flag_mask;

    flag_mask = RDP_CONNECTION_FEATURE_KEEPALIVE;
    if (!c)
    {
        return RDP_CONNECTION_SEND_INVALID_ARGUMENT;
    }

    rdp_lock_connection(c);
    if (on)
    {
        c->cn_flags |= flag_mask;
    }
    else
    {
        c->cn_flags &= ~flag_mask;
    }
    rdp_resort(c, 1);
    rdp_unlock(c);
    return RDP_CONNECTION_SEND_OK;
}
#endif

void connection_set_send_buffer_size(connection_t *c, uint32_t send_buffer_size)
{
    c->tx_send_buffer_size = send_buffer_size;
}

// unused, retained for historical interest
#ifdef RDP_DEAD_CODE
uint32_t connection_set_timeouts(connection_t *c, uint32_t max_message_age, uint32_t max_service_outage)
{
    uint32_t result;

    result = RDP_CONNECTION_SEND_INVALID_ARGUMENT;
    if (c)
    {
        rdp_lock_connection(c);
        c->tx_max_message_age = max_message_age;
        c->tx_max_service_outage = max_service_outage;
        rdp_unlock(c);
        return RDP_CONNECTION_SEND_OK;
    }
    return result;
}
#endif

uint32_t connection_connected(connection_t *c)
{
    return c->tx_connected;
}

void **connection_app_ptr(connection_t *c)
{
    return c->cn_app_ptr;
}

struct sockaddr *connection_get_remote_addr(connection_t *c)
{
    return &c->tx_remote_addr;
}

// unused, retained for historical interest
#ifdef RDP_DEAD_CODE
uint32_t connection_get_last_rt_time(connection_t *c)
{
    return c->tx_last_rt_time;
}
#endif

void connection_get_perf_stats(connection_t *c, perf_stats_t *stats)
{
    rdp_lock_connection(c);
    stats->time_last_arrival = c->rx_time_last_arrival;
    stats->recent_seqnum_history = c->rx_recent_seqnum_history;
    stats->highest_seqnum_received = c->rx_highest_seqnum_received;
    stats->average_rt_time = c->tx_rt_tracker.weighted_avg;
    stats->std_deviation = c->tx_rt_tracker.std_deviation;
    stats->last_rt_time = c->tx_last_rt_time;
    stats->queue_size = tx_get_queue_size(c);
    stats->stall_time = tx_get_stall_time(c);
    rdp_unlock(c);

    if (c->tx_remote_addr.sa_family == 69)
    {
        stats->stall_time = rdp_get_serial_stall_time(c->cn_rdp);
    }
}

void connection_get_disconnect_info(connection_t *c, disconnect_info_t *info, uint32_t size)
{
    if (size == sizeof(*info))
    {
        info->disconnect_reason = c->tx_disconnect_reason;
        info->icmp_code = c->rx_icmp_code;
        info->icmp_type = c->rx_icmp_type;
        info->icmp_from = c->rx_icmp_from.s_addr;
    }
    else
    {
#ifdef RDPLIB_DEBUG
        assert(!"invalid disconnect_info_t size");
#endif
    }
}
