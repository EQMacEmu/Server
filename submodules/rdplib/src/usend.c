// Copyright (c) 2026 solar
// SPDX-License-Identifier: MIT

#include "usend.h"

#ifdef RDPLIB_DEBUG
#include <assert.h>
#endif
#include <stdlib.h>
#include <string.h>

#ifndef RDPLIB_SOURCE_FAITHFUL
#include "connection.h"
#endif
#include "crc.h"
#include "cypher.h"
#include "rdplib_platform.h"

#define BLOCKSIZE 8
#define USEND_MAX_SIZE 32768

enum
{
    RDP_USEND_WOULD_BLOCK = 10035,
    RDP_USEND_NO_BUFFER_SPACE = 10055,
    RDP_USEND_INVALID_ARGUMENT = 6,
    RDP_USEND_CAPACITY_EXCEEDED = 18
};

static uint32_t rdplib_usend_socket_error_is_retryable(uint32_t error, int32_t sent)
{
#if defined(__linux__) && !defined(RDPLIB_SOURCE_FAITHFUL)
    // Linux can report another destination's pending error on this shared
    // socket. Attribute peer failures through the receive-side error record;
    // preserve retry state instead of aborting the current sender.
    if (sent < 0)
    {
        return 1;
    }
#else
    (void)sent;
#endif
    if (error == RDP_USEND_WOULD_BLOCK)
    {
        return 1;
    }
#ifndef RDPLIB_SOURCE_FAITHFUL
    if (error == RDP_USEND_NO_BUFFER_SPACE)
    {
        return 1;
    }
#endif
    return 0;
}

#ifndef RDPLIB_SOURCE_FAITHFUL
static uint32_t rdplib_usend_validate(iov_t *iov, uint32_t iov_len, struct sockaddr *remote_addr, uint32_t encrypt, uint32_t crc, uint32_t *buffer_size);
static uint32_t rdplib_usend_send_joined(intptr_t socket, char *buffer, int32_t buffer_size, struct sockaddr *remote_addr, uint32_t encrypt, uint32_t crc);
static uint32_t rdplib_usend_with_callback(connection_t *connection, intptr_t socket, iov_t *iov, uint32_t iov_len, struct sockaddr *remote_addr, uint32_t encrypt, uint32_t crc, uint32_t buffer_size);
#endif

int32_t rdp_append_crc(char *buffer, int32_t buffer_size)
{
    uint32_t crc;

    crc = htonl(rdp_crc(0, buffer, (uint32_t)buffer_size));
    memcpy(buffer + buffer_size, &crc, sizeof(crc));
    buffer_size += (int32_t)sizeof(crc);
    return buffer_size;
}

int32_t rdp_encode_data(char *buffer, int32_t buffer_size)
{
    int32_t original_size;
    int32_t i;
    uint8_t pad_size;

    original_size = buffer_size;
    buffer_size = (buffer_size + BLOCKSIZE) & ~(BLOCKSIZE - 1);
    for (i = original_size; i < buffer_size; ++i)
    {
        buffer[i] = (char)rand();
    }

#ifdef RDPLIB_DEBUG
    assert(buffer_size - original_size <= BLOCKSIZE);
#endif
    pad_size = (uint8_t)(buffer_size - original_size);
    buffer[buffer_size - 1] = (char)((buffer[buffer_size - 1] & 0xf0) | pad_size);

    // The May 2002 debug self check uses fixed 600 byte locals and is unsafe
    // for larger encoded buffers. Retail clients retain only the encode call.
#ifdef RDPLIB_DEBUG
    {
        char plain[600];
        char cypher[600];

        memcpy(plain, buffer, buffer_size);
        rdp_encode(plain, buffer_size / BLOCKSIZE);
        memcpy(cypher, plain, buffer_size);
        rdp_decode(plain, buffer_size / BLOCKSIZE);
        assert(0 == memcmp( plain, buffer, buffer_size ));
        memcpy(buffer, cypher, buffer_size);
    }
#else
    rdp_encode(buffer, buffer_size / BLOCKSIZE);
#endif
    return buffer_size;
}

#ifndef RDPLIB_SOURCE_FAITHFUL
uint32_t rdplib_usend_framed_size(uint32_t plaintext_bytes, uint32_t encrypt, uint32_t crc)
{
    if (crc || encrypt)
    {
        plaintext_bytes += sizeof(uint32_t);
    }
    if (encrypt)
    {
        plaintext_bytes = (plaintext_bytes + BLOCKSIZE) & ~(uint32_t)(BLOCKSIZE - 1);
    }
    return plaintext_bytes;
}
#endif

