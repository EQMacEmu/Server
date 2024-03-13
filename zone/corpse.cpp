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
/*
New class for handling corpses and everything associated with them.
Child of the Mob class.
-Quagmire
*/

#ifdef _WINDOWS
	#if (!defined(_MSC_VER) || (defined(_MSC_VER) && _MSC_VER < 1900))
		#define snprintf	_snprintf
		#define vsnprintf	_vsnprintf
	#endif
    #define strncasecmp	_strnicmp
    #define strcasecmp	_stricmp
#endif

#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include "../common/rulesys.h"
#include "../common/strings.h"

#include "client.h"
#include "corpse.h"
#include "entity.h"
#include "groups.h"
#include "mob.h"
#include "raids.h"

#include "quest_parser_collection.h"
#include "string_ids.h"
#include "worldserver.h"
#include "queryserv.h"
#include <iostream>

extern EntityList entity_list;
extern Zone* zone;
extern WorldServer worldserver;
extern QueryServ* QServ;

void Corpse::SendEndLootErrorPacket(Client* client) {
	auto outapp = new EQApplicationPacket(OP_LootComplete, 0);
	client->QueuePacket(outapp);
	safe_delete(outapp);
}

void Corpse::SendLootReqErrorPacket(Client* client, uint8 response) {
	auto outapp = new EQApplicationPacket(OP_MoneyOnCorpse, sizeof(moneyOnCorpseStruct));
	moneyOnCorpseStruct* d = (moneyOnCorpseStruct*)outapp->pBuffer;
	d->response = response;
	d->unknown1 = 0x5a;
	d->unknown2 = 0x40;
	client->QueuePacket(outapp);
	safe_delete(outapp);
}

Corpse* Corpse::LoadCharacterCorpseEntity(uint32 in_dbid, uint32 in_charid, std::string in_charname, const glm::vec4& position, uint32 timestamp, bool rezzed, bool was_at_graveyard){
	uint32 item_count = database.GetCharacterCorpseItemCount(in_dbid);
	auto buffer =
	    new char[sizeof(PlayerCorpse_Struct) + (item_count * sizeof(ServerLootItem_Struct))];
	PlayerCorpse_Struct *pcs = (PlayerCorpse_Struct*)buffer;
	database.LoadCharacterCorpseData(in_dbid, pcs);

	/* Load Items */ 
	ItemList itemlist;
	ServerLootItem_Struct* tmp = nullptr;
	for (unsigned int i = 0; i < pcs->itemcount; i++) {
		tmp = new ServerLootItem_Struct;
		memcpy(tmp, &pcs->items[i], sizeof(ServerLootItem_Struct));
		itemlist.push_back(tmp);
	}

	/* Create Corpse Entity */
	auto pc = new Corpse(
		in_dbid,			   // uint32 in_dbid
		in_charid,			   // uint32 in_charid
		in_charname.c_str(),   // char* in_charname
		&itemlist,			   // ItemList* in_itemlist
		pcs->copper,		   // uint32 in_copper
		pcs->silver,		   // uint32 in_silver
		pcs->gold,			   // uint32 in_gold
		pcs->plat,			   // uint32 in_plat
		position,
		pcs->size,			   // float in_size
		pcs->gender,		   // uint8 in_gender
		pcs->race,			   // uint16 in_race
		pcs->class_,		   // uint8 in_class
		pcs->deity,			   // uint8 in_deity
		pcs->level,			   // uint8 in_level
		pcs->texture,		   // uint8 in_texture
		pcs->helmtexture,	   // uint8 in_helmtexture
		pcs->exp,			   // uint32 in_rezexp
		pcs->gmexp,			   // uint32 in_gmrezexp
		pcs->killedby,		   // uint8 in_killedby
		pcs->rezzable,		   // bool rezzable
		pcs->rez_time,		   // uint32 rez_time
		was_at_graveyard	   // bool wasAtGraveyard
	);
	if (pcs->locked){
		pc->Lock();
	}

	/* Load Item Tints */
	pc->item_tint.Head.Color = pcs->item_tint.Head.Color;
	pc->item_tint.Chest.Color = pcs->item_tint.Chest.Color;
	pc->item_tint.Arms.Color = pcs->item_tint.Arms.Color;
	pc->item_tint.Wrist.Color = pcs->item_tint.Wrist.Color;
	pc->item_tint.Hands.Color = pcs->item_tint.Hands.Color;
	pc->item_tint.Legs.Color = pcs->item_tint.Legs.Color;
	pc->item_tint.Feet.Color = pcs->item_tint.Feet.Color;
	pc->item_tint.Primary.Color = pcs->item_tint.Primary.Color;
	pc->item_tint.Secondary.Color = pcs->item_tint.Secondary.Color;

	/* Load Physical Appearance */
	pc->haircolor = pcs->haircolor;
	pc->beardcolor = pcs->beardcolor;
	pc->eyecolor1 = pcs->eyecolor1;
	pc->eyecolor2 = pcs->eyecolor2;
	pc->hairstyle = pcs->hairstyle;
	pc->luclinface = pcs->face;
	pc->beard = pcs->beard;
	pc->IsRezzed(rezzed);
	pc->become_npc = false;
	pc->time_of_death = timestamp;

	pc->UpdateEquipmentLight(); // itemlist populated above..need to determine actual values

	safe_delete_array(pcs);

	return pc;
}

Corpse::Corpse(NPC* in_npc, ItemList* in_itemlist, uint32 in_npctypeid, const NPCType** in_npctypedata, uint32 in_decaytime, bool is_client_pet)
	: Mob("Unnamed_Corpse",		// const char* in_name,
	"",							// const char* in_lastname,
	0,							// int32		in_cur_hp,
	0,							// int32		in_max_hp,
	in_npc->GetGender(),		// uint8		in_gender,
	in_npc->GetRace(),			// uint16		in_race,
	in_npc->GetClass(),			// uint8		in_class,
	BT_Humanoid,				// bodyType	in_bodytype,
	in_npc->GetDeity(),			// uint8		in_deity,
	in_npc->GetLevel(),			// uint8		in_level,
	in_npc->GetNPCTypeID(),		// uint32		in_npctype_id,
	in_npc->GetSize(),			// float		in_size,
	0,							// float		in_runspeed,
	in_npc->GetPosition(),		// float		in_position
	in_npc->GetInnateLightType(),	// uint8		in_light,
	in_npc->GetTexture(),		// uint8		in_texture,
	in_npc->GetHelmTexture(),	// uint8		in_helmtexture,
	0,							// uint16		in_ac,
	0,							// uint16		in_atk,
	0,							// uint16		in_str,
	0,							// uint16		in_sta,
	0,							// uint16		in_dex,
	0,							// uint16		in_agi,
	0,							// uint16		in_int,
	0,							// uint16		in_wis,
	0,							// uint16		in_cha,
	0,							// uint8		in_haircolor,
	0,							// uint8		in_beardcolor,
	0,							// uint8		in_eyecolor1, // the eyecolors always seem to be the same, maybe left and right eye?
	0,							// uint8		in_eyecolor2,
	0,							// uint8		in_hairstyle,
	0,							// uint8		in_luclinface,
	0,							// uint8		in_beard,
	EQ::TintProfile(),			// uint32		in_armor_tint[EQ::textures::TextureCount], 
	0xff,						// uint8		in_aa_title,
	0,							// uint8		in_see_invis, // see through invis/ivu
	0,							// uint8		in_see_invis_undead,
	0,							// uint8		in_see_sneak,
	0,							// uint8		in_see_improved_hide,
	0,							// int32		in_hp_regen,
	0,							// int32		in_mana_regen,
	0,							// uint8		in_qglobal,
	0,							// uint8		in_maxlevel,
	0,							// uint32		in_scalerate
	0,							// uint8		in_armtexture,
	0,							// uint8		in_bracertexture,
	0,							// uint8		in_handtexture,
	0,							// uint8		in_legtexture,
	0,							// uint8		in_feettexture,
	0							// uint8		in_chesttexture,
),									  
	corpse_decay_timer(in_decaytime),
	corpse_rez_timer(0),
	corpse_delay_timer(RuleI(NPC, CorpseUnlockTimer)),
	corpse_graveyard_timer(0),
	loot_cooldown_timer(10)
{
	corpse_graveyard_timer.Disable();
	corpse_graveyard_moved_timer.Disable();

	is_corpse_changed = false;
	is_player_corpse = false;
	rezzable = false;
	is_owner_online = false;
	is_locked = false;
	flymode = 0;
	being_looted_by = 0xFFFFFFFF;
	if (in_itemlist) {
		itemlist = *in_itemlist;
		in_itemlist->clear();
	}

	SetCash(in_npc->GetCopper(), in_npc->GetSilver(), in_npc->GetGold(), in_npc->GetPlatinum());

	npctype_id = in_npctypeid;
	SetPlayerKillItemID(0);
	char_id = 0;
	corpse_db_id = 0;
	player_corpse_depop = false;
	strcpy(corpse_name, in_npc->GetName());
	strcpy(name, in_npc->GetName());

	if (IsEmpty() && in_decaytime > RuleI(NPC, EmptyNPCCorpseDecayTimeMS))
	{
		corpse_decay_timer.Start(RuleI(NPC, EmptyNPCCorpseDecayTimeMS) + 1000);
	}
	
	if(in_npc->HasPrivateCorpse()) {
		corpse_delay_timer.Start(GetDecayTime() + 1000);
	}

	for (int i = 0; i < MAX_LOOTERS; i++){
		allowed_looters[i] = 0;
	}
	if (is_client_pet)
	{
		allowed_looters[0] = sizeof(int);		// corpses without looters are apparently lootable by anybody, so doing this to make it unlootable
		corpse_decay_timer.Start(3000);
	}

	this->rez_experience = 0;
	this->gm_rez_experience = 0;
	UpdateEquipmentLight();
	UpdateActiveLight();
}

