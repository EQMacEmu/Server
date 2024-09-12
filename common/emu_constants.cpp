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

#include "emu_constants.h"
#include "data_verification.h"
#include "eqemu_logsys.h"
#include "eqemu_logsys_log_aliases.h"
#include "rulesys.h"

int16 EQ::invtype::GetInvTypeSize(int16 inv_type) {
	static const int16 local_array[] = {
		POSSESSIONS_SIZE,
		BANK_SIZE,
		TRADE_SIZE,
		WORLD_SIZE,
		LIMBO_SIZE,
		MERCHANT_SIZE,
		CORPSE_SIZE,
		BAZAAR_SIZE,
		INSPECT_SIZE,
	};

	if (inv_type < TYPE_BEGIN || inv_type > TYPE_END)
		return INULL;

	return local_array[inv_type];
}

const char* EQ::bug::CategoryIDToCategoryName(CategoryID category_id) {
	switch (category_id) {
	case catVideo:
		return "Video";
	case catAudio:
		return "Audio";
	case catPathing:
		return "Pathing";
	case catQuest:
		return "Quest";
	case catTradeskills:
		return "Tradeskills";
	case catSpellStacking:
		return "Spell stacking";
	case catDoorsPortals:
		return "Doors/Portals";
	case catItems:
		return "Items";
	case catNPC:
		return "NPC";
	case catDialogs:
		return "Dialogs";
	case catOther:
	default:
		return "Other";
	}
}

EQ::bug::CategoryID EQ::bug::CategoryNameToCategoryID(const char* category_name) {
	if (!category_name)
		return catOther;

	if (!strcmp(category_name, "Video"))
		return catVideo;
	if (!strcmp(category_name, "Audio"))
		return catAudio;
	if (!strcmp(category_name, "Pathing"))
		return catPathing;
	if (!strcmp(category_name, "Quest"))
		return catQuest;
	if (!strcmp(category_name, "Tradeskills"))
		return catTradeskills;
	if (!strcmp(category_name, "Spell stacking"))
		return catSpellStacking;
	if (!strcmp(category_name, "Doors/Portals"))
		return catDoorsPortals;
	if (!strcmp(category_name, "Items"))
		return catItems;
	if (!strcmp(category_name, "NPC"))
		return catNPC;
	if (!strcmp(category_name, "Dialogs"))
		return catDialogs;

	return catOther;
}

const std::map<uint8, std::string>& EQ::constants::GetLanguageMap()
{
	static const std::map<uint8, std::string> language_map = {
			{ Language::CommonTongue,  "Common Tongue" },
			{ Language::Barbarian,     "Barbarian" },
			{ Language::Erudian,       "Erudian" },
			{ Language::Elvish,        "Elvish" },
			{ Language::DarkElvish,    "Dark Elvish" },
			{ Language::Dwarvish,      "Dwarvish" },
			{ Language::Troll,         "Troll" },
			{ Language::Ogre,          "Ogre" },
			{ Language::Gnomish,       "Gnomish" },
			{ Language::Halfling,      "Halfling" },
			{ Language::ThievesCant,   "Thieves Cant" },
			{ Language::OldErudian,    "Old Erudian" },
			{ Language::ElderElvish,   "Elder Elvish" },
			{ Language::Froglok,       "Froglok" },
			{ Language::Goblin,        "Goblin" },
			{ Language::Gnoll,         "Gnoll" },
			{ Language::CombineTongue, "Combine Tongue" },
			{ Language::ElderTeirDal,  "Elder Teir'Dal" },
			{ Language::Lizardman,     "Lizardman" },
			{ Language::Orcish,        "Orcish" },
			{ Language::Faerie,        "Faerie" },
			{ Language::Dragon,        "Dragon" },
			{ Language::ElderDragon,   "Elder Dragon" },
			{ Language::DarkSpeech,    "Dark Speech" },
			{ Language::VahShir,       "Vah Shir" },
			{ Language::Unknown25,      "Unknown25" },
			{ Language::Unknown26,      "Unknown26" }
	};
	return language_map;
}

std::string EQ::constants::GetLanguageName(uint8 language_id)
{
	if (EQ::ValueWithin(language_id, Language::CommonTongue, Language::Unknown26)) {
		return EQ::constants::GetLanguageMap().find(language_id)->second;
	}
	return std::string();
}

