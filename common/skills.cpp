/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2002 EQEMu Development Team (http://eqemu.org)

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

#include "skills.h"

#include <string.h>

bool EQ::skills::IsTradeskill(SkillType skill)
{
	switch (skill) {
	case SkillFishing:
	case SkillMakePoison:
	case SkillTinkering:
	case SkillResearch:
	case SkillAlchemy:
	case SkillBaking:
	case SkillTailoring:
	case SkillBlacksmithing:
	case SkillFletching:
	case SkillBrewing:
	case SkillPottery:
	case SkillJewelryMaking:
		return true;
	default:
		return false;
	}
}

bool EQ::skills::IsBasicTradeskill(SkillType skill)
{
	// These tradeskill all cap at 250 so it's useful to use if mod is presented for these.
	// Mod is capped at 15 after 250 at least for makepoison
	switch (skill) {
	case SkillMakePoison:
	case SkillTinkering:
	case SkillBaking:
	case SkillTailoring:
	case SkillBlacksmithing:
	case SkillFletching:
	case SkillBrewing:
	case SkillPottery:
	case SkillJewelryMaking:
		return true;
	default:
		return false;
	}
}

bool EQ::skills::IsSpecializedSkill(SkillType skill)
{
	// this could be a simple if, but if this is more portable if any IDs change (probably won't)
	// or any other specialized are added (also unlikely)
	switch (skill) {
	case SkillSpecializeAbjure:
	case SkillSpecializeAlteration:
	case SkillSpecializeConjuration:
	case SkillSpecializeDivination:
	case SkillSpecializeEvocation:
		return true;
	default:
		return false;
	}
}

bool EQ::skills::IsSpellSkill(SkillType skill)
{
	switch (skill) 
	{
		case SkillAbjuration:
		case SkillAlteration: 
		case SkillBrassInstruments: 
		case SkillChanneling:
		case SkillConjuration: 
		case SkillDivination: 
		case SkillEvocation: 
		case SkillMeditate:
		case SkillSinging: 
		case SkillSpecializeAbjure: 
		case SkillSpecializeAlteration: 
		case SkillSpecializeConjuration:
		case SkillSpecializeDivination: 
		case SkillSpecializeEvocation: 
		case SkillStringedInstruments: 
		case SkillWindInstruments: 
		case SkillResearch: 
		case SkillPercussionInstruments:
			return true;
		default:
			return false;
	}
}

bool EQ::skills::IsMeleeWeaponSkill(SkillType skill)
{
	switch (skill)
	{
		case SkillHandtoHand:
		case Skill1HBlunt:
		case Skill1HSlashing:
		case Skill1HPiercing:
		case Skill2HBlunt:
		case Skill2HSlashing:
		case SkillBash:
		case SkillKick:
		case SkillBackstab:
		case SkillDragonPunch:
		case SkillFlyingKick:
		case SkillRoundKick:
		// case SkillTigerClaw:		// note: tiger claw is commented out because many spells are flagged as tiger claw (for whatever reason)
		return true;
	default:
		return false;
	}
}

// returns the base damage value for special skills, except backstab.  taken from decompiles.  accurate to PoP era.
int EQ::skills::GetSkillBaseDamage(SkillType skill, int skillLevel)
{
	int base = 0;

	switch (skill)
	{
	case SkillBash:
		base = 2;
		break;
	case SkillKick:
		base = 3;
		break;
	case SkillDragonPunch:
		base = 12;
		break;
	case SkillFlyingKick:
		base = 25;
		break;
	case SkillRoundKick:
		base = 5;
		break;
	case SkillEagleStrike:
		base = 7;
		break;
	case SkillTigerClaw:
		base = 4;
		break;
	case SkillIntimidation:
		base = 2;
		break;
	default:
		return 0;
	}

	if (skillLevel >= 25)
		base++;
	if (skillLevel >= 75)
		base++;
	if (skillLevel >= 125)
		base++;
	if (skillLevel >= 175)
		base++;

	return base;
}

const std::map<EQ::skills::SkillType, std::string>& EQ::skills::GetSkillTypeMap()
{
	static const std::map<SkillType, std::string> skill_type_map = {
		{ Skill1HBlunt, "1H Blunt" },
		{ Skill1HSlashing, "1H Slashing" },
		{ Skill2HBlunt, "2H Blunt" },
		{ Skill2HSlashing, "2H Slashing" },
		{ SkillAbjuration, "Abjuration" },
		{ SkillAlteration, "Alteration" },
		{ SkillApplyPoison, "Apply Poison" },
		{ SkillArchery, "Archery" },
		{ SkillBackstab, "Backstab" },
		{ SkillBindWound, "Bind Wound" },
		{ SkillBash, "Bash" },
		{ SkillBlock, "Block" },
		{ SkillBrassInstruments, "Brass Instruments" },
		{ SkillChanneling, "Channeling" },
		{ SkillConjuration, "Conjuration" },
		{ SkillDefense, "Defense" },
		{ SkillDisarm, "Disarm" },
		{ SkillDisarmTraps, "Disarm Traps" },
		{ SkillDivination, "Divination" },
		{ SkillDodge, "Dodge" },
		{ SkillDoubleAttack, "Double Attack" },
		{ SkillDragonPunch, "Dragon Punch" },
		{ SkillDualWield, "Dual Wield" },
		{ SkillEagleStrike, "Eagle Strike" },
		{ SkillEvocation, "Evocation" },
		{ SkillFeignDeath, "Feign Death" },
		{ SkillFlyingKick, "Flying Kick" },
		{ SkillForage, "Forage" },
		{ SkillHandtoHand, "Hand to Hand" },
		{ SkillHide, "Hide" },
		{ SkillKick, "Kick" },
		{ SkillMeditate, "Meditate" },
		{ SkillMend, "Mend" },
		{ SkillOffense, "Offense" },
		{ SkillParry, "Parry" },
		{ SkillPickLock, "Pick Lock" },
		{ Skill1HPiercing, "1H Piercing" },
		{ SkillRiposte, "Riposte" },
		{ SkillRoundKick, "Round Kick" },
		{ SkillSafeFall, "Safe Fall" },
		{ SkillSenseHeading, "Sense Heading" },
		{ SkillSinging, "Singing" },
		{ SkillSneak, "Sneak" },
		{ SkillSpecializeAbjure, "Specialize Abjuration" },
		{ SkillSpecializeAlteration, "Specialize Alteration" },
		{ SkillSpecializeConjuration, "Specialize Conjuration" },
		{ SkillSpecializeDivination, "Specialize Divination" },
		{ SkillSpecializeEvocation, "Specialize Evocation" },
		{ SkillPickPockets, "Pick Pockets" },
		{ SkillStringedInstruments, "Stringed Instruments" },
		{ SkillSwimming, "Swimming" },
		{ SkillThrowing, "Throwing" },
		{ SkillTigerClaw, "Tiger Claw" },
		{ SkillTracking, "Tracking" },
		{ SkillWindInstruments, "Wind Instruments" },
		{ SkillFishing, "Fishing" },
		{ SkillMakePoison, "Make Poison" },
		{ SkillTinkering, "Tinkering" },
		{ SkillResearch, "Research" },
		{ SkillAlchemy, "Alchemy" },
		{ SkillBaking, "Baking" },
		{ SkillTailoring, "Tailoring" },
		{ SkillSenseTraps, "Sense Traps" },
		{ SkillBlacksmithing, "Blacksmithing" },
		{ SkillFletching, "Fletching" },
		{ SkillBrewing, "Brewing" },
		{ SkillAlcoholTolerance, "Alcohol Tolerance" },
		{ SkillBegging, "Begging" },
		{ SkillJewelryMaking, "Jewelry Making" },
		{ SkillPottery, "Pottery" },
		{ SkillPercussionInstruments, "Percussion Instruments" },
		{ SkillIntimidation, "Intimidation" },
		{ SkillBerserking, "Berserking" },
		{ SkillTaunt, "Taunt" }
	};
	return skill_type_map;
}

std::string EQ::skills::GetSkillName(SkillType skill)
{
	if (skill >= Skill1HBlunt && skill <= SkillTaunt) {
		std::map<SkillType, std::string> skills = GetSkillTypeMap();
		for (auto current_skill : skills) {
			if (skill == current_skill.first) {
				return current_skill.second;
			}
		}
	}
	return std::string();
}

EQ::SkillProfile::SkillProfile()
{
	memset(&Skill, 0, (sizeof(uint32) * PACKET_SKILL_ARRAY_SIZE));
}

uint16 EQ::SkillProfile::GetSkill(int skill_id)
{
	if (skill_id < 0 || skill_id >= PACKET_SKILL_ARRAY_SIZE)
		return 0;

	return Skill[skill_id];
}