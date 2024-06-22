/*	EQEMu: Everquest Server Emulator

	Copyright (C) 2001-2022 EQEMu Development Team (http://eqemulator.net)

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
#ifndef COMMON_SKILLS_H
#define COMMON_SKILLS_H

#include "types.h"

#include <string>
#include <map>


namespace EQ
{
	namespace skills {
		enum SkillType : int {
			/*13855*/	Skill1HBlunt = 0,
			/*13856*/	Skill1HSlashing,
			/*13857*/	Skill2HBlunt,
			/*13858*/	Skill2HSlashing,
			/*13859*/	SkillAbjuration,
			/*13861*/	SkillAlteration,
			/*13862*/	SkillApplyPoison,
			/*13863*/	SkillArchery,
			/*13864*/	SkillBackstab,
			/*13866*/	SkillBindWound,
			/*13867*/	SkillBash,
			/*13871*/	SkillBlock,
			/*13872*/	SkillBrassInstruments,
			/*13874*/	SkillChanneling,
			/*13875*/	SkillConjuration,
			/*13876*/	SkillDefense,
			/*13877*/	SkillDisarm,
			/*13878*/	SkillDisarmTraps,
			/*13879*/	SkillDivination,
			/*13880*/	SkillDodge,
			/*13881*/	SkillDoubleAttack,
			/*13882*/	SkillDragonPunch,
			/*13924*/	SkillTailRake = SkillDragonPunch, // Iksar Monk equivilent
			/*13883*/	SkillDualWield,
			/*13884*/	SkillEagleStrike,
			/*13885*/	SkillEvocation,
			/*13886*/	SkillFeignDeath,
			/*13888*/	SkillFlyingKick,
			/*13889*/	SkillForage,
			/*13890*/	SkillHandtoHand,
			/*13891*/	SkillHide,
			/*13893*/	SkillKick,
			/*13894*/	SkillMeditate,
			/*13895*/	SkillMend,
			/*13896*/	SkillOffense,
			/*13897*/	SkillParry,
			/*13899*/	SkillPickLock,
			/*13900*/	Skill1HPiercing,				// Changed in RoF2(05-10-2013)
			/*13903*/	SkillRiposte,
			/*13904*/	SkillRoundKick,
			/*13905*/	SkillSafeFall,
			/*13906*/	SkillSenseHeading,
			/*13908*/	SkillSinging,
			/*13909*/	SkillSneak,
			/*13910*/	SkillSpecializeAbjure,			// No idea why they truncated this one..especially when there are longer ones...
			/*13911*/	SkillSpecializeAlteration,
			/*13912*/	SkillSpecializeConjuration,
			/*13913*/	SkillSpecializeDivination,
			/*13914*/	SkillSpecializeEvocation,
			/*13915*/	SkillPickPockets,
			/*13916*/	SkillStringedInstruments,
			/*13917*/	SkillSwimming,
			/*13919*/	SkillThrowing,
			/*13920*/	SkillTigerClaw,
			/*13921*/	SkillTracking,
			/*13923*/	SkillWindInstruments,
			/*13854*/	SkillFishing,
			/*13853*/	SkillMakePoison,
			/*13852*/	SkillTinkering,
			/*13851*/	SkillResearch,
			/*13850*/	SkillAlchemy,
			/*13865*/	SkillBaking,
			/*13918*/	SkillTailoring,
			/*13907*/	SkillSenseTraps,
			/*13870*/	SkillBlacksmithing,
			/*13887*/	SkillFletching,
			/*13873*/	SkillBrewing,
			/*13860*/	SkillAlcoholTolerance,
			/*13868*/	SkillBegging,
			/*13892*/	SkillJewelryMaking,
			/*13901*/	SkillPottery,
			/*13898*/	SkillPercussionInstruments,
			/*13922*/	SkillIntimidation,
			/*13869*/	SkillBerserking,
			/*13902*/	SkillTaunt,
			/*00000*/	SkillCount					// move to last position of active enumeration labels
		};

		// temporary until it can be sorted out...
#define HIGHEST_SKILL	SkillTaunt

