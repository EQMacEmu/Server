// Copyright (c) 2026 solar
// SPDX-License-Identifier: MIT

#if defined(_MSC_VER) && (defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL))
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include "rdp.h"

#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
#include <stdio.h>
#endif
#include <limits.h>
#include <string.h>

#include "crc.h"
#include "cypher.h"
#ifdef RDPLIB_DEBUG
#include "dpf.h"
#endif
#include "fast.h"
#ifdef RDPLIB_SOURCE_FAITHFUL
#include "log.h"
#endif
#if defined(RDPLIB_DEBUG) || !defined(RDPLIB_SOURCE_FAITHFUL)
#include "protocol_limits.h"
#endif
#ifndef RDPLIB_SOURCE_FAITHFUL
#include "rdplib_wire.h"
#endif
#include "rdplib_rdp.h"
#include "rxq.h"
#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
#include "ustrerror.h"
#endif

#ifdef RDPLIB_DEBUG
#define URESULT_OK 0u
#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE ((void *)(intptr_t)-1)
#endif
#endif

uint32_t disable_blocking(intptr_t s);

enum
{
    RDP_SOCKET_TYPE_DATAGRAM = 2,
    RDP_SOCKET_TYPE_RAW = 3,
    RDP_SOCKET_LEVEL = 0xFFFF,
    RDP_SOCKET_SEND_BUFFER = 0x1001,
    RDP_SOCKET_RECEIVE_BUFFER = 0x1002,
    RDP_SOCKET_BROADCAST = 32,
    RDP_IP_LEVEL = 0,
    RDP_IP_TTL = 7,
    RDP_SOCKET_ERROR_ADDRESS_IN_USE = 10048,
    RDP_SOCKET_ERROR_OPTION_UNSUPPORTED = 10042
};

#ifdef RDPLIB_DEBUG
static uint16_t rdplib_rdp_load_native_u16(const void *bytes);
#endif

uint16_t g_next_local_port = 1024;

uint32_t rdp_wake(rdp_t *rdp, uint32_t msg)
{
    int32_t char_sent;
    struct sockaddr_in loop;

    char_sent = (int32_t)sizeof(msg);
    if (rdp->udp_socket == -1)
    {
        if (rdp->ipx_socket != -1)
        {
            char_sent = rdplib_platform_send_datagram_to(rdp->ipx_socket, (const uint8_t *)&msg, sizeof(msg), &rdp->local_ipx_addr, sizeof(rdp->local_ipx_addr));
        }
    }
    else
    {
        memcpy(&loop, &rdp->local_udp_addr, sizeof(loop));
        loop.sin_addr.s_addr = htonl(UINT32_C(0x7F000001));
        char_sent = rdplib_platform_send_datagram_to(rdp->udp_socket, (const uint8_t *)&msg, sizeof(msg), &loop, sizeof(loop));
    }
    return char_sent != (int32_t)sizeof(msg);
}

void rdp_resort(connection_t *c, uint32_t wakeup_iothread)
{
    rdp_t *rdp;
    struct _timeout_data next_event;

    rdp = c->cn_rdp;
#ifdef RDPLIB_DEBUG
    assert(umutex_owner( &c->cn_lock ));
#endif
    connection_recalc_event_timeout(c, &next_event);
    eventq_lock(&rdp->conn_eventq);
    if (memcmp(&c->cn_event_time, &next_event, sizeof(next_event)) != 0)
    {
        c->cn_event_time = next_event;
        eventq_resort_by_ptr(&rdp->conn_eventq, c);
        if (c == eventq_peek_head(&rdp->conn_eventq) && !rdp->wake_sent && wakeup_iothread)
        {
            (void)rdp_wake(rdp, 0);
            rdp->wake_sent = 1;
        }
    }
    eventq_unlock(&rdp->conn_eventq);
}

void rdp_unlock(connection_t *c)
{
    rdp_t *rdp;

    rdp = c->cn_rdp;
    c = connhash_unlock(&rdp->addr_map, c);
    if (c)
    {
        eventq_lock(&rdp->conn_eventq);
        (void)eventq_remove_by_ptr(&rdp->conn_eventq, c);
        eventq_unlock(&rdp->conn_eventq);
#ifdef RDPLIB_DEBUG
        dpf(0x2000u, "[0x%08x] deleted (unlock)\n", (uint32_t)(uintptr_t)c);
#endif
        connection_destroy(c);
        rdplib_platform_free(c);
    }
}

#ifdef RDP_DEAD_CODE
// unused, retained for historical interest
void rdp_set_socket_rcvbuf(rdp_t *rdp, int size)
{
    int result;

    result = rdplib_platform_socket_set_option(rdp->udp_socket, RDP_SOCKET_LEVEL, RDP_SOCKET_RECEIVE_BUFFER, &size, sizeof(size));
#ifdef RDPLIB_DEBUG
    assert(result != -1);
#endif
}

void rdp_set_socket_sndbuf(rdp_t *rdp, int size)
{
    int result;

    result = rdplib_platform_socket_set_option(rdp->udp_socket, RDP_SOCKET_LEVEL, RDP_SOCKET_SEND_BUFFER, &size, sizeof(size));
#ifdef RDPLIB_DEBUG
    assert(result != -1);
#endif
}

int rdp_get_socket_sndbuf(rdp_t *rdp)
{
    uint32_t optlen;
    int size;
    int result;

    optlen = sizeof(size);
    result = rdplib_platform_socket_get_option(rdp->udp_socket, RDP_SOCKET_LEVEL, RDP_SOCKET_SEND_BUFFER, &size, &optlen);
#ifdef RDPLIB_DEBUG
    assert(result != -1);
#endif
    return size;
}
#endif

static int rdplib_rdp_set_socket_buffer_size(rdp_t *rdp, int option, uint32_t bytes)
{
    int value;

    if (bytes > (uint32_t)INT_MAX)
    {
        return -1;
    }
    value = (int)bytes;
    return rdplib_platform_socket_set_option(rdp->udp_socket, RDP_SOCKET_LEVEL, option, &value, sizeof(value));
}

static int rdplib_rdp_get_socket_buffer_size(const rdp_t *rdp, int option, uint32_t *bytes)
{
    uint32_t value_bytes;
    int value;
    int result;

    value_bytes = sizeof(value);
    result = rdplib_platform_socket_get_option(rdp->udp_socket, RDP_SOCKET_LEVEL, option, &value, &value_bytes);
    if (result == 0 && value >= 0)
    {
        *bytes = (uint32_t)value;
    }
    return result == 0 && value >= 0 ? 0 : -1;
}

int rdplib_rdp_set_socket_receive_buffer_size(rdp_t *rdp, uint32_t bytes)
{
    return rdplib_rdp_set_socket_buffer_size(rdp, RDP_SOCKET_RECEIVE_BUFFER, bytes);
}

int rdplib_rdp_set_socket_send_buffer_size(rdp_t *rdp, uint32_t bytes)
{
    return rdplib_rdp_set_socket_buffer_size(rdp, RDP_SOCKET_SEND_BUFFER, bytes);
}

int rdplib_rdp_get_socket_receive_buffer_size(const rdp_t *rdp, uint32_t *bytes)
{
    return rdplib_rdp_get_socket_buffer_size(rdp, RDP_SOCKET_RECEIVE_BUFFER, bytes);
}

int rdplib_rdp_get_socket_send_buffer_size(const rdp_t *rdp, uint32_t *bytes)
{
    return rdplib_rdp_get_socket_buffer_size(rdp, RDP_SOCKET_SEND_BUFFER, bytes);
}

void rdp_enqueue_arrival(rdp_t *rdp, msg_arrival_t *arrival)
{
#ifndef RDPLIB_SOURCE_FAITHFUL
    rdplib_rdp_arrival_ready_callback_t arrival_ready_callback = NULL;
    void *arrival_ready_context = NULL;
#endif
#ifdef RDPLIB_DEBUG
    assert(( arrival->sender == NULL ) || ( arrival->sender->cn_rdp == rdp ));
#endif
    if (arrival->sender && arrival->sender->cn_closed)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x2000u, "[0x%08x] discarded message that arrived after close\n", (uint32_t)(uintptr_t)arrival->sender);
#endif
        fast_free(arrival);
    }
    else
    {
        msg_arrival_t *queue_head;

        msg_arrival_prepare_for_rxq(arrival);
        umutex_lock(&rdp->message_rxq_mutex);
        queue_head = rxq_peek_head(&rdp->message_rxq);
#ifdef RDPLIB_DEBUG
        arrival->enqueue_time = time_get_ms();
#elif defined(RDPLIB_SOURCE_FAITHFUL)
        arrival->enqueue_time = 0;
#endif
        rxq_add_tail(&rdp->message_rxq, arrival);
        if (!queue_head)
        {
            usemaphore_increment(&rdp->receive_semaphore);
#ifndef RDPLIB_SOURCE_FAITHFUL
            arrival_ready_callback = rdp->rdplib_arrival_ready_callback;
            arrival_ready_context = rdp->rdplib_arrival_ready_context;
#endif
        }
        umutex_unlock(&rdp->message_rxq_mutex);
#ifndef RDPLIB_SOURCE_FAITHFUL
        if (arrival_ready_callback)
        {
            arrival_ready_callback(arrival_ready_context);
        }
#endif
    }
}

