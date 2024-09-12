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
#include "global_define.h"
#include "faction.h"
#include "races.h"

const char *FactionValueToString(FACTION_VALUE faction_value) {
	switch(faction_value) {
	case FACTION_ALLY:
		return "Ally";
	case FACTION_WARMLY:
		return "Warmly";
	case FACTION_KINDLY:
		return "Kindly";
	case FACTION_AMIABLY:
		return "Amiably";
	case FACTION_INDIFFERENTLY:
		return "Indifferent";
	case FACTION_APPREHENSIVELY:
		return "Apprehensively";
	case FACTION_DUBIOUSLY:
		return "Dubiously";
	case FACTION_THREATENINGLY:
		return "Threateningly";
	case FACTION_SCOWLS:
		return "Scowls.";
	default:
		break;
	}
	return "Unknown";
}


//o--------------------------------------------------------------
//| Name: CalculateFaction; Dec. 16, 2001
//o--------------------------------------------------------------
//| Notes: Returns the faction message value.
//|		Modify these values to taste.
//o--------------------------------------------------------------
FACTION_VALUE CalculateFaction(FactionMods* fm, int32 tmpCharacter_value, bool lua)
{
	int32 character_value = tmpCharacter_value;
	if (fm)
		character_value += fm->base + fm->class_mod + fm->race_mod + fm->deity_mod;

	if (lua)
	{
		if (character_value >= 2000) return FACTION_MAX_ALLY;
		if (character_value <= -2000) return FACTION_MAX_SCOWLS;
	}

	if(character_value >= 1100) return FACTION_ALLY;
	if(character_value >= 750 && character_value <= 1099) return FACTION_WARMLY;
	if(character_value >= 500 && character_value <= 749) return FACTION_KINDLY;
	if(character_value >= 100 && character_value <= 499) return FACTION_AMIABLY;
	if(character_value >= 0 && character_value <= 99) return FACTION_INDIFFERENTLY;
	if(character_value >= -100 && character_value <= -1) return FACTION_APPREHENSIVELY;
	if(character_value >= -500 && character_value <= -101) return FACTION_DUBIOUSLY;
	if(character_value >= -750 && character_value <= -501) return FACTION_THREATENINGLY;
	if(character_value <= -751) return FACTION_SCOWLS;
	return FACTION_INDIFFERENTLY;
}

// this function should check if some races have more than one race define
bool IsOfEqualRace(int r1, int r2)
{
	if (r1 == r2)
		return true;
	// TODO: add more values
	switch(r1)
	{
	case DARK_ELF:
		if (r2 == Race::NeriakCitizen)
			return true;
		break;
	case BARBARIAN:
		if (r2 == Race::HalasCitizen)
			return true;
	}
	return false;
}

// trolls endure ogres, dark elves, ...
bool IsOfIndiffRace(int r1, int r2)
{
	if (r1 == r2)
		return true;
	// TODO: add more values
	switch(r1)
	{
	case DARK_ELF:
	case OGRE:
	case TROLL:
		if (r2 == OGRE || r2 == TROLL || r2 == DARK_ELF)
			return true;
		break;
	case HUMAN:
	case BARBARIAN:
	case HALF_ELF:
	case GNOME:
	case HALFLING:
	case WOOD_ELF:
		if (r2 == HUMAN ||
			r2 == BARBARIAN ||
			r2 == ERUDITE ||
			r2 == HALF_ELF ||
			r2 == GNOME ||
			r2 == HALFLING ||
			r2 == DWARF ||
			r2 == HIGH_ELF ||
			r2 == WOOD_ELF)
			return true;
		break;
	case ERUDITE:
		if (r2 == HUMAN || r2 == HALF_ELF)
			return true;
		break;
	case DWARF:
		if (r2 == HALFLING || r2 == GNOME)
			return true;
		break;
	case HIGH_ELF:
		if (r2 == WOOD_ELF)
			return true;
		break;
	case VAHSHIR:
		return true;
	case IKSAR:
		return false;
	}
	return false;
}

