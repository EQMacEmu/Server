// Copyright (c) 2026 solar
// SPDX-License-Identifier: MIT

#ifndef RDPLIB_RDP_INTERNAL_H
#define RDPLIB_RDP_INTERNAL_H

#include <stdint.h>

#include "rdplib_platform.h"

typedef struct rdp_t rdp_t;

typedef struct rdplib_rdp_input_rate_t
{
    uint32_t bytes_per_second;
    uint32_t duplicate_bytes_per_second;
} rdplib_rdp_input_rate_t;

#ifdef __cplusplus
extern "C"
{
#endif

// Reports that rdp_receive may return an arrival. The callback can run concurrently on any producer thread and is only a readiness hint.
// It must return promptly, must not call rdplib, and must not throw. Its context must remain valid until synchronous endpoint destruction completes.
typedef void (*rdplib_rdp_arrival_ready_callback_t)(void *context);

// Host error adapter. This is intentionally not part of the recovered raw API.
void rdplib_rdp_handle_reported_icmp(rdp_t *rdp, struct sockaddr *remote_addr, uint8_t type, uint8_t code, uint8_t trace_response, uint8_t trace_sample, struct sockaddr_in *source_addr);

#if defined(__linux__) && !defined(RDPLIB_SOURCE_FAITHFUL)
// IO-thread-only: collect queued ICMP records before locking their peers.
void rdplib_rdp_drain_icmp_errors(rdp_t *rdp);
#endif

// Normal API creation entry point. This is intentionally not part of the recovered raw API.
uint32_t rdplib_rdp_create(rdp_t **out_rdp, uint16_t local_port, uint32_t connections, uint32_t flags, uint32_t receive_socket_buffer_bytes, uint32_t send_socket_buffer_bytes,
                           rdplib_rdp_arrival_ready_callback_t arrival_ready_callback, void *arrival_ready_context);
int rdplib_rdp_set_socket_receive_buffer_size(rdp_t *rdp, uint32_t bytes);
int rdplib_rdp_set_socket_send_buffer_size(rdp_t *rdp, uint32_t bytes);
int rdplib_rdp_get_socket_receive_buffer_size(const rdp_t *rdp, uint32_t *bytes);
int rdplib_rdp_get_socket_send_buffer_size(const rdp_t *rdp, uint32_t *bytes);
void rdplib_rdp_get_input_rate(rdp_t *rdp, rdplib_rdp_input_rate_t *input_rate);

#ifdef __cplusplus
}
#endif

#endif /* RDPLIB_RDP_INTERNAL_H */