void rdp_handle_complete_arrival(rdp_t *rdp, connection_t *c, msg_arrival_t *arrival)
{
    uint8_t stream;
    uint32_t in_order;

#ifdef RDPLIB_DEBUG
    assert(rdp == c->cn_rdp);
    assert(c == arrival->sender);
#endif
    if (arrival->options & RDP_FLAG_FIN)
    {
        rx_save_fin_arrival(c, arrival);
    }
    else if (arrival->options & RDP_FLAG_SEQUENCED)
    {
        if (arrival->options & RDP_FLAG_MSGID)
        {
            stream = arrival->stream;
#ifdef RDPLIB_DEBUG
            assert(stream < STREAMS_PER_CONNECTION);
#endif
            rx_sort_into_sequence(c, arrival);
            while ((arrival = rx_get_next_in_sequence(c, stream)) != NULL)
            {
                rdp_enqueue_arrival(rdp, arrival);
            }
        }
        else
        {
            in_order = rx_in_sequence(c, arrival);
            if (in_order)
            {
                rdp_enqueue_arrival(rdp, arrival);
            }
            else
            {
                fast_free(arrival);
            }
        }
    }
    else
    {
        rdp_enqueue_arrival(rdp, arrival);
    }

    if (rx_rcvd_all_msgids(c) && rx_fin_waiting(c))
    {
        arrival = rx_load_fin_arrival(c);
        rdp_enqueue_arrival(rdp, arrival);
    }
}

void rdp_handle_connectionless(rdp_t *rdp, const char *data, uint32_t size, struct sockaddr *remote_addr)
{
    msg_arrival_t *arrival;

    arrival = (msg_arrival_t *)fast_malloc((uint32_t)sizeof(*arrival) + size);
    memset(arrival, 0, sizeof(*arrival));
    msg_arrival_init(arrival, 0);
    arrival->size = size;
    arrival->options = UINT16_C(0xFFFF);
    memcpy(msg_arrival_get_data(arrival), data, size);
    memcpy(&arrival->from, remote_addr, sizeof(arrival->from));
    rdp_enqueue_arrival(rdp, arrival);
}

int32_t rdp_verify_crc(char *buffer, int32_t buffer_size)
{
    uint32_t crc;

    if (buffer_size < 6)
    {
        return 0;
    }
    buffer_size -= 4;
    memcpy(&crc, buffer + buffer_size, sizeof(crc));
    crc = ntohl(crc);
    if (crc != rdp_crc(0, buffer, (uint32_t)buffer_size))
    {
        return 0;
    }
    return buffer_size;
}

int32_t rdp_decode_data(char *scratch, int32_t char_recv)
{
    uint8_t pad_size;

    rdp_decode(scratch, char_recv / 8);
    pad_size = (uint8_t)scratch[char_recv - 1] & UINT8_C(0x0F);
    if (pad_size <= 8u)
    {
        return char_recv - pad_size;
    }
    return 0;
}

uint32_t rdp_handle_data_recv(rdp_t *rdp, char *scratch, int32_t char_recv, struct sockaddr *remote_addr)
{
    uint32_t quit;
    uint32_t duplicate;
    rdp_header_t rdp_header;
    connection_t *c;
    uint16_t options;
    rdp_rx_arrival_disposition_t validation;
    msg_arrival_t *complete;
    uint16_t remote_family;

    quit = 0;
    if (char_recv < 2)
    {
        return quit;
    }

#ifdef RDPLIB_SOURCE_FAITHFUL
    remote_family = (uint16_t)remote_addr->sa_family;
    if ((remote_family == RDP_TRANSMIT_ADDRESS_IPV4 && *(uint16_t *)remote_addr->sa_data == rdp->local_udp_addr.sin_port &&
         *(uint32_t *)&remote_addr->sa_data[2] == htonl(UINT32_C(0x7F000001))) ||
        memcmp(remote_addr, &rdp->local_ipx_addr, sizeof(struct sockaddr)) == 0)
#else
    memcpy(&remote_family, remote_addr, sizeof(remote_family));
    if ((remote_family == RDP_TRANSMIT_ADDRESS_IPV4 && memcmp(remote_addr->sa_data, &rdp->local_udp_addr.sin_port, 2) == 0 &&
         memcmp(&remote_addr->sa_data[2], "\x7f\x00\x00\x01", 4) == 0) ||
        (remote_family == RDP_TRANSMIT_ADDRESS_IPX && memcmp(remote_addr, &rdp->local_ipx_addr, sizeof(rdp->local_ipx_addr)) == 0))
#endif
    {
        if (char_recv == (int32_t)sizeof(quit))
        {
            memcpy(&quit, scratch, sizeof(quit));
        }
        else
        {
            ++g_rdp_stat->discarded_invalid_localhost;
        }
        return quit;
    }

    c = NULL;
    duplicate = 1;
    rdp->bytes_recvd += (uint32_t)char_recv + 28u;
    if (rdp->encrypt)
    {
        if (char_recv != 0 && (char_recv & 7) == 0)
        {
            char_recv = rdp_decode_data(scratch, char_recv);
        }
        else
        {
#ifdef RDPLIB_SOURCE_FAITHFUL
            char addr[64];
            format_sockaddr(addr, remote_addr);
            discard_log_append("%s invalid size (%u)\n", addr, (uint32_t)char_recv);
#endif
            ++g_rdp_stat->discarded_bad_size;
        }
    }

    if ((rdp->crc || rdp->encrypt) && char_recv >= 6)
    {
#ifdef RDPLIB_SOURCE_FAITHFUL
        options = ntohs(*(uint16_t *)scratch);
#else
        options = rdplib_load_network_u16((const uint8_t *)scratch);
#endif
        if (options != UINT16_C(0xFFFF))
        {
            char_recv = rdp_verify_crc(scratch, char_recv);
            if (!char_recv)
            {
#ifdef RDPLIB_SOURCE_FAITHFUL
                char addr[64];
                format_sockaddr(addr, remote_addr);
                discard_log_append("%s illegal message\n", addr);
#endif
                ++g_rdp_stat->discarded_bad_crc;
                return quit;
            }
        }
    }

    if (char_recv < 2)
    {
        return quit;
    }
#ifdef RDPLIB_SOURCE_FAITHFUL
    options = ntohs(*(uint16_t *)scratch);
#else
    options = rdplib_load_network_u16((const uint8_t *)scratch);
#endif
    if (options == UINT16_C(0xFFFF))
    {
        rdp_handle_connectionless(rdp, scratch + 2, (uint32_t)char_recv - 2u, remote_addr);
        return quit;
    }

    c = rdp_lock_addr(rdp, remote_addr);
    if (!c)
    {
        if (options & RDP_FLAG_SYN)
        {
            uint32_t result;

            result = rdp_connection_create_internal(rdp, &c, remote_addr, 0);
#ifdef RDPLIB_DEBUG
            assert(result == URESULT_OK);
            assert(umutex_owner( &c->cn_lock ));
#endif
            (void)result;
            ++g_rdp_stat->connection_accepts;
        }
        else
        {
#ifdef RDPLIB_DEBUG
            dpf(0x4000u, "ignoring packet received without connection or SYN\n");
#endif
            ++g_rdp_stat->packets_without_connection;
        }
    }

    if (!c || c->cn_abort)
    {
        return quit;
    }

#ifndef RDPLIB_SOURCE_FAITHFUL
    if (c->rdplib_packet_drop_callback && c->rdplib_packet_drop_callback(c->rdplib_packet_drop_context, RDPLIB_PACKET_DROP_INBOUND, (const uint8_t *)scratch, (uint32_t)char_recv))
    {
        rdp_unlock(c);
        return quit;
    }
#endif

#ifdef RDPLIB_DEBUG
    assert(umutex_owner( &c->cn_lock ));
#endif
    validation = (rdp_rx_arrival_disposition_t)connection_parse_and_validate_arrival(c, (uint16_t *)(void *)scratch, (uint16_t)char_recv, &rdp_header);
#ifdef RDPLIB_SOURCE_FAITHFUL
    (void)time_get_ms();
#endif
#ifdef RDPLIB_DEBUG
    dpf(0x40000u, "recvfrom TIME: %4u SEQNUM: %4u SIZE: %4u\n", time_get_ms(), rdp_header.seqnum, (uint32_t)char_recv);
#endif
    if (validation != RDP_RX_ACCEPT)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x4000u, "ignoring packet that failed validation\n");
#endif
#ifdef RDPLIB_SOURCE_FAITHFUL
        if (rdp_header.options & RDP_FLAG_SYN)
#else
        if (options & RDP_FLAG_SYN)
#endif
        {
            ++g_rdp_stat->bad_connection_attempts;
        }
    }
    else
    {
        connection_record_arrival(c, &rdp_header, &duplicate);
        if (duplicate)
        {
            rdp->duplicate_bytes_recvd += (uint32_t)char_recv + 28u;
        }
    }

    if (tx_needs_disconnect_msg(c))
    {
        rdp_enqueue_disconnect_msg(rdp, c);
    }
    rdp_resort(c, 0);
    if (c->tx_connected)
    {
        if (c->cn_closed)
        {
            ++g_rdp_stat->packets_received_after_close;
        }
#ifdef RDPLIB_SOURCE_FAITHFUL
        else if (!(rdp_header.options & RDP_FLAG_SYSTEM) && !duplicate && validation == RDP_RX_ACCEPT &&
                 (rdp_header.data_size || (rdp_header.options & RDP_FLAG_FIN)))
#else
        else if (validation == RDP_RX_ACCEPT && !(rdp_header.options & RDP_FLAG_SYSTEM) && !duplicate &&
                 (rdp_header.data_size || (rdp_header.options & RDP_FLAG_FIN)))
#endif
        {
            complete = rx_assemble(c, &rdp_header, scratch + rdp_header.header_size);
            if (complete)
            {
                rdp_handle_complete_arrival(rdp, c, complete);
            }
        }
    }
    else
    {
        ++g_rdp_stat->packets_received_after_disconnect;
    }
    rdp_unlock(c);
    return quit;
}

