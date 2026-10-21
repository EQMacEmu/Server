#include "rdp_stream.h"

#include "../eq_packet.h"
#include "../eq_packet_translator.h"
#include "../eqemu_logsys.h"
#include "../opcodemgr.h"

#include <cstring>
#include <new>
#include <utility>

RDPStream::RDPStream(EQPacketTranslator &translator, std::unique_ptr<RDPConnection> connection)
	: m_translator(&translator),
	  m_connection(std::move(connection)),
	  m_position_update_batch()
{
}

RDPStream::~RDPStream()
{
	Close(0);
}

int RDPStream::EnableKeepalive()
{
	return m_connection != nullptr ? m_connection->EnableKeepalive() : RDPLIB_ERROR_NOT_USABLE;
}

int RDPStream::EnableKeepalive(uint32 interval_ms)
{
	return m_connection != nullptr ? m_connection->EnableKeepalive(interval_ms) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPStream::SetDataRate(uint32 bytes_per_second)
{
	return m_connection != nullptr ? m_connection->SetDataRate(bytes_per_second) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPStream::SetSendBufferSize(uint32 bytes)
{
	return m_connection != nullptr ? m_connection->SetSendBufferSize(bytes) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPStream::Send(EQApplicationPacket **packet, bool reliable)
{
	if (packet == nullptr || *packet == nullptr)
		return RDPLIB_ERROR_INVALID_ARGUMENT;

	// translate packet emu -> eq
	EQPacketEncodeResult encode_result;
	m_translator->Encode(packet, &encode_result, reliable);
	if (encode_result.Packets().empty())
		return RDPLIB_OK;

	int final_result = RDPLIB_OK;

	for (const auto &item : encode_result.Packets())
	{
		const EQApplicationPacket *encoded = item.packet;
		if (encoded == nullptr)
			continue;

		bool pkt_reliable = item.reliable;

		// intercept 1 count OP_MobUpdate packets for batching
		if (IsBatchablePositionUpdate(encoded, pkt_reliable))
		{
			if (m_connection == nullptr)
				return RDPLIB_ERROR_NOT_USABLE;

			const SpawnPositionUpdates_Struct *updates = reinterpret_cast<const SpawnPositionUpdates_Struct *>(encoded->pBuffer);
			if (!m_position_update_batch.Append(updates->spawn_update))
			{
				int result = FlushPositionUpdates();
				if (result != RDPLIB_OK)
					return result;

				(void)m_position_update_batch.Append(updates->spawn_update);
			}

			// one batch can hold 33 updates
			int result = m_position_update_batch.Full() ? FlushPositionUpdates() : RDPLIB_OK;
			if (result != RDPLIB_OK)
				final_result = result;
			continue;
		}

		// opcode bypass allows specifying the opcode directly
		uint16 opcode = encoded->GetOpcodeBypass();

		// map opcode emu -> eq
		if (opcode == 0)
			opcode = m_translator->EmuToEQ(encoded->GetOpcode());
		if (opcode == 0 || opcode == 0xffff) {
			LogError("RDPStream::Send: opcode {:#06x} ({}) translated to invalid EQ opcode {:#06x}", encoded->GetOpcode(), OpcodeManager::EmuToName(encoded->GetOpcode()), opcode);
			return RDPLIB_CONNECTION_SEND_INVALID_ARGUMENT;
		}
		if (encoded->size > UINT32_MAX - 2)
			return RDPLIB_CONNECTION_SEND_PAYLOAD_TOO_LARGE;

		// assemble payload buffer
		uint32 bytes = encoded->size + 2;
		uint8 *data = new (std::nothrow) uint8[bytes];
		if (data == nullptr)
			return RDPLIB_CONNECTION_SEND_ALLOCATION_FAILED;

		// EQ application opcodes are little-endian. RDP transport headers use a
		// separate big-endian encoding inside RDPConnection.
		data[0] = static_cast<uint8>(opcode);
		data[1] = static_cast<uint8>(opcode >> 8);

		if (encoded->size != 0)
			std::memcpy(data + 2, encoded->pBuffer, encoded->size);

		// discard return value, we aren't going to retry it
		(void)FlushPositionUpdates();

		// queue the translated/mapped packet in the transport
		// if ack history is exhausted or a reliable send finds the send queue full,
		// RDPConnection latches the failure and reports it on the next Receive().
		// this eventually causes the client to be linkdead in zone
		int result = m_connection != nullptr ? m_connection->Send(data, bytes, SendStreamNumber, pkt_reliable ? RDPLIB_SEND_RELIABLE : RDPLIB_SEND_UNRELIABLE) : RDPLIB_ERROR_NOT_USABLE;

		LogInfo("RDPStream::Send: emu opcode {:#06x} ({}), EQ opcode {:#06x}, bytes {}, reliable {} -> result {}",
			encoded->GetOpcode(), OpcodeManager::EmuToName(encoded->GetOpcode()), opcode, bytes, pkt_reliable, result);

		if (result != RDPLIB_OK) {
			LogInfo("RDPStream::Send NON-OK RESULT: result {}, emu opcode {:#06x} ({}), EQ opcode {:#06x}, bytes {}, reliable {}",
				result, encoded->GetOpcode(), OpcodeManager::EmuToName(encoded->GetOpcode()), opcode, bytes, pkt_reliable);
			final_result = result;
		}

		delete[] data;
		if (result != RDPLIB_OK)
			return result;
	}

	return final_result;
}

// this matches 1 count OP_MobUpdate packets
bool RDPStream::IsBatchablePositionUpdate(const EQApplicationPacket *packet, bool reliable) const
{
	if (packet == nullptr || reliable || packet->GetOpcodeBypass() != 0 || packet->GetOpcode() != OP_MobUpdate)
		return false;
	if (packet->pBuffer == nullptr || packet->size != sizeof(SpawnPositionUpdates_Struct))
		return false;

	const SpawnPositionUpdates_Struct *updates = reinterpret_cast<const SpawnPositionUpdates_Struct *>(packet->pBuffer);
	return updates->num_updates == 1;
}

int RDPStream::FlushPositionUpdates()
{
	if (m_position_update_batch.Empty())
		return RDPLIB_OK;

	// map opcode emu -> eq
	uint16 opcode = m_translator->EmuToEQ(OP_MobUpdate);
	if (opcode == 0 || opcode == 0xffff)
	{
		m_position_update_batch.Clear();
		return RDPLIB_CONNECTION_SEND_INVALID_ARGUMENT;
	}

	// send the combined batch
	uint8 data[PositionUpdateBatch::MaximumMessageBytes];
	uint32 bytes = m_position_update_batch.Write(data, opcode);
	int result = m_connection != nullptr
		? m_connection->Send(data, bytes, SendStreamNumber, RDPLIB_SEND_UNRELIABLE)
		: RDPLIB_ERROR_NOT_USABLE;

	// queued movement is stale if the connection could not accept it.
	m_position_update_batch.Clear();
	return result;
}

RDPStream::ReceiveResult RDPStream::Receive(EQApplicationPacket **packet, uint32 *disconnect_reason)
{
	if (packet == nullptr)
		return NoData;

	*packet = nullptr;
	if (disconnect_reason != nullptr)
		*disconnect_reason = 0;

	if (m_connection == nullptr)
		return NoData;

	for (;;)
	{
		// pop a message if we have one waiting, also reports if transport connection is lost
		RDPMessage message;
		RDPConnection::ReceiveResult result = m_connection->Receive(&message, disconnect_reason);

		if (result == RDPConnection::NoData)
			return NoData;
		if (result == RDPConnection::PeerClosed) {
			LogInfo("RDPStream::Receive: PeerClosed (disconnect_reason={:#010x})", disconnect_reason ? *disconnect_reason : 0);
			return PeerClosed;
		}
		if (result == RDPConnection::ConnectionLost) {
			LogInfo("RDPStream::Receive: ConnectionLost (disconnect_reason={:#010x})", disconnect_reason ? *disconnect_reason : 0);
			return ConnectionLost;
		}

		uint32 bytes = message.Size();
		const uint8 *data = message.Data();

		// the application payload has to contain at least an opcode
		if (bytes < 2 || data == nullptr)
			continue;

		// EQ application opcodes are little-endian. RDP transport headers are
		// decoded separately by RDPConnection.
		uint16 opcode = static_cast<uint16>(data[0] | (static_cast<uint16>(data[1]) << 8));
		if (opcode == 0 || opcode == 0xffff)
			continue;

		// map opcode eq -> emu
		EQApplicationPacket *application_packet = new EQApplicationPacket(m_translator->EQToEmu(opcode), data + 2, bytes - 2);
		application_packet->SetProtocolOpcode(opcode);

		LogInfo("RDPStream::Receive: bytes {}, stream {}, flags {:#06x}, EQ opcode {:#06x}, emu opcode [{}] ({:#06x})",
			bytes, message.StreamNumber(), message.Flags(), opcode, OpcodeManager::EmuToName(application_packet->GetOpcode()), application_packet->GetOpcode());

		// translate packet eq -> emu
		m_translator->Decode(application_packet);

		*packet = application_packet;
		return PacketReceived;
	}
}

void RDPStream::Close(uint32 linger_timeout_ms)
{
	m_position_update_batch.Clear();

	if (m_connection == nullptr)
		return;

	m_connection->Close(linger_timeout_ms);
}

int RDPStream::GetRemoteAddress(uint8 address[4], uint16 &port) const
{
	return m_connection != nullptr ? m_connection->GetRemoteAddress(address, port) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPStream::GetStatistics(rdplib_connection_perf_stats_t &statistics) const
{
	return m_connection != nullptr ? m_connection->GetStatistics(statistics) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPStream::GetConnectionSnapshot(ConnectionSnapshot &snapshot) const
{
	return m_connection != nullptr ? m_connection->GetSnapshot(snapshot) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPStream::SetPacketDropCallback(rdplib_packet_drop_callback_t callback, void *context)
{
	return m_connection != nullptr ? m_connection->SetPacketDropCallback(callback, context) : RDPLIB_ERROR_NOT_USABLE;
}