const std::map<int8, std::string>& EQ::constants::GetFlyModeMap()
{
	static const std::map<int8, std::string> flymode_map = {
		{ EQ::constants::GravityBehavior::Ground, "Ground" },
		{ EQ::constants::GravityBehavior::Flying, "Flying" },
		{ EQ::constants::GravityBehavior::Levitating, "Levitating" },
		{ EQ::constants::GravityBehavior::Water, "Water" }
	};
	return flymode_map;
}

std::string EQ::constants::GetFlyModeName(int8 flymode_id)
{
	if (EQ::ValueWithin(flymode_id, GravityBehavior::Ground, GravityBehavior::Water)) {
		return EQ::constants::GetFlyModeMap().find(flymode_id)->second;
	}
	return std::string();
}

std::string SpecialAbility::GetName(int ability_id)
{
	return IsValid(ability_id) ? special_ability_names[ability_id] : "UNKNOWN SPECIAL ABILITY";
}

bool SpecialAbility::IsValid(int ability_id)
{
	return special_ability_names.find(ability_id) != special_ability_names.end();
}

const std::map<uint8, std::string>& EQ::constants::GetAccountStatusMap()
{
	static const std::map<uint8, std::string> account_status_map = {
		{ AccountStatus::Player, "Player" },
		{ AccountStatus::Steward, "Steward" },
		{ AccountStatus::ApprenticeGuide, "Apprentice Guide" },
		{ AccountStatus::Guide, "Guide" },
		{ AccountStatus::QuestTroupe, "Quest Troupe" },
		{ AccountStatus::SeniorGuide, "Senior Guide" },
		{ AccountStatus::GMTester, "GM Tester" },
		{ AccountStatus::EQSupport, "EQ Support" },
		{ AccountStatus::GMStaff, "GM Staff" },
		{ AccountStatus::GMAdmin, "GM Admin" },
		{ AccountStatus::GMLeadAdmin, "GM Lead Admin" },
		{ AccountStatus::QuestMaster, "Quest Master" },
		{ AccountStatus::GMAreas, "GM Areas" },
		{ AccountStatus::GMCoder, "GM Coder" },
		{ AccountStatus::GMMgmt, "GM Mgmt" },
		{ AccountStatus::GMImpossible, "GM Impossible" },
		{ AccountStatus::Max, "GM Max" }
	};

	return account_status_map;
}

std::string EQ::constants::GetAccountStatusName(uint8 account_status)
{
	for (
		auto status_level = EQ::constants::GetAccountStatusMap().rbegin();
		status_level != EQ::constants::GetAccountStatusMap().rend();
		++status_level
		) {
		if (account_status >= status_level->first) {
			return status_level->second;
		}
	}

	return std::string();
}

const std::map<uint8, std::string>& EQ::constants::GetConsiderLevelMap()
{
	static const std::map<uint8, std::string> consider_level_map = {
		{ ConsiderLevel::Ally, "Ally" },
		{ ConsiderLevel::Warmly, "Warmly" },
		{ ConsiderLevel::Kindly, "Kindly" },
		{ ConsiderLevel::Amiably, "Amiably" },
		{ ConsiderLevel::Indifferently, "Indifferently" },
		{ ConsiderLevel::Apprehensively, "Apprehensively" },
		{ ConsiderLevel::Dubiously, "Dubiously" },
		{ ConsiderLevel::Threateningly, "Threateningly" },
		{ ConsiderLevel::Scowls, "Scowls" }
	};

	return consider_level_map;
}

std::string EQ::constants::GetConsiderLevelName(uint8 faction_consider_level)
{
	if (EQ::constants::GetConsiderLevelMap().find(faction_consider_level) != EQ::constants::GetConsiderLevelMap().end()) {
		return EQ::constants::GetConsiderLevelMap().find(faction_consider_level)->second;
	}

	return std::string();
}

const std::map<uint8, std::string>& EQ::constants::GetSpawnAnimationMap()
{
	static const std::map<uint8, std::string> spawn_animation_map = {
		{ SpawnAnimations::Standing, "Standing" },
		{ SpawnAnimations::Sitting, "Sitting" },
		{ SpawnAnimations::Crouching, "Crouching" },
		{ SpawnAnimations::Laying, "Laying" },
		{ SpawnAnimations::Looting, "Looting" }
	};

	return spawn_animation_map;
}