void rdp_handle_icmp_recv(rdp_t *rdp, char *scratch, int32_t char_recv, struct sockaddr_in *remote_addr)
{
#ifdef RDPLIB_DEBUG
    char src_addr[20];
    uint32_t dpf_mask;
#endif
    uint16_t rejected_ip_header_len;
    uint8_t index;
#ifdef RDPLIB_DEBUG
    char gateway[20];
#endif
    uint8_t *icmp_header;
    uint16_t ip_header_len;
    uint8_t *rejected_ip_header;
    uint8_t *rejected_udp_header;
#ifdef RDPLIB_DEBUG
    char dst_addr[20];
#endif
    uint8_t *ip_header;
    connection_t *c;
    struct sockaddr_in sin;
    uint32_t sum;
    uint16_t remote_port;
    uint16_t network_word;
    uint8_t trace_response;

    c = NULL;
    if (char_recv == -1)
    {
        return;
    }
    rdp->bytes_recvd += (uint32_t)char_recv + 28u;
    if (char_recv < 28)
    {
        return;
    }

    ip_header = (uint8_t *)scratch;
    ip_header_len = (uint16_t)(4u * (ip_header[0] & 0x0Fu));
    if ((uint32_t)char_recv < (uint32_t)ip_header_len + 8u)
    {
        return;
    }
    icmp_header = ip_header + ip_header_len;
    if ((icmp_header[0] != 3 && icmp_header[0] != 4 && icmp_header[0] != 11) || (uint32_t)char_recv < (uint32_t)ip_header_len + 28u)
    {
        return;
    }

    rejected_ip_header = icmp_header + 8;
    rejected_ip_header_len = (uint16_t)(4u * (rejected_ip_header[0] & 0x0Fu));
    if ((uint32_t)char_recv < (uint32_t)ip_header_len + rejected_ip_header_len + 16u || rejected_ip_header[9] != 17)
    {
        return;
    }
    rejected_udp_header = rejected_ip_header + rejected_ip_header_len;
    if (memcmp(rejected_udp_header, &rdp->local_udp_addr.sin_port, 2) != 0 && memcmp(rejected_udp_header, &rdp->trace_local_addr.sin_port, 2) != 0)
    {
        return;
    }

#ifdef RDPLIB_DEBUG
    dpf_mask = UINT32_C(0x800000);
#endif
    trace_response = (uint8_t)(memcmp(rejected_udp_header, &rdp->trace_local_addr.sin_port, 2) == 0);
    if (trace_response)
    {
#ifdef RDPLIB_DEBUG
        dpf_mask |= UINT32_C(0x10000000);
#endif
    }

#ifdef RDPLIB_DEBUG
    sprintf(gateway, "%u.%u.%u.%u", (uint32_t)((const uint8_t *)&remote_addr->sin_addr)[0], (uint32_t)((const uint8_t *)&remote_addr->sin_addr)[1],
            (uint32_t)((const uint8_t *)&remote_addr->sin_addr)[2], (uint32_t)((const uint8_t *)&remote_addr->sin_addr)[3]);
    sprintf(src_addr, "%u.%u.%u.%u", (uint32_t)rejected_ip_header[12], (uint32_t)rejected_ip_header[13], (uint32_t)rejected_ip_header[14], (uint32_t)rejected_ip_header[15]);
    sprintf(dst_addr, "%u.%u.%u.%u", (uint32_t)rejected_ip_header[16], (uint32_t)rejected_ip_header[17], (uint32_t)rejected_ip_header[18], (uint32_t)rejected_ip_header[19]);
    dpf(dpf_mask, "ICMP from: %s for UDP: %s:%u->%s:%u ", gateway, src_addr, ntohs(rdplib_rdp_load_native_u16(rejected_udp_header)), dst_addr,
        ntohs(rdplib_rdp_load_native_u16(rejected_udp_header + 2)));
#endif

    memcpy(&network_word, rejected_udp_header + 2, sizeof(network_word));
    remote_port = ntohs(network_word);
    index = 0;
    if (trace_response)
    {
        sum = 0;
        memcpy(&network_word, rejected_ip_header + 12, 2);
        sum += network_word;
        memcpy(&network_word, rejected_ip_header + 14, 2);
        sum += network_word;
        memcpy(&network_word, rejected_ip_header + 16, 2);
        sum += network_word;
        memcpy(&network_word, rejected_ip_header + 18, 2);
        sum += network_word;
        sum += rejected_ip_header[9];
        memcpy(&network_word, rejected_udp_header + 4, 2);
        sum += network_word;
        memcpy(&network_word, rejected_udp_header, 2);
        sum += network_word;
        memcpy(&network_word, rejected_udp_header + 2, 2);
        sum += network_word;
        memcpy(&network_word, rejected_udp_header + 4, 2);
        sum += network_word;
        memcpy(&network_word, rejected_udp_header + 6, 2);
        sum += network_word;
        sum = (sum & UINT32_C(0xFFFF)) + (sum >> 16);
        sum += sum >> 16;
        sum = (sum & UINT32_C(0xFF)) + (sum >> 8);
        sum += sum >> 8;
        index = (uint8_t)~sum;
        remote_port &= UINT16_C(0x7FFF);
#ifdef RDPLIB_DEBUG
        dpf(dpf_mask, "probe index: %u ", index);
#endif
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = RDP_TRANSMIT_ADDRESS_IPV4;
    sin.sin_port = htons(remote_port);
    memcpy(&sin.sin_addr, rejected_ip_header + 16, sizeof(sin.sin_addr));
    c = rdp_lock_addr(rdp, (struct sockaddr *)&sin);
#ifdef RDPLIB_DEBUG
    dpf(dpf_mask, "(connection_t: 0x%08x) ", (uint32_t)(uintptr_t)c);

    if (icmp_header[0] == 3)
    {
        switch (icmp_header[1])
        {
        case 0: dpf(dpf_mask, "net unreachable\n"); break;
        case 1: dpf(dpf_mask, "host unreachable\n"); break;
        case 2: dpf(dpf_mask, "protocol unreachable\n"); break;
        case 3: dpf(dpf_mask, "port unreachable\n"); break;
        case 4: dpf(dpf_mask, "needs fragmentation\n"); break;
        case 5: dpf(dpf_mask, "source route failed\n"); break;
        case 6: dpf(dpf_mask, "net unknown\n"); break;
        case 7: dpf(dpf_mask, "host unknown\n"); break;
        case 8: dpf(dpf_mask, "isolated\n"); break;
        default: dpf(dpf_mask, "unreachable CODE: %u\n", icmp_header[1]); break;
        }
    }
    else if (icmp_header[0] == 4)
    {
        dpf(dpf_mask | UINT32_C(0x40000000), "source quench\n");
    }
    else if (icmp_header[1])
    {
        dpf(dpf_mask, "time exceeded CODE: %u\n", icmp_header[1]);
    }
    else
    {
        dpf(dpf_mask, "time exceeded in transit\n");
    }
#endif

    if (c)
    {
#ifdef RDPLIB_DEBUG
        assert(umutex_owner( &c->cn_lock ));
#endif
        connection_handle_icmp(c, icmp_header[0], icmp_header[1], trace_response, index, remote_addr);
        if (tx_needs_disconnect_msg(c))
        {
            rdp_enqueue_disconnect_msg(rdp, c);
        }
        rdp_resort(c, 0);
        rdp_unlock(c);
    }
}

void rdp_serial_drain(rdp_t *rdp)
{
    int32_t char_recv;
    struct sockaddr remote_addr;
    char scratch[536];

    do
    {
        char_recv = serial_recv_from(&rdp->serial, scratch, sizeof(scratch), (sockaddr_com *)&remote_addr);
        (void)rdp_handle_data_recv(rdp, scratch, char_recv, &remote_addr);
    } while (char_recv != -1);
    serial_set_time_next_recv(&rdp->serial, time_get_ms() + 100u);
}

#ifdef RDP_DEAD_CODE
// unused, retained for historical interest
uint32_t rdp_attach(rdp_t *rdp, int32_t file)
{
    uint32_t ok;
    uint32_t result;

    result = 0;
#ifdef RDPLIB_DEBUG
    assert(rdp->serial.file == INVALID_HANDLE_VALUE);
#endif
    if ((intptr_t)rdp->serial.file != -1)
    {
        return 9;
    }
#ifdef _WIN32
    {
        COMMTIMEOUTS ctmo;

        memset(&ctmo, 0, sizeof(ctmo));
        ctmo.ReadIntervalTimeout = UINT32_MAX;
        ok = SetCommTimeouts((HANDLE)(intptr_t)file, &ctmo) != 0;
    }
#else
    ok = 1;
#endif
    if (!ok)
    {
        return 1;
    }
    rdp->serial.file = (void *)(intptr_t)file;
    (void)rdp_wake(rdp, 0);
    rdp->wake_sent = 1;
    return result;
}
#endif

void rdp_destroy_internal(rdp_t *rdp)
{
    msg_arrival_t *msg;
    connection_t *c;

    while ((c = eventq_remove_head(&rdp->conn_eventq)) != NULL)
    {
        c = connhash_subref(&rdp->addr_map, c);
#ifdef RDPLIB_DEBUG
        dpf(0x20u, "rdp_shutdown: connection leaked %#08x\n", (uint32_t)(uintptr_t)c);
#endif
        connection_destroy(c);
        rdplib_platform_free(c);
    }
    while ((msg = rxq_remove_head(&rdp->message_rxq)) != NULL)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x20u, "rdp_shutdown: message leaked %#08x\n", (uint32_t)(uintptr_t)msg);
#endif
        fast_free(msg);
    }
    while ((msg = rxq_remove_head(&rdp->external_rxq)) != NULL)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x20u, "rdp_shutdown: message leaked %#08x\n", (uint32_t)(uintptr_t)msg);