Corpse::Corpse(Client* client, int32 in_rezexp, uint8 in_killedby) : Mob (
	"Unnamed_Corpse",				  // const char*	in_name,
	"",								  // const char*	in_lastname,
	0,								  // int32		in_cur_hp,
	0,								  // int32		in_max_hp,
	client->GetGender(),			  // uint8		in_gender,
	client->GetRace(),				  // uint16		in_race,
	client->GetClass(),				  // uint8		in_class,
	BT_Humanoid,					  // bodyType	in_bodytype,
	client->GetDeity(),				  // uint8		in_deity,
	client->GetLevel(),				  // uint8		in_level,
	0,								  // uint32		in_npctype_id,
	client->GetSize(),				  // float		in_size,
	0,								  // float		in_runspeed,
	client->GetPosition(),
	client->GetInnateLightType(),	  // uint8		in_light, - verified for client innate_light value
	client->GetTexture(),			  // uint8		in_texture,
	client->GetHelmTexture(),		  // uint8		in_helmtexture,
	0,								  // uint16		in_ac,
	0,								  // uint16		in_atk,
	0,								  // uint16		in_str,
	0,								  // uint16		in_sta,
	0,								  // uint16		in_dex,
	0,								  // uint16		in_agi,
	0,								  // uint16		in_int,
	0,								  // uint16		in_wis,
	0,								  // uint16		in_cha,
	client->GetPP().haircolor,		  // uint8		in_haircolor,
	client->GetPP().beardcolor,		  // uint8		in_beardcolor,
	client->GetPP().eyecolor1,		  // uint8		in_eyecolor1, // the eye colors always seem to be the same, maybe left and right eye?
	client->GetPP().eyecolor2,		  // uint8		in_eyecolor2,
	client->GetPP().hairstyle,		  // uint8		in_hairstyle,
	client->GetPP().face,			  // uint8		in_luclinface,
	client->GetPP().beard,			  // uint8		in_beard,
	EQ::TintProfile(),					// uint32		in_armor_tint[EQ::textures::TextureCount],
	0xff,							  // uint8		in_aa_title,
	0,								  // uint8		in_see_invis, // see through invis
	0,								  // uint8		in_see_invis_undead, // see through invis vs. un dead
	0,								  // uint8		in_see_sneak,
	0,								  // uint8		in_see_improved_hide,
	0,								  // int32		in_hp_regen,
	0,								  // int32		in_mana_regen,
	0,								  // uint8		in_qglobal,
	0,								  // uint8		in_maxlevel,
	0,								  // uint32		in_scalerate
	0,								  // uint8		in_armtexture,
	0,								  // uint8		in_bracertexture,
	0,								  // uint8		in_handtexture,
	0,								  // uint8		in_legtexture,
	0,								  // uint8		in_feettexture,
	0								  // uint8		in_chesttexture,
	),
	corpse_decay_timer(RuleI(Character, EmptyCorpseDecayTimeMS)),
	corpse_rez_timer(RuleI(Character, CorpseResTimeMS)),
	corpse_delay_timer(RuleI(NPC, CorpseUnlockTimer)),
	corpse_graveyard_timer(zone->graveyard_timer() * 60000),
	loot_cooldown_timer(10)
{
	int i;

	PlayerProfile_Struct *pp = &client->GetPP();
	EQ::ItemInstance *item = nullptr;

	/* Check if Zone has Graveyard First */
	if(!zone->HasGraveyard()) {
		corpse_graveyard_timer.Disable();
	}
	corpse_graveyard_moved_timer.Disable();

	for (i = 0; i < MAX_LOOTERS; i++){
		allowed_looters[i] = 0;
	}

	is_corpse_changed		= true;
	rez_experience			= in_rezexp;
	gm_rez_experience		= in_rezexp;
	is_player_corpse	= true;
	is_locked			= false;
	being_looted_by	= 0xFFFFFFFF;
	char_id			= client->CharacterID();
	corpse_db_id	= 0;
	player_corpse_depop			= false;
	copper			= 0;
	silver			= 0;
	gold			= 0;
	platinum		= 0;
	killedby		= in_killedby;
	rezzable		= true;
	rez_time		= 0;
	is_owner_online = false; // We can't assume they are online just because they just died. Perhaps they rage smashed their router.
	flymode			= 0;
	time_of_death	= static_cast<uint32>(time(nullptr));

	owner_online_timer.Start(RuleI(Character, CorpseOwnerOnlineTimeMS));

	corpse_rez_timer.Disable();
	SetRezTimer(true);

	strcpy(corpse_name, pp->name);
	strcpy(name, pp->name);

	/* become_npc was not being initialized which led to some pretty funky things with newly created corpses */
	become_npc = false;

	SetPlayerKillItemID(0);

	/* Check Rule to see if we can leave corpses */
	if(!RuleB(Character, LeaveNakedCorpses) ||
		RuleB(Character, LeaveCorpses) &&
		GetLevel() >= RuleI(Character, DeathItemLossLevel)) {
		// cash
		SetCash(pp->copper, pp->silver, pp->gold, pp->platinum);
		pp->copper = 0;
		pp->silver = 0;
		pp->gold = 0;
		pp->platinum = 0;

		// get their tints
		memcpy(&item_tint.Slot, &client->GetPP().item_material, sizeof(item_tint));

		// worn + inventory + cursor
		std::list<uint32> removed_list;
		bool cursor = false;

		for (i = EQ::invslot::SLOT_BEGIN; i < EQ::invslot::POSSESSIONS_COUNT; i++)
		{
			item = client->GetInv().GetItem(i);
			if (item && !item->GetItem()->Soulbound)
			{
				if (!client->IsBecomeNPC() || (client->IsBecomeNPC() && !item->GetItem()->NoRent))
				{
					std::list<uint32> slot_list = MoveItemToCorpse(client, item, i);
					removed_list.merge(slot_list);
				}
			}
			else if (item && item->GetItem()->Soulbound)
				Log(Logs::Moderate, Logs::Inventory, "Skipping Soulbound item %s in slot %d", item->GetItem()->Name, i);
		}

		for (i = EQ::invslot::TRADE_BEGIN; i < EQ::invslot::TRADE_END; i++)
		{
			item = client->GetInv().GetItem(i);
			if (item )
			{
				if (!client->IsBecomeNPC())
				{
					std::list<uint32> slot_list = MoveItemToCorpse(client, item, i);
					removed_list.merge(slot_list);
				}
			}
		}

		database.TransactionBegin();
		if (removed_list.size() != 0) {
			std::stringstream ss("");
			ss << "DELETE FROM character_inventory WHERE id=" << client->CharacterID();
			ss << " AND (";
			std::list<uint32>::const_iterator iter = removed_list.begin();
			bool first = true;
			while (iter != removed_list.end()) {
				if (first) {
					first = false;
				}
				else {
					ss << " OR ";
				}
				ss << "slotid=" << (*iter);
				++iter;
			}
			ss << ")";
			database.QueryDatabase(ss.str().c_str());
		}

		auto start = client->GetInv().cursor_cbegin();
		auto finish = client->GetInv().cursor_cend();
		// If soulbound items were moved to the cursor, they need to be moved to a primary inventory slot.
		database.SaveSoulboundItems(client, start, finish);

		// handle any left over items on cursor queue
		if (client->GetInv().CursorSize() > 0) {
			i = EQ::invslot::CURSOR_QUEUE_BEGIN + 1;
			// see if we have any non-soulboud items left in cursor queue
			while ( (item = client->GetInv().PopItem(EQ::invslot::slotCursor)) ) {
				if (item && !item->GetItem()->Soulbound) // soulbound were moved earlier
					AddItem(item->GetItem()->ID, item->GetCharges(), i++);
			}
			if (i > (EQ::invslot::CURSOR_QUEUE_BEGIN + 1)) {
				// now wipe out the cursor items in the db
				std::string query = StringFormat("DELETE FROM character_inventory WHERE id = %i "
					"AND ((slotid >= %i AND slotid <= %i) "
					"OR slotid = %i OR (slotid >= %i AND slotid <= %i) )",
					client->CharacterID(), EQ::invslot::CURSOR_QUEUE_BEGIN, EQ::invslot::CURSOR_QUEUE_END, 
					EQ::invslot::slotCursor, EQ::invbag::CURSOR_BAG_BEGIN, EQ::invbag::CURSOR_BAG_END);
				auto results = database.QueryDatabase(query);
			}
		}
			
		client->CalcBonuses(); // will only affect offline profile viewing of dead characters..unneeded overhead
		client->Save();

		IsRezzed(false);
		Save();
		database.TransactionCommit();

		if(!IsEmpty()) {
			corpse_decay_timer.Start(RuleI(Character, CorpseDecayTimeMS));
		}
		else if (IsEmpty() && RuleB(Character, SacrificeCorpseDepop) && killedby == Killed_Sac &&
			(GetZoneID() == poknowledge || GetZoneID() == nexus || GetZoneID() == bazaar))
		{
			corpse_decay_timer.Start(180000);
		}
		return;
	} //end "not leaving naked corpses"

	UpdateEquipmentLight();
	UpdateActiveLight();

	IsRezzed(false);
	Save();
}

