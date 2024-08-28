/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2022 EQEMu Development Team (http://eqemu.org)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	*/
#include <fmt/format.h>
#include "../common/global_define.h"
#include "../common/classes.h"
#include "data_verification.h"

const char* GetClassIDName(uint8 class_id, uint8 level)
{
	switch(class_id) {
		case Class::Warrior:
			if (level >= 65)
				return "Overlord"; //Baron-Sprite: LEAVE MY CLASSES ALONE.
			else if (level >= 60)
				return "Warlord";
			else if (level >= 55)
				return "Myrmidon";
			else if (level >= 51)
				return "Champion";
			else
				return "Warrior";
		case Class::Cleric:
			if (level >= 65)
				return "Archon";
			else if (level >= 60)
				return "High Priest";
			else if (level >= 55)
				return "Templar";
			else if (level >= 51)
				return "Vicar";
			else
				return "Cleric";
		case Class::Paladin:
			if (level >= 65)
				return "Lord Protector";
			else if (level >= 60)
				return "Crusader";
			else if (level >= 55)
				return "Knight";
			else if (level >= 51)
				return "Cavalier";
			else
				return "Paladin";
		case Class::Ranger:
			if (level >= 65)
				return "Forest Stalker";
			else if (level >= 60)
				return "Warder";
			else if (level >= 55)
				return "Outrider";
			else if (level >= 51)
				return "Pathfinder";
			else
				return "Ranger";
		case Class::ShadowKnight:
			if (level >= 65)
				return "Dread Lord";
			else if (level >= 60)
				return "Grave Lord";
			else if (level >= 55)
				return "Revenant";
			else if (level >= 51)
				return "Reaver";
			else
				return "Shadowknight";
		case Class::Druid:
			if (level >= 65)
				return "Storm Warden";
			else if (level >= 60)
				return "Hierophant";
			else if (level >= 55)
				return "Preserver";
			else if (level >= 51)
				return "Wanderer";
			else
				return "Druid";
		case Class::Monk:
			if (level >= 65)
				return "Transcendent";
			else if (level >= 60)
				return "Grandmaster";
			else if (level >= 55)
				return "Master";
			else if (level >= 51)
				return "Disciple";
			else
				return "Monk";
		case Class::Bard:
			if (level >= 65)
				return "Maestro";
			else if (level >= 60)
				return "Virtuoso";
			else if (level >= 55)
				return "Troubadour";
			else if (level >= 51)
				return "Minstrel";
			else
				return "Bard";
		case Class::Rogue:
			if (level >= 65)
				return "Deceiver";
			else if (level >= 60)
				return "Assassin";
			else if (level >= 55)
				return "Blackguard";
			else if (level >= 51)
				return "Rake";
			else
				return "Rogue";
		case Class::Shaman:
			if (level >= 65)
				return "Prophet";
			else if (level >= 60)
				return "Oracle";
			else if (level >= 55)
				return "Luminary";
			else if (level >= 51)
				return "Mystic";
			else
				return "Shaman";
		case Class::Necromancer:
			if (level >= 65)
				return "Arch Lich";
			else if (level >= 60)
				return "Warlock";
			else if (level >= 55)
				return "Defiler";
			else if (level >= 51)
				return "Heretic";
			else
				return "Necromancer";
		case Class::Wizard:
			if (level >= 65)
				return "Arcanist";
			else if (level >= 60)
				return "Sorcerer";
			else if (level >= 55)
				return "Evoker";
			else if (level >= 51)
				return "Channeler";
			else
				return "Wizard";
		case Class::Magician:
			if (level >= 65)
				return "Arch Convoker";
			else if (level >= 60)
				return "Arch Mage";
			else if (level >= 55)
				return "Conjurer";
			if (level >= 51)
				return "Elementalist";
			else
				return "Magician";
		case Class::Enchanter:
			if (level >= 65)
				return "Coercer";
			else if (level >= 60)
				return "Phantasmist";
			else if (level >= 55)
				return "Beguiler";
			else if (level >= 51)
				return "Illusionist";
			else
				return "Enchanter";
		case Class::Beastlord:
			if (level >= 65)
				return "Feral Lord";
			else if (level >= 60)
				return "Savage Lord";
			else if (level >= 55)
				return "Animist";
			else if (level >= 51)
				return "Primalist";
			else
				return "Beastlord";
		case Class::Banker:
				return "Banker";
		case Class::WarriorGM:
			return "Warrior Guildmaster";
		case Class::ClericGM:
			return "Cleric Guildmaster";
		case Class::PaladinGM:
			return "Paladin Guildmaster";
		case Class::RangerGM:
			return "Ranger Guildmaster";
		case Class::ShadowKnightGM:
			return "Shadowknight Guildmaster";
		case Class::DruidGM:
			return "Druid Guildmaster";
		case Class::MonkGM:
			return "Monk Guildmaster";
		case Class::BardGM:
			return "Bard Guildmaster";
		case Class::RogueGM:
			return "Rogue Guildmaster";
		case Class::ShamanGM:
			return "Shaman Guildmaster";
		case Class::NecromancerGM:
			return "Necromancer Guildmaster";
		case Class::WizardGM:
			return "Wizard Guildmaster";
		case Class::MagicianGM:
			return "Magician Guildmaster";
		case  Class::EnchanterGM:
			return "Enchanter Guildmaster";
		case Class::BeastlordGM:
			return "Beastlord Guildmaster";
		case Class::Merchant:
			return "Merchant";
		case Class::CorpseClass:
			return "Corpse Class";
		default:
			return "Unknown";
	}
}

uint8 GetPlayerClassValue(uint8 class_id)
{
	if (!IsPlayerClass(class_id)) {
		return 0;
	}

	return class_id;
}

uint16 GetPlayerClassBit(uint8 class_id)
{
	if (!IsPlayerClass(class_id)) {
		return 0;
	}

	return player_class_bitmasks[class_id];
}

bool IsFighterClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Warrior:
	case Class::Paladin:
	case Class::Ranger:
	case Class::ShadowKnight:
	case Class::Monk:
	case Class::Bard:
	case Class::Rogue:
	case Class::Beastlord:
		return true;
	default:
		return false;
	}
}