#endif
        fast_free(msg);
    }

    if (rdp->udp_socket != -1)
    {
        rdplib_platform_socket_close(rdp->udp_socket);
    }
    rdp->udp_socket = -1;
    if (rdp->icmp_socket != -1)
    {
        rdplib_platform_socket_close(rdp->icmp_socket);
    }
    rdp->icmp_socket = -1;
    if (rdp->trace_socket != -1)
    {
        rdplib_platform_socket_close(rdp->trace_socket);
    }
    rdp->trace_socket = -1;
    if (rdp->ipx_socket != -1)
    {
        rdplib_platform_socket_close(rdp->ipx_socket);
    }
    rdp->ipx_socket = -1;
    if (rdp->startup)
    {
        rdplib_platform_network_cleanup();
        rdp->startup = 0;
    }

    serial_destroy(&rdp->serial);
    connhash_destroy(&rdp->addr_map);
    eventq_destroy(&rdp->conn_eventq);
    usemaphore_destroy(&rdp->receive_semaphore);
    rxq_destroy(&rdp->message_rxq);
    umutex_destroy(&rdp->message_rxq_mutex);
    rxq_destroy(&rdp->external_rxq);
    umutex_destroy(&rdp->external_rxq_mutex);
    if (!rdp->app_is_waiting_for_exit)
    {
        uthread_destroy(&rdp->io_thread);
        rdplib_platform_free(rdp);
    }
}

#if defined(__linux__) && !defined(RDPLIB_SOURCE_FAITHFUL)
void rdplib_rdp_drain_icmp_errors(rdp_t *rdp)
{
    enum { RDP_ICMP_ERROR_BATCH_SIZE = 32 };
    rdplib_platform_icmp_error_t errors[RDP_ICMP_ERROR_BATCH_SIZE];
    uint32_t count;
    uint32_t i;

    do
    {
        // Clear a burst before dispatch can wait on a peer's connection lock.
        for (count = 0; count < RDP_ICMP_ERROR_BATCH_SIZE; ++count)
        {
            if (!rdplib_platform_receive_icmp_error(rdp->udp_socket, &errors[count]))
            {
                break;
            }
        }
        for (i = 0; i < count; ++i)
        {
            rdplib_platform_icmp_error_t *error = &errors[i];

            if (error->type == 3 || error->type == 4 || error->type == 11)
            {
                struct sockaddr_in remote_addr;
                struct sockaddr_in source_addr;

                // Error records are byte-aligned; dispatch needs aligned addresses.
                memcpy(&remote_addr, error->remote_address, sizeof(remote_addr));
                memcpy(&source_addr, error->source_address, sizeof(source_addr));
                rdplib_rdp_handle_reported_icmp(rdp, (struct sockaddr *)&remote_addr, error->type, error->code, 0, 0, &source_addr);
            }
        }
    } while (count == RDP_ICMP_ERROR_BATCH_SIZE);
}
#endif

void rdp_io_thread(void *data)
{
    rdp_t *rdp;
    struct timeval *tv;
    struct timeval timeout_data;
    uint32_t max_time;
#ifdef RDPLIB_DEBUG
    uint32_t loop_time;
#endif
    uint32_t current_time;
    uint32_t timeout;
    uint32_t elapsed_time;
    uint32_t input_rate;
    uint32_t duplicate_input_rate;
    connection_t *c;
    int32_t ready;
    int32_t max_timeout;
    uint32_t event_processing_time;
    int32_t char_recv;
    struct sockaddr remote_addr;
#ifdef RDPLIB_SOURCE_FAITHFUL
    char scratch[536];
#else
    char scratch[RDP_MAX_CONNECTED_DATAGRAM_BYTES];
#endif
    uint32_t enabled_sources;
    uint32_t ready_sources;
    struct _timeout_data next_event;
#if defined(_WIN32) && !defined(RDPLIB_SOURCE_FAITHFUL)
    struct sockaddr_in unknown_icmp_source;
#endif

    rdp = (rdp_t *)data;
    tv = NULL;
    max_time = time_get_ms();
#ifdef RDPLIB_DEBUG
    loop_time = time_get_ms();
    (void)loop_time;
#elif defined(RDPLIB_SOURCE_FAITHFUL)
    (void)time_get_ms();
#endif
#ifdef RDPLIB_DEBUG
    dpf(0x20u, "io_thread running\n");
#endif

    while (rdp->io_thread_running)
    {
        enabled_sources = RDPLIB_PLATFORM_IO_SOURCE_NONE;
        if (rdp->udp_socket != -1)
        {
            enabled_sources |= RDPLIB_PLATFORM_IO_SOURCE_IPV4;
        }
        if (rdp->ipx_socket != -1)
        {
            enabled_sources |= RDPLIB_PLATFORM_IO_SOURCE_IPX;
        }
        if (rdp->icmp_socket != -1)
        {
            enabled_sources |= RDPLIB_PLATFORM_IO_SOURCE_ICMP;
        }
        ready_sources = enabled_sources;

        eventq_lock(&rdp->conn_eventq);
        tv = eventq_get_event_timeout(&rdp->conn_eventq, &timeout_data);
        if (tv)
        {
            timeout = 1000u * (uint32_t)tv->tv_sec + (uint32_t)tv->tv_usec / 1000u;
        }
        else
        {
            timeout = UINT32_MAX;
        }
        rdp->wake_sent = 0;
        eventq_unlock(&rdp->conn_eventq);

        if ((intptr_t)rdp->serial.file != -1)
        {
            max_timeout = (int32_t)(serial_get_time_next_recv(&rdp->serial) - time_get_ms());
            max_timeout = max_timeout <= 0 ? 0 : 1000 * max_timeout;
            if (!tv || tv->tv_usec > max_timeout || tv->tv_sec)
            {
                tv = &timeout_data;
                timeout_data.tv_sec = 0;
                timeout_data.tv_usec = max_timeout;
                timeout = (uint32_t)max_timeout / 1000u;
            }
        }

        if ((intptr_t)rdp->serial.file != -1 && rdp->ipx_socket == -1 && rdp->udp_socket == -1)
        {
            sleep_ms(timeout);
#ifndef RDPLIB_SOURCE_FAITHFUL
            ready = 0;
#endif
        }
        else if (!tv || tv->tv_usec || tv->tv_sec)
        {
            if (rdp->ipx_socket == -1 && rdp->udp_socket == -1)
            {
                ready = 0;
                sleep_ms(100);
            }
            else
            {
                ready = rdplib_platform_wait(rdp->udp_socket, rdp->ipx_socket, rdp->icmp_socket, enabled_sources, tv == NULL,
                                             tv ? (uint32_t)tv->tv_sec : 0, tv ? (uint32_t)tv->tv_usec : 0, &ready_sources);
            }
        }
        else
        {
            event_processing_time = time_get_ms() - max_time;
            if (event_processing_time < 4u)
            {
                timeout = 4u - event_processing_time;
                sleep_ms(timeout);
            }
            ready = 2;
        }

#ifdef RDPLIB_DEBUG
        loop_time = time_get_ms();
#elif defined(RDPLIB_SOURCE_FAITHFUL)
        (void)time_get_ms();
#endif
#ifdef RDPLIB_DEBUG
        dpf(0x40u, "io_thread: select complete\n");
#endif
        if ((intptr_t)rdp->serial.file != -1 && (int32_t)(time_get_ms() - serial_get_time_next_recv(&rdp->serial)) > 0)
        {
            rdp_serial_drain(rdp);
        }

        if (ready <= 0)
        {
            ++g_rdp_stat->retx_timeouts;
        }
        else
        {
            if ((ready_sources & RDPLIB_PLATFORM_IO_SOURCE_IPV4) != 0 && rdp->udp_socket != -1)
            {
#if defined(__linux__) && !defined(RDPLIB_SOURCE_FAITHFUL)
                if (rdp->icmp_socket == -1)
                {
                    rdplib_rdp_drain_icmp_errors(rdp);
                }
#endif
                do
                {
                    uint32_t quit;

                    char_recv = rdplib_platform_receive_datagram(rdp->udp_socket, (uint8_t *)scratch, sizeof(scratch), (uint8_t *)&remote_addr);
#if defined(_WIN32) && !defined(RDPLIB_SOURCE_FAITHFUL)
                    if (char_recv == RDPLIB_PLATFORM_RECEIVE_ICMP_PORT_UNREACHABLE)
                    {
                        memset(&unknown_icmp_source, 0, sizeof(unknown_icmp_source));
                        quit = 0;
                        rdplib_rdp_handle_reported_icmp(rdp, &remote_addr, 3, 3, 0, 0, &unknown_icmp_source);
                    }
                    else
#endif
                    {
                        quit = rdp_handle_data_recv(rdp, scratch, char_recv, &remote_addr);
                    }
                    if (quit || char_recv == -1)
                    {
                        break;
                    }
                } while (1);
            }

            if ((ready_sources & RDPLIB_PLATFORM_IO_SOURCE_IPX) != 0 && rdp->ipx_socket != -1)
            {
                do
                {
                    uint32_t quit;

                    memset(&remote_addr, 0, sizeof(remote_addr));
                    char_recv = rdplib_platform_receive_datagram(rdp->ipx_socket, (uint8_t *)scratch, sizeof(scratch), (uint8_t *)&remote_addr);
                    quit = rdp_handle_data_recv(rdp, scratch, char_recv, &remote_addr);
                    if (quit || char_recv == -1)
                    {
                        break;
                    }
                } while (1);
            }

            if ((ready_sources & RDPLIB_PLATFORM_IO_SOURCE_ICMP) != 0 && rdp->icmp_socket != -1)
            {
                do
                {
                    memset(&remote_addr, 0, sizeof(remote_addr));
                    char_recv = rdplib_platform_receive_datagram(rdp->icmp_socket, (uint8_t *)scratch, sizeof(scratch), (uint8_t *)&remote_addr);
                    rdp_handle_icmp_recv(rdp, scratch, char_recv, (struct sockaddr_in *)&remote_addr);
                } while (char_recv != -1);
            }
        }

        current_time = time_get_ms();
        elapsed_time = current_time - rdp->last_sample;
        if (elapsed_time > 1000u)
        {
#ifdef RDPLIB_SOURCE_FAITHFUL
            input_rate = 1000u * rdp->bytes_recvd / elapsed_time;
            duplicate_input_rate = 1000u * rdp->duplicate_bytes_recvd / elapsed_time;
#else
            input_rate = (uint32_t)(UINT64_C(1000) * rdp->bytes_recvd / elapsed_time);
            duplicate_input_rate = (uint32_t)(UINT64_C(1000) * rdp->duplicate_bytes_recvd / elapsed_time);
#endif
            rdp->bytes_recvd = 0;
            rdp->duplicate_bytes_recvd = 0;
            umutex_lock(&rdp->message_rxq_mutex);
            rdp->bytes_per_second = input_rate;
            rdp->duplicate_bytes_per_second = duplicate_input_rate;
            rdp->last_sample = current_time;
            umutex_unlock(&rdp->message_rxq_mutex);
        }

        max_time = time_get_ms();
        for (;;)
        {
            struct sockaddr addr;

            eventq_lock(&rdp->conn_eventq);
            c = eventq_peek_head(&rdp->conn_eventq);
            if (!c || c->cn_event_time.infinite || (int32_t)(max_time - c->cn_event_time.time) < 0)
            {
                c = NULL;
            }
            else
            {
                memcpy(&addr, &c->tx_remote_addr, sizeof(addr));
            }
            eventq_unlock(&rdp->conn_eventq);
            if (!c)
            {
                break;
            }

            c = rdp_lock_addr(rdp, &addr);
            if (c)
            {
                connection_event_process(c, max_time, &next_event);
                if (connection_linger_expired(c))
                {
                    rdp_connection_mark_for_delete(rdp, c);
                }
                else
                {
                    if (tx_needs_disconnect_msg(c))
                    {
                        rdp_enqueue_disconnect_msg(rdp, c);
                    }
                    eventq_lock(&rdp->conn_eventq);
                    if (memcmp(&c->cn_event_time, &next_event, sizeof(next_event)) != 0)
                    {
                        c->cn_event_time = next_event;
                        eventq_resort_by_ptr(&rdp->conn_eventq, c);
                    }
                    eventq_unlock(&rdp->conn_eventq);
                }
                rdp_unlock(c);
            }
        }
    }

    rdp_destroy_internal(rdp);
#ifdef RDPLIB_DEBUG
    dpf(0x20u, "io_thread exiting\n");
#endif
}