std::list<uint32> Corpse::MoveItemToCorpse(Client *client, EQ::ItemInstance *item, int16 equipslot)
{
	int bagindex;
	int16 interior_slot;
	EQ::ItemInstance *interior_item;
	std::list<uint32> returnlist;

	AddItem(item->GetItem()->ID, item->GetCharges(), equipslot);
	returnlist.push_back(equipslot);

	// Qualified bag slot iterations. processing bag slots that don't exist is probably not a good idea.
	// Limit the bag check to inventory, trade, and cursor slots.
	if (item->IsClassBag() &&
		(equipslot == EQ::invslot::SLOT_BEGIN ||
		(equipslot >= EQ::invslot::GENERAL_BEGIN && equipslot <= EQ::invslot::GENERAL_END) || 
		(equipslot >= EQ::invslot::TRADE_BEGIN && equipslot <= EQ::invslot::TRADE_END)))
	{ 
		for (bagindex = EQ::invbag::SLOT_BEGIN; bagindex <= EQ::invbag::SLOT_END; bagindex++) 
		{
			// For empty bags in cursor queue, slot was previously being resolved as SLOT_INVALID (-1)
			interior_slot = EQ::InventoryProfile::CalcSlotId(equipslot, bagindex);
			interior_item = client->GetInv().GetItem(interior_slot);

			if (interior_item && !interior_item->GetItem()->Soulbound) 
			{
				AddItem(interior_item->GetItem()->ID, interior_item->GetCharges(), interior_slot);
				returnlist.push_back(EQ::InventoryProfile::CalcSlotId(equipslot, bagindex));
				client->DeleteItemInInventory(interior_slot);
			}
			else if(interior_item && interior_item->GetItem()->Soulbound)
			{
				client->PushItemOnCursor(*interior_item, true); // Push to cursor for now, since parent bag is about to be deleted.
				client->DeleteItemInInventory(interior_slot);
				Log(Logs::Moderate, Logs::Inventory, "Skipping Soulbound item %s in slot %d", interior_item->GetItem()->Name, interior_slot);
			}
		}
	}
	client->DeleteItemInInventory(equipslot);
	return returnlist;
}

/* Called from Database Load */

Corpse::Corpse(uint32 in_dbid, uint32 in_charid, const char* in_charname, ItemList* in_itemlist, uint32 in_copper, uint32 in_silver, uint32 in_gold, uint32 in_plat, const glm::vec4& position, float in_size, uint8 in_gender, uint16 in_race, uint8 in_class, uint8 in_deity, uint8 in_level, uint8 in_texture, uint8 in_helmtexture, uint32 in_rezexp, uint32 in_gmrezexp, uint8 in_killedby, bool in_rezzable, uint32 in_rez_time, bool wasAtGraveyard)
	: Mob("Unnamed_Corpse", // const char* in_name,
	"",						// const char* in_lastname,
	0,						// int32		in_cur_hp,
	0,						// int32		in_max_hp,
	in_gender,				// uint8		in_gender,
	in_race,				// uint16		in_race,
	in_class,				// uint8		in_class,
	BT_Humanoid,			// bodyType	in_bodytype,
	in_deity,				// uint8		in_deity,
	in_level,				// uint8		in_level,
	0,						// uint32		in_npctype_id,
	in_size,				// float		in_size,
	0,						// float		in_runspeed,
	position,
	0,						// uint8		in_light,
	in_texture,				// uint8		in_texture,
	in_helmtexture,			// uint8		in_helmtexture,
	0,						// uint16		in_ac,
	0,						// uint16		in_atk,
	0,						// uint16		in_str,
	0,						// uint16		in_sta,
	0,						// uint16		in_dex,
	0,						// uint16		in_agi,
	0,						// uint16		in_int,
	0,						// uint16		in_wis,
	0,						// uint16		in_cha,
	0,						// uint8		in_haircolor,
	0,						// uint8		in_beardcolor,
	0,						// uint8		in_eyecolor1, // the eyecolors always seem to be the same, maybe left and right eye?
	0,						// uint8		in_eyecolor2,
	0,						// uint8		in_hairstyle,
	0,						// uint8		in_luclinface,
	0,						// uint8		in_beard,
	EQ::TintProfile(),		// uint32		in_armor_tint[EQ::textures::TextureCount], 
	0xff,					// uint8		in_aa_title,
	0,						// uint8		in_see_invis, // see through invis/ivu
	0,						// uint8		in_see_invis_undead,
	0,						// uint8		in_see_sneak,
	0,						// uint8		in_see_improved_hide,
	0,						// int32		in_hp_regen,
	0,						// int32		in_mana_regen,
	0,						// uint8		in_qglobal,
	0,						// uint8		in_maxlevel,
	0,						// uint32		in_scalerate
	0,						// uint8		in_armtexture,
	0,						// uint8		in_bracertexture,
	0,						// uint8		in_handtexture,
	0,						// uint8		in_legtexture,
	0,						// uint8		in_feettexture,
	0						// uint8		in_chesttexture,
	),						
	corpse_decay_timer(RuleI(Character, EmptyCorpseDecayTimeMS)),
	corpse_rez_timer(RuleI(Character, CorpseResTimeMS)),
	corpse_delay_timer(RuleI(NPC, CorpseUnlockTimer)),
	corpse_graveyard_timer(zone->graveyard_timer() * 60000),
	loot_cooldown_timer(10)
{
	// The timer needs to know if the corpse has items or not before we actually apply the items
	// to the corpse. The corpse seems to poof shortly after the timer is applied if it is done so
	// after items are loaded.
	bool empty = true;
	if (!in_itemlist->empty() || in_copper != 0 || in_silver != 0 || in_gold != 0 || in_plat != 0)
		empty = false;

	LoadPlayerCorpseDecayTime(in_dbid, empty);

	if (!zone->HasGraveyard() || wasAtGraveyard){
		corpse_graveyard_timer.Disable();
	}
	corpse_graveyard_moved_timer.Disable();

	is_corpse_changed = false;
	is_player_corpse = true;
	is_locked = false;
	being_looted_by = 0xFFFFFFFF;
	corpse_db_id = in_dbid;
	player_corpse_depop = false;
	char_id = in_charid;
	itemlist = *in_itemlist;
	in_itemlist->clear();

	strcpy(corpse_name, in_charname);
	strcpy(name, in_charname);

	this->copper = in_copper;
	this->silver = in_silver;
	this->gold = in_gold;
	this->platinum = in_plat;

	rez_experience = in_rezexp;
	gm_rez_experience = in_gmrezexp;
	killedby = in_killedby;
	rezzable = in_rezzable;
	rez_time = in_rez_time;
	is_owner_online = false;

	owner_online_timer.Start(RuleI(Character, CorpseOwnerOnlineTimeMS));

	corpse_rez_timer.Disable();
	SetRezTimer();

	for (int i = 0; i < MAX_LOOTERS; i++){
		allowed_looters[i] = 0;
	}
	SetPlayerKillItemID(0);

	UpdateEquipmentLight();

	UpdateActiveLight();
}

