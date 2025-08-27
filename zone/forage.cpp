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

#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include "../common/fastmath.h"
#include "../common/misc_functions.h"
#include "../common/rulesys.h"
#include "../common/strings.h"
#include "../common/zone_store.h"

#include "entity.h"
#include "forage.h"
#include "client.h"
#include "quest_parser_collection.h"
#include "string_ids.h"
#include "titles.h"
#include "water_map.h"
#include "zonedb.h"
#include "../common/events/player_event_logs.h"
#include "worldserver.h"
#include "queryserv.h"
#include "../common/repositories/criteria/content_filter_criteria.h"

extern WorldServer worldserver;
extern QueryServ   *QServ;

#include <iostream>

#ifdef _WINDOWS
#define snprintf	_snprintf
#endif

struct NPCType;

//max number of items which can be in the foraging table
//for a given zone.
#define FORAGE_ITEM_LIMIT 50

uint32 ZoneDatabase::GetZoneForage(uint32 ZoneID, uint8 skill) {

	uint32 item[FORAGE_ITEM_LIMIT];
	uint32 chance[FORAGE_ITEM_LIMIT];
	uint32 ret;

	for (int c=0; c < FORAGE_ITEM_LIMIT; c++) {
		item[c] = 0;
	}

	uint32 chancepool = 0;
    std::string query = fmt::format(
		SQL(
			SELECT
			  itemid,
			  chance 
			FROM
			  forage
			WHERE
			  zoneid = '{}' 
			AND 
			  level <= '{}'
	          {}
			LIMIT
			  {}
		), 
		ZoneID, 
	    skill, 
		ContentFilterCriteria::apply(),
		FORAGE_ITEM_LIMIT
	);
    auto results = QueryDatabase(query);
	if (!results.Success()) {
		return 0;
	}

	uint8 index = 0;
    for (auto row = results.begin(); row != results.end(); ++row, ++index) {
        if (index >= FORAGE_ITEM_LIMIT)
            break;

        item[index] = atoi(row[0]);
        chance[index] = atoi(row[1]) + chancepool;
		LogInfo("Possible Forage: [{}] with a [{}] chance total to [{}] chancepool", item[index], chance[index] - chancepool, chance[index]);
        chancepool = chance[index];
    }

	if(chancepool == 0 || index < 1)
		return 0;

	if(index == 1) {
		return item[0];
	}

	ret = 0;

	uint32 rindex = zone->random.Int(1, chancepool);

	for(int i = 0; i < index; i++) {
		if(rindex <= chance[i]) {
			ret = item[i];
			break;
		}
	}

	return ret;
}