void rdp_enqueue_disconnect_msg(rdp_t *rdp, connection_t *c)
{
    msg_arrival_t *not_connected;

#ifdef RDPLIB_DEBUG
    dpf(0x2000u, "enqueue_disconnect_msg\n");
#endif
    not_connected = (msg_arrival_t *)fast_malloc((uint32_t)sizeof(*not_connected));
    msg_arrival_init_disconnect_msg(not_connected, c);
    rdp_enqueue_arrival(rdp, not_connected);
    c->tx_enqueued_disconnect_msg = 1;
}

void rdp_init(rdp_t *rdp)
{
    rdp->startup = 0;
    rdp->udp_socket = -1;
    rdp->ipx_socket = -1;
    rdp->icmp_socket = -1;
    rdp->trace_socket = -1;
    memset(&rdp->local_udp_addr, 0, sizeof(rdp->local_udp_addr));
    memset(&rdp->trace_local_addr, 0, sizeof(rdp->trace_local_addr));
#if !defined(RDPLIB_DEBUG) && !defined(RDPLIB_SOURCE_FAITHFUL)
    // Localhost rejection can inspect the complete normalized address record even when no IPX socket was created.
    memset(&rdp->local_ipx_addr, 0, offsetof(rdp_t, udp_socket_ttl) - offsetof(rdp_t, local_ipx_addr));
#endif
    rdp->udp_socket_ttl = 0;
    rdp->udp_broadcast = 0;
    rdp->ipx_broadcast = 0;
    rdp->bytes_recvd = 0;
    rdp->duplicate_bytes_recvd = 0;
#ifdef RDPLIB_SOURCE_FAITHFUL
    rdp->bytes_recvd = 0;
#else
    rdp->bytes_per_second = 0;
#endif
    rdp->duplicate_bytes_per_second = 0;
    serial_init(&rdp->serial);
    connhash_init(&rdp->addr_map);
    eventq_init(&rdp->conn_eventq);
    rdp->wake_sent = 0;
    uthread_init(&rdp->io_thread);
    usemaphore_init(&rdp->receive_semaphore);
    rxq_init(&rdp->message_rxq);
    umutex_create(&rdp->message_rxq_mutex);
    rxq_init(&rdp->external_rxq);
    umutex_create(&rdp->external_rxq_mutex);
    rdp->last_sample = time_get_ms();
#ifndef RDPLIB_SOURCE_FAITHFUL
    rdp->rdplib_arrival_ready_callback = NULL;
    rdp->rdplib_arrival_ready_context = NULL;
#endif
}

static uint32_t rdp_create_internal(rdp_t **out_rdp, uint16_t local_port, uint32_t connections, uint32_t flags, uint32_t receive_socket_buffer_bytes, uint32_t send_socket_buffer_bytes,
                                    rdplib_rdp_arrival_ready_callback_t arrival_ready_callback, void *arrival_ready_context);

uint32_t rdp_create(rdp_t **out_rdp, uint16_t local_port, uint32_t connections, uint32_t flags)
{
    return rdp_create_internal(out_rdp, local_port, connections, flags, 0, 0, NULL, NULL);
}

uint32_t rdplib_rdp_create(rdp_t **out_rdp, uint16_t local_port, uint32_t connections, uint32_t flags, uint32_t receive_socket_buffer_bytes, uint32_t send_socket_buffer_bytes,
                           rdplib_rdp_arrival_ready_callback_t arrival_ready_callback, void *arrival_ready_context)
{
    return rdp_create_internal(out_rdp, local_port, connections, flags, receive_socket_buffer_bytes, send_socket_buffer_bytes, arrival_ready_callback, arrival_ready_context);
}

