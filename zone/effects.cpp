/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2003 EQEMu Development Team (http://eqemulator.net)

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

#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include "../common/spdat.h"

#include "client.h"
#include "entity.h"
#include "mob.h"
#include "beacon.h"

#include "string_ids.h"
#include "worldserver.h"
#include "zonedb.h"
#include "position.h"

float Client::GetActSpellRange(uint16 spell_id, float range, std::string& item)
{
	std::string item_name;
	float extrange = GetFocusEffect(focusRange, spell_id, item_name) + 100;
	item = item_name;
	casting_spell_focus_range = extrange;

	return (range * extrange) / 100;
}

float Client::GetSpellRange(uint16 spell_id, float range)
{
	if (casting_spell_focus_range > 100)
	{
		return (range * casting_spell_focus_range) / 100;
	}

	return range;
}

// This also handles NPC cast DoTs.
int32 NPC::GetActSpellDamage(uint16 spell_id, int32 value,  Mob* target) {

	//Quest scale all NPC spell damage via $npc->SetSpellFocusDMG(value)
	//DoT Damage - Mob::DoBuffTic [spell_effects.cpp] / Direct Damage Mob::SpellEffect [spell_effects.cpp]

	int32 dmg = value;

	value += dmg*GetSpellFocusDMG()/100;

	if (AI_HasSpellsEffects()){
		int16 chance = 0;
		int ratio = 0;

		if (spells[spell_id].buffduration == 0) {

			if (chance && zone->random.Roll(chance)) {
				value += (value*ratio)/100;
				entity_list.MessageClose_StringID(this, true, 100, MT_SpellCrits, OTHER_CRIT_BLAST, GetCleanName(), itoa(-value));
			}
		}
		else {
			chance += spellbonuses.CriticalDoTChance;
			if (chance && zone->random.Roll(chance)) {
				value += (value*ratio)/100;
			}
		}
	}

	return value;
}

// maxHit set to 0 ignores the check; it's just used for manaburn.  need it here because otherwise crits would multiply it
// damage should be negative
int32 Client::TryWizardInnateCrit(uint16 spell_id, int32 damage, int32 focusDmg, int32 maxHit)
{
	if (GetClass() == WIZARD && GetLevel() >= RuleI(Spells, WizCritLevel))
	{
		double wizCritChance = (((std::min(GetINT(), 255) + std::min(GetDEX(), 255)) / 2.0) + 32.0) / 10000.0;
		bool critSuccess = zone->random.Roll(wizCritChance);

		if (critSuccess)
		{
			int32 mult = zone->random.Int(1, 50);

			damage += damage * mult / 100 + focusDmg;

			if (maxHit && damage < maxHit)
				damage = maxHit;

			entity_list.MessageClose_StringID(this, true, 100, MT_SpellCrits,
				OTHER_CRIT_BLAST, GetName(), itoa(-damage));
			Message_StringID(MT_SpellCrits, YOU_CRIT_BLAST, itoa(-damage));
		}
		else
			damage += focusDmg;
	}
	return damage;
}

// handle crits and apply AA and disc bonuses/modifers to spell damage cast by clients.  dmg should be negative
int32 Client::GetActSpellDamage(uint16 spell_id, int32 dmg, Mob* target) 
{

	if (spells[spell_id].targettype == ST_Self)
		return dmg;

	if (spell_id == SPELL_IMP_HARM_TOUCH)	// Improved Harm Touch AA skill
		dmg -= GetAA(aaUnholyTouch) * 450;	// Unholy Touch AA

	if ((spell_id == SPELL_HARM_TOUCH || spell_id == SPELL_HARM_TOUCH2 || spell_id == SPELL_IMP_HARM_TOUCH) && HasInstantDisc(spell_id))		// Unholy Aura disc; 50% dmg is guaranteed
		dmg = dmg * 150 / 100;

	std::string item_name;
	int32 focusDmg = 0;
	focusDmg = dmg * GetFocusEffect(focusImprovedDamage, spell_id, item_name) / 100;
	if (focusDmg)
		Log(Logs::General, Logs::Focus, "focusImprovedDamage improved damage from %d to %d", dmg, focusDmg + dmg);

	// SK AA Soul Abrasion; only SKs get something with focusSpellDamageMult
	if (GetClass() == SHADOWKNIGHT)
	{
		dmg += dmg * GetFocusEffect(focusSpellDamageMult, spell_id, item_name) / 100;	// the AA bonus only applies to spells with spellgroup 99, so don't need spell ID check here
	}

	bool critical = false;
	int critChanceAA = itembonuses.CriticalSpellChance + spellbonuses.CriticalSpellChance + aabonuses.CriticalSpellChance;

	if (critChanceAA && zone->random.Roll(critChanceAA))
		critical = true;

	// Improved Harm Touch is a guaranteed crit if you have at least one level of SCF.
	if (spell_id == SPELL_IMP_HARM_TOUCH && (GetAA(aaSpellCastingFury) > 0) && (GetAA(aaUnholyTouch) > 0))
		critical = true;

	if (critical)
	{
		int mult = 100;
		if (GetAA(aaSpellCastingFury) == 1)	// lower ranks do not do double damage
			mult = 33;
		else if (GetAA(aaSpellCastingFury) == 2)
			mult = 66;

		dmg += dmg * mult / 100 + focusDmg;		// focused damage is not multiplied by crit

		entity_list.MessageClose_StringID(this, false, 100, MT_SpellCrits,
				OTHER_CRIT_BLAST, GetName(), itoa(-dmg));
		Message_StringID(MT_SpellCrits, YOU_CRIT_BLAST, itoa(-dmg));
	}
	else if (GetClass() == WIZARD)
		dmg = TryWizardInnateCrit(spell_id, dmg, focusDmg);
	else
		dmg += focusDmg;

	return dmg;
}

int32 Client::GetActDoTDamage(uint16 spell_id, int32 value, Mob* target) {

	if (target == nullptr)
		return value;

	int32 value_BaseEffect = value;
	int16 chance = 0;
	chance += itembonuses.CriticalDoTChance + spellbonuses.CriticalDoTChance + aabonuses.CriticalDoTChance;

	std::string item_name;
	if (chance > 0 && (zone->random.Roll(chance))) {
		int32 ratio = 200;
		value = value_BaseEffect*ratio/100;
		int32 tmp_val = value;
		value += int(value_BaseEffect*GetFocusEffect(focusImprovedDamage, spell_id, item_name, true)/100)*ratio/100;
		if (tmp_val != value)
			Log(Logs::General, Logs::Focus, "focusImprovedDamage improved DOT damage from %d to %d", tmp_val, value);

		return value;
	}

	value = value_BaseEffect;
	value += value_BaseEffect*GetFocusEffect(focusImprovedDamage, spell_id, item_name, true)/100;

	return value;
}

int32 Mob::GetExtraSpellAmt(uint16 spell_id, int32 extra_spell_amt, int32 base_spell_dmg)
{
	int total_cast_time = 0;

	if (spells[spell_id].recast_time >= spells[spell_id].recovery_time)
			total_cast_time = spells[spell_id].recast_time + spells[spell_id].cast_time;
	else
		total_cast_time = spells[spell_id].recovery_time + spells[spell_id].cast_time;

	if (total_cast_time > 0 && total_cast_time <= 2500)
		extra_spell_amt = extra_spell_amt*25/100; 
	 else if (total_cast_time > 2500 && total_cast_time < 7000) 
		 extra_spell_amt = extra_spell_amt*(167*((total_cast_time - 1000)/1000)) / 1000; 
	 else 
		 extra_spell_amt = extra_spell_amt * total_cast_time / 7000; 

		if(extra_spell_amt*2 < base_spell_dmg)
			return 0;

		return extra_spell_amt;
}


int32 NPC::GetActSpellHealing(uint16 spell_id, int32 value, Mob* target, bool hot) {

	//Scale all NPC spell healing via SetSpellFocusHeal(value)

	value += value*GetSpellFocusHeal()/100;

	 if (target) {
		value += value*target->GetHealRate(spell_id, this)/100;
	 }

	 //Allow for critical heal chance if NPC is loading spell effect bonuses.
	 if (AI_HasSpellsEffects()){
		if(spells[spell_id].buffduration < 1) {
			if(spellbonuses.CriticalHealChance && (zone->random.Roll(spellbonuses.CriticalHealChance))) {
				value = value*2;
				entity_list.MessageClose_StringID(this, true, 100, MT_SpellCrits, OTHER_CRIT_HEAL, GetCleanName(), itoa(value));
			}
		}
	 }

	return value;
}

int32 Client::GetActSpellHealing(uint16 spell_id, int32 value, Mob* target, bool hot) {

	if (target == nullptr)
		target = this;

	int32 value_BaseEffect = value;
	int16 chance = 0;
	int8 modifier = 1;
	bool Critical = false;

	if (!hot)
	{
		std::string item_name;
		value += int(value_BaseEffect*GetFocusEffect(focusImprovedHeal, spell_id, item_name) / 100);
		if (value_BaseEffect != value)
		{
			int32 tmp_val = GetHP() + value > GetMaxHP() ? GetMaxHP() : value;
			Log(Logs::General, Logs::Focus, "focusImprovedHeal improved heal from %d to %d", value_BaseEffect, tmp_val);
		}
	}

	// Instant Heals
	if(spells[spell_id].buffduration < 1) {

		chance += itembonuses.CriticalHealChance + spellbonuses.CriticalHealChance + aabonuses.CriticalHealChance;

		if (IsPercentalHealSpell(spell_id)) // these don't crit - Tunare's Renewal, Kragg's Mending, Karana's Renewal
		{
			chance = 0;
		}

		if(chance && (zone->random.Roll(chance))) {
			Critical = true;
			modifier = 2; //At present time no critical heal amount modifier SPA exists.
		}

		value *= modifier;
		value += value*target->GetHealRate(spell_id, this)/100;

		if (Critical) {
			entity_list.MessageClose_StringID(this, false, 100, MT_SpellCrits,
					OTHER_CRIT_HEAL, GetName(), itoa(value));
			Message_StringID(MT_SpellCrits, YOU_CRIT_HEAL, itoa(value));
		}

		return value;
	}

	return value;
}


int32 Client::GetActSpellCost(uint16 spell_id, int32 cost)
{
	// This formula was derived from the following resource:
	// http://www.eqsummoners.com/eq1/specialization-library.html
	// WildcardX
	float PercentManaReduction = 0.0f;
	float SpecializeSkill = GetSpecializeSkillValue(spell_id);

	if (SpecializeSkill > 0.0f)
	{
		PercentManaReduction = 1 + SpecializeSkill / 20.0f;
		switch(GetAA(aaSpellCastingMastery))
		{
		case 1:
			PercentManaReduction += 2.5f;
			break;
		case 2:
			PercentManaReduction += 5.0f;
			break;
		case 3:
			PercentManaReduction += 10.0f;
			break;
		}
	}

	cost -= (cost * (PercentManaReduction / 100));

	if(cost < 0)
		cost = 0;

	return cost;
}

int32 Client::GetActSpellDuration(uint16 spell_id, int32 duration)
{
	int increase = 100;
	int tic_inc = 0;

	return (((duration * increase) / 100) + tic_inc);
}

int32 Client::GetActSpellCasttime(uint16 spell_id, int32 casttime)
{
	std::string item_name; // not used

	int32 aa_casting_time_mod = GetAACastingTimeModifier(spell_id, casttime);
	int32 focus_casting_time_mod = casttime * (100 - GetFocusEffect(focusSpellHaste, spell_id, item_name)) / 100 - casttime;

	int32 modified_cast_time = casttime + aa_casting_time_mod + focus_casting_time_mod;
	modified_cast_time = modified_cast_time < casttime / 2 ? casttime / 2 : modified_cast_time;

	if(modified_cast_time != casttime)
		Log(Logs::General, Logs::Focus, "Spell %d casttime %d modified %d aa_mod %d focus_mod %d", spell_id, casttime, modified_cast_time, aa_casting_time_mod, focus_casting_time_mod);

	return modified_cast_time;
}

int32 Client::GetAACastingTimeModifier(uint16 spell_id, int32 casttime)
{
	// this function is based on a client decompile
	const struct SPDat_Spell_Struct *spell = &spells[spell_id];
	int32 modified_cast_time = casttime;

	// Spell Casting Deftness and Quick Buff
	if (spell->goodEffect != 0 && casttime > 3999 && spell->buffduration)
	{
		uint32 SpellCastingDeftness_AA_Level = GetAA(aaSpellCastingDeftness);
		if (SpellCastingDeftness_AA_Level > 0)
		{
			int32 percent_mod = 100;
			switch (SpellCastingDeftness_AA_Level)
			{
			case 1: percent_mod = 95; break; // Spell Casting Deftness 1 - 5% cast time reduction
			case 2: percent_mod = 85; break; // Spell Casting Deftness 2 - 15% cast time reduction
			case 3: percent_mod = 75; break; // Spell Casting Deftness 3 - 25% cast time reduction
			}
			modified_cast_time = modified_cast_time * percent_mod / 100;
		}

		uint32 QuickBuff_AA_Level = GetAA(aaQuickBuff);
		if (QuickBuff_AA_Level > 0)
		{
			int32 percent_mod = 100;
			switch (QuickBuff_AA_Level)
			{
			case 1: percent_mod = 90; break; // Quick Buff 1 - 10% cast time reduction
			case 2: percent_mod = 75; break; // Quick Buff 2 - 25% cast time reduction
			case 3: percent_mod = 50; break; // Quick Buff 3 - 50% cast time reduction
			}
			modified_cast_time = modified_cast_time * percent_mod / 100;
		}
	}

	// Quick Damage
	if (spell->goodEffect == 0 && casttime > 3999 && spell->buffduration == 0 && IsEffectInSpell(spell_id, SE_CurrentHP) && GetAA(aaSpellCastingFury) == 3)
	{
		uint32 QuickDamage_AA_Level = GetAA(aaQuickDamage);
		if (QuickDamage_AA_Level > 0)
		{
			int32 percent_mod = 100;
			switch (QuickDamage_AA_Level)
			{
			case 1: percent_mod = 98; break; // Quick Damage 1 - 2% cast time reduction
			case 2: percent_mod = 95; break; // Quick Damage 2 - 5% cast time reduction
			case 3: percent_mod = 90; break; // Quick Damage 3 - 10% cast time reduction
			}
			modified_cast_time = modified_cast_time * percent_mod / 100;
		}
	}

	// Quick Evacuation
	if (IsEffectInSpell(spell_id, SE_Succor))
	{
		uint32 QuickEvacuation_AA_Level = GetAA(aaQuickEvacuation);
		if (QuickEvacuation_AA_Level > 0)
		{
			int32 percent_mod = 100;
			switch (QuickEvacuation_AA_Level)
			{
			case 1: percent_mod = 90; break; // Quick Evacuation 1 - 10% cast time reduction
			case 2: percent_mod = 75; break; // Quick Evacuation 2 - 25% cast time reduction
			case 3: percent_mod = 50; break; // Quick Evacuation 3 - 50% cast time reduction
			}
			modified_cast_time = modified_cast_time * percent_mod / 100;
		}
	}

	// Quick Summoning
	if (GetClass() == MAGICIAN)
	{
		if (IsEffectInSpell(spell_id, SE_SummonItem) || IsEffectInSpell(spell_id, SE_SummonPet) || spell_id == SPELL_MANIFEST_ELEMENTS || spell_id == SPELL_CALL_OF_THE_HERO)
		{
			uint32 QuickSummoning_AA_Level = GetAA(aaQuickSummoning);
			if (QuickSummoning_AA_Level > 0)
			{
				int32 percent_mod = 100;
				switch (QuickSummoning_AA_Level)
				{
				case 1: percent_mod = 90; break; // Quick Summoning 1 - 10% cast time reduction
				case 2: percent_mod = 75; break; // Quick Summoning 2 - 25% cast time reduction
				case 3: percent_mod = 50; break; // Quick Summoning 3 - 50% cast time reduction
				}
				modified_cast_time = modified_cast_time * percent_mod / 100;
			}
		}
	}

	return modified_cast_time - casttime;
}

bool Client::UseDiscipline(uint8 disc_id) 
{
	// Dont let client waste a reuse timer if they can't use the disc
	if (IsFeared() || IsMezzed() || IsAmnesiad() || IsPet())
	{
		return(false);
	}

	//Check the disc timer
	uint32 remain = CheckDiscTimer(pTimerDisciplineReuseStart + GetDiscTimerID(disc_id));
	if(remain > 0 && !GetGM())
	{
		char val1[20]={0};
		char val2[20]={0};
		Message_StringID(CC_User_Disciplines, DISCIPLINE_CANUSEIN, ConvertArray((remain)/60,val1), ConvertArray(remain%60,val2));
		return(false);
	}

	bool active = disc_ability_timer.Enabled();
	if(active)
	{
		Message(CC_User_Disciplines, "You must wait before using this discipline."); //find correct message
		return(false);
	}

	//can we use the disc? the client checks this for us, but we should also confirm server side.
	uint8 level_to_use = DisciplineUseLevel(disc_id);
	if(level_to_use > GetLevel() || level_to_use == 0) 
	{
		Message_StringID(CC_User_Disciplines, DISC_LEVEL_USE_ERROR);
		return(false);
	}

	// Disciplines with no ability timer (ashenhand, silentfist, thunderkick, and unholyaura) will remain on the player until they either 
	// use the skill the disc affects successfully, camp/zone, or attempt to use another disc. If we're here, clear that disc so they can 
	// cast a new one.
	if(GetActiveDisc() != 0)
	{
		Log(Logs::General, Logs::Discs, "Clearing disc %d so that disc %d can be cast.", GetActiveDisc(), disc_id);
		FadeDisc();
	}

	//cast the disc
	if(CastDiscipline(disc_id, level_to_use))
		return(true);
	else
		return(false);
}

uint8 Client::DisciplineUseLevel(uint8 disc_id)
{
	switch(disc_id) 
	{
		case disc_aggressive:
			if(GetClass() == WARRIOR)
				return 60;
			else
				return 0;
			break;
		case disc_precision:
			if(GetClass() == WARRIOR)
				return 57;
			else
				return 0;
			break;
		case disc_defensive:
			if(GetClass() == WARRIOR)
				return 55;
			else
				return 0;
			break;
		case disc_evasive:
			if(GetClass() == WARRIOR)
				return 52;
			else
				return 0;
			break;
		case disc_ashenhand:
			if(GetClass() == MONK)
				return 60;
			else
				return 0;
			break;
		case disc_furious:
			if(GetClass() == WARRIOR)
				return 56;
			else if(GetClass() == MONK || GetClass() == ROGUE)
				return 53;
			else
				return 0;
			break;
		case disc_stonestance:
			if(GetClass() == MONK)
				return 51;
			else if(GetClass() == BEASTLORD)
				return 55;
			else
				return 0;
			break;
		case disc_thunderkick:
			if(GetClass() == MONK)
				return 52;
			else
				return 0;
			break;
		case disc_fortitude:
			if(GetClass() == WARRIOR)
				return 59;
			else if(GetClass() == MONK)
				return 54;
			else
				return 0;
			break;
		case disc_fellstrike:
			if(GetClass() == WARRIOR)
				return 58;
			else if(GetClass() == MONK)
				return 56;
			else if(GetClass() == BEASTLORD)
				return 60;
			else if(GetClass() == ROGUE)
				return 59;
			else
				return 0;
			break;
		case disc_hundredfist:
			if(GetClass() == MONK)
				return 57;
			else if(GetClass() == ROGUE)
				return 58;
			else
				return 0;
			break;
		case disc_charge:
			if(GetClass() == WARRIOR)
				return 53;
			else if(GetClass() == ROGUE)
				return 54;
			else
				return 0;
			break;
		case disc_mightystrike:
			if(GetClass() == WARRIOR)
				return 54;
			else
				return 0;
			break;
		case disc_nimble:
			if(GetClass() == ROGUE)
				return 55;
			else
				return 0;
			break;
		case disc_silentfist:
			if(GetClass() == MONK)
				return 59;
			else
				return 0;
			break;
		case disc_kinesthetics:
			if(GetClass() == ROGUE)
				return 57;
			else
				return 0;
			break;
		case disc_holyforge:
			if(GetClass() == PALADIN)
				return 55;
			else
				return 0;
			break;
		case disc_sanctification:
			if(GetClass() == PALADIN)
				return 60;
			else
				return 0;
			break;
		case disc_trueshot:
			if(GetClass() == RANGER)
				return 55;
			else
				return 0;
			break;
		case disc_weaponshield:
			if(GetClass() == RANGER)
				return 60;
			else
				return 0;
			break;
		case disc_unholyaura:
			if(GetClass() == SHADOWKNIGHT)
				return 55;
			else
				return 0;
			break;
		case disc_leechcurse:
			if(GetClass() == SHADOWKNIGHT)
				return 60;
			else
				return 0;
			break;
		case disc_deftdance:
			if(GetClass() == BARD)
				return 55;
			else
				return 0;
			break;
		case disc_puretone:
			if(GetClass() == BARD)
				return 60;
			else
				return 0;
			break;
		case disc_resistant:
			if(GetClass() == WARRIOR || GetClass() == MONK || GetClass() == ROGUE)
				return 30;
			else if(GetClass() == PALADIN || GetClass() == RANGER || GetClass() == SHADOWKNIGHT || GetClass() == BARD || GetClass() == BEASTLORD)
				return 51;
			else
				return 0;
			break;
		case disc_fearless:
			if(GetClass() == WARRIOR || GetClass() == MONK || GetClass() == ROGUE)
				return 40;
			else if(GetClass() == PALADIN || GetClass() == RANGER || GetClass() == SHADOWKNIGHT || GetClass() == BARD || GetClass() == BEASTLORD)
				return 54;
			else
				return 0;
			break;
		default:
			return 0;
			break;
	}
}

bool Client::CastDiscipline(uint8 disc_id, uint8 level_to_use)
{
	uint8 current_level = GetLevel();

	if(level_to_use > current_level || level_to_use == 0)
		return false;

	// reuse_timer is in seconds, ability_timer is in milliseconds.
	int32 reuse_timer = 0, ability_timer = 0, string = 0;
	int16 spellid = 0;

	switch(disc_id)
	{
		case disc_aggressive:
			reuse_timer = 1620;
			ability_timer = 180000;
			spellid = 4498;
			string = DISCIPLINE_AGRESSIVE;

			break;
		case disc_precision:
			reuse_timer = 1800;
			ability_timer = 180000;
			spellid = 4501;
			string = DISCIPLINE_PRECISION;

			break;
		case disc_defensive:
			reuse_timer = 900;
			ability_timer = 180000;
			spellid = 4499;
			string = DISCIPLINE_DEFENSIVE;

			break;
		case disc_evasive:
			reuse_timer = 900;
			ability_timer = 180000;
			spellid = 4503;
			string = DISCIPLINE_EVASIVE;

			break;
		case disc_ashenhand:
			reuse_timer = 4320;
			spellid = 4508;
			string = DISCIPLINE_ASHENHAND;

			break;
		case disc_furious: // furious (WAR), whirlwind (MNK), counterattack (ROG)
			reuse_timer = 3600;
			ability_timer = 9000;
			if(GetBaseClass() == WARRIOR)
				spellid = 4674;
			else if(GetBaseClass() == MONK)
				spellid = 4509;
			else if(GetBaseClass() == ROGUE)
				spellid = 4673;
			string = DISCIPLINE_FURIOUS;

			break;
		case disc_stonestance: // stonestance (MNK), protectivespirit (BST)
			reuse_timer = 720;
			ability_timer = 12000;
			if(GetBaseClass() == MONK)
				spellid = 4510;
			else if(GetBaseClass() == BEASTLORD)
				spellid = 4671;
			string = DISCIPLINE_STONESTANCE;

			break;
		case disc_thunderkick:
			reuse_timer = 540;
			spellid = 4511;
			string = DISCIPLINE_THUNDERKICK;

			break;
		case disc_fortitude: // fortitude (WAR), voiddance (MNK)
			reuse_timer = 3600;
			ability_timer = 8000;
			if(GetBaseClass() == WARRIOR)
				spellid = 4670;
			else if(GetBaseClass() == MONK)
				spellid = 4502;
			string = DISCIPLINE_FORTITUDE;

			break;
		case disc_fellstrike: // fellstrike (WAR), bestialrage (BST), innerflame (MNK), duelist (ROG)
			reuse_timer = 1800;
			ability_timer = 12000;
			if(GetBaseClass() == WARRIOR)
				spellid = 4675;
			else if(GetBaseClass() == MONK)
				spellid = 4512;
			else if(GetBaseClass() == ROGUE)
				spellid = 4676;
			else if(GetBaseClass() == BEASTLORD)
				spellid = 4678;
			string = DISCIPLINE_FELLSTRIKE;

			break;
		case disc_hundredfist: // hundredfist (MNK), blindingspeed (ROG)
			reuse_timer = 1800;
			ability_timer = 15000;
			if(GetBaseClass() == MONK)
				spellid = 4513;
			else if(GetBaseClass() == ROGUE)
				spellid = 4677;
			string = DISCIPLINE_HUNDREDFIST;

			break;
		case disc_charge: // charge (WAR), deadeye (ROG)
			reuse_timer = 1800;
			ability_timer = 14000;
			if(GetBaseClass() == WARRIOR)
				spellid = 4672;
			else if(GetBaseClass() == ROGUE)
				spellid = 4505;
			string = DISCIPLINE_CHARGE;

			break;
		case disc_mightystrike:
			reuse_timer = 3600;
			ability_timer = 10000;
			spellid = 4514;
			string = DISCIPLINE_MIGHTYSTRIKE;

			break;
		case disc_nimble:
			reuse_timer = 1800;
			ability_timer = 12000;
			spellid = 4515;
			string = DISCIPLINE_NIMBLE;

			break;
		case disc_silentfist:
			reuse_timer = 594;
			spellid = 4507;
			if(GetRace() == IKSAR)
				string = DISCIPLINE_SILENTFIST_IKSAR;
			else
				string = DISCIPLINE_SILENTFIST;

			break;
		case disc_kinesthetics:
			reuse_timer = 1800;
			ability_timer = 18000;
			spellid = 4517;
			string = DISCIPLINE_KINESTHETICS;

			break;
		case disc_holyforge:
			reuse_timer = 4320;
			ability_timer = 300000;
			spellid = 4500;
			string = DISCIPLINE_HOLYFORGE;

			break;
		case disc_sanctification:
			reuse_timer = 4320;
			ability_timer = 15000;
			spellid = 4518;
			string = DISCIPLINE_SANCTIFICATION;

			break;
		case disc_trueshot:
			reuse_timer = 4320;
			ability_timer = 120000;
			spellid = 4506;
			string = DISCIPLINE_TRUESHOT;

			break;
		case disc_weaponshield:
			reuse_timer = 4320;
			ability_timer = 20000;
			spellid = 4519;
			if(GetGender() == 0)
				string = DISCIPLINE_WPNSLD_MALE;
			else if(GetGender() == 1)
				string = DISCIPLINE_WPNSLD_FEMALE;
			else
				string = DISCIPLINE_WPNSLD_MONSTER;

			break;
		case disc_unholyaura:
			reuse_timer = 4320;
			spellid = 4520;
			string = DISCIPLINE_UNHOLYAURA;

			break;
		case disc_leechcurse:
			reuse_timer = 4320;
			ability_timer = 20000;
			spellid = 4504;
			string = DISCIPLINE_LEECHCURSE;

			break;
		case disc_deftdance:
			reuse_timer = 4320;
			ability_timer = 15000;
			spellid = 4516;
			string = DISCIPLINE_DEFTDANCE;

			break;
		case disc_puretone:
			reuse_timer = 4320;
			ability_timer = 240000;
			spellid = 4586;
			string = DISCIPLINE_PURETONE;

			break;
		case disc_resistant:
			reuse_timer = 3600;
			ability_timer = 300000;
			spellid = 4585;
			string = DISCIPLINE_RESISTANT;

			break;
		case disc_fearless:
			reuse_timer = 3600;
			ability_timer = 11000;
			spellid = 4587;
			string = DISCIPLINE_FEARLESS;

			break;
		default:
			Log(Logs::General, Logs::Discs, "Invalid disc id %d was passed to CastDiscipline.", disc_id);
			return false;
	}

	if(string > 0 && IsValidSpell(spellid) && IsDisc(spellid))
	{
		entity_list.MessageClose_StringID(this, true, 50, CC_User_Disciplines, string, GetName());

		if (reuse_timer < 1620 && current_level > 60)
			current_level = 60;
		int32 reuse_timer_mod = 0 - ((current_level - level_to_use) * 54);
		reuse_timer += reuse_timer_mod;
		if (reuse_timer > 4320)
			reuse_timer = 4320; // 72:00 maximum reuse time
		if (reuse_timer < 234)
			reuse_timer = 234; // 3:54 minimum reuse time

		p_timers.Start(pTimerDisciplineReuseStart + GetDiscTimerID(disc_id), reuse_timer);
		if(ability_timer > 0)
		{
			disc_ability_timer.Start(ability_timer);
		}
		else
		{
			Log(Logs::General, Logs::Discs, "Disc %d is an instant effect", disc_id);
		}

		SetActiveDisc(disc_id, spellid);
		SpellFinished(spellid, this);
	}
	else
	{
		Log(Logs::General, Logs::Discs, "Disc: %d Invalid stringid or spellid specified.", disc_id);
		return false;
	}

	auto outapp = new EQApplicationPacket(OP_DisciplineChange, sizeof(ClientDiscipline_Struct));
	ClientDiscipline_Struct *d = (ClientDiscipline_Struct*)outapp->pBuffer;
	d->disc_id = disc_id;
	QueuePacket(outapp);
	safe_delete(outapp);

	// warrior /shield doesn't work with discs
	EndShield();

	return true;
}

void EntityList::AETaunt(Client* taunter, float range)
{
	// Note: This AA was changed to a spell in May 2004; comments after that date are not applicable.
	// In our era it had no LoS check and would aggro mobs on other floors or behind walls, so the reach
	// was limited which made it very unreliable on large NPCs. (AoW and Fennin for example)
	if (range <= 0.0f)
		range = 25.0f;

	range = range * range;

	auto it = npc_list.begin();
	while (it != npc_list.end())
	{
		NPC *them = it->second;

		float zdiff = std::abs(taunter->GetZ() - them->GetZ());
		if (zdiff < 25.0f			// our Z offets are much lower than Sony's, which could go up over 25 units.  Ours maxes ~12
			&& taunter->IsAttackAllowed(them)
			&& DistanceSquaredNoZ(taunter->GetPosition(), them->GetPosition()) <= range
		) {
			//if (taunter->CheckLosFN(them))
			//{
				taunter->Taunt(them, true, 100);
				taunter->Message_StringID(CC_User_Skills, TAUNT_SUCCESS, them->GetCleanName());
			//}
		}
		++it;
	}
}

// causes caster to hit every mob within dist range of center with spell_id.
// NPC spells will only affect other NPCs with compatible faction
// resisted determines if the spell landed on the original target or not, so we know whether to count them towards the limit
void EntityList::AESpell(Mob *caster, Mob *center, uint16 spell_id, bool affect_caster, int16 resist_adjust, Mob* spell_target, bool initial_cast)
{
	Mob *curmob = nullptr;

	if (!caster) return;
	float dist = caster->GetAOERange(spell_id);
	// raid boss NPCs get a radius extension to PBAoE spells
	if (caster->IsNPC() && spells[spell_id].targettype == ST_AECaster && spells[spell_id].mana == 0)
		dist *= 1.25f;

	float dist2 = dist * dist;
	float dist_targ = 0;

	bool detrimental = IsDetrimentalSpell(spell_id);
	bool clientcaster = caster->IsClient();
	int MAX_TARGETS_ALLOWED = 5;

	// Wizard's Al'Kabor line of spells hits 5 targets.
	static const int16 target_exemptions[] = { 382, 458, 459, 460, 731, 1650, 1651, 1652 };

	if (caster->IsNPC())
		MAX_TARGETS_ALLOWED = 999;
	else if (HasDirectDamageEffect(spell_id))
	{
		// Damage Spells were limited to 4 targets.
		bool exempt = false;
		int8 size = sizeof(target_exemptions) / sizeof(target_exemptions[0]);
		for (int i = 0; i < size; i++) {
			if (spell_id == target_exemptions[i])
			{
				exempt = true;
				break;
			}
		}

		if (!exempt)
		{
			MAX_TARGETS_ALLOWED = 4;
		}
	}

	int targets_hit = 0;
	if (center->IsBeacon())
		targets_hit = center->CastToBeacon()->GetTargetsHit();

	for (auto it = mob_list.begin(); it != mob_list.end(); ++it) {
		curmob = it->second;
		if (!curmob)
			continue;
		// test to fix possible cause of random zone crashes..external methods accessing client properties before they're initialized
		if (curmob->IsClient() && !curmob->CastToClient()->ClientFinishedLoading())
			continue;
		if (curmob == center && (center->IsBeacon() || (spells[spell_id].targettype != ST_AETarget && spells[spell_id].targettype != ST_GroupTeleport && spells[spell_id].targettype != ST_Group)))
			continue;
		if (curmob == caster && !affect_caster)	//watch for caster too
			continue;
		// Some scripts have the trigger cast on itself (Nexus Scions) so we want to allow that.
		if (curmob->IsUnTargetable() && (curmob != spell_target || clientcaster))
		{
			Log(Logs::Detail, Logs::Spells, "Invalid target: Attempting to cast an AE spell on %s, which is untargetable.", curmob->GetName());
			continue;
		}
		if (curmob->IsTrap())
			continue;
		if (curmob->IsHorse())
			continue;
		// undead aoe
		if (spells[spell_id].targettype == ST_UndeadAE)
		{
			if (curmob->GetOrigBodyType() != BT_SummonedUndead && curmob->GetOrigBodyType() != BT_Undead && curmob->GetOrigBodyType() != BT_Vampire)
				continue;
		}
		// summoned aoe
		if (spells[spell_id].targettype == ST_SummonedAE)
		{
			if (curmob->GetOrigBodyType() != BT_SummonedUndead && curmob->GetOrigBodyType() != BT_Summoned && curmob->GetOrigBodyType() != BT_Summoned2 && curmob->GetOrigBodyType() != BT_Summoned3)
				continue;
		}

		dist_targ = DistanceSquared(curmob->GetPosition(), center->GetPosition());

		if (dist_targ > dist2)	//make sure they are in range
			continue;
		if (!clientcaster && curmob->IsNPC()) {	//check npc->npc casting
			FACTION_VALUE f = curmob->GetReverseFactionCon(caster);
			if (detrimental) {
				//affect mobs that are on our hate list, or
				//which have bad faction with us
				if (!(caster->CheckAggro(curmob) || f == FACTION_THREATENINGLY || f == FACTION_SCOWLS))
					continue;
			}
			else {
				//only affect mobs we would assist.
				if (!(f <= FACTION_AMIABLY) && !IsLuclinPortSpell(spell_id))
					continue;
				if ((spells[spell_id].targettype == ST_Group || spells[spell_id].targettype == ST_GroupTeleport) && curmob->IsPet())
					continue;
			}
		}
		if (detrimental) {
			// aoe spells do hit other players except if in same raid or group.  their pets get hit even when grouped.  SpellOnTarget checks pvp protection
			if (caster->InSameGroup(curmob) || caster->InSameRaid(curmob))
				continue;

			if (!zone->SkipLoS() && !spells[spell_id].npc_no_los && curmob != caster && !center->CheckLosFN(curmob, true))
				continue;
		}
		else { 
			// Balance of the Nameless, Cazic's Gift, recourse spells
			// AESpell is called for ST_GroupTeleport spells cast by NPCs.  The faction check above already filtered out unfriendly NPCs.
			if(!clientcaster && (spells[spell_id].targettype == ST_GroupTeleport || spells[spell_id].targettype == ST_Group))
			{
				if (!curmob->IsNPC())
					continue;
			}
			// check to stop casting beneficial ae buffs (to wit: bard songs) on enemies...
			// This does not check faction for beneficial AE buffs..only agro and attackable.
			// I've tested for spells that I can find without problem, but a faction-based
			// check may still be needed. Any changes here should also reflect in AEBardPulse()
			else if (!IsNeutralSpell(spell_id))
			{
				if (caster->IsAttackAllowed(curmob, true))
				{
					Log(Logs::General, Logs::Spells, "Invalid target: Attempting to cast a beneficial AE spell/song on %s.", curmob->GetName());
					if(!IsBardSong(spell_id) && spells[spell_id].targettype != ST_AEBard)
						caster->Message_StringID(CC_User_SpellFailure, SPELL_NO_HOLD);
					continue;
				}
				else if (IsBardSong(spell_id) && curmob->IsPet())
				{
					Log(Logs::General, Logs::Spells, "Invalid target: Attempting to cast a beneficial AE song on %s who is a pet.", curmob->GetName());
					//caster->Message_StringID(CC_User_SpellFailure, SPELL_NO_HOLD);
					continue;
				}
			}
			if (caster->CheckAggro(curmob) && spell_id != SPELL_DIMENSIONAL_RETURN) // exception for An_unseen_entity returning people from stomach event in potorment
				continue;
		}

		//Check Journey: Luclin spell for Spire Stone in the player's inventory.
		if(spell_id == 2935 && curmob->IsClient())
		{
			int16 slotid = curmob->CastToClient()->GetInv().HasItem(19720);
			if(slotid == INVALID_INDEX)
			{
				curmob->Message_StringID(CC_User_SpellFailure, NO_COMPONENT_LUCLIN);
				Log(Logs::Moderate, Logs::Spells, "%s does not have the correct component to travel to Luclin.", curmob->GetName());
				continue;
			}
			else
			{
				curmob->CastToClient()->DeleteItemInInventory(slotid);
			}
		}

		uint16 ae_caster_id = center && !initial_cast ? center->GetID() : 0;

		//if we get here... cast the spell.
		if (IsTargetableAESpell(spell_id) && detrimental && !IsHarmonySpell(spell_id) && (!IsMemBlurSpell(spell_id) || IsMezSpell(spell_id))) 
		{
			if (targets_hit < MAX_TARGETS_ALLOWED || curmob->IsClient())
			{
				caster->SpellOnTarget(spell_id, curmob, false, true, resist_adjust, false, ae_caster_id);
				if (curmob->IsNPC() && caster->IsAttackAllowed(curmob, true, spell_id))
					++targets_hit;
				Log(Logs::Moderate, Logs::Spells, "Targeted AE Spell: %d has hit target #%d/%d: %s", spell_id, targets_hit, MAX_TARGETS_ALLOWED, curmob->GetCleanName());
			}
		}
		else 
		{
			Log(Logs::Moderate, Logs::Spells, "Non-limited AE Spell: %d has hit target %s", spell_id, curmob->GetCleanName());
			caster->SpellOnTarget(spell_id, curmob, false, true, resist_adjust, false, ae_caster_id);
		}
	}

	if(center->IsBeacon())
		center->CastToBeacon()->SetTargetsHit(targets_hit);
}

void EntityList::MassGroupBuff(Mob *caster, Mob *center, uint16 spell_id)
{
	float dist = caster->GetAOERange(spell_id);
	float dist2 = dist * dist;

	for (auto it = client_list.begin(); it != client_list.end(); ++it)
	{
		Client *curclient = it->second;
		if (!curclient->CastToClient()->ClientFinishedLoading())
			continue;
		if (DistanceSquared(center->GetPosition(), curclient->GetPosition()) > dist2)	//make sure they are in range
			continue;

		caster->SpellOnTarget(spell_id, curclient);
	}
}

// Dook- Rampage and stuff for clients.  NPCs handle it differently in Mob::Rampage
// a targetLimit of 0 means unlimited
void EntityList::AEAttack(Mob *attacker, float dist, int targetLimit)
{
// Dook- Will need tweaking, currently no pets or players or horses
	Mob *curmob;

	float dist2 = dist * dist;

	int hit = 0;

	for (auto it = mob_list.begin(); it != mob_list.end(); ++it) {
		curmob = it->second;
		if (!curmob) {
			continue;
		}
		if (curmob->IsNPC()
				&& curmob != attacker //this is not needed unless NPCs can use this
				&&(attacker->IsAttackAllowed(curmob))
				&& curmob->GetRace() != HORSE /* dont attack horses */
				&& (DistanceSquared(curmob->GetPosition(), attacker->GetPosition()) <= dist2)
		) {

			// this stuff should really be in a client subroutine, but the client process is one massive blob so this is code duplication
			attacker->Attack(curmob, EQ::invslot::slotPrimary);
			if (attacker->CheckDoubleAttack())
			{
				attacker->Attack(curmob, EQ::invslot::slotPrimary);
				// Triple attack: Warriors and Monks over level 60 do this.  10% chance on a double
				if ((attacker->GetClass() == MONK || attacker->GetClass() == WARRIOR) && attacker->GetLevel() >= 60 && zone->random.Roll(10))
					attacker->Attack(curmob, EQ::invslot::slotPrimary);
			}

			if (attacker->IsDualWielding())
			{
				attacker->Attack(curmob, EQ::invslot::slotSecondary);
				if (attacker->CheckDoubleAttack())
					attacker->Attack(curmob, EQ::invslot::slotSecondary);
			}

			hit++;
			if (targetLimit != 0 && hit >= targetLimit)
				return;
		}
	}
}
