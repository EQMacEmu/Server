#include "eq_packet_translator.h"

#include "eq_packet.h"
#include "eqemu_logsys.h"

#include <cassert>

EQPacketEncodeResult::EQPacketEncodeResult()
{
}

EQPacketEncodeResult::~EQPacketEncodeResult()
{
	for (auto &p : m_packets)
	{
		delete p.packet;
	}
}

void EQPacketEncodeResult::SetPacket(EQApplicationPacket **packet, bool reliable)
{
	if (packet == nullptr || *packet == nullptr)
		return;

	m_packets.push_back({ *packet, reliable });
	*packet = nullptr;
}

EQPacketTranslator::EQPacketTranslator()
{
	for (int i = 0; i < _maxEmuOpcode; ++i)
	{
		m_encoders[i] = IdentityEncoder;
		m_decoders[i] = IdentityDecoder;
	}
}

bool EQPacketTranslator::LoadOpcodes(const char *filename, bool report_errors)
{
	return m_opcodes.LoadOpcodes(filename, report_errors);
}

uint16 EQPacketTranslator::EmuToEQ(EmuOpcode opcode)
{
	if (!ValidOpcode(opcode))
		return 0;

	return m_opcodes.EmuToEQ(opcode);
}

EmuOpcode EQPacketTranslator::EQToEmu(uint16 opcode)
{
	return m_opcodes.EQToEmu(opcode);
}

void EQPacketTranslator::Encode(EQApplicationPacket **packet, EQPacketEncodeResult *result, bool reliable) const
{
	if (packet == nullptr || *packet == nullptr || result == nullptr)
		return;

	if ((*packet)->GetOpcodeBypass() != 0)
	{
		IdentityEncoder(packet, result, reliable);
		return;
	}

	EmuOpcode opcode = (*packet)->GetOpcode();
	if (!ValidOpcode(opcode))
	{
		LogNetcode("[STRUCTS] Invalid outbound opcode [{}]. Dropping.", static_cast<int>(opcode));
		delete *packet;
		*packet = nullptr;
		return;
	}

	m_encoders[opcode](packet, result, reliable);
}

void EQPacketTranslator::Decode(EQApplicationPacket *packet) const
{
	if (packet == nullptr)
		return;

	EmuOpcode opcode = packet->GetOpcode();
	if (!ValidOpcode(opcode))
	{
		LogNetcode("[STRUCTS] Invalid inbound opcode [{}].", static_cast<int>(opcode));
		packet->SetOpcode(OP_Unknown);
		return;
	}

	m_decoders[opcode](packet);
}

void EQPacketTranslator::SetEncoder(EmuOpcode opcode, Encoder encoder)
{
	if (!ValidOpcode(opcode))
		return;

	m_encoders[opcode] = encoder != nullptr ? encoder : IdentityEncoder;
}

void EQPacketTranslator::SetDecoder(EmuOpcode opcode, Decoder decoder)
{
	if (!ValidOpcode(opcode))
		return;

	m_decoders[opcode] = decoder != nullptr ? decoder : IdentityDecoder;
}

bool EQPacketTranslator::ValidOpcode(EmuOpcode opcode)
{
	return static_cast<unsigned int>(opcode) < static_cast<unsigned int>(_maxEmuOpcode);
}

void EQPacketTranslator::IdentityEncoder(EQApplicationPacket **packet, EQPacketEncodeResult *result, bool reliable)
{
	result->SetPacket(packet, reliable);
}

void EQPacketTranslator::IdentityDecoder(EQApplicationPacket *packet)
{
}