static uint32_t rdp_create_internal(rdp_t **out_rdp, uint16_t local_port, uint32_t connections, uint32_t flags, uint32_t receive_socket_buffer_bytes, uint32_t send_socket_buffer_bytes,
                                    rdplib_rdp_arrival_ready_callback_t arrival_ready_callback, void *arrival_ready_context)
{
    rdp_t *rdp;
    uint32_t namelen;
#ifdef RDPLIB_SOURCE_FAITHFUL
    int ipproto_icmp;
#endif
    int ipproto_udp;
#ifdef RDPLIB_SOURCE_FAITHFUL
    uint32_t udp_ttl_len;
#endif
    int err;
    uint32_t result;
    int32_t on;
    uint16_t ver;
#ifdef RDPLIB_SOURCE_FAITHFUL
    struct sockaddr_in local_addr;
#endif
    struct sockaddr_ipx sipx;

    result = 0;
    *out_rdp = NULL;
#ifdef RDPLIB_SOURCE_FAITHFUL
    if (arrival_ready_callback)
    {
        return RDP_CREATE_FAILED;
    }
    (void)arrival_ready_context;
#endif
    rdp = (rdp_t *)rdplib_platform_malloc(sizeof(*rdp));
    if (!rdp)
    {
        return 2;
    }
    rdp_init(rdp);
#ifndef RDPLIB_SOURCE_FAITHFUL
    rdp->rdplib_arrival_ready_callback = arrival_ready_callback;
    rdp->rdplib_arrival_ready_context = arrival_ready_context;
#endif

    ver = UINT16_C(0x0101);
    err = rdplib_platform_network_startup(ver);
    if (err)
    {
        result = 1;
        goto exit;
    }
    rdp->startup = 1;

    result = serial_create(&rdp->serial, g_next_local_port++);
    if (result)
    {
        goto exit;
    }
    serial_set_time_next_recv(&rdp->serial, time_get_ms());
    result = connhash_create(&rdp->addr_map, (uint16_t)(connections == 1 ? 1 : 12));
    if (result)
    {
        goto exit;
    }
    result = eventq_create(&rdp->conn_eventq, 2u * connections);
    if (result)
    {
        goto exit;
    }
    result = usemaphore_create(&rdp->receive_semaphore);
    if (result)
    {
        goto exit;
    }

    ipproto_udp = rdplib_platform_protocol_number("udp", 17);
    if ((flags & RDP_CREATE_REQUIRE_IPV4) || (flags & RDP_CREATE_ATTEMPT_BOTH))
    {
        rdp->udp_socket = rdplib_platform_socket_create(AF_INET, RDP_SOCKET_TYPE_DATAGRAM, ipproto_udp);
    }
    if ((flags & RDP_CREATE_REQUIRE_IPV4) && rdp->udp_socket == -1)
    {
        result = 10;
        goto exit;
    }
    if ((flags & RDP_CREATE_REQUIRE_IPX) || (flags & RDP_CREATE_ATTEMPT_BOTH))
    {
        rdp->ipx_socket = rdplib_platform_socket_create(RDP_TRANSMIT_ADDRESS_IPX, RDP_SOCKET_TYPE_DATAGRAM, 1000);
    }
    if ((flags & RDP_CREATE_REQUIRE_IPX) && rdp->ipx_socket == -1)
    {
        result = 10;
        goto exit;
    }

    if (rdp->udp_socket != -1)
    {
        memset(&rdp->local_udp_addr, 0, sizeof(rdp->local_udp_addr));
        rdp->local_udp_addr.sin_family = AF_INET;
        rdp->local_udp_addr.sin_port = htons(local_port);
        err = rdplib_platform_socket_bind(rdp->udp_socket, (const uint8_t *)&rdp->local_udp_addr, sizeof(rdp->local_udp_addr));
        if (err == -1)
        {
            result = rdplib_platform_last_socket_error() == RDP_SOCKET_ERROR_ADDRESS_IN_USE ? 11u : 1u;
            goto exit;
        }
        on = 1;
        err = rdplib_platform_socket_set_option(rdp->udp_socket, RDP_SOCKET_LEVEL, RDP_SOCKET_BROADCAST, &on, sizeof(on));
        rdp->udp_broadcast = err == 0;
        namelen = sizeof(rdp->local_udp_addr);
        err = rdplib_platform_socket_get_name(rdp->udp_socket, (uint8_t *)&rdp->local_udp_addr, &namelen);
        if (err == -1)
        {
            result = 1;
            goto exit;
        }
        if (receive_socket_buffer_bytes)
        {
            if (rdplib_rdp_set_socket_receive_buffer_size(rdp, receive_socket_buffer_bytes) != 0)
            {
                result = 1;
                goto exit;
            }
        }
        if (send_socket_buffer_bytes)
        {
            if (rdplib_rdp_set_socket_send_buffer_size(rdp, send_socket_buffer_bytes) != 0)
            {
                result = 1;
                goto exit;
            }
        }
        result = disable_blocking(rdp->udp_socket);
        if (result)
        {
            goto exit;
        }

#ifndef RDPLIB_SOURCE_FAITHFUL
#if defined(__linux__) || defined(_WIN32)
        if (rdplib_platform_enable_icmp_errors(rdp->udp_socket) != 0)
        {
            rdplib_platform_record_socket_error(rdplib_platform_last_socket_error());
        }
#endif
#else
        ipproto_icmp = rdplib_platform_protocol_number("icmp", 1);
        rdp->icmp_socket = rdplib_platform_socket_create(AF_INET, RDP_SOCKET_TYPE_RAW, ipproto_icmp);
        if (rdp->icmp_socket == -1)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x800020u, "icmp not available: %s\n", net_strerror((uint32_t)rdplib_platform_last_socket_error()));
#endif
            (void)net_strerror((uint32_t)rdplib_platform_last_socket_error());
        }
        else
        {
            result = disable_blocking(rdp->icmp_socket);
            if (result)
            {
                goto exit;
            }
            memset(&local_addr, 0, sizeof(local_addr));
            local_addr.sin_family = AF_INET;
            local_addr.sin_port = 0;
            err = rdplib_platform_socket_bind(rdp->icmp_socket, (const uint8_t *)&local_addr, sizeof(local_addr));
            if (err == -1)
            {
                result = 1;
                goto exit;
            }
#ifdef RDPLIB_DEBUG
            dpf(0x800020u, "icmp socket created\n");
#endif

            rdp->trace_socket = rdplib_platform_socket_create(AF_INET, RDP_SOCKET_TYPE_DATAGRAM, ipproto_udp);
            if (rdp->trace_socket == -1)
            {
                result = 1;
                goto exit;
            }
            memset(&rdp->trace_local_addr, 0, sizeof(rdp->trace_local_addr));
            rdp->trace_local_addr.sin_family = AF_INET;
            err = rdplib_platform_socket_bind(rdp->trace_socket, (const uint8_t *)&rdp->trace_local_addr, sizeof(rdp->trace_local_addr));
            if (err == -1)
            {
                result = 1;
                goto exit;
            }
            namelen = sizeof(rdp->trace_local_addr);
            err = rdplib_platform_socket_get_name(rdp->trace_socket, (uint8_t *)&rdp->trace_local_addr, &namelen);
            if (err == -1)
            {
                result = 1;
                goto exit;
            }
            result = disable_blocking(rdp->trace_socket);
            if (result)
            {
                goto exit;
            }

            udp_ttl_len = sizeof(rdp->udp_socket_ttl);
            err = rdplib_platform_socket_get_option(rdp->trace_socket, RDP_IP_LEVEL, RDP_IP_TTL, &rdp->udp_socket_ttl, &udp_ttl_len);
            if (err)
            {
                int socket_error = rdplib_platform_last_socket_error();
                if (socket_error != RDP_SOCKET_ERROR_OPTION_UNSUPPORTED)
                {
#ifdef RDPLIB_DEBUG
                    dpf(0x10000020u, "setsockopt: %s\n", net_strerror((uint32_t)rdplib_platform_last_socket_error()));
#endif
                    (void)net_strerror((uint32_t)rdplib_platform_last_socket_error());
                    result = 1;
                    goto exit;
                }
#ifdef RDPLIB_DEBUG
                dpf(0x10000020u, "winsock provider does not support getsockopt( IPPROTO_IP, IP_TTL )\n");
#endif
                rdplib_platform_socket_close(rdp->trace_socket);
                rdp->trace_socket = -1;
            }
            if (rdp->trace_socket != -1)
            {
                err = rdplib_platform_socket_set_option(rdp->trace_socket, RDP_IP_LEVEL, RDP_IP_TTL, &rdp->udp_socket_ttl, sizeof(rdp->udp_socket_ttl));
                if (err)
                {
                    int socket_error = rdplib_platform_last_socket_error();
                    if (socket_error != RDP_SOCKET_ERROR_OPTION_UNSUPPORTED)
                    {
#ifdef RDPLIB_DEBUG
                        dpf(0x10000020u, "setsockopt: %s\n", net_strerror((uint32_t)rdplib_platform_last_socket_error()));
#endif
                        (void)net_strerror((uint32_t)rdplib_platform_last_socket_error());
                        result = 1;
                        goto exit;
                    }
#ifdef RDPLIB_DEBUG
                    dpf(0x10000020u, "winsock provider does not support setsockopt( IPPROTO_IP, IP_TTL )\n");
#endif
                    rdplib_platform_socket_close(rdp->trace_socket);
                    rdp->trace_socket = -1;
                }
            }
            if (rdp->trace_socket != -1)
            {
#ifdef RDPLIB_DEBUG
                dpf(0x10000020u, "trace socket created\n");
#endif
            }
        }
#endif
    }

    if (rdp->ipx_socket != -1)
    {
        memset(&sipx, 0, sizeof(sipx));
        sipx.sa_family = RDP_TRANSMIT_ADDRESS_IPX;
        sipx.sa_socket = local_port;
        err = rdplib_platform_socket_bind(rdp->ipx_socket, (const uint8_t *)&sipx, sizeof(sipx));
        if (err == -1)
        {
            result = rdplib_platform_last_socket_error() == RDP_SOCKET_ERROR_ADDRESS_IN_USE ? 11u : 1u;
            goto exit;
        }
        on = 1;
        err = rdplib_platform_socket_set_option(rdp->ipx_socket, RDP_SOCKET_LEVEL, RDP_SOCKET_BROADCAST, &on, sizeof(on));
        rdp->ipx_broadcast = err == 0;
        namelen = sizeof(rdp->local_ipx_addr);
        err = rdplib_platform_socket_get_name(rdp->ipx_socket, (uint8_t *)&rdp->local_ipx_addr, &namelen);
        if (err == -1)
        {
            result = 1;
            goto exit;
        }
        result = disable_blocking(rdp->ipx_socket);
        if (result)
        {
            goto exit;
        }
    }

    rdp->encrypt = (flags & RDP_CREATE_USE_ENCRYPTION) != 0;
    rdp->crc = (flags & RDP_CREATE_USE_CRC) != 0;
    rdp->io_thread_running = 1;
    result = uthread_create(&rdp->io_thread, rdp_io_thread, rdp);
    if (result)
    {
        rdp->io_thread_running = 0;
    }
    else
    {
        *out_rdp = rdp;
    }

exit:
    if (result)
    {
        rdp->app_is_waiting_for_exit = 0;
        rdp_destroy_internal(rdp);
    }
    return result;
}

#ifdef RDP_DEAD_CODE
// unused, retained for historical interest
void rdp_shutdown(uint32_t linger_time)
{
#ifdef RDPLIB_DEBUG
    dpf(0x20u, "rdp_shutdown()\n");
#endif
    sleep_ms(linger_time);
}
#endif

void rdp_destroy(rdp_t *rdp, int wait)
{
    uint32_t exit_code;

#ifdef RDPLIB_DEBUG
    dpf(0x20u, "rdp_destroy\n");
#endif
    if (rdp->io_thread_running == 1)
    {
        rdp->app_is_waiting_for_exit = wait != 0;
        rdp->io_thread_running = 0;
        (void)rdp_wake(rdp, 1);
        if (wait)
        {
#ifdef RDPLIB_DEBUG
            dpf(0x20u, "waiting for thread exit\n");
#endif
            (void)uthread_wait_exit_code(&rdp->io_thread, &exit_code);
            uthread_destroy(&rdp->io_thread);
            rdplib_platform_free(rdp);
        }
    }
}

uint32_t rdp_connect_sa(rdp_t *rdp, connection_t **new_c, struct sockaddr *remote_addr, uint32_t flags)
{
    connection_t *c;
    uint32_t result;

    ++g_rdp_stat->connection_requests;
    result = rdp_connection_create_internal(rdp, &c, remote_addr, flags);
    if (!result)
    {
        c->cn_accepted = 1;
        rdp_unlock(c);
        *new_c = c;
    }
    return result;
}

