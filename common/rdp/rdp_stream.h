#ifndef RDP_STREAM_H
#define RDP_STREAM_H

#include "../types.h"

#include "position_update_batch.h"
#include "rdp_connection.h"

#include <memory>

class EQApplicationPacket;
class EQPacketTranslator;

// This is the EQ packet stream interface between the network transport and the game application.
// It uses an EQPacketTranslator to map internal/external opcodes and do arbitrary game packet encodes/decodes.
// For normal gameplay Send/Receive/Close are all that's needed, the other methods here are for setup/diagnostics.
// Owns one RDPConnection and borrows its packet translator.  Its endpoint and packet translator must outlive it.
class RDPStream
{
public:
	using ConnectionSnapshot = RDPConnection::Snapshot;

	enum ReceiveResult
	{
		NoData,
		PacketReceived,
		PeerClosed,
		ConnectionLost
	};

	RDPStream(EQPacketTranslator &translator, std::unique_ptr<RDPConnection> connection);
	~RDPStream();

	RDPStream(const RDPStream &) = delete;
	RDPStream &operator=(const RDPStream &) = delete;

	// send game packets on the same stream number used by the client.
	static constexpr uint32 SendStreamNumber = 1;

	// the rate is bytes per second and the buffer is queued bytes, not guaranteed reliable ID space
	static constexpr uint32 DefaultDataRate = 1024 * 400; // 400 KiB/s - there is little benefit to going higher, it just becomes more bursty
	static constexpr uint32 DefaultSendBufferSize = 1048576; // 1 MiB - this is an upper limit on the send queue before the connection is dropped
	static constexpr uint32 DefaultLingerTimeout = 1000; // matches the client's normal connection close timeout
	static constexpr uint32 DefaultKeepaliveInterval = RDPLIB_DEFAULT_KEEPALIVE_INTERVAL_MS; // 10 seconds, client and AK server both used this value

	// client enables this in its connect options.  accepted connections start with it disabled but the real EQ servers enabled this.
	int EnableKeepalive();
	// this overload is available in the normal rdplib build.  the source faithful build returns RDPLIB_ERROR_NOT_SUPPORTED.
	int EnableKeepalive(uint32 interval_ms);

	// client uses 5120 (5 KiB/s) but it's only sending its own traffic.
	int SetDataRate(uint32 bytes_per_second = DefaultDataRate);

	// client uses a 256 KiB buffer.
	int SetSendBufferSize(uint32 bytes = DefaultSendBufferSize);

	// takes ownership of *packet and clears the caller's pointer.  The first packet sent in either direction should be reliable.
	int Send(EQApplicationPacket **packet, bool reliable = true);

	// sends a partial movement batch.  the zone calls this once after it finishes producing packets for the current tick.
	int FlushPositionUpdates();

	// this does not block.  PacketReceived gives ownership of *packet to the caller and ConnectionLost optionally returns the rdplib disconnect reason
	ReceiveResult Receive(EQApplicationPacket **packet, uint32 *disconnect_reason = nullptr);

	// Releases this stream's connection handle and returns immediately.
	// A nonzero linger leaves the connection with its endpoint to send FIN until the deadline.  Zero removes it locally without sending FIN.  Calls may be repeated.
	void Close(uint32 linger_timeout_ms = DefaultLingerTimeout);

	int GetRemoteAddress(uint8 address[4], uint16 &port) const;
	int GetStatistics(rdplib_connection_perf_stats_t &statistics) const;
	int GetConnectionSnapshot(ConnectionSnapshot &snapshot) const;

	// The callback runs under the connection lock during an immediate send or on the RDP I/O thread.  Passing nullptr waits for an active call and removes it.
	int SetPacketDropCallback(rdplib_packet_drop_callback_t callback, void *context = nullptr);

	void SetTranslator(EQPacketTranslator &translator) { m_translator = &translator; }
	EQPacketTranslator *GetTranslator() const { return m_translator; }

private:
	bool IsBatchablePositionUpdate(const EQApplicationPacket *packet, bool reliable) const;

	EQPacketTranslator *m_translator;
	std::unique_ptr<RDPConnection> m_connection;
	PositionUpdateBatch m_position_update_batch;
};

#endif
