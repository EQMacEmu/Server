#include "patches.h"
#include "mac.h"
#include "trilogy.h"
#include "../path_manager.h"
#include "../eqemu_logsys.h"

namespace Patches {
	static EQPacketTranslator mac_translator;
	static EQPacketTranslator trilogy_translator;
	static bool initialized = false;

	bool LoadAll()
	{
		if (initialized)
			return true;

		std::string mac_opcode_file = fmt::format("{}/patch_Mac.conf", PathManager::Instance()->GetPatchPath());
		if (!mac_translator.LoadOpcodes(mac_opcode_file.c_str()))
		{
			LogError("Failed to load opcode file [{}]", mac_opcode_file);
			return false;
		}
		Mac::Register(mac_translator);

		std::string trilogy_opcode_file = fmt::format("{}/patch_Trilogy.conf", PathManager::Instance()->GetPatchPath());
		if (!trilogy_translator.LoadOpcodes(trilogy_opcode_file.c_str()))
		{
			LogError("Failed to load opcode file [{}]", trilogy_opcode_file);
			return false;
		}
		Trilogy::Register(trilogy_translator);

		initialized = true;
		return true;
	}

	EQPacketTranslator &GetTranslator(EQ::versions::ClientVersion version)
	{
		if (version == EQ::versions::ClientVersion::Trilogy)
			return trilogy_translator;
		return mac_translator;
	}

	EQPacketTranslator &GetTranslatorByBit(uint32 version_bit)
	{
		if (version_bit == EQ::versions::ClientVersionBit::bit_Trilogy)
			return trilogy_translator;
		return mac_translator;
	}
}