#ifdef RDP_DEAD_CODE
// unused, retained for historical interest
uint32_t rdp_send(rdp_t *rdp, struct sockaddr *remote_addr, uint16_t port, const char *data, uint32_t size)
{
    iov_t iov[2];
    uint32_t result;
    uint16_t options;
    struct sockaddr_ipx sipx;
    struct sockaddr_in sin;
    sockaddr_com scom;

    result = 0;
    options = htons(UINT16_C(0xFFFF));
    if (!rdp)
    {
        return 6;
    }
    if (size > 512)
    {
#ifdef RDPLIB_DEBUG
        dpf(0x400000u, "RDP_SEND_ERROR_TOO_BIG %u bytes\n", size);
#endif
        return RDP_SEND_ERROR_TOO_BIG;
    }
    iov[0].data = &options;
    iov[0].size = sizeof(options);
    iov[1].data = (void *)data;
    iov[1].size = size;
    if (remote_addr)
    {
        switch (remote_addr->sa_family)
        {
        case RDP_TRANSMIT_ADDRESS_IPV4:
            result = usend(rdp->udp_socket, iov, 2, remote_addr, rdp->encrypt, 0);
            break;
        case RDP_TRANSMIT_ADDRESS_IPX:
            result = usend(rdp->ipx_socket, iov, 2, remote_addr, rdp->encrypt, 0);
            break;
        case RDP_TRANSMIT_ADDRESS_SERIAL:
            result = serial_send(&rdp->serial, iov, 2, (sockaddr_com *)remote_addr);
            break;
        }
        return result;
    }
    if (rdp->ipx_broadcast)
    {
        memset(&sipx, 0, sizeof(sipx));
        sipx.sa_family = RDP_TRANSMIT_ADDRESS_IPX;
        sipx.sa_socket = port;
        memset(sipx.sa_nodenum, 0xFF, sizeof(sipx.sa_nodenum));
        result = usend(rdp->ipx_socket, iov, 2, (struct sockaddr *)&sipx, rdp->encrypt, 0);
    }
    if (rdp->udp_broadcast)
    {
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = RDP_TRANSMIT_ADDRESS_IPV4;
        sin.sin_addr.s_addr = htonl(UINT32_C(0xFFFFFFFF));
        sin.sin_port = htons(port);
        result = usend(rdp->udp_socket, iov, 2, (struct sockaddr *)&sin, rdp->encrypt, 0);
    }
    if ((intptr_t)rdp->serial.file != -1)
    {
        memset(&scom, 0, sizeof(scom));
        scom.scom_family = RDP_TRANSMIT_ADDRESS_SERIAL;
        result = serial_send(&rdp->serial, iov, 2, &scom);
    }
    (void)result;
    return 0;
}

uint32_t rdp_send_oversized(rdp_t *rdp, struct sockaddr *remote_addr, uint16_t port, const char *data, uint32_t size)
{
    iov_t iov[2];
    uint32_t result;
    uint16_t options;
    struct sockaddr_ipx sipx;
    struct sockaddr_in sin;
    sockaddr_com scom;

    result = 0;
    options = htons(UINT16_C(0xFFFF));
    if (!rdp)
    {
        return 6;
    }
    iov[0].data = &options;
    iov[0].size = sizeof(options);
    iov[1].data = (void *)data;
    iov[1].size = size;
    if (remote_addr)
    {
        switch (remote_addr->sa_family)
        {
        case RDP_TRANSMIT_ADDRESS_IPV4:
            result = usend(rdp->udp_socket, iov, 2, remote_addr, rdp->encrypt, 0);
            break;
        case RDP_TRANSMIT_ADDRESS_IPX:
            result = usend(rdp->ipx_socket, iov, 2, remote_addr, rdp->encrypt, 0);
            break;
        case RDP_TRANSMIT_ADDRESS_SERIAL:
            result = serial_send(&rdp->serial, iov, 2, (sockaddr_com *)remote_addr);
            break;
        }
        return result;
    }
    if (rdp->ipx_broadcast)
    {
        memset(&sipx, 0, sizeof(sipx));
        sipx.sa_family = RDP_TRANSMIT_ADDRESS_IPX;
        sipx.sa_socket = port;
        memset(sipx.sa_nodenum, 0xFF, sizeof(sipx.sa_nodenum));
        result = usend(rdp->ipx_socket, iov, 2, (struct sockaddr *)&sipx, rdp->encrypt, 0);
    }
    if (rdp->udp_broadcast)
    {
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = RDP_TRANSMIT_ADDRESS_IPV4;
        sin.sin_addr.s_addr = htonl(UINT32_C(0xFFFFFFFF));
        sin.sin_port = htons(port);
        result = usend(rdp->udp_socket, iov, 2, (struct sockaddr *)&sin, rdp->encrypt, 0);
    }
    if ((intptr_t)rdp->serial.file != -1)
    {
        memset(&scom, 0, sizeof(scom));
        scom.scom_family = RDP_TRANSMIT_ADDRESS_SERIAL;
        result = serial_send(&rdp->serial, iov, 2, &scom);
    }
    (void)result;
    return 0;
}
#endif

#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
void format_sockaddr(char *buf, struct sockaddr *sa)
{
    struct sockaddr_in *sin;
#if defined(RDPLIB_DEBUG) || defined(_WIN32)
    char netnum[16];
    struct sockaddr_ipx *sipx;
    char nodenum[16];
#endif
    sockaddr_com *scom;

    switch (sa->sa_family)
    {
    case RDP_TRANSMIT_ADDRESS_IPV4:
        sin = (struct sockaddr_in *)sa;
        sprintf(buf, "AF_INET %s:%u", inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));
        break;
    case RDP_TRANSMIT_ADDRESS_IPX:
#ifdef RDPLIB_SOURCE_FAITHFUL
#ifdef _WIN32
        sipx = (struct sockaddr_ipx *)sa;
        // Unsafe historical behavior: the Windows clients passed both uninitialized arrays to %s.
        sprintf(buf, "AF_IPX %s:%s:%u", netnum, nodenum, sipx->sa_socket);
        break;
#else
        return;
#endif
#elif defined(RDPLIB_DEBUG)
        sipx = (struct sockaddr_ipx *)sa;
        data_format(netnum, (const uint8_t *)sipx->sa_netnum, sizeof(sipx->sa_netnum));
        data_format(nodenum, (const uint8_t *)sipx->sa_nodenum, sizeof(sipx->sa_nodenum));
        sprintf(buf, "AF_IPX %s:%s:%u", netnum, nodenum, sipx->sa_socket);
        break;
#else
        return;
#endif
    case RDP_TRANSMIT_ADDRESS_SERIAL:
        scom = (sockaddr_com *)sa;
        sprintf(buf, "AF_COMPORT %u", (uint32_t)(int32_t)scom->scom_port);
        break;
    default:
        sprintf(buf, "unknown address family (%u)", (uint32_t)(uint16_t)sa->sa_family);
        break;
    }
}
#endif

uint32_t rdp_connection_create_internal(rdp_t *rdp, connection_t **new_c, struct sockaddr *remote_addr, uint32_t flags)
{
    uint32_t flag_mask;
    connection_t *c;
    uint32_t result;
#if defined(RDPLIB_DEBUG) || defined(RDPLIB_SOURCE_FAITHFUL)
    char temp[64];
#endif

    result = 0;
    c = NULL;
    flag_mask = ~UINT32_C(1);
    if (flags & flag_mask)
    {
        result = 6;
    }
    else
    {
        c = (connection_t *)rdplib_platform_malloc(sizeof(*c));
        if (c)
        {
            connection_init(c, rdp, remote_addr, flags);
            result = connection_create(c);
            if (!result)
            {
#ifdef RDPLIB_SOURCE_FAITHFUL
                format_sockaddr(temp, &c->tx_remote_addr);
#endif
#ifdef RDPLIB_DEBUG
#ifndef RDPLIB_SOURCE_FAITHFUL
                format_sockaddr(temp, &c->tx_remote_addr);
#endif
                dpf(0x4000u, "[0x%08x] new connection %s\n", (uint32_t)(uintptr_t)c, temp);
#endif
                eventq_lock(&rdp->conn_eventq);
                (void)eventq_insert(&rdp->conn_eventq, c);
                connhash_insert(&rdp->addr_map, c);
                eventq_unlock(&rdp->conn_eventq);
                (void)rdp_lock_connection(c);
                *new_c = c;
            }
        }
        else
        {
            result = 2;
        }
    }
    if (result && c)
    {
        connection_destroy(c);
        rdplib_platform_free(c);
    }
    return result;
}

uint32_t rdp_connect(rdp_t *rdp, connection_t **new_c, char *hostname, uint16_t port, uint32_t flags)
{
    struct sockaddr_in sin;
    uint32_t result;

    result = 0;
    memset(&sin, 0, sizeof(sin));
    if (rdplib_platform_resolve_ipv4(hostname, port, (uint8_t *)&sin) == 0)
    {
        result = rdp_connect_sa(rdp, new_c, (struct sockaddr *)&sin, flags);
    }
    else
    {
        result = 12;
    }
    return result;
}

void rdp_connection_mark_for_delete(rdp_t *rdp, connection_t *c)
{
    connection_t *removed;
#ifdef RDPLIB_DEBUG
    uint32_t messages_removed;
#endif

#ifdef RDPLIB_DEBUG
    assert(umutex_owner( &c->cn_lock ));
#endif
#ifdef RDPLIB_DEBUG
    assert(c->cn_rdp == rdp);
#endif
    removed = connhash_subref(&rdp->addr_map, c);
#ifdef RDPLIB_DEBUG
    assert(removed == NULL);
#endif
    (void)removed;
#ifdef RDPLIB_DEBUG
    // The flush was historically inside assert; keep the debug bookkeeping alive when NDEBUG removes assertions.
    umutex_lock(&rdp->message_rxq_mutex);
    messages_removed = rxq_flush_all_messages(&rdp->message_rxq, c);
    assert(0 == messages_removed);
    umutex_unlock(&rdp->message_rxq_mutex);
#endif
#ifdef RDPLIB_DEBUG
    dpf(0x2000u, "[0x%08x] removed\n", (uint32_t)(uintptr_t)c);
#endif
}