// server profile does not reflect this yet..so, prefixed with 'PACKET_'
#define PACKET_SKILL_ARRAY_SIZE 100

		constexpr int format_as(SkillType skill) { return static_cast<int>(skill); }

		enum InnateSkills : int
		{
			Innate0 = 0,
			InnateAwareness = 1,
			InnateBashDoor = 2,
			InnateBreatheFire = 3,
			InnateHarmony = 4,
			InnateHarmTouch = 5,
			InnateInfravision = 6,
			InnateLayHands = 7,
			InnateLore = 8,
			InnateNoBash = 9,
			InnateRegeneration = 10,
			InnateSlam = 11,
			InnateSurprise = 12,
			InnateUltravision = 13
		};

		bool IsTradeskill(SkillType skill);
		bool IsBasicTradeskill(SkillType skill);
		bool IsSpecializedSkill(SkillType skill);
		bool IsSpellSkill(SkillType skillid);
		bool IsMeleeWeaponSkill(SkillType skill);
		int  GetSkillBaseDamage(SkillType skill, int skillLevel);
		std::string GetSkillName(SkillType skill);

		const std::map<SkillType, std::string>& GetSkillTypeMap();

	} /*skills*/
	struct SkillProfile
	{
		union {
			struct {
				uint16 _1HBlunt;
				uint16 _1HSlashing;
				uint16 _2HBlunt;
				uint16 _2HSlashing;
				uint16 Abjuration;
				uint16 Alteration;
				uint16 ApplyPoison;
				uint16 Archery;
				uint16 Backstab;
				uint16 BindWound;
				uint16 Bash;
				uint16 Block;
				uint16 BrassInstruments;
				uint16 Channeling;
				uint16 Conjuration;
				uint16 Defense;
				uint16 Disarm;
				uint16 DisarmTraps;
				uint16 Divination;
				uint16 Dodge;
				uint16 DoubleAttack;
				union {
					uint16 DragonPunch;
					uint16 TailRake;
				};
				uint16 DualWield;
				uint16 EagleStrike;
				uint16 Evocation;
				uint16 FeignDeath;
				uint16 FlyingKick;
				uint16 Forage;
				uint16 HandtoHand;
				uint16 Hide;
				uint16 Kick;
				uint16 Meditate;
				uint16 Mend;
				uint16 Offense;
				uint16 Parry;
				uint16 PickLock;
				uint16 _1HPiercing;
				uint16 Riposte;
				uint16 RoundKick;
				uint16 SafeFall;
				uint16 SenseHeading;
				uint16 Singing;
				uint16 Sneak;
				uint16 SpecializeAbjure;
				uint16 SpecializeAlteration;
				uint16 SpecializeConjuration;
				uint16 SpecializeDivination;
				uint16 SpecializeEvocation;
				uint16 PickPockets;
				uint16 StringedInstruments;
				uint16 Swimming;
				uint16 Throwing;
				uint16 TigerClaw;
				uint16 Tracking;
				uint16 WindInstruments;
				uint16 Fishing;
				uint16 MakePoison;
				uint16 Tinkering;
				uint16 Research;
				uint16 Alchemy;
				uint16 Baking;
				uint16 Tailoring;
				uint16 SenseTraps;
				uint16 Blacksmithing;
				uint16 Fletching;
				uint16 Brewing;
				uint16 AlcoholTolerance;
				uint16 Begging;
				uint16 JewelryMaking;
				uint16 Pottery;
				uint16 PercussionInstruments;
				uint16 Intimidation;
				uint16 Berserking;
				uint16 Taunt;
				uint16 unused1;
				uint16 unused2;
				uint16 unused3;
				uint16 unused4;
				uint16 unused5;
				uint16 unused6;
				uint16 unused7;
				uint16 unused8;
				uint16 unused9;
				uint16 unused10;
				uint16 unused11;
				uint16 unused12;
				uint16 unused13;
				uint16 unused14;
				uint16 unused15;
				uint16 unused16;
				uint16 unused17;
				uint16 unused18;
				uint16 unused19;
				uint16 unused20;
				uint16 unused21;
				uint16 unused22;
				uint16 unused23;
				uint16 unused24;
				uint16 unused25;
				uint16 unused26;
			};
			uint16 Skill[PACKET_SKILL_ARRAY_SIZE];
		};

		SkillProfile();

		uint16* GetSkills() { return reinterpret_cast<uint16*>(&Skill); }

		skills::SkillType GetLastUseableSkill() { return EQ::skills::SkillTaunt; }

		size_t GetSkillsArraySize() { return PACKET_SKILL_ARRAY_SIZE; }
		uint16 GetSkill(int skill_id);

		uint16 operator[](int skill_id) { return GetSkill(skill_id); }

		// const
		uint16* GetSkills() const { return const_cast<SkillProfile*>(this)->GetSkills(); }

		skills::SkillType GetLastUseableSkill() const { return const_cast<SkillProfile*>(this)->GetLastUseableSkill(); }

		size_t GetSkillsArraySize() const { return const_cast<SkillProfile*>(this)->GetSkillsArraySize(); }
		uint16 GetSkill(int skill_id) const { return const_cast<SkillProfile*>(this)->GetSkill(skill_id); }

		uint16 operator[](int skill_id) const { return const_cast<SkillProfile*>(this)->GetSkill(skill_id); }
	};
	

} /*EQEmu*/


#endif /*COMMON_SKILLS_H*/