Corpse::~Corpse() {
	if (is_player_corpse && !(player_corpse_depop && corpse_db_id == 0)) {
		Save();
	}
	ItemList::iterator cur, end;
	cur = itemlist.begin();
	end = itemlist.end();
	for (; cur != end; ++cur) {
		ServerLootItem_Struct* item = *cur;
		safe_delete(item);
	}
	itemlist.clear();
}

/*
this needs to be called AFTER the entity_id is set
the client does this too, so it's unchangeable
*/
void Corpse::CalcCorpseName() {
	EntityList::RemoveNumbers(name);
	char tmp[64];
	if (is_player_corpse){
		snprintf(tmp, sizeof(tmp), "'s corpse%d", GetID());
	}
	else{
		snprintf(tmp, sizeof(tmp), "`s_corpse%d", GetID());
	}
	name[(sizeof(name) - 1) - strlen(tmp)] = 0;
	strcat(name, tmp);
}

bool Corpse::Save() {
	if (!is_player_corpse)
		return true;
	if (!is_corpse_changed)
		return true;

	uint32 tmp = this->CountItems();
	uint32 tmpsize = sizeof(PlayerCorpse_Struct) + (tmp * sizeof(ServerLootItem_Struct));

	PlayerCorpse_Struct* dbpc = (PlayerCorpse_Struct*) new uchar[tmpsize];
	memset(dbpc, 0, tmpsize);
	dbpc->itemcount = tmp;
	dbpc->size = this->size;
	dbpc->locked = is_locked;
	dbpc->copper = this->copper;
	dbpc->silver = this->silver;
	dbpc->gold = this->gold;
	dbpc->plat = this->platinum;
	dbpc->race = this->race;
	dbpc->class_ = class_;
	dbpc->gender = gender;
	dbpc->deity = deity;
	dbpc->level = level;
	dbpc->texture = this->texture;
	dbpc->helmtexture = this->helmtexture;
	dbpc->exp = rez_experience;
	dbpc->gmexp = gm_rez_experience;
	dbpc->killedby = killedby;
	dbpc->rezzable = rezzable;
	dbpc->rez_time = rez_time;

	memcpy(&dbpc->item_tint.Slot, &item_tint.Slot, sizeof(dbpc->item_tint));
	dbpc->haircolor = haircolor;
	dbpc->beardcolor = beardcolor;
	dbpc->eyecolor2 = eyecolor1;
	dbpc->hairstyle = hairstyle;
	dbpc->face = luclinface;
	dbpc->beard = beard;
	dbpc->time_of_death = time_of_death;

	uint32 x = 0;
	ItemList::iterator cur, end;
	cur = itemlist.begin();
	end = itemlist.end();
	for (; cur != end; ++cur) {
		ServerLootItem_Struct* item = *cur;
		memcpy((char*)&dbpc->items[x++], (char*)item, sizeof(ServerLootItem_Struct));
	}

	/* Create New Corpse*/
	if (corpse_db_id == 0) {
		corpse_db_id = database.SaveCharacterCorpse(char_id, corpse_name, zone->GetZoneID(), dbpc, m_Position);
		if(!IsEmpty() && RuleB(Character, UsePlayerCorpseBackups))
		{
			database.SaveCharacterCorpseBackup(corpse_db_id, char_id, corpse_name, zone->GetZoneID(), dbpc, m_Position);
		}
	}
	/* Update Corpse Data */
	else{
		corpse_db_id = database.UpdateCharacterCorpse(corpse_db_id, char_id, corpse_name, zone->GetZoneID(), dbpc, m_Position, IsRezzed());
		if(!IsEmpty() && RuleB(Character, UsePlayerCorpseBackups))
		{
			database.UpdateCharacterCorpseBackup(corpse_db_id, char_id, corpse_name, zone->GetZoneID(), dbpc, m_Position, IsRezzed());
		}
	}

	safe_delete_array(dbpc);

	return true;
}

void Corpse::Delete() 
{
	if (IsPlayerCorpse() && corpse_db_id != 0)
	{
		RevokeConsent();
		database.DeleteCharacterCorpse(corpse_db_id);
	}

	corpse_db_id = 0;
	player_corpse_depop = true;
}

void Corpse::Bury() 
{

	if (IsPlayerCorpse() && corpse_db_id != 0)
	{
		RevokeConsent();
		database.BuryCharacterCorpse(corpse_db_id);
	}

	corpse_db_id = 0;
	player_corpse_depop = true;
}

void Corpse::DepopNPCCorpse() {
	if (IsNPCCorpse())
		player_corpse_depop = true;
}

void Corpse::DepopPlayerCorpse() {
	player_corpse_depop = true;
}

uint32 Corpse::CountItems() {
	return itemlist.size();
}

void Corpse::AddItem(uint32 itemnum, int8 charges, int16 slot) {
	if (!database.GetItem(itemnum))
		return;

	Log(Logs::Detail, Logs::Inventory, "(Corpse) AddItem(%i, %i, %i)", slot, charges, itemnum);

	is_corpse_changed = true;

	auto item = new ServerLootItem_Struct;
	
	memset(item, 0, sizeof(ServerLootItem_Struct));
	item->item_id = itemnum;
	item->charges = charges;
	item->equip_slot = slot;
	itemlist.push_back(item);

	UpdateEquipmentLight();
}

ServerLootItem_Struct* Corpse::GetItem(uint16 lootslot, ServerLootItem_Struct** bag_item_data) {
	ServerLootItem_Struct *sitem = nullptr, *sitem2 = nullptr;

	ItemList::iterator cur, end;
	cur = itemlist.begin();
	end = itemlist.end();
	for(; cur != end; ++cur) {
		if((*cur)->lootslot == lootslot) {
			sitem = *cur;
			break;
		}
	}

	if (sitem && bag_item_data && EQ::InventoryProfile::SupportsContainers(sitem->equip_slot)) {
		int16 bagstart = EQ::InventoryProfile::CalcSlotId(sitem->equip_slot, EQ::invbag::SLOT_BEGIN);

		cur = itemlist.begin();
		end = itemlist.end();
		for (; cur != end; ++cur) {
			sitem2 = *cur;
			if (sitem2->equip_slot >= bagstart && sitem2->equip_slot < bagstart + 10) {
				bag_item_data[sitem2->equip_slot - bagstart] = sitem2;
			}
		}
	}

	return sitem;
}

uint32 Corpse::GetWornItem(int16 equipSlot) const {
	ItemList::const_iterator cur, end;
	cur = itemlist.begin();
	end = itemlist.end();
	for (; cur != end; ++cur) {
		ServerLootItem_Struct* item = *cur;
		if (item->equip_slot == equipSlot) {
			return item->item_id;
		}
	}

	return 0;
}

void Corpse::RemoveItem(uint16 lootslot) {
	if (lootslot == 0xFFFF)
		return;

	ItemList::iterator cur, end;
	cur = itemlist.begin();
	end = itemlist.end();
	for (; cur != end; ++cur) {
		ServerLootItem_Struct* sitem = *cur;
		if (sitem->lootslot == lootslot) {
			RemoveItem(sitem);
			return;
		}
	}
}

void Corpse::RemoveItem(ServerLootItem_Struct* item_data)
{
	for (auto iter = itemlist.begin(); iter != itemlist.end(); ++iter) {
		auto sitem = *iter;
		if (sitem != item_data) { continue; }

		is_corpse_changed = true;
		itemlist.erase(iter);

		uint8 material = EQ::InventoryProfile::CalcMaterialFromSlot(sitem->equip_slot); // autos to unsigned char

		if (IsNPCCorpse() && sitem->equip_slot == EQ::invslot::slotRange && material == EQ::textures::materialInvalid)
		{
			material = EQ::textures::weaponPrimary;
		}

		if (material != EQ::textures::materialInvalid)
			SendWearChange(material);

		UpdateEquipmentLight();
		if (UpdateActiveLight())
			SendAppearancePacket(AppearanceType::Light, GetActiveLightType());

		safe_delete(sitem);
		return;
	}
}