uint32 ZoneDatabase::GetZoneFishing(uint32 ZoneID, uint8 skill)
{
	uint32 item[50];
	uint32 chance[50];
	uint32 chancepool = 0;
	uint32 ret = 0;

	for (int c=0; c<50; c++) {
		item[c]=0;
		chance[c]=0;
	}

    std::string query = fmt::format(
		SQL(
			SELECT 
			  itemid,
			  chance
			FROM 
			  fishing 
			WHERE 
			  zoneid = '{}'
			AND 
			  skill_level <= '{}'
			  {}
			),
			ZoneID, 
			skill,
			ContentFilterCriteria::apply()
		);
    auto results = QueryDatabase(query);
    if (!results.Success()) {
		return 0;
    }

    uint8 index = 0;

	// these items seem to be available in every zone.  note that the junk items here can also be earned on a critical failure outcome.
	// the chances might not be a real thing on live eq, maybe every item has the same chance, this part is not verified
	item[index] = 13019; // Fresh Fish
	chance[index] = 100 + chancepool;
	chancepool = chance[index];
	index++;

	item[index] = 13076; // Fish Scales
	chance[index] = 100 + chancepool;
	chancepool = chance[index];
	index++;

	item[index] = 7007; // Rusty Dagger
	chance[index] = 100 + chancepool;
	chancepool = chance[index];
	index++;

	item[index] = 1038; // Tattered Cloth Sandal
	chance[index] = 100 + chancepool;
	chancepool = chance[index];
	index++;

    for (auto row = results.begin(); row != results.end(); ++row) {
        if (index >= 50)
            break;

		int item_id = atoi(row[0]);

		if (item_id == 13019 || item_id == 13076 || item_id == 7007 || item_id == 1038) // don't double up on the junk if it's in the database
			continue;

        item[index] = item_id;
        //chance[index] = atoi(row[1])+chancepool; // i think this variable chance might be made up so taking it out for now and weighting things equally
		chance[index] = 100 + chancepool;
		chancepool = chance[index];
		index++;
    }

	if (index <= 0)
        return 0;

    uint32 random = zone->random.Int(1, chancepool);
    for (int i = 0; i < index; i++)
    {
        if (random > chance[i])
            continue;

        ret = item[i];
        break;
    }

	return ret;
}
//we need this function to immediately determine, after we receive OP_Fishing, if we can even try to fish, otherwise we have to wait a while to get the failure
bool Client::CanFish() {

	if (fishing_timer.Enabled()) {
		Message_StringID(Chat::White, StringID::ALREADY_FISHING);	//You are already fishing!
		return false;
	}

	if (m_inv.GetItem(EQ::invslot::slotCursor)) {
		Message_StringID(Chat::Skills, StringID::FISHING_HANDS_FULL);
		return false;
	}

	//make sure we still have a fishing pole on:
	const EQ::ItemInstance *Pole = m_inv[EQ::invslot::slotPrimary];
	int32 bslot = m_inv.HasItemByUse(EQ::item::ItemTypeFishingBait, 1, invWhereWorn | invWherePersonal);
	const EQ::ItemInstance *Bait = nullptr;
	if (bslot != INVALID_INDEX) {
		Bait = m_inv.GetItem(bslot);
	}

	if (!Pole || !Pole->IsClassCommon() || Pole->GetItem()->ItemType != EQ::item::ItemTypeFishingPole) {
		if (m_inv.HasItemByUse(EQ::item::ItemTypeFishingPole, 1, invWhereWorn | invWherePersonal | invWhereBank | invWhereTrading | invWhereCursor))	//We have a fishing pole somewhere, just not equipped
			Message_StringID(Chat::Skills, StringID::FISHING_EQUIP_POLE);	//You need to put your fishing pole in your primary hand.
		else	//We don't have a fishing pole anywhere
			Message_StringID(Chat::Skills, StringID::FISHING_NO_POLE);	//You can't fish without a fishing pole, go buy one.
		return false;
	}

	if (!Bait || !Bait->IsClassCommon() || Bait->GetItem()->ItemType != EQ::item::ItemTypeFishingBait) {
		Message_StringID(Chat::Skills, StringID::FISHING_NO_BAIT);	//You can't fish without fishing bait, go buy some.
		return false;
	}

	if (zone->zonemap != nullptr && zone->watermap != nullptr && RuleB(Watermap, CheckForWaterWhenFishing)) {
		glm::vec3 rod_position;
		// Tweak Rod and LineLength if required
		const float rod_length = RuleR(Watermap, FishingRodLength);
		const float line_length = RuleR(Watermap, FishingLineLength);
		float client_heading = GetHeading() * 2;
		int heading_degrees;

		heading_degrees = (int)((client_heading * 360) / 512);
		heading_degrees = heading_degrees % 360;

		LogMaps("Heading is at {}, GetHeading() is {:.f}", heading_degrees, client_heading);

		rod_position.x = m_Position.x + rod_length * sin(heading_degrees * M_PI / 180.0f);
		rod_position.y = m_Position.y + rod_length * cos(heading_degrees * M_PI / 180.0f);
		rod_position.z = m_Position.z;

		float bestz = zone->zonemap->FindBestZ(rod_position, nullptr);
		float len = m_Position.z - bestz;
		if (len > line_length || len < 0.0f) {
			Message_StringID(Chat::Skills, StringID::FISHING_LAND);
			return false;
		}

		glm::vec3 dest;
		dest.x = rod_position.x;
		dest.y = rod_position.y;
		dest.z = rod_position.z;

		if (!CheckLosFN(dest.x, dest.y, dest.z, 0.0f)) {
			LogMaps("Failing to fish because of CheckLosFN");
			// fishing into a wall to reach water on other side?
			Message_StringID(Chat::Skills, StringID::FISHING_LAND);	//Trying to catch land sharks perhaps?
			return false;
		}

		float step_size = RuleR(Watermap, FishingLineStepSize);

		for (float i = 0.0f; i < line_length; i += step_size) {
			glm::vec3 dest(rod_position.x, rod_position.y, m_Position.z - i);

			bool in_lava = zone->watermap->InLava(dest);
			bool in_water = zone->watermap->InWater(dest) || zone->watermap->InVWater(dest);

			LogMaps("Fishing step size is : {}", i);

			if (GetZoneID() == Zones::POWATER) {
				if (zone->IsWaterZone(rod_position.z)) {
					in_water = true;
				}
				else {
					in_water = false;
				}
			}

			if (in_lava) {
				Message_StringID(Chat::Skills, StringID::FISHING_LAVA);	//Trying to catch a fire elemental or something?
				return false;
			}

			if (in_water) {
				LogMaps("fishing is in water : region [{}]", in_water);
				return true;
			}
		}

		Message_StringID(Chat::Skills, StringID::FISHING_LAND);
		return false;
	}
	return true;
}

