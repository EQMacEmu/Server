/*	EQEMu:  Everquest Server Emulator
	
	Copyright (C) 2001-2022 EQEMu Development Team (http://eqemulator.net)
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

#include "mac_limits.h"

#include "../strings.h"


int16 Mac::invtype::GetInvTypeSize(int16 inv_type)
{
	switch (inv_type) {
	case invtype::typePossessions:
		return invtype::POSSESSIONS_SIZE;
	case invtype::typeBank:
		return invtype::BANK_SIZE;
	case invtype::typeTrade:
		return invtype::TRADE_SIZE;
	case invtype::typeWorld:
		return invtype::WORLD_SIZE;
	case invtype::typeLimbo:
		return invtype::LIMBO_SIZE;
	case invtype::typeMerchant:
		return invtype::MERCHANT_SIZE;
	case invtype::typeCorpse:
		return invtype::CORPSE_SIZE;
	case invtype::typeBazaar:
		return invtype::BAZAAR_SIZE;
	case invtype::typeInspect:
		return invtype::INSPECT_SIZE;
	default:
		return INULL;
	}
}

const char* Mac::invtype::GetInvTypeName(int16  inv_type)
{
	switch (inv_type) {
	case invtype::TYPE_INVALID:
		return "Invalid Type";
	case invtype::typePossessions:
		return "Possessions";
	case invtype::typeBank:
		return "Bank";
	case invtype::typeTrade:
		return "Trade";
	case invtype::typeWorld:
		return "World";
	case invtype::typeLimbo:
		return "Limbo";
	case invtype::typeMerchant:
		return "Merchant";
	case invtype::typeCorpse:
		return "Corpse";
	case invtype::typeBazaar:
		return "Bazaar";
	case invtype::typeInspect:
		return "Inspect";
	default:
		return "Unknown Type";
	}
}

bool Mac::invtype::IsInvTypePersistent(int16 inv_type)
{
	switch (inv_type) {
	case invtype::typePossessions:
	case invtype::typeBank:
	case invtype::typeTrade:
	case invtype::typeWorld:
	case invtype::typeLimbo:
		return true;
	default:
		return false;
	}
}

const char* Mac::invslot::GetInvPossessionsSlotName(int16 inv_slot)
{
	switch (inv_slot) {
	case invslot::SLOT_INVALID:
		return "Invalid Slot";
	case invslot::slotCursor:
		return "Cursor";
	case invslot::slotEar1:
		return "Ear 1";
	case invslot::slotHead:
		return "Head";
	case invslot::slotFace:
		return "Face";
	case invslot::slotEar2:
		return "Ear 2";
	case invslot::slotNeck:
		return "Neck";
	case invslot::slotShoulders:
		return "Shoulders";
	case invslot::slotArms:
		return "Arms";
	case invslot::slotBack:
		return "Back";
	case invslot::slotWrist1:
		return "Wrist 1";
	case invslot::slotWrist2:
		return "Wrist 2";
	case invslot::slotRange:
		return "Range";
	case invslot::slotHands:
		return "Hands";
	case invslot::slotPrimary:
		return "Primary";
	case invslot::slotSecondary:
		return "Secondary";
	case invslot::slotFinger1:
		return "Finger 1";
	case invslot::slotFinger2:
		return "Finger 2";
	case invslot::slotChest:
		return "Chest";
	case invslot::slotLegs:
		return "Legs";
	case invslot::slotFeet:
		return "Feet";
	case invslot::slotWaist:
		return "Waist";
	case invslot::slotAmmo:
		return "Ammo";
	case invslot::slotGeneral1:
		return "General 1";
	case invslot::slotGeneral2:
		return "General 2";
	case invslot::slotGeneral3:
		return "General 3";
	case invslot::slotGeneral4:
		return "General 4";
	case invslot::slotGeneral5:
		return "General 5";
	case invslot::slotGeneral6:
		return "General 6";
	case invslot::slotGeneral7:
		return "General 7";
	case invslot::slotGeneral8:
		return "General 8";
	default:
		return "Unknown Slot";
	}
}

const char* Mac::invslot::GetInvCorpseSlotName(int16 inv_slot)
{
	if (!invtype::GetInvTypeSize(invtype::typeCorpse) || inv_slot == invslot::SLOT_INVALID)
		return "Invalid Slot";

	// needs work
	if ((inv_slot + 1) < invslot::CORPSE_BEGIN || (inv_slot + 1) >= invslot::CORPSE_END)
		return "Unknown Slot";

	static std::string ret_str;
	ret_str = StringFormat("Slot %i", (inv_slot + 1));

	return ret_str.c_str();
}

const char* Mac::invslot::GetInvSlotName(int16 inv_type, int16 inv_slot)
{
	if (inv_type == invtype::typePossessions)
		return invslot::GetInvPossessionsSlotName(inv_slot);
	else if (inv_type == invtype::typeCorpse)
		return invslot::GetInvCorpseSlotName(inv_slot);

	int16 type_size = invtype::GetInvTypeSize(inv_type);

	if (!type_size || inv_slot == invslot::SLOT_INVALID)
		return "Invalid Slot";

	if ((size_t)(inv_slot + 1) >= type_size)
		return "Unknown Slot";

	static std::string ret_str;
	ret_str = StringFormat("Slot %i", (inv_slot + 1));

	return ret_str.c_str();
}

const char* Mac::invbag::GetInvBagIndexName(int16 bag_index)
{
	if (bag_index == invbag::SLOT_INVALID)
		return "Invalid Bag";

	if ((size_t)bag_index >= invbag::SLOT_COUNT)
		return "Unknown Bag";

	static std::string ret_str;
	ret_str = StringFormat("Bag %i", (bag_index + 1));

	return ret_str.c_str();
}
