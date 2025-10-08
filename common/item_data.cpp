/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2016 EQEMu Development Team (http://eqemulator.net)
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
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "item_data.h"
#include "classes.h"
#include "races.h"

bool EQ::ItemData::IsEquipable(uint16 race_id, uint16 class_id) const
{
	if (!(Races & GetPlayerRaceBit(race_id)))
		return false;

	if (!(Classes & GetPlayerClassBit(GetPlayerClassValue(class_id))))
		return false;

	return true;
}

bool EQ::ItemData::IsClassCommon() const
{
	return (ItemClass == item::ItemClassCommon);
}

bool EQ::ItemData::IsClassBag() const
{
	return (ItemClass == item::ItemClassBag);
}

bool EQ::ItemData::IsClassBook() const
{
	return (ItemClass == item::ItemClassBook);
}

bool EQ::ItemData::IsType1HWeapon() const
{
	return ((ItemType == item::ItemType1HBlunt) || (ItemType == item::ItemType1HSlash) || (ItemType == item::ItemType1HPiercing));
}

bool EQ::ItemData::IsType2HWeapon() const
{
	return ((ItemType == item::ItemType2HBlunt) || (ItemType == item::ItemType2HSlash) || (ItemType == item::ItemType2HPiercing));
}

bool EQ::ItemData::IsTypeShield() const
{
	return (ItemType == item::ItemTypeShield);
}

bool EQ::ItemData::IsStackable() const
{
	return 
		ItemClass == EQ::item::ItemClass::ItemClassCommon &&
		MaxCharges > 0 &&
		(
			ItemType == EQ::item::ItemTypeFood ||
			ItemType == EQ::item::ItemTypeDrink ||
			ItemType == EQ::item::ItemTypeCombinable ||
			ItemType == EQ::item::ItemTypeBandage ||
			ItemType == EQ::item::ItemTypeSmallThrowing ||
			ItemType == EQ::item::ItemTypeArrow ||
			ItemType == EQ::item::ItemTypeUnknown4 ||
			ItemType == EQ::item::ItemTypeFishingBait ||
			ItemType == EQ::item::ItemTypeAlcohol
		);
}