void Corpse::SetCash(uint32 in_copper, uint32 in_silver, uint32 in_gold, uint32 in_platinum) {
	this->copper = in_copper;
	this->silver = in_silver;
	this->gold = in_gold;
	this->platinum = in_platinum;
	is_corpse_changed = true;
}

void Corpse::RemoveCash() {
	this->copper = 0;
	this->silver = 0;
	this->gold = 0;
	this->platinum = 0;
	is_corpse_changed = true;
}

bool Corpse::IsEmpty() const {
	if (copper != 0 || silver != 0 || gold != 0 || platinum != 0)
		return false;

	return itemlist.empty();
}

bool Corpse::DepopProcess() {
	if (player_corpse_depop) {
		return false;
	}
	return true;
}

bool Corpse::Process() {
	if (player_corpse_depop){
		return false;
	}

	if(owner_online_timer.Check() && rezzable) 
	{
		IsOwnerOnline();
	}

	if (corpse_delay_timer.Check()) {
		for (int i = 0; i < MAX_LOOTERS; i++){
			allowed_looters[i] = 0;
		}
		corpse_delay_timer.Disable();
		return true;
	}

	if (worldserver.Connected() && corpse_graveyard_timer.Check()) {
		if (zone->HasGraveyard()) {
			Save();

			if (being_looted_by != 0xFFFFFFFF)
			{
				Client *looter = entity_list.GetClientByID(being_looted_by);
				if (looter)
				{
					EndLoot(looter, nullptr);
				}
			}

			player_corpse_depop = true;
			database.SendCharacterCorpseToGraveyard(corpse_db_id, zone->graveyard_zoneid(), zone->GetGraveyardPoint());
			corpse_graveyard_timer.Disable();
			auto pack = new ServerPacket(ServerOP_SpawnPlayerCorpse, sizeof(SpawnPlayerCorpse_Struct));
			SpawnPlayerCorpse_Struct* spc = (SpawnPlayerCorpse_Struct*)pack->pBuffer;
			spc->player_corpse_id = corpse_db_id;
			spc->zone_id = zone->graveyard_zoneid();
			worldserver.SendPacket(pack);
			safe_delete(pack);
			Log(Logs::General, Logs::Corpse, "Moved %s player corpse to the designated graveyard in zone %s.", this->GetName(), database.GetZoneName(zone->graveyard_zoneid()));
			corpse_db_id = 0;
		}

		corpse_graveyard_timer.Disable();
		return false;
	}

	if (IsPlayerCorpse() && zone->HasGraveyard() && !corpse_graveyard_timer.Enabled())
	{
		// it already went to GY, prevent dragging it back out
		if (!corpse_graveyard_moved_timer.Enabled())
			corpse_graveyard_moved_timer.Start(2000);
		
		if (corpse_graveyard_moved_timer.Check() && corpse_graveyard_moved_lastpos != GetPosition())
		{
			if (DistanceSquaredNoZ(zone->GetGraveyardPoint(), GetPosition()) > 75.0f * 75.0f)
			{
				Log(Logs::General, Logs::Corpse, "Graveyard corpse %s was moved too far from the graveyard, returning it.", GetName());
				double xcorpse = (zone->GetGraveyardPoint().x + zone->random.Real(-20, 20));
				double ycorpse = (zone->GetGraveyardPoint().y + zone->random.Real(-20, 20));
				GMMove(xcorpse, ycorpse, zone->GetGraveyardPoint().z);
				is_corpse_changed = true;
				Save();
			}
			corpse_graveyard_moved_lastpos = GetPosition();
		}
	}

	//Player is offline. If rez timer is enabled, disable it and save corpse.
	if(!is_owner_online && rezzable)
	{
		if(corpse_rez_timer.Enabled())
		{
			rez_time = corpse_rez_timer.GetRemainingTime();
			corpse_rez_timer.Disable();
			is_corpse_changed = true;
			Save();
		}
	}
	//Player is online. If rez timer is disabled, enable it.
	else if(is_owner_online && rezzable)
	{
		if(corpse_rez_timer.Enabled())
		{
			rez_time = corpse_rez_timer.GetRemainingTime();
		}
		else
		{
			SetRezTimer();
		}
	}

	if(corpse_rez_timer.Check()) 
	{
		CompleteResurrection(true);
	}	

	/* This is when a corpse hits decay timer and does checks*/
	if (corpse_decay_timer.Check()) 
	{
		/* NPC */
		if (IsNPCCorpse())
		{
			corpse_decay_timer.Disable();
			return false;
		}
		else
		{
			/* Client */
			if (!RuleB(Zone, EnableShadowrest)) 
			{
				Delete();
			}
			else 
			{
				Bury();
			}
			corpse_decay_timer.Disable();
			return false;
		}
	}

	return true;
}

void Corpse::SetDecayTimer(uint32 decaytime) {
	if (decaytime == 0)
		corpse_decay_timer.Trigger();
	else
		corpse_decay_timer.Start(decaytime);
}

bool Corpse::CanPlayerLoot(int charid) {
	uint8 looters = 0;
	for (int i = 0; i < MAX_LOOTERS; i++) {
		if (allowed_looters[i] != 0){
			looters++;
		}

		if (allowed_looters[i] == charid){
			return true;
		}
	}
	/* If we have no looters, obviously client can loot */
	if (looters == 0){
		return true;
	}
	return false;
}

void Corpse::AllowPlayerLoot(Mob *them, uint8 slot) {
	if (slot >= MAX_LOOTERS)
		return;
	if (them == nullptr || !them->IsClient())
		return;

	allowed_looters[slot] = them->CastToClient()->CharacterID();
}

