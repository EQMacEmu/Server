/*	EQEMu:  Everquest Server Emulator
	
	Copyright (C) 2001-2016 EQEMu Development Team (http://eqemulator.net)
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "emu_versions.h"

bool EQ::versions::IsValidClientVersion(ClientVersion client_version)
{
	if (client_version <= ClientVersion::Unknown || client_version > LastClientVersion)
		return false;

	return true;
}

EQ::versions::ClientVersion EQ::versions::ValidateClientVersion(ClientVersion client_version)
{
	if (client_version <= ClientVersion::Unknown || client_version > LastClientVersion)
		return ClientVersion::Unknown;

	return client_version;
}

const char* EQ::versions::ClientVersionName(ClientVersion client_version)
{
	switch (client_version) {
	case ClientVersion::Unknown:
		return "Unknown Version";
	case ClientVersion::Unused:
		return "Unused";
	case ClientVersion::MacPC:
		return "PC Version";
	case ClientVersion::MacIntel:
		return "Intel Version";
	case ClientVersion::MacPPC:
		return "PPC Version";
	case ClientVersion::Mac:
		return "All Clients";
	default:
		return "Invalid Version";
	};
}

uint32 EQ::versions::ConvertClientVersionToClientVersionBit(ClientVersion client_version)
{
	switch (client_version) {
	case ClientVersion::Unknown:
		return bit_Unknown;
	case ClientVersion::Unused:
		return bit_Unused;
	case ClientVersion::MacPC:
		return bit_MacPC;
	case ClientVersion::MacIntel:
		return bit_MacIntel;
	case ClientVersion::MacPPC:
		return bit_MacPPC;
	case ClientVersion::Mac:
		return bit_Mac;
	default:
		return bit_Unknown;
	};
}

EQ::versions::ClientVersion EQ::versions::ConvertClientVersionBitToClientVersion(uint32 client_version_bit)
{
	switch (client_version_bit) {
		case (uint32)static_cast<unsigned int>(ClientVersion::Unknown) :
		case ((uint32)1 << (static_cast<unsigned int>(ClientVersion::Unused) - 1)):
			return ClientVersion::Unused;
		case ((uint32)1 << (static_cast<unsigned int>(ClientVersion::MacPC) - 1)):
			return ClientVersion::MacPC;
		case ((uint32)1 << (static_cast<unsigned int>(ClientVersion::MacIntel) - 1)):
			return ClientVersion::MacIntel;
		case ((uint32)1 << (static_cast<unsigned int>(ClientVersion::MacPPC) - 1)):
			return ClientVersion::MacPPC;
		case ((uint32)1 << (static_cast<unsigned int>(ClientVersion::Mac) - 1)):
			return ClientVersion::Mac;
		default:
			return ClientVersion::Unknown;
	}
}

bool EQ::versions::IsValidMobVersion(MobVersion Mob_version)
{
	if (Mob_version <= MobVersion::Unknown || Mob_version > LastMobVersion)
		return false;

	return true;
}

bool EQ::versions::IsValidPCMobVersion(MobVersion Mob_version)
{
	if (Mob_version <= MobVersion::Unknown || Mob_version > LastPCMobVersion)
		return false;

	return true;
}

bool EQ::versions::IsValidNonPCMobVersion(MobVersion Mob_version)
{
	if (Mob_version <= LastPCMobVersion || Mob_version > LastNonPCMobVersion)
		return false;

	return true;
}

bool EQ::versions::IsValidOfflinePCMobVersion(MobVersion Mob_version)
{
	if (Mob_version <= LastNonPCMobVersion || Mob_version > LastOfflinePCMobVersion)
		return false;

	return true;
}


EQ::versions::MobVersion EQ::versions::ValidateMobVersion(MobVersion Mob_version)
{
	if (Mob_version <= MobVersion::Unknown || Mob_version > LastMobVersion)
		return MobVersion::Unknown;

	return Mob_version;
}

EQ::versions::MobVersion EQ::versions::ValidatePCMobVersion(MobVersion Mob_version)
{
	if (Mob_version <= MobVersion::Unknown || Mob_version > LastPCMobVersion)
		return MobVersion::Unknown;

	return Mob_version;
}

EQ::versions::MobVersion EQ::versions::ValidateNonPCMobVersion(MobVersion Mob_version)
{
	if (Mob_version <= LastPCMobVersion || Mob_version > LastNonPCMobVersion)
		return MobVersion::Unknown;

	return Mob_version;
}

EQ::versions::MobVersion EQ::versions::ValidateOfflinePCMobVersion(MobVersion Mob_version)
{
	if (Mob_version <= LastNonPCMobVersion || Mob_version > LastOfflinePCMobVersion)
		return MobVersion::Unknown;

	return Mob_version;
}

const char* EQ::versions::MobVersionName(MobVersion Mob_version)
{
	switch (Mob_version) {
	case MobVersion::Unknown:
		return "Unknown Version";
	case MobVersion::Unused:
		return "Unused";
	case MobVersion::Mac:
		return "Mac";
	case MobVersion::NPC:
		return "NPC";
	case MobVersion::NPCMerchant:
		return "NPC Merchant";
	case MobVersion::ClientPet:
		return "Client Pet";
	case MobVersion::NPCPet:
		return "NPC Pet";
	case MobVersion::OfflineMac:
		return "Offline Mac";
	default:
		return "Invalid Version";
	};
}

EQ::versions::ClientVersion EQ::versions::ConvertMobVersionToClientVersion(MobVersion Mob_version)
{
	switch (Mob_version) {
	case MobVersion::Unknown:
	case MobVersion::Unused:
		return ClientVersion::Unused;
	case MobVersion::Mac:
		return ClientVersion::Mac;
	default:
		return ClientVersion::Unknown;
	}
}

EQ::versions::MobVersion EQ::versions::ConvertClientVersionToMobVersion(ClientVersion client_version)
{
	switch (client_version) {
	case ClientVersion::Unknown:
	case ClientVersion::Unused:
		return MobVersion::Unused;
	case ClientVersion::Mac:
		return MobVersion::Mac;
	default:
		return MobVersion::Unknown;
	}
}

EQ::versions::MobVersion EQ::versions::ConvertPCMobVersionToOfflinePCMobVersion(MobVersion Mob_version)
{
	switch (Mob_version) {
	case MobVersion::Mac:
		return MobVersion::OfflineMac;
	default:
		return MobVersion::Unknown;
	}
}

EQ::versions::MobVersion EQ::versions::ConvertOfflinePCMobVersionToPCMobVersion(MobVersion Mob_version)
{
	switch (Mob_version) {
	case MobVersion::OfflineMac:
		return MobVersion::Mac;
	default:
		return MobVersion::Unknown;
	}
}

EQ::versions::ClientVersion EQ::versions::ConvertOfflinePCMobVersionToClientVersion(MobVersion Mob_version)
{
	switch (Mob_version) {
	case MobVersion::OfflineMac:
		return ClientVersion::Mac;
	default:
		return ClientVersion::Unknown;
	}
}

EQ::versions::MobVersion EQ::versions::ConvertClientVersionToOfflinePCMobVersion(ClientVersion client_version)
{
	switch (client_version) {
	case ClientVersion::Mac:
		return MobVersion::OfflineMac;
	default:
		return MobVersion::Unknown;
	}
}