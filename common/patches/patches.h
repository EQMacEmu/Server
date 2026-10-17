#ifndef PATCHES_H_
#define PATCHES_H_

#include "../emu_versions.h"
#include "../eq_packet_translator.h"

namespace Patches {
	bool LoadAll();
	EQPacketTranslator &GetTranslator(EQ::versions::ClientVersion version);
	EQPacketTranslator &GetTranslatorByBit(uint32 version_bit);
}

#endif /* PATCHES_H_ */