void Corpse::MakeLootRequestPackets(Client* client, const EQApplicationPacket* app) {
	// Added 12/08. Started compressing loot struct on live.
	if(player_corpse_depop) {
		SendEndLootErrorPacket(client);
		return;
	}

	if(IsPlayerCorpse() && corpse_db_id == 0) {
		// SendLootReqErrorPacket(client, 0);
		client->Message(CC_Red, "Warning: Corpse's dbid = 0! Corpse will not survive zone shutdown!");
		std::cout << "Error: PlayerCorpse::MakeLootRequestPackets: dbid = 0!" << std::endl;
		// return;
	}

	if(is_locked && client->Admin() < AccountStatus::GMAdmin) {
		SendLootReqErrorPacket(client, 0);
		client->Message(CC_Red, "Error: Corpse locked by GM.");
		return;
	}

	if(being_looted_by == 0) { 
		being_looted_by = 0xFFFFFFFF; 
	}

	if(this->being_looted_by != 0xFFFFFFFF) {
		// lets double check....
		Entity* looter = entity_list.GetID(this->being_looted_by);
		if(looter == 0) { 
			ResetLooter();
		}
	}

	uint8 Loot_Request_Type = 1;
	bool loot_coin = false;
	std::string tmp;
	if(database.GetVariable("LootCoin", tmp))
		loot_coin = tmp[0] == 1 && tmp[1] == '\0';

	if (this->being_looted_by != 0xFFFFFFFF && this->being_looted_by != client->GetID()) {
		SendLootReqErrorPacket(client, 0);
		Loot_Request_Type = 0;
	}
	else if (IsPlayerCorpse() && char_id == client->CharacterID()) {
		Loot_Request_Type = 2;
	}
	else if ((IsNPCCorpse() || become_npc) && CanPlayerLoot(client->CharacterID())) {
		Loot_Request_Type = 2;
	}
	else if (GetPlayerKillItem() == -1 && CanPlayerLoot(client->CharacterID())) { /* PVP loot all items, variable cash */
		Loot_Request_Type = 3;
	}
	else if (GetPlayerKillItem() == 1 && CanPlayerLoot(client->CharacterID())) { /* PVP loot 1 item, variable cash */
		Loot_Request_Type = 4;
	}
	else if (GetPlayerKillItem() > 1 && CanPlayerLoot(client->CharacterID())) { /* PVP loot 1 set item, variable cash */
		Loot_Request_Type = 5;
	}

	if (Loot_Request_Type == 1) {
		if (client->Admin() < AccountStatus::GMAdmin || !client->GetGM()) {
			SendLootReqErrorPacket(client, 2);
		}
	}

	if(Loot_Request_Type >= 2 || (Loot_Request_Type == 1 && client->Admin() >= AccountStatus::GMAdmin && client->GetGM())) {
		this->being_looted_by = client->GetID();
		auto outapp = new EQApplicationPacket(OP_MoneyOnCorpse, sizeof(moneyOnCorpseStruct));
		moneyOnCorpseStruct* d = (moneyOnCorpseStruct*)outapp->pBuffer;

		d->response		= 1;
		d->unknown1		= 0x42;
		d->unknown2		= 0xef;

		/* Don't take the coin off if it's a gm peeking at the corpse */
		if(Loot_Request_Type == 2 || (Loot_Request_Type >= 3 && loot_coin)) { 
			if(!IsPlayerCorpse() && client->IsGrouped() && client->AutoSplitEnabled() && client->GetGroup()) {
				d->copper		= 0;
				d->silver		= 0;
				d->gold			= 0;
				d->platinum		= 0;
				Group *cgroup = client->GetGroup();
				cgroup->SplitMoney(GetCopper(), GetSilver(), GetGold(), GetPlatinum(), client);
				/* QS: Player_Log_Looting */
				if (RuleB(QueryServ, PlayerLogLoot))
				{
					QServ->QSLootRecords(client->CharacterID(), corpse_name, "CASH-SPLIT", client->GetZoneID(), 0, "null", 0, GetPlatinum(), GetGold(), GetSilver(), GetCopper());
				}
			}
			else {
				d->copper = this->GetCopper();
				d->silver = this->GetSilver();
				d->gold = this->GetGold();
				d->platinum = this->GetPlatinum();
				client->AddMoneyToPP(GetCopper(), GetSilver(), GetGold(), GetPlatinum(), false);
				/* QS: Player_Log_Looting */
				if (RuleB(QueryServ, PlayerLogLoot))
				{
					QServ->QSLootRecords(client->CharacterID(), corpse_name, "CASH", client->GetZoneID(), 0, "null", 0, GetPlatinum(), GetGold(), GetSilver(), GetCopper());
				}
			}

			RemoveCash();
			Save();
		}

		outapp->priority = 6;
		client->QueuePacket(outapp);
		safe_delete(outapp);
		if(Loot_Request_Type == 5) {
			int pkitem = GetPlayerKillItem();
			const EQ::ItemData* item = database.GetItem(pkitem);
			EQ::ItemInstance* inst = database.CreateItem(item, item->MaxCharges);
			if (inst) {
				client->SendItemPacket(EQ::invslot::CORPSE_BEGIN, inst, ItemPacketLoot);
				safe_delete(inst);
			}
			else { client->Message(CC_Red, "Could not find item number %i to send!!", GetPlayerKillItem()); }

			client->QueuePacket(app);
			return;
		}

		int i = 0;
		const EQ::ItemData* item = nullptr;
		ItemList::iterator cur, end;
		cur = itemlist.begin();
		end = itemlist.end();

		int corpselootlimit = EQ::inventory::Lookup(EQ::versions::ConvertClientVersionToMobVersion(client->ClientVersion()))->InventoryTypeSize[EQ::invtype::typeCorpse];

		for (; cur != end; ++cur) {
			ServerLootItem_Struct* item_data = *cur;
			item_data->lootslot = 0xFFFF;

			// Don't display the item if it's in a bag
			// Added cursor queue slots to corpse item visibility list. Nothing else should be making it to corpse.
			if (!IsPlayerCorpse() || item_data->equip_slot <= EQ::invslot::GENERAL_END || Loot_Request_Type >= 3 ||
				(item_data->equip_slot >= EQ::invslot::TRADE_BEGIN && item_data->equip_slot <= EQ::invslot::TRADE_END) || 
				(item_data->equip_slot >= EQ::invslot::CURSOR_QUEUE_BEGIN && item_data->equip_slot <= EQ::invslot::CURSOR_QUEUE_END)) {
				if (i < corpselootlimit) 
				{
					if(!RuleB(NPC, IgnoreQuestLoot) || (RuleB(NPC, IgnoreQuestLoot) && item_data->quest == 0))
					{
						item = database.GetItem(item_data->item_id);
						if(client && item && (item_data->quest == 0 || (item_data->quest == 1 && item->NoDrop != 0))) 
						{
							EQ::ItemInstance* inst = database.CreateItem(item, item_data->charges);
							if(inst) {
								// SlotGeneral1 is the corpse inventory start offset for Ti(EMu) - CORPSE_END = SlotGeneral1 + SlotCursor
								client->SendItemPacket(i, inst, ItemPacketLoot);
								safe_delete(inst);
							}
						}

						item_data->lootslot = i;
					}
				}

				i++;
			}
		}

		if(IsPlayerCorpse() && (char_id == client->CharacterID() || client->GetGM())) {
			if(i > corpselootlimit) {
				client->Message(CC_Yellow, "*** This corpse contains more items than can be displayed! ***");
				client->Message(CC_Default, "Remove items and re-loot corpse to access remaining inventory.");
				client->Message(CC_Default, "(%s contains %i additional %s.)", GetName(), (i - corpselootlimit), (i - corpselootlimit) == 1 ? "item" : "items");
			}
			// this will happen if an item in a bag slot is on the corpse without its parent bag.
			if (IsPlayerCorpse() && i == 0 && itemlist.size() > 0) 
			{
				cur = itemlist.begin();
				end = itemlist.end();
				int8 slot = EQ::invslot::GENERAL_BEGIN;
				for (; cur != end; ++cur) 
				{
					ServerLootItem_Struct* item_data = *cur;
					item = database.GetItem(item_data->item_id);
					if(item)
					{
						Log(Logs::General, Logs::Corpse, "Corpse Looting: %s was not sent to client loot window. Resetting to slot %d...", item->Name, slot);
						client->Message(CC_Default, "Inaccessible Corpse Item: %s. Please close the window and try to loot again.", item->Name);
						item_data->equip_slot = slot;
						Save();
					}
					++slot;
					if(slot > EQ::invslot::GENERAL_END)
						break;
				}
			}
		}
	}

	// Disgrace: Client seems to require that we send the packet back...
	client->QueuePacket(app);
}