bool IsSpellFighterClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Paladin:
	case Class::Ranger:
	case Class::ShadowKnight:
	case Class::Beastlord:
		return true;
	default:
		return false;
	}
}

bool IsNonSpellFighterClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Warrior:
	case Class::Monk:
	case Class::Bard:
	case Class::Rogue:
		return true;
	default:
		return false;
	}
}

bool IsHybridClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Paladin:
	case Class::Ranger:
	case Class::ShadowKnight:
	case Class::Bard:
	case Class::Beastlord:
		return true;
	default:
		return false;
	}
}

bool IsCasterClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Cleric:
	case Class::Druid:
	case Class::Shaman:
	case Class::Necromancer:
	case Class::Wizard:
	case Class::Magician:
	case Class::Enchanter:
		return true;
	default:
		return false;
	}
}

bool IsINTCasterClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Necromancer:
	case Class::Wizard:
	case Class::Magician:
	case Class::Enchanter:
		return true;
	default:
		return false;
	}
}

bool IsWISCasterClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Cleric:
	case Class::Druid:
	case Class::Shaman:
		return true;
	default:
		return false;
	}
}

bool IsPlateClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Warrior:
	case Class::Cleric:
	case Class::Paladin:
	case Class::ShadowKnight:
	case Class::Bard:
		return true;
	default:
		return false;
	}
}

bool IsChainClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Ranger:
	case Class::Rogue:
	case Class::Shaman:
		return true;
	default:
		return false;
	}
}

bool IsLeatherClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Druid:
	case Class::Monk:
	case Class::Beastlord:
		return true;
	default:
		return false;
	}
}

bool IsClothClass(uint8 class_id)
{
	switch (class_id) {
	case Class::Necromancer:
	case Class::Wizard:
	case Class::Magician:
	case Class::Enchanter:
		return true;
	default:
		return false;
	}
}

uint8 ClassArmorType(uint8 class_id)
{
	if (IsChainClass(class_id)) {
		return ARMOR_TYPE_CHAIN;
	}
	else if (IsClothClass(class_id)) {
		return ARMOR_TYPE_CLOTH;
	}
	else if (IsLeatherClass(class_id)) {
		return ARMOR_TYPE_LEATHER;
	}
	else if (IsPlateClass(class_id)) {
		return ARMOR_TYPE_PLATE;
	}

	return ARMOR_TYPE_UNKNOWN;
}

const std::string GetPlayerClassAbbreviation(uint8 class_id)
{
	if (!EQ::ValueWithin(class_id, Class::Warrior, Class::Beastlord)) {
		return std::string("UNK");
	}

	switch (class_id) {
	case Class::Warrior:
		return "WAR";
	case Class::Cleric:
		return "CLR";
	case Class::Paladin:
		return "PAL";
	case Class::Ranger:
		return "RNG";
	case Class::ShadowKnight:
		return "SHD";
	case Class::Druid:
		return "DRU";
	case Class::Monk:
		return "MNK";
	case Class::Bard:
		return "BRD";
	case Class::Rogue:
		return "ROG";
	case Class::Shaman:
		return "SHM";
	case Class::Necromancer:
		return "NEC";
	case Class::Wizard:
		return "WIZ";
	case Class::Magician:
		return "MAG";
	case Class::Enchanter:
		return "ENC";
	case Class::Beastlord:
		return "BST";
	}

	return std::string("UNK");
}

bool IsPlayerClass(uint8 class_id) {
	return EQ::ValueWithin(class_id, Class::Warrior, Class::Beastlord);
}