uint32_t usend(intptr_t socket, iov_t *iov, uint32_t iov_len, struct sockaddr *remote_addr, uint32_t encrypt, uint32_t crc)
{
    int32_t buffer_size;
    char buffer[USEND_MAX_SIZE];
    uint32_t i;
    int32_t char_sent;
    uint32_t result;

    buffer_size = 0;
    result = 0;

#ifndef RDPLIB_SOURCE_FAITHFUL
    {
        uint32_t checked_buffer_size;

        result = rdplib_usend_validate(iov, iov_len, remote_addr, encrypt, crc, &checked_buffer_size);
        if (result)
        {
            return result;
        }
    }
#endif

    for (i = 0; i < iov_len; ++i)
    {
#ifdef RDPLIB_DEBUG
        assert(buffer_size+iov[i].size < USEND_MAX_SIZE);
#endif
#ifndef RDPLIB_SOURCE_FAITHFUL
        if (iov[i].size == 0)
        {
            continue;
        }
#endif
        memcpy(buffer + buffer_size, iov[i].data, iov[i].size);
        buffer_size += (int32_t)iov[i].size;
    }

    if (crc || encrypt)
    {
        buffer_size = rdp_append_crc(buffer, buffer_size);
    }
    if (encrypt)
    {
        buffer_size = rdp_encode_data(buffer, buffer_size);
    }

    char_sent = rdplib_platform_send_datagram(socket, (const uint8_t *)buffer, (uint32_t)buffer_size, (const uint8_t *)remote_addr);
    if (char_sent != buffer_size)
    {
        if (rdplib_usend_socket_error_is_retryable(rdplib_platform_last_socket_error(), char_sent))
        {
            result = 5;
        }
        else
        {
            result = 1;
        }
    }
    return result;
}

#ifndef RDPLIB_SOURCE_FAITHFUL
static uint32_t rdplib_usend_validate(iov_t *iov, uint32_t iov_len, struct sockaddr *remote_addr, uint32_t encrypt, uint32_t crc, uint32_t *buffer_size)
{
    uint32_t framed_size;
    uint32_t i;

    if ((!iov && iov_len != 0) || !remote_addr || !buffer_size)
    {
        return RDP_USEND_INVALID_ARGUMENT;
    }

    *buffer_size = 0;
    for (i = 0; i < iov_len; ++i)
    {
        if (!iov[i].data && iov[i].size != 0)
        {
            return RDP_USEND_INVALID_ARGUMENT;
        }
        if (iov[i].size > USEND_MAX_SIZE - *buffer_size)
        {
            return RDP_USEND_CAPACITY_EXCEEDED;
        }
        *buffer_size += iov[i].size;
    }

    framed_size = rdplib_usend_framed_size(*buffer_size, encrypt, crc);
    if (framed_size > USEND_MAX_SIZE)
    {
        return RDP_USEND_CAPACITY_EXCEEDED;
    }
    return 0;
}

static uint32_t rdplib_usend_send_joined(intptr_t socket, char *buffer, int32_t buffer_size, struct sockaddr *remote_addr, uint32_t encrypt, uint32_t crc)
{
    int32_t char_sent;

    if (crc || encrypt)
    {
        buffer_size = rdp_append_crc(buffer, buffer_size);
    }
    if (encrypt)
    {
        buffer_size = rdp_encode_data(buffer, buffer_size);
    }

    char_sent = rdplib_platform_send_datagram(socket, (const uint8_t *)buffer, (uint32_t)buffer_size, (const uint8_t *)remote_addr);
    if (char_sent != buffer_size)
    {
        return rdplib_usend_socket_error_is_retryable(rdplib_platform_last_socket_error(), char_sent) ? 5u : 1u;
    }
    return 0;
}

static uint32_t rdplib_usend_with_callback(connection_t *connection, intptr_t socket, iov_t *iov, uint32_t iov_len, struct sockaddr *remote_addr, uint32_t encrypt, uint32_t crc, uint32_t buffer_size)
{
    char buffer[USEND_MAX_SIZE];
    uint32_t copied_size;
    uint32_t i;

    copied_size = 0;
    for (i = 0; i < iov_len; ++i)
    {
        if (iov[i].size != 0)
        {
            memcpy(buffer + copied_size, iov[i].data, iov[i].size);
            copied_size += iov[i].size;
        }
    }
    if (connection->rdplib_packet_drop_callback(connection->rdplib_packet_drop_context, RDPLIB_PACKET_DROP_OUTBOUND, (const uint8_t *)buffer, buffer_size))
    {
        return 0;
    }
    return rdplib_usend_send_joined(socket, buffer, (int32_t)buffer_size, remote_addr, encrypt, crc);
}

uint32_t rdplib_usend(connection_t *connection, intptr_t socket, iov_t *iov, uint32_t iov_len, struct sockaddr *remote_addr, uint32_t encrypt, uint32_t crc)
{
    uint32_t buffer_size;
    uint32_t result;

    result = rdplib_usend_validate(iov, iov_len, remote_addr, encrypt, crc, &buffer_size);
    if (result)
    {
        return result;
    }

    if (connection && connection->rdplib_packet_drop_callback)
    {
        return rdplib_usend_with_callback(connection, socket, iov, iov_len, remote_addr, encrypt, crc, buffer_size);
    }

    return usend(socket, iov, iov_len, remote_addr, encrypt, crc);
}
#endif