void Corpse::LootItem(Client* client, const EQApplicationPacket* app) {
	/* This gets sent no matter what as a sort of ACK */
	client->QueuePacket(app);

	if (!loot_cooldown_timer.Check()) {
		SendEndLootErrorPacket(client);
		//unlock corpse for others
		if (this->being_looted_by == client->GetID()) {
			ResetLooter();
		}
		return;
	}

	/* To prevent item loss for a player using 'Loot All' who doesn't have inventory space for all their items. */
	if (RuleB(Character, CheckCursorEmptyWhenLooting) && !client->GetInv().CursorEmpty()) {
		client->Message(CC_Red, "You may not loot an item while you have an item on your cursor.");
		SendEndLootErrorPacket(client);
		/* Unlock corpse for others */
		if (this->being_looted_by == client->GetID()) {
			ResetLooter();
		}
		return;
	}

	LootingItem_Struct* lootitem = (LootingItem_Struct*)app->pBuffer;

	if (this->being_looted_by != client->GetID()) {
		client->Message(CC_Red, "Error: Corpse::LootItem: BeingLootedBy != client");
		SendEndLootErrorPacket(client);
		return;
	}
	if (IsPlayerCorpse() && !CanPlayerLoot(client->CharacterID()) && !become_npc && (char_id != client->CharacterID() && client->Admin() < AccountStatus::GMLeadAdmin)) {
		client->Message(CC_Red, "Error: This is a player corpse and you don't own it.");
		SendEndLootErrorPacket(client);
		return;
	}
	if (is_locked && client->Admin() < AccountStatus::GMAdmin) {
		SendLootReqErrorPacket(client, 0);
		client->Message(CC_Red, "Error: Corpse locked by GM.");
		return;
	}
	if (IsPlayerCorpse() && (char_id != client->CharacterID()) && CanPlayerLoot(client->CharacterID()) && GetPlayerKillItem() == 0){
		client->Message(CC_Red, "Error: You cannot loot any more items from this corpse.");
		SendEndLootErrorPacket(client);
		ResetLooter();
		return;
	}
	const EQ::ItemData* item = nullptr;
	EQ::ItemInstance *inst = nullptr;
	ServerLootItem_Struct* item_data = nullptr, * bag_item_data[10] = {};

	memset(bag_item_data, 0, sizeof(bag_item_data));
	if (GetPlayerKillItem() > 1){
		item = database.GetItem(GetPlayerKillItem());
	}
	else if (GetPlayerKillItem() == -1 || GetPlayerKillItem() == 1){
		item_data = GetItem(lootitem->slot_id); //don't allow them to loot entire bags of items as pvp reward
	}
	else{
		item_data = GetItem(lootitem->slot_id, bag_item_data);
	}

	if (GetPlayerKillItem()<=1 && item_data != 0) {
		item = database.GetItem(item_data->item_id);
	}

	if (item != 0) {
		if (item_data){
			inst = database.CreateItem(item, item_data ? item_data->charges : 0);
		}
		else {
			inst = database.CreateItem(item);
		}
	}

	if (client && inst) {
		if (client->CheckLoreConflict(item)) {
			client->Message_StringID(0, LOOT_LORE_ERROR);
			SendEndLootErrorPacket(client);
			ResetLooter();
			delete inst;
			return;
		}
		// search through bags for lore items
		if (item && item->IsClassBag()) {
			for (int i = 0; i < 10; i++) {
				if(bag_item_data[i])
				{
					const EQ::ItemData* bag_item = 0;
					bag_item = database.GetItem(bag_item_data[i]->item_id);
					if (bag_item && client->CheckLoreConflict(bag_item))
					{
						client->Message(CC_Default, "You cannot loot this container. The %s inside is a Lore Item and you already have one.", bag_item->Name);
						SendEndLootErrorPacket(client);
						ResetLooter();
						delete inst;
						return;
					}
				}
			}
		}

		char buf[88];
		char corpse_name[64];
		strcpy(corpse_name, GetName());
		snprintf(buf, 87, "%d %d %s", inst->GetItem()->ID, inst->GetCharges(), EntityList::RemoveNumbers(corpse_name));
		buf[87] = '\0';
		std::vector<std::any> args;
		args.push_back(inst);
		args.push_back(this);
		parse->EventPlayer(EVENT_LOOT, client, buf, 0, &args);
		parse->EventItem(EVENT_LOOT, client, inst, this, buf, 0);

		if ((RuleB(Character, EnableDiscoveredItems))) {
			if (client && !client->GetGM() && !client->IsDiscovered(inst->GetItem()->ID))
				client->DiscoverItem(inst->GetItem()->ID);
		}


		/* First add it to the looter - this will do the bag contents too */
		if (lootitem->auto_loot) {
			if (!client->AutoPutLootInInventory(*inst, true, true, bag_item_data))
				client->PutLootInInventory(EQ::invslot::slotCursor, *inst, bag_item_data);
		}
		else {
			client->PutLootInInventory(EQ::invslot::slotCursor, *inst, bag_item_data);
		}

		/* Remove it from Corpse */
		if (item_data){
			/* Delete needs to be before RemoveItem because its deletes the pointer for item_data/bag_item_data */
			database.DeleteItemOffCharacterCorpse(this->corpse_db_id, item_data->equip_slot, item_data->item_id);
			/* Delete Item Instance */
			RemoveItem(item_data->lootslot);
		}

		/* Remove Bag Contents */
		if (item->IsClassBag() && (GetPlayerKillItem() != -1 || GetPlayerKillItem() != 1)) {
			for (int i = EQ::invbag::SLOT_BEGIN; i <= EQ::invbag::SLOT_END; i++) {
				if (bag_item_data[i]) {
					/* Delete needs to be before RemoveItem because its deletes the pointer for item_data/bag_item_data */
					database.DeleteItemOffCharacterCorpse(this->corpse_db_id, bag_item_data[i]->equip_slot, bag_item_data[i]->item_id);
					/* Delete Item Instance */
					RemoveItem(bag_item_data[i]);
				}
			}
		}

		if (GetPlayerKillItem() != -1){
			SetPlayerKillItemID(0);
		}

		//std::string item_link;
		EQ::SayLinkEngine linker;
		linker.SetLinkType(EQ::saylink::SayLinkItemInst);
		linker.SetItemInst(inst);

		linker.GenerateLink();

		if (!IsPlayerCorpse()) 
		{
			client->Message_StringID(MT_LootMessages, LOOTED_MESSAGE, linker.Link().c_str());

			Group *g = client->GetGroup();
			if (g != nullptr) 
			{
				g->GroupMessage_StringID(client, MT_LootMessages, OTHER_LOOTED_MESSAGE, client->GetName(), linker.Link().c_str());
			}
			else
			{
				Raid *r = client->GetRaid();
				if (r != nullptr) 
				{
					r->RaidMessage_StringID(client, MT_LootMessages, OTHER_LOOTED_MESSAGE, client->GetName(), linker.Link().c_str());
				}
			}
		}
	}
	else {
		SendEndLootErrorPacket(client);
		safe_delete(inst);
		return;
	}

	/* QS: Player_Log_Looting */
	if (RuleB(QueryServ, PlayerLogLoot))
	{
		QServ->QSLootRecords(client->CharacterID(), corpse_name, "ITEM", client->GetZoneID(), item->ID, item->Name, inst->GetCharges(), GetPlatinum(), GetGold(), GetSilver(), GetCopper());
	}

	safe_delete(inst);
}

void Corpse::EndLoot(Client* client, const EQApplicationPacket* app)
{
	if (!client)
	{
		Save();
		return;
	}

	auto outapp = new EQApplicationPacket;
	outapp->SetOpcode(OP_LootComplete);
	outapp->size = 0;
	client->QueuePacket(outapp);
	safe_delete(outapp);

	ResetLooter();
	if (this->IsEmpty())
	{
		Delete();
	}
	else
	{
		Save();
	}
}

void Corpse::FillSpawnStruct(NewSpawn_Struct* ns, Mob* ForWho) {
	Mob::FillSpawnStruct(ns, ForWho);

	ns->spawn.max_hp = 120;

	if (IsPlayerCorpse())
		ns->spawn.NPC = 3;
	else
		ns->spawn.NPC = 2;

	UpdateActiveLight();
	ns->spawn.light = m_Light.Type[EQ::lightsource::LightActive];
}

void Corpse::QueryLoot(Client* to) {
	if (itemlist.size() > 0) {
		int player_corpse_limit = EQ::inventory::Lookup(EQ::versions::ConvertClientVersionToMobVersion(to->ClientVersion()))->InventoryTypeSize[EQ::invtype::typeCorpse];
		to->Message(
			CC_Default,
			fmt::format(
				"Loot | Name: {} ID: {}",
				GetName(),
				GetNPCTypeID()
			).c_str()
		);

		int item_count = 0;
		for (auto current_item : itemlist) {
			int item_number = (item_count + 1);
			if (!current_item) {
				LogError("Corpse::QueryLoot() - ItemList error, null item.");
				continue;
			}

			if (!current_item->item_id || !database.GetItem(current_item->item_id)) {
				LogError("Corpse::QueryLoot() - Database error, invalid item.");
				continue;
			}

			EQ::SayLinkEngine linker;
			linker.SetLinkType(EQ::saylink::SayLinkLootItem);
			linker.SetLootData(current_item);

			to->Message(
				CC_Default,
				fmt::format(
					"Item {} | Name: {} ({}){}",
					item_number,
					linker.GenerateLink().c_str(),
					current_item->item_id,
					(
						current_item->charges > 1 ?
						fmt::format(
							" Amount: {}",
							current_item->charges
						) :
						""
						)
				).c_str()
			);
			item_count++;
		}
	}

	bool has_money = (
		platinum > 0 ||
		gold > 0 ||
		silver > 0 ||
		copper > 0
		);
	if (has_money) {
		to->Message(
			CC_Default,
			fmt::format(
				"Money | {}",
				Strings::Money(
					platinum,
					gold,
					silver,
					copper
				)
			).c_str()
		);
	}
}

bool Corpse::Summon(Client* client, bool spell, bool CheckDistance) {
	float dist2 = 2500.0f; // /corpse range was likely 50 units before 2004
	float z_dist = 1000.0f; // a Z check was very likely implemented shortly after Velious launch, at least for some zones
	switch (GetZoneID())
	{
	case thurgadinb:
	case velketor:
	case crystal:
	case necropolis:
	case vexthal:
	case sseru:
	case ssratemple:
	case potorment:
	case bothunder:
	case solrotower:
	case poair:
		z_dist = 150.0f;
	}

	// check GM lock
	if (IsLocked() && client->Admin() < AccountStatus::GMAdmin) {
		client->Message(CC_Red, "That corpse is locked by a GM.");
		return false;
	}

	// check consent if not own corpse
	if (this->GetCharID() != client->CharacterID())
	{
		bool consented = spell; // Conjure Corpse does not require consent.  Target restrictions are checked in spell code, not here.
		std::list<CharacterConsent>::iterator itr;
		for (itr = client->consent_list.begin(); itr != client->consent_list.end(); ++itr)
		{
			if (strcmp(this->GetOwnerName(), itr->consenter.c_str()) == 0 && itr->corpse_id == GetCorpseDBID())
			{
				consented = true;
				break;
			}
		}
		if (!consented) {
			client->Message_StringID(CC_Default, CONSENT_NONE);
			return false;
		}
	}

	// check range if not spell
	if (!spell && CheckDistance)
	{
		bool in_range = DistanceSquaredNoZ(m_Position, client->GetPosition()) <= dist2 && fabs(m_Position.z - client->GetPosition().z) < z_dist;
		if (!in_range)
		{
			client->Message_StringID(CC_Default, CORPSE_TOO_FAR);
			return false;
		}
	}

	// move corpse
	client->CorpseSummoned(this);
	GMMove(client->GetX(), client->GetY(), client->GetZ());
	is_corpse_changed = true;
	Save();

	return true;
}