std::string EQ::constants::GetSpawnAnimationName(uint8 animation_id)
{
	if (EQ::ValueWithin(animation_id, SpawnAnimations::Standing, SpawnAnimations::Looting)) {
		return EQ::constants::GetSpawnAnimationMap().find(animation_id)->second;
	}

	return std::string();
}

const std::map<uint32, std::string> &EQ::constants::GetAppearanceTypeMap()
{
	static const std::map<uint32, std::string> appearance_type_map = {
		{ AppearanceType::Die, "Die" },
		{ AppearanceType::WhoLevel, "Who Level" },
		{ AppearanceType::MaxHealth, "Max Health" },
		{ AppearanceType::Invisibility, "Invisibility" },
		{ AppearanceType::PVP, "PVP" },
		{ AppearanceType::Light, "Light" },
		{ AppearanceType::Animation, "Animation" },
		{ AppearanceType::Sneak, "Sneak" },
		{ AppearanceType::SpawnID, "Spawn ID" },
		{ AppearanceType::Health, "Health" },
		{ AppearanceType::Linkdead, "Linkdead" },
		{ AppearanceType::FlyMode, "Fly Mode" },
		{ AppearanceType::GM, "GM" },
		{ AppearanceType::Anonymous, "Anonymous" },
		{ AppearanceType::GuildID, "Guild ID" },
		{ AppearanceType::GuildRank, "Guild Rank" },
		{ AppearanceType::AFK, "AFK" },
		{ AppearanceType::Pet, "Pet" },
		{ AppearanceType::Summoned, "Summoned" },
		{ AppearanceType::Split, "Split" },
		{ AppearanceType::Size, "Size" },
		{ AppearanceType::SetType, "Set Type" },
		{ AppearanceType::NPCName, "NPCName" },
		{ AppearanceType::DamageState, "Damage State" },
	};

	return appearance_type_map;
}

std::string EQ::constants::GetAppearanceTypeName(uint32 appearance_type)
{
	const auto &a = EQ::constants::GetAppearanceTypeMap().find(appearance_type);
	if (a != EQ::constants::GetAppearanceTypeMap().end()) {
		return a->second;
	}

	return std::string();
}


const std::map<uint8, std::string> &EQ::constants::GetEmoteEventTypeMap()
{
	static const std::map<uint8, std::string> emote_event_type_map = {
		{ EmoteEventTypes::LeaveCombat, "Leave Combat" },
		{ EmoteEventTypes::EnterCombat, "Enter Combat" },
		{ EmoteEventTypes::OnDeath, "On Death" },
		{ EmoteEventTypes::AfterDeath, "After Death" },
		{ EmoteEventTypes::Hailed, "Hailed" },
		{ EmoteEventTypes::KilledPC, "Killed PC" },
		{ EmoteEventTypes::KilledNPC, "Killed NPC" },
		{ EmoteEventTypes::OnSpawn, "On Spawn" },
		{ EmoteEventTypes::OnDespawn, "On Despawn" },
		{ EmoteEventTypes::Killed, "Killed" }
	};

	return emote_event_type_map;
}

std::string EQ::constants::GetEmoteEventTypeName(uint8 emote_event_type)
{
	if (EQ::ValueWithin(emote_event_type, EmoteEventTypes::LeaveCombat, EmoteEventTypes::Killed)) {
		return EQ::constants::GetEmoteEventTypeMap().find(emote_event_type)->second;
	}

	return std::string();
}

const std::map<uint8, std::string> &EQ::constants::GetEmoteTypeMap()
{
	static const std::map<uint8, std::string> emote_type_map = {
		{ EmoteTypes::Say, "Say" },
		{ EmoteTypes::Emote, "Emote" },
		{ EmoteTypes::Shout, "Shout" },
		{ EmoteTypes::Proximity, "Proximity" }
	};

	return emote_type_map;
}

std::string EQ::constants::GetEmoteTypeName(uint8 emote_type)
{
	if (EQ::ValueWithin(emote_type, EmoteTypes::Emote, EmoteTypes::Proximity)) {
		return EQ::constants::GetEmoteTypeMap().find(emote_type)->second;
	}

	return std::string();
}