bool Client::TryFishing()
{
	if (!p_timers.Expired(&database, pTimerFishing, false)) {
		LogError("Ability recovery time not yet met.");
		return false;
	}

	if (CanFish())
	{
		fishingStartPosition = GetPosition();
		p_timers.Start(pTimerFishing, FishingReuseTime - 1);
		fishing_timer.Start(11000);
		parse->EventPlayer(EVENT_FISH_START, this, "", 0);

		return true;
	}

	return false;
}

void Client::GoFish(bool guarantee, bool use_bait)
{
	if (fishingStartPosition != glm::vec3(GetPosition())) {
		fishing_timer.Disable();
		fishingStartPosition = glm::vec3(0.0f);
		Message_StringID(Chat::Skills, StringID::FISHING_STOP);
	}

	if (fishing_timer.Check()) {
		fishing_timer.Disable();
		fishingStartPosition = glm::vec3(0.0f);

		//we're doing this a second time (1st in Client::Handle_OP_Fishing) to make sure that, between when we started fishing & now, we're still able to fish (in case we move, change equip, etc)
		if (!CanFish()) {	//if we can't fish here, we don't need to bother with the rest
			return;
		}

		int fishing_skill = GetSkill(EQ::skills::SkillFishing);	//will take into account skill bonuses on pole & bait

		//make sure we still have a fishing pole on:
		int32 bslot = m_inv.HasItemByUse(EQ::item::ItemTypeFishingBait, 1, invWhereWorn | invWherePersonal);
		const EQ::ItemInstance *Bait = nullptr;
		if (bslot != INVALID_INDEX) {
			Bait = m_inv.GetItem(bslot);
		}

		//if the bait isnt equipped, need to add its skill bonus
		if (bslot >= EQ::invslot::GENERAL_BEGIN && Bait != nullptr && Bait->GetItem()->SkillModType == EQ::skills::SkillFishing) {
			fishing_skill += fishing_skill * (Bait->GetItem()->SkillModValue / 100);

			if (fishing_skill > HARD_SKILL_CAP){
				fishing_skill = HARD_SKILL_CAP;
			}
		}

		// determine the outcome of the fishing event - this logic was taken from a beta client.  
		// it may not be era accurate but this fishing code is no longer present in the eqmac client so can't verify.
		int skillFactor = fishing_skill + zone->random.Int(20, 50);
		int roll260 = zone->random.Int(1, 260);
		int resultingEvent; // the random outcome of the fishing event
		if (roll260 > skillFactor) {
			if (roll260 <= 235) {
				if (roll260 > 225 || roll260 - skillFactor > 200) {
					resultingEvent = 3; // "You lost your bait!"
				}
				else {
					resultingEvent = 2; // "You didn't catch anything."
				}
			}
			else {
				resultingEvent = 5; // critical failure, pole can break
			}
		}
		else {
			resultingEvent = 4; // success
		}

		if (guarantee) {
			resultingEvent = 4;
		}

		switch (resultingEvent) {
			case 2: { // "You didn't catch anything."
				Message_StringID(Chat::Skills, StringID::FISHING_FAILED);
				CheckIncreaseSkill(EQ::skills::SkillFishing, nullptr, zone->skill_difficulty[EQ::skills::SkillFishing].difficulty[GetClass()], SKILLUP_FAILURE);
				parse->EventPlayer(EVENT_FISH_FAILURE, this, "", 0);
				break;
			}

			case 3: { // "You lost your bait!"
				Message_StringID(Chat::Skills, StringID::FISHING_LOST_BAIT);
				if (use_bait) {
					DeleteItemInInventory(bslot, 1, true);
				}
				CheckIncreaseSkill(EQ::skills::SkillFishing, nullptr, zone->skill_difficulty[EQ::skills::SkillFishing].difficulty[GetClass()], SKILLUP_SUCCESS);
				parse->EventPlayer(EVENT_FISH_FAILURE, this, "", 0);
				break;
			}

			case 4: { // successful catch - uses up bait
				CheckIncreaseSkill(EQ::skills::SkillFishing, nullptr, zone->skill_difficulty[EQ::skills::SkillFishing].difficulty[GetClass()], SKILLUP_SUCCESS);
				if (use_bait) {
					DeleteItemInInventory(bslot, 1, true);
				}

				// get an item id from the database
				uint32 caught_item_id = database.GetZoneFishing(m_pp.zone_id, 999); // the minimum skill thing looks like it's for custom behavior so taking it out for now
				if (caught_item_id == 0) {
					caught_item_id = 13019; // Fresh Fish
				}

				const EQ::ItemData *caught_item = database.GetItem(caught_item_id);

				EQ::ItemInstance *inst = database.CreateItem(caught_item, 1);
				if (inst != nullptr) {
					if (CheckLoreConflict(inst->GetItem())) {
						Message_StringID(Chat::White, StringID::DUP_LORE);
						safe_delete(inst);
					}
					else {
						Message_StringID(Chat::Skills, StringID::FISHING_SUCCESS, caught_item->Name);
						PushItemOnCursorWithoutQueue(inst);
						safe_delete(inst);
						inst = m_inv.GetItem(EQ::invslot::slotCursor);
					}

					if (inst) {
						if (PlayerEventLogs::Instance()->IsEventEnabled(PlayerEvent::FISH_SUCCESS)) {
							auto e = PlayerEvent::FishSuccessEvent{
								.item_id = inst->GetItem()->ID,
								.item_name = inst->GetItem()->Name,
							};

							RecordPlayerEventLog(PlayerEvent::FISH_SUCCESS, e);
						}

						if (parse->PlayerHasQuestSub(EVENT_FISH_SUCCESS)) {
							std::vector<std::any> args = { inst };
							parse->EventPlayer(EVENT_FISH_SUCCESS, this, "", inst->GetID(), &args);
						}
					}
					else {
						RecordPlayerEventLog(PlayerEvent::FISH_FAILURE, PlayerEvent::EmptyEvent{});
						if (parse->PlayerHasQuestSub(EVENT_FISH_FAILURE)) {
							parse->EventPlayer(EVENT_FISH_FAILURE, this, "", 0);
						}
					}
				}
				break;
			}

			case 5: { // critical failure - can break fishing pole
				int roll100 = zone->random.Int(1, 100);
				if (roll100 <= 98) {
					if (roll100 > 80) {
						Message_StringID(Chat::Skills, StringID::FISHING_SUCCESS_SOMETHING);
						SummonItem(13076, 1); // Fish Scales 
					} 
					else if (roll100 > 70) {
						Message_StringID(Chat::Skills, StringID::FISHING_SUCCESS_SOMETHING);
						SummonItem(7007, 1); // Rusty Dagger
					}
					else if (roll100 > 60) {
						Message_StringID(Chat::Skills, StringID::FISHING_SUCCESS_SOMETHING);
						SummonItem(1038, 1); // Tattered Cloth Sandal
					}
					else {
						Message_StringID(Chat::Skills, StringID::FISHING_SPILL_BEER);
					}
				}
				else {
					Message_StringID(Chat::Skills, StringID::FISHING_POLE_BROKE);
					DeleteItemInInventory(EQ::invslot::slotPrimary, 0, true);
				}
				CheckIncreaseSkill(EQ::skills::SkillFishing, nullptr, zone->skill_difficulty[EQ::skills::SkillFishing].difficulty[GetClass()], SKILLUP_SUCCESS);
				break;
			}
		}
	}
}

