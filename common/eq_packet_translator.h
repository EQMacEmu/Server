#ifndef EQ_PACKET_TRANSLATOR_H
#define EQ_PACKET_TRANSLATOR_H

#include "emu_opcodes.h"
#include "opcodemgr.h"

#include <vector>

class EQApplicationPacket;

class EQPacketEncodeResult
{
public:
	struct EncodedPacket {
		EQApplicationPacket *packet;
		bool reliable;
	};

	EQPacketEncodeResult();
	~EQPacketEncodeResult();

	EQPacketEncodeResult(const EQPacketEncodeResult &) = delete;
	EQPacketEncodeResult &operator=(const EQPacketEncodeResult &) = delete;

	// takes ownership of *packet and clears the caller's pointer
	void SetPacket(EQApplicationPacket **packet, bool reliable = true);
	const std::vector<EncodedPacket> &Packets() const
	{
		return m_packets;
	}
	const EQApplicationPacket *Packet() const
	{
		return m_packets.empty() ? nullptr : m_packets.front().packet;
	}
	bool Reliable() const
	{
		return m_packets.empty() ? true : m_packets.front().reliable;
	}

private:
	std::vector<EncodedPacket> m_packets;
};

class EQPacketTranslator
{
public:
	typedef void (*Encoder)(EQApplicationPacket **packet, EQPacketEncodeResult *result, bool reliable);
	typedef void (*Decoder)(EQApplicationPacket *packet);

	EQPacketTranslator();
	EQPacketTranslator(const EQPacketTranslator &) = delete;
	EQPacketTranslator &operator=(const EQPacketTranslator &) = delete;

	bool LoadOpcodes(const char *filename, bool report_errors = false);

	uint16 EmuToEQ(EmuOpcode opcode);
	EmuOpcode EQToEmu(uint16 opcode);

	// encoder owns *packet and either adds an output packet or drops it
	void Encode(EQApplicationPacket **packet, EQPacketEncodeResult *result, bool reliable = true) const;
	// decoder keeps the same EQApplicationPacket object but can replace its pBuffer and size with the translated server structure.
	void Decode(EQApplicationPacket *packet) const;

	void SetEncoder(EmuOpcode opcode, Encoder encoder);
	void SetDecoder(EmuOpcode opcode, Decoder decoder);

private:
	static bool ValidOpcode(EmuOpcode opcode);
	static void IdentityEncoder(EQApplicationPacket **packet, EQPacketEncodeResult *result, bool reliable);
	static void IdentityDecoder(EQApplicationPacket *packet);

	RegularOpcodeManager m_opcodes;
	Encoder m_encoders[_maxEmuOpcode];
	Decoder m_decoders[_maxEmuOpcode];
};

#endif