void Corpse::CompleteResurrection(bool timer_expired)
{
	if(timer_expired)
	{
		rez_time = 0;
		rezzable = false; // Players can no longer rez this corpse.
		corpse_rez_timer.Disable();
	}
	else
	{
		rez_time = corpse_rez_timer.Enabled () ? corpse_rez_timer.GetRemainingTime() : 0;
	}

	IsRezzed(true); // Players can rez this corpse for no XP (corpse gate) provided rezzable is true.
	rez_experience = 0;
	is_corpse_changed = true;
	this->Save();
}

void Corpse::Spawn() {
	EQApplicationPacket app;
	this->CreateSpawnPacket(&app, this);
	entity_list.QueueClients(this, &app);
	safe_delete_array(app.pBuffer);
}

uint32 Corpse::GetEquipment(uint8 material_slot) const {
	int invslot;

	if(material_slot > EQ::textures::LastTexture) {
		return 0;
	}

	invslot = EQ::InventoryProfile::CalcSlotFromMaterial(material_slot);
	if (invslot == INVALID_INDEX) // GetWornItem() should be returning a NO_ITEM for any invalid index...
		return 0;

	return GetWornItem(invslot);
}

uint32 Corpse::GetEquipmentColor(uint8 material_slot) const {
	const EQ::ItemData *item = nullptr;

	if(material_slot > EQ::textures::LastTexture) {
		return 0;
	}

	item = database.GetItem(GetEquipment(material_slot));
	if(item != 0) {
		return item_tint.Slot[material_slot].UseTint ?
			item_tint.Slot[material_slot].Color :
			item->Color;
	}

	return 0;
}

void Corpse::UpdateEquipmentLight()
{
	m_Light.Type[EQ::lightsource::LightEquipment] = 0;
	m_Light.Level[EQ::lightsource::LightEquipment] = 0;

	for (auto iter = itemlist.begin(); iter != itemlist.end(); ++iter) {
		if (((*iter)->equip_slot < EQ::invslot::EQUIPMENT_BEGIN || (*iter)->equip_slot > EQ::invslot::EQUIPMENT_END)) { continue; }
		
		auto item = database.GetItem((*iter)->item_id);
		if (item == nullptr) { continue; }
		
		if (EQ::lightsource::IsLevelGreater(item->Light, m_Light.Type[EQ::lightsource::LightEquipment]))
			m_Light.Type[EQ::lightsource::LightEquipment] = item->Light;
	}
	
	uint8 general_light_type = 0;
	for (auto iter = itemlist.begin(); iter != itemlist.end(); ++iter) {
		if ((*iter)->equip_slot < EQ::invslot::GENERAL_BEGIN || (*iter)->equip_slot > EQ::invslot::GENERAL_END) { continue; }
		
		auto item = database.GetItem((*iter)->item_id);
		if (item == nullptr) { continue; }
		
		if (item->ItemClass != EQ::item::ItemClassCommon) { continue; }
		if (m_Light.Type[EQ::lightsource::LightEquipment] != 0 && (item->Light < 9 || item->Light > 13)) { continue; }

		if (EQ::lightsource::TypeToLevel(item->Light))
			general_light_type = item->Light;
	}

	if (EQ::lightsource::IsLevelGreater(general_light_type, m_Light.Type[EQ::lightsource::LightEquipment]))
		m_Light.Type[EQ::lightsource::LightEquipment] = general_light_type;

	m_Light.Level[EQ::lightsource::LightEquipment] = EQ::lightsource::TypeToLevel(m_Light.Type[EQ::lightsource::LightEquipment]);
}

void Corpse::AddLooter(Mob* who) {
	for (int i = 0; i < MAX_LOOTERS; i++) {
		if (allowed_looters[i] == 0) {
			allowed_looters[i] = who->CastToClient()->CharacterID();
			break;
		}
	}
}

void Corpse::LoadPlayerCorpseDecayTime(uint32 corpse_db_id, bool empty){
	if(!corpse_db_id)
		return;

	corpse_graveyard_moved_lastpos = glm::vec4();
	uint32 active_corpse_decay_timer = database.GetCharacterCorpseDecayTimer(corpse_db_id);
	int32 corpse_decay;
	if(empty)
	{
		corpse_decay = RuleI(Character, EmptyCorpseDecayTimeMS);
	}
	else
	{
		corpse_decay = RuleI(Character, CorpseDecayTimeMS);
	}

	if (active_corpse_decay_timer > 0 && corpse_decay > (active_corpse_decay_timer * 1000)) {
		corpse_decay_timer.Start(corpse_decay - (active_corpse_decay_timer * 1000));
	}
	else {
		corpse_decay_timer.Start(2000);
	}
	if (active_corpse_decay_timer > 0 && (zone->graveyard_timer() * 60000) > (active_corpse_decay_timer * 1000)) {
		corpse_graveyard_timer.Start((zone->graveyard_timer() * 60000) - (active_corpse_decay_timer * 1000));
	}
	else {
		corpse_graveyard_timer.Start(3000);
	}
}

void Corpse::SetRezTimer(bool initial_timer)
{

	if(!rezzable)
	{
		if(corpse_rez_timer.Enabled())
			corpse_rez_timer.Disable();

		return;
	}

	IsOwnerOnline();
	if(!is_owner_online && !initial_timer)
	{
		if(corpse_rez_timer.Enabled())
			corpse_rez_timer.Disable();

		return;
    }

	if(corpse_rez_timer.Enabled() && !initial_timer)
	{
		return;
	}

	if(initial_timer)
	{
		uint32 timer = RuleI(Character, CorpseResTimeMS);
		if(killedby == Killed_DUEL)
		{
			timer = RuleI(Character, DuelCorpseResTimeMS);
		}
		else if (IsEmpty() && RuleB(Character, SacrificeCorpseDepop) && killedby == Killed_Sac &&
			(GetZoneID() == poknowledge || GetZoneID() == nexus || GetZoneID() == bazaar))
		{
			if (corpse_rez_timer.Enabled())
				corpse_rez_timer.Disable();

			return;
		}
		rez_time = timer;
	}

	if(rez_time < 1)
	{
		// Corpse is no longer rezzable
		CompleteResurrection(true);
		return;
	}

	corpse_rez_timer.Start(rez_time);

}

void Corpse::IsOwnerOnline()
{
	Client* client = entity_list.GetClientByCharID(GetCharID());

	if(!client)
	{
		uint32 accountid = database.GetAccountIDByChar(GetCharID());
		client = entity_list.GetClientByAccID(accountid);

		if(!client)
		{
			// Client is not in the corpse's zone, send a packet to world to have it check.
			auto pack = new ServerPacket(ServerOP_IsOwnerOnline, sizeof(ServerIsOwnerOnline_Struct));
			ServerIsOwnerOnline_Struct* online = (ServerIsOwnerOnline_Struct*)pack->pBuffer;
			strncpy(online->name, GetOwnerName(), 64);
			online->corpseid = this->GetID();
			online->zoneid = zone->GetZoneID();
			online->online = 0;
			online->accountid = accountid;
			worldserver.SendPacket(pack);
			safe_delete(pack);
		}
		else
		{
			SetOwnerOnline(true);
		}
	}
	else
	{
		SetOwnerOnline(true);
	}
}

void Corpse::RevokeConsent()
{
	if (IsPlayerCorpse())
	{
		Log(Logs::General, Logs::Corpse, "%s is denying consent to all players with corpse_id %u", GetOwnerName(), GetCorpseDBID());

		auto pack = new ServerPacket(ServerOP_ConsentDenyByID, sizeof(ServerOP_ConsentDenyByID_Struct));
		ServerOP_ConsentDenyByID_Struct* scs = (ServerOP_ConsentDenyByID_Struct*)pack->pBuffer;
		strncpy(scs->ownername, GetOwnerName(), 64);
		scs->corpse_id = GetCorpseDBID();
		worldserver.SendPacket(pack);
		safe_delete(pack);
	}
}