uint32_t connection_close_wait(connection_t *c, uint32_t linger_time, uint32_t *all_acked)
{
    uevent_t all_acked_event;
    uint32_t result;

    result = 0;
    uevent_init(&all_acked_event);
    result = uevent_create(&all_acked_event);
    if (result)
    {
        result = 1;
    }
    else
    {
        (void)connection_close(c, linger_time, all_acked, &all_acked_event);
        uevent_wait(&all_acked_event);
    }
    uevent_destroy(&all_acked_event);
    return result;
}

msg_arrival_t *rdp_receive(rdp_t *rdp, uint32_t timeout)
{
    msg_arrival_t *msg;
    uint32_t corrected_timeout;
    uint32_t stop_wait_time = 0; // Every path that uses stop_wait_time assigns it first; initialize it for MSVC C4701.
#ifdef RDPLIB_DEBUG
    uint32_t current_time;
    uint32_t queueing_delay;
    char temp[128];
#endif

    msg = NULL;
    corrected_timeout = timeout;
    if (rdp)
    {
        umutex_lock(&rdp->external_rxq_mutex);
        msg = rxq_remove_head(&rdp->external_rxq);
        umutex_unlock(&rdp->external_rxq_mutex);
        if (!msg)
        {
            if (timeout && timeout != UINT32_MAX)
            {
                stop_wait_time = time_get_ms() + timeout;
            }
            for (;;)
            {
                if ((intptr_t)rdp->serial.file != -1)
                {
                    rdp_serial_drain(rdp);
                }
                if (!usemaphore_decrement(&rdp->receive_semaphore, corrected_timeout))
                {
                    break;
                }
                umutex_lock(&rdp->message_rxq_mutex);
                umutex_lock(&rdp->external_rxq_mutex);
                memcpy(&rdp->external_rxq, &rdp->message_rxq, sizeof(rdp->external_rxq));
                memset(&rdp->message_rxq, 0, sizeof(rdp->message_rxq));
                msg = rxq_remove_head(&rdp->external_rxq);
                umutex_unlock(&rdp->external_rxq_mutex);
                umutex_unlock(&rdp->message_rxq_mutex);
                if (msg)
                {
                    break;
                }
                if (!timeout)
                {
                    break;
                }
                if (timeout != UINT32_MAX)
                {
                    corrected_timeout = stop_wait_time - time_get_ms();
                    if ((int32_t)corrected_timeout <= 0)
                    {
                        break;
                    }
                }
            }
        }

#ifdef RDPLIB_DEBUG
        if (msg)
        {
            current_time = time_get_ms();
            queueing_delay = current_time - msg->enqueue_time;
            if (g_rdp_stat->rqd_samples)
            {
                if (queueing_delay < g_rdp_stat->rqd_min)
                {
                    g_rdp_stat->rqd_min = queueing_delay;
                }
                if (queueing_delay > g_rdp_stat->rqd_max)
                {
                    g_rdp_stat->rqd_max = queueing_delay;
                }
                g_rdp_stat->rqd_sum += queueing_delay;
                g_rdp_stat->rqd_bytes += msg->size;
            }
            else
            {
                g_rdp_stat->rqd_sum = queueing_delay;
                g_rdp_stat->rqd_max = g_rdp_stat->rqd_sum;
                g_rdp_stat->rqd_min = g_rdp_stat->rqd_max;
            }
            ++g_rdp_stat->rqd_samples;

            if (current_time - g_rdp_stat->rqd_last_interval > 10000u)
            {
                g_rdp_stat->rqd_last_interval = current_time;
                sprintf(temp, "%I64u rx queueing delay min:%I64u max:%I64u avg:%I64u (%I64u bytes)\n", g_rdp_stat->rqd_samples, g_rdp_stat->rqd_min,
                        g_rdp_stat->rqd_max, g_rdp_stat->rqd_sum / g_rdp_stat->rqd_samples, g_rdp_stat->rqd_bytes);
                dpf(0x80u, "%s", temp);
                g_rdp_stat->rqd_bytes = 0;
                g_rdp_stat->rqd_sum = 0;
                g_rdp_stat->rqd_max = 0;
                g_rdp_stat->rqd_min = 0;
                g_rdp_stat->rqd_samples = 0;
            }
        }
#endif
    }
    return msg;
}

#ifdef RDP_DEAD_CODE
// unused, retained for historical interest
uint32_t rdp_trace_capable(rdp_t *rdp)
{
    return rdp->trace_socket != -1;
}

struct sockaddr *rdp_get_local_addr(rdp_t *rdp, int16_t family)
{
    struct sockaddr *sa;

    sa = NULL;
    if (family == RDP_TRANSMIT_ADDRESS_IPV4)
    {
        sa = (struct sockaddr *)&rdp->local_udp_addr;
    }
    else if (family == RDP_TRANSMIT_ADDRESS_IPX)
    {
        sa = (struct sockaddr *)&rdp->local_ipx_addr;
    }
    else if (family == RDP_TRANSMIT_ADDRESS_SERIAL)
    {
#ifdef RDPLIB_DEBUG
        assert(!"why do you want the local address of the com port?");
#endif
    }
    return sa;
}

uint32_t rdp_get_transport_mask(void)
{
    int ipproto_udp;
    uint32_t avail_transports;
    intptr_t s;
    uint16_t ver;
    int err;
    struct sockaddr_ipx sipx;

    avail_transports = 0;
    ver = UINT16_C(0x0101);
    err = rdplib_platform_network_startup(ver);
    if (!err)
    {
        ipproto_udp = rdplib_platform_protocol_number("udp", 17);
        s = rdplib_platform_socket_create(AF_INET, RDP_SOCKET_TYPE_DATAGRAM, ipproto_udp);
        if (s != -1)
        {
            avail_transports |= 2;
            rdplib_platform_socket_close(s);
        }
        s = rdplib_platform_socket_create(RDP_TRANSMIT_ADDRESS_IPX, RDP_SOCKET_TYPE_DATAGRAM, 1000);
        if (s != -1)
        {
            uint32_t namelen;
            uint8_t zero[8] = {0};

            memset(&sipx, 0, sizeof(sipx));
            sipx.sa_family = RDP_TRANSMIT_ADDRESS_IPX;
            err = rdplib_platform_socket_bind(s, (const uint8_t *)&sipx, sizeof(sipx));
            if (err != -1)
            {
                namelen = sizeof(sipx);
                err = rdplib_platform_socket_get_name(s, (uint8_t *)&sipx, &namelen);
                if (err != -1 && memcmp(zero, sipx.sa_netnum, sizeof(sipx.sa_netnum)) && memcmp(zero, sipx.sa_nodenum, sizeof(sipx.sa_nodenum)))
                {
                    avail_transports |= 4;
                }
            }
            rdplib_platform_socket_close(s);
        }
        rdplib_platform_network_cleanup();
    }
    return avail_transports;
}
#endif

void rdplib_rdp_get_input_rate(rdp_t *rdp, rdplib_rdp_input_rate_t *input_rate)
{
    umutex_lock(&rdp->message_rxq_mutex);
    input_rate->bytes_per_second = rdp->bytes_per_second;
    input_rate->duplicate_bytes_per_second = rdp->duplicate_bytes_per_second;
    umutex_unlock(&rdp->message_rxq_mutex);
}

uint32_t rdp_get_input_rate(rdp_t *rdp)
{
#ifndef RDPLIB_SOURCE_FAITHFUL
    rdplib_rdp_input_rate_t input_rate;

    rdplib_rdp_get_input_rate(rdp, &input_rate);
    return input_rate.bytes_per_second;
#else
    return rdp->bytes_per_second;
#endif
}

#ifdef RDP_DEAD_CODE
// unused, retained for historical interest
uint32_t rdp_get_duplicate_input_rate(rdp_t *rdp)
{
#ifndef RDPLIB_SOURCE_FAITHFUL
    rdplib_rdp_input_rate_t input_rate;

    rdplib_rdp_get_input_rate(rdp, &input_rate);
    return input_rate.duplicate_bytes_per_second;
#else
    return rdp->duplicate_bytes_per_second;
#endif
}
#endif

uint32_t rdp_serial_tx_ready(rdp_t *rdp)
{
    return serial_tx_ready(&rdp->serial);
}

uint32_t rdp_serial_get_time_empty(rdp_t *rdp)
{
    return serial_get_time_empty(&rdp->serial);
}

#ifdef RDP_DEAD_CODE
// unused, retained for historical interest
void rdp_get_serial_stats(rdp_t *rdp, serial_stat_t *stats)
{
    serial_get_stats(&rdp->serial, stats);
}
#endif

uint32_t rdp_get_serial_stall_time(rdp_t *rdp)
{
    return serial_get_stall_time(&rdp->serial);
}

void rdplib_rdp_handle_reported_icmp(rdp_t *rdp, struct sockaddr *remote_addr, uint8_t type, uint8_t code, uint8_t trace_response, uint8_t trace_sample,
                                     struct sockaddr_in *source_addr)
{
    connection_t *c;

    c = rdp_lock_addr(rdp, remote_addr);
    if (!c)
    {
        return;
    }
#ifndef RDPLIB_SOURCE_FAITHFUL
    if (!trace_response && !c->tx_connected && c->tx_enqueued_disconnect_msg)
    {
        rdp_unlock(c);
        return;
    }
#endif
    connection_handle_icmp(c, type, code, trace_response, trace_sample, source_addr);
    if (tx_needs_disconnect_msg(c))
    {
        rdp_enqueue_disconnect_msg(rdp, c);
    }
    rdp_resort(c, 0);
    rdp_unlock(c);
}

#ifdef RDPLIB_DEBUG
static uint16_t rdplib_rdp_load_native_u16(const void *bytes)
{
    uint16_t value;

    memcpy(&value, bytes, sizeof(value));
    return value;
}
#endif