void Client::ForageItem(bool guarantee) {

	int skill_level = GetSkill(EQ::skills::SkillForage);
	uint8 success = SKILLUP_FAILURE;

	// these may need to be fine tuned, I am just guessing here
	if (guarantee || zone->random.Int(0,199) < skill_level) 
	{
		uint32 foragedfood = 0;
		uint32 stringid = StringID::FORAGE_NOEAT;

		if (RuleB(Character, ForageNeedFoodorDrink))
		{
			// If we're hungry or thirsty, we want to prefer food or water.
			if(Hungry() && Thirsty())
			{
				if(m_pp.hunger_level <= m_pp.thirst_level)
					foragedfood = 13047;
				else
					foragedfood = 13044;
			}
			// We only need food.
			else if(Hungry())
			{
				foragedfood = 13047;
			}
			// We only need water.
			else if(Thirsty())
			{
				foragedfood = 13044;
			}
		}

		if (foragedfood == 0)
			foragedfood = database.GetZoneForage(m_pp.zone_id, skill_level);

		if (foragedfood > 0)
		{
			int16 inv_slot_id = GetInv().HasItem(foragedfood, 1, invWhereCursor);
			if (inv_slot_id != INVALID_INDEX && !GetGM()) {
				// we already have this item on the cursor - so stop sending it here.
				Message_StringID(Chat::Skills, StringID::FORAGE_FAILED);
				return;
			}
		}

		const EQ::ItemData* food_item = database.GetItem(foragedfood);

		if(!food_item)
		{
			LogError("nullptr returned from database.GetItem ([{}]) in ClientForageItem", foragedfood);
			return;
		}

		if(foragedfood == 13106)
			stringid = StringID::FORAGE_GRUBS;
		else
		{
			switch(food_item->ItemType) 
			{
				case EQ::item::ItemTypeFood:
					stringid = StringID::FORAGE_FOOD;
					break;

				case EQ::item::ItemTypeDrink:
					if(strstr(food_item->Name, "Water"))
						stringid = StringID::FORAGE_WATER;
					else
						stringid = StringID::FORAGE_DRINK;
					break;

				default:
					break;
			}
		}

		EQ::ItemInstance* inst = database.CreateItem(food_item, 1);
		if(inst != nullptr) 
		{
			// check to make sure it isn't a foraged lore item
			if(CheckLoreConflict(inst->GetItem()))
			{
				Message_StringID(Chat::White, StringID::DUP_LORE);
				safe_delete(inst);
			}
			else
			{
				Message_StringID(Chat::Skills, stringid);
				SummonItem(inst->GetID(), inst->GetCharges());
				safe_delete(inst);
				inst = m_inv.GetItem(EQ::invslot::slotCursor);
			}

			if(inst) {
				if (PlayerEventLogs::Instance()->IsEventEnabled(PlayerEvent::FORAGE_SUCCESS)) {
					auto e = PlayerEvent::ForageSuccessEvent{
						.item_id = inst->GetItem()->ID,
						.item_name = inst->GetItem()->Name
					};
					RecordPlayerEventLog(PlayerEvent::FORAGE_SUCCESS, e);
				}

				if (parse->PlayerHasQuestSub(EVENT_FORAGE_SUCCESS)) {
					std::vector<std::any> args = { inst };
					parse->EventPlayer(EVENT_FORAGE_SUCCESS, this, "", inst->GetID(), &args);
				}
			}

			success = SKILLUP_SUCCESS;
		}
	} 
	else
	{
		Message_StringID(Chat::Skills, StringID::FORAGE_FAILED);
		RecordPlayerEventLog(PlayerEvent::FORAGE_FAILURE, PlayerEvent::EmptyEvent{});

		if (parse->PlayerHasQuestSub(EVENT_FORAGE_FAILURE)) {
			parse->EventPlayer(EVENT_FORAGE_FAILURE, this, "", 0);
		}
	}

	CheckIncreaseSkill(EQ::skills::SkillForage, nullptr, zone->skill_difficulty[EQ::skills::SkillForage].difficulty[GetClass()], success);

}

