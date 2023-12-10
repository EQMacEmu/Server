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
#include "../common/strings.h"

#include "client.h"
#include "doors.h"
#include "raids.h"
#include "entity.h"
#include "guild_mgr.h"
#include "mob.h"
#include "string_ids.h"
#include "worldserver.h"
#include "zonedb.h"

#include <iostream>
#include <string.h>

#define DOOR_STOP_TRAP 0x00
#define DOOR_RESET_TRAP 0x01
#define OPEN_DOOR 0x02
#define CLOSE_DOOR 0x03
#define OPEN_INVDOOR 0x03
#define CLOSE_INVDOOR 0x02

extern EntityList entity_list;
extern WorldServer worldserver;

Doors::Doors(const Door* door) :
    close_timer(5000),
	lift_timer(20000),
    m_Position(door->pos_x, door->pos_y, door->pos_z, door->heading),
    m_Destination(door->dest_x, door->dest_y, door->dest_z, door->dest_heading)
{
	strn0cpy(zone_name, door->zone_name, 32);
	strn0cpy(door_name, door->door_name, 32);
	strn0cpy(dest_zone, door->dest_zone, 16);
	this->db_id			= door->db_id;
	this->door_id		= door->door_id;
	this->incline		= door->incline;
	this->opentype		= door->opentype;
	this->lockpick		= door->lock_pick;
	this->keyitem		= door->keyitem;
	this->nokeyring		= door->nokeyring;
	this->altkeyitem	= door->altkeyitem;
	this->trigger_door	= door->trigger_door;
	this->trigger_type	= door->trigger_type;
	this->triggered		= false;
	this->door_param	= door->door_param;
	this->size			= door->size;
	this->invert_state	= door->invert_state;
	this->islift		= door->islift;
	this->close_time	= door->close_time;
	this->can_open		= door->can_open;
	this->guildzonedoor = door->guildzonedoor;

	SetOpenState(false);

	close_timer.Disable();
	lift_timer.Disable();

	

	client_version_mask = door->client_version_mask;
	if (strncmp(dest_zone, "NONE", strlen("NONE")) != 0)
	{
		teleport = true;
	}
	else
	{
		teleport = false;
	}

}

Doors::Doors(const char *dmodel, const glm::vec4& position, uint8 dopentype, uint16 dsize) :
    close_timer(5000),
	lift_timer(20000),
    m_Position(position),
    m_Destination(glm::vec4())
{
	strn0cpy(zone_name, zone->GetShortName(), 32);
	strn0cpy(door_name, dmodel, 32);
	strn0cpy(dest_zone, "NONE", 32);

	this->db_id		= database.GetDoorsCountPlusOne(zone->GetShortName());
	this->door_id	= database.GetDoorsDBCountPlusOne(zone->GetShortName());

	this->incline		= 0;
	this->opentype		= dopentype;
	this->lockpick		= 0;
	this->keyitem		= 0;
	this->nokeyring		= 0;
	this->altkeyitem	= 0;
	this->trigger_door	= 0;
	this->trigger_type	= 0;
	this->triggered		= false;
	this->door_param	= 0;
	this->size			= dsize;
	this->invert_state	= 0;
	this->islift		= 0;
	this->close_time	= 0;
	this->can_open		= 0;
	this->guildzonedoor = 0;
	this->client_version_mask = 4294967295u;

	SetOpenState(false);
	teleport = false;
	close_timer.Disable();
	lift_timer.Disable();
}


Doors::~Doors()
{
}

bool Doors::Process()
{
	if(close_timer.Enabled() && close_timer.Check() && IsDoorOpen())
	{
		if (opentype == 40 || GetTriggerType() == 1 || close_time != 5)
		{
			auto outapp = new EQApplicationPacket(OP_MoveDoor, sizeof(MoveDoor_Struct));
			MoveDoor_Struct* md = (MoveDoor_Struct*)outapp->pBuffer;
			md->doorid = door_id;
			md->action = invert_state == 0 ? CLOSE_DOOR : CLOSE_INVDOOR;
			entity_list.QueueClients(0, outapp);

			if (GetTriggerDoorID() != 0)
			{
				Doors* triggerdoor = entity_list.FindDoor(GetTriggerDoorID());
				if (triggerdoor && triggerdoor->IsDoorOpen())
				{
					md->doorid = triggerdoor->door_id;
					md->action = triggerdoor->invert_state == 0 ? CLOSE_DOOR : CLOSE_INVDOOR;
					entity_list.QueueClients(0, outapp);

					triggerdoor->triggered = false;
					triggerdoor->close_timer.Disable();
					triggerdoor->SetOpenState(false);
				}
			}

			safe_delete(outapp);
		}

		triggered=false;
		close_timer.Disable();
		SetOpenState(false);
	}
	else if(lift_timer.Check() && IsDoorOpen())
	{
		auto outapp = new EQApplicationPacket(OP_MoveDoor, sizeof(MoveDoor_Struct));
		MoveDoor_Struct* md = (MoveDoor_Struct*)outapp->pBuffer;
		md->doorid = door_id;
		md->action = invert_state == 1 ? CLOSE_INVDOOR : CLOSE_DOOR;
		entity_list.QueueClients(0, outapp);
		safe_delete(outapp);

		triggered = false;
		lift_timer.Disable();
		SetOpenState(false);
		if (door_param > 1)
			Log(Logs::General, Logs::Doors, "Lift %d is being closed by process.", db_id);
	
	}
	return true;
}

void Doors::HandleClick(Client* sender, uint8 trigger, bool floor_port)
{
	//door debugging info dump
	Log(Logs::General, Logs::Doors, "%s clicked door %s (dbid %d, eqid %d) at %s", sender->GetName(), this->door_name, this->db_id, this->door_id, to_string(m_Position).c_str());
	Log(Logs::Detail, Logs::Doors, "  incline %d, opentype %d, lockpick %d, keys %d %d, nokeyring %d, trigger %d type %d, param %d", this->incline, this->opentype, this->lockpick, this->keyitem, this->altkeyitem, this->nokeyring, this->trigger_door, this->trigger_type, this->door_param);
	Log(Logs::Detail, Logs::Doors, "  size %d, invert %d, dest: %s %s open %d lift: %d closetime: %d", this->size, this->invert_state, this->dest_zone, to_string(m_Destination).c_str(), this->isopen, this->islift, this->close_time);

	if (!IsMoveable())
	{
		Log(Logs::General, Logs::Doors, "%s clicked door %d that doesn't open.", sender->GetName(), door_id);
		return;
	}

	if (islift)
	{
		HandleLift(sender);
		return;
	}

	auto outapp = new EQApplicationPacket(OP_MoveDoor, sizeof(MoveDoor_Struct));
	MoveDoor_Struct* md = (MoveDoor_Struct*)outapp->pBuffer;
	md->doorid = door_id;

	// Traps. 120 is ceiling spears. 125 is wall spears. 130 is swinging axe. 140 is falling block trap. 
	if (opentype == 120 || opentype == 125 || opentype == 130 || opentype == 140)
	{
		Log(Logs::General, Logs::Doors, "Clicking a door that is a trap!");
		if(sender->HasSkill(EQ::skills::SkillDisarmTraps))
		{
			uint8 success = SKILLUP_FAILURE;
			int uskill = sender->GetSkill(EQ::skills::SkillDisarmTraps);
			if ((zone->random.Int(0, 49) + uskill) >= (zone->random.Int(0, 49) + 1))
			{
				Log(Logs::General, Logs::Traps, "Door Trap %d is disarmed.", door_id);
				success = SKILLUP_SUCCESS;
				sender->Message_StringID(MT_Skills, DISARMED_TRAP);
				md->action = DOOR_RESET_TRAP;
			}
			else
			{
				sender->Message_StringID(MT_Skills, FAILED_DISARM_TRAP);
				sender->CommonBreakInvisible();
			}

			sender->CheckIncreaseSkill(EQ::skills::SkillDisarmTraps, nullptr, zone->skill_difficulty[EQ::skills::SkillDisarmTraps].difficulty, success);
		}
		else
		{
			sender->CommonBreakInvisible();
		}

		entity_list.QueueClients(sender, outapp, false);
		safe_delete(outapp);
		return;
	}

	if(GetTriggerType() == 255)
	{ // this object isnt triggered
		if(trigger == 1)
		{ // this door is only triggered by an object
			if(!IsDoorOpen() || (opentype == 58))
			{
				md->action = invert_state == 0 ? OPEN_DOOR : OPEN_INVDOOR;
			}
			else
			{
				md->action = invert_state == 0 ? CLOSE_DOOR : CLOSE_INVDOOR;
			}
		}
		else
		{
			safe_delete(outapp);
			return;
		}
	}

	uint32 key = 0;
	if (!DoorKeyCheck(sender, key))
	{
		safe_delete(outapp);
		return;
	}
	else
	{
		if (!IsDoorOpen() || (opentype == 58))
		{
			md->action = invert_state == 0 ? OPEN_DOOR : OPEN_INVDOOR;
		}
		else
		{
			md->action = invert_state == 0 ? CLOSE_DOOR : CLOSE_INVDOOR;
		}
	}

	if ((!floor_port && opentype != 58) || strncmp(dest_zone, "NONE", strlen("NONE")) == 0)
	{
		entity_list.QueueClients(sender, outapp, false);
		if (!IsDoorOpen())
		{
			if (close_time > 0)
				close_timer.Start(close_time * 1000);
			SetOpenState(true);
		}
		else
		{
			if (close_time > 0)
				close_timer.Disable();
			SetOpenState(false);
		}
	}

	if(GetTriggerDoorID() != 0)
	{
		Doors* triggerdoor = entity_list.FindDoor(GetTriggerDoorID());
		if (triggerdoor)
		{
			if (!triggerdoor->triggered)
			{
				triggered = true;
				uint8 trigger = GetTriggerType() == 1 ? 1 : 0;
				triggerdoor->HandleClick(sender, trigger);
			}
			else
			{
				triggered = false;
				triggerdoor->triggered = false;
			}
		}
	}

	//everything past this point assumes we opened the door
	//and met all the reqs for opening
	//everything to do with closed doors has already been taken care of
	//we return because we don't want people using teleports on an unlocked door (exploit!)
	if((md->action == CLOSE_DOOR && invert_state == 0) || (md->action == CLOSE_INVDOOR && invert_state == 1))
	{
		safe_delete(outapp);
		return;
	}

	safe_delete(outapp);

	// Teleport door!
	if((floor_port || opentype == 58) && IsTeleport() && sender && !sender->HasDied())
	{

		uint32 keyneeded = GetKeyItem();
		uint32 playerkey = key;
		uint8 keepoffkeyring = GetNoKeyring();
		uint32 zoneid = database.GetZoneID(dest_zone);
		float temp_x = m_Destination.x;
		float temp_y = m_Destination.y;
		uint32 zoneguildid = GUILD_NONE;

		if (zoneid != zone->GetZoneID() && !sender->CanBeInZone(zoneid))
		{
			return;
		}

		if (guildzonedoor)
		{

			if (!sender)
				return;

			Raid* player_raid = sender->GetRaid();

			if (!player_raid)
			{
				sender->Message(CC_Red, "You are unable to enter a guild instance because you are not a part of a raid containing at least a guild officer as its leader with %i guild members present, and %i players at or above level %i present total.",
					RuleI(Quarm, AutomatedRaidRotationRaidGuildMemberCountRequirement),
					RuleI(Quarm, AutomatedRaidRotationRaidNonMemberCountRequirement),
					RuleI(Quarm, AutomatedRaidRotationRaidGuildLevelRequirement));
				return;
			}

			if (!player_raid->CanRaidEngageRaidTarget(player_raid->GetLeaderGuildID()))
			{
				sender->Message(CC_Red, "You are unable to enter a guild instance because you are not a part of a raid containing at least a guild officer as its leader with %i guild members present, and %i players at or above level %i present total.", 
					RuleI(Quarm, AutomatedRaidRotationRaidGuildMemberCountRequirement),
					RuleI(Quarm, AutomatedRaidRotationRaidNonMemberCountRequirement),
					RuleI(Quarm, AutomatedRaidRotationRaidGuildLevelRequirement));
				return;
			}
			zoneguildid = player_raid->GetLeaderGuildID();
		}

		if ((floor_port || strncmp(dest_zone,zone_name,strlen(zone_name)) == 0) && !keyneeded)
		{
			if(!keepoffkeyring)
			{
				sender->KeyRingAdd(playerkey);
			}
			sender->MovePCGuildID(zone->GetZoneID(), zoneguildid, m_Destination.x, m_Destination.y, m_Destination.z, m_Destination.w);
		}
		else if ((!IsDoorOpen() || opentype == 58 || floor_port) && (keyneeded && ((keyneeded == playerkey) || sender->GetGM())))
		{
			if(!keepoffkeyring)
			{
				sender->KeyRingAdd(playerkey);
			}
			if(zoneid == zone->GetZoneID())
			{
				sender->MovePCGuildID(zone->GetZoneID(), zoneguildid, m_Destination.x, m_Destination.y, m_Destination.z, m_Destination.w);
			}
			else
			{
				zone->ApplyRandomLoc(zoneid, temp_x, temp_y);
				sender->MovePCGuildID(zoneid, zoneguildid, temp_x, temp_y, m_Destination.z, m_Destination.w);
			}
		}

		if ((!IsDoorOpen() || opentype == 58) && !keyneeded)
		{
			if(zoneid == zone->GetZoneID())
			{
				sender->MovePCGuildID(zone->GetZoneID(), zoneguildid, m_Destination.x, m_Destination.y, m_Destination.z, m_Destination.w);
			}
			else
			{
				zone->ApplyRandomLoc(zoneid, temp_x, temp_y);
				sender->MovePCGuildID(zoneid, zoneguildid, temp_x, temp_y, m_Destination.z, m_Destination.w);
			}
		}
	}
}

void Doors::HandleLift(Client* sender)
{
	if (door_param > 1)
		Log(Logs::General, Logs::Doors, "%s activated lift %s (dbid %d, eqid %d) at %s which moves %d coords.", sender->GetName(), door_name, db_id, door_id, to_string(m_Position).c_str(), door_param);

	auto outapp = new EQApplicationPacket(OP_MoveDoor, sizeof(MoveDoor_Struct));
	MoveDoor_Struct* md = (MoveDoor_Struct*)outapp->pBuffer;
	md->doorid = door_id;

	uint32 key = 0;
	if (!DoorKeyCheck(sender, key))
	{
		safe_delete(outapp);
		return;
	}

	if (!IsDoorOpen())
	{
		md->action = invert_state == 1 ? OPEN_INVDOOR : OPEN_DOOR;
		if (door_param > 1)
			Log(Logs::General, Logs::Doors, "Lift %d is opening (%d) %s", db_id, md->action, door_param > 100 ? "." : "starting lift timer.");
		if (close_time > 0)
			lift_timer.Start(close_time * 1000);
		SetOpenState(true);
	}
	else
	{
		md->action = invert_state == 1 ? CLOSE_INVDOOR : CLOSE_DOOR;
		if (door_param > 1)
			Log(Logs::General, Logs::Doors, "Lift %d is closing (%d) %s", db_id, md->action, door_param > 100 ? "." : "disabling lift timer.");
		if (close_time > 0)
			lift_timer.Disable();
		SetOpenState(false);
	}

	entity_list.QueueClients(sender, outapp);
	safe_delete(outapp);

	Doors* triggerdoor = entity_list.FindDoor(GetTriggerDoorID());
	if (triggerdoor)
	{
		if (!triggerdoor->triggered)
		{
			triggered = true;
			triggerdoor->HandleLift(sender);
		}
		else
		{
			triggered = false;
			triggerdoor->triggered = false;
		}
	}
}

bool Doors::DoorKeyCheck(Client* sender, uint32& key)
{
	uint32 keyneeded = GetKeyItem();
	uint32 altkey = GetAltKeyItem();
	uint8 keepoffkeyring = GetNoKeyring();
	uint32 haskey = 0;
	uint32 hasaltkey = 0;
	uint32 playerkey = 0;
	const EQ::ItemInstance *lockpicks = sender->GetInv().GetItem(EQ::invslot::slotCursor);

	if (lockpicks != nullptr)
	{
		if (lockpicks->GetItem()->ItemType != EQ::item::ItemTypeLockPick)
		{
			lockpicks = nullptr;
		}
	}

	haskey = sender->GetInv().HasItem(keyneeded, 1, invWhereCursor);
	hasaltkey = sender->GetInv().HasItem(altkey, 1, invWhereCursor);

	if (haskey == EQ::invslot::slotCursor)
	{
		playerkey = keyneeded;
	}
	else if (hasaltkey == EQ::invslot::slotCursor)
	{
		playerkey = altkey;
	}

	key = playerkey;

	if ((keyneeded == 0 && altkey == 0 && GetLockpick() == 0) ||
		(IsDoorOpen() && opentype == 58))
	{	
		//door not locked
		return true;
	}
	else
	{
		// a key is required or the door is locked but can be picked or both
		if (sender->GetGM())
		{
			// GM can always open locks
			sender->Message_StringID(CC_Blue, DOORS_GM);
			return true;
		}
		else if (playerkey)
		{	// they have something they are trying to open it with
			if (keyneeded && (keyneeded == playerkey) || altkey && (altkey == playerkey))
			{
				// key required and client is using the right key
				if (!keepoffkeyring)
				{
					sender->KeyRingAdd(playerkey);
				}

				return true;
			}
		}
		else if (lockpicks != nullptr)
		{
			if (sender->GetSkill(EQ::skills::SkillPickLock))
			{
				if (lockpicks->GetItem()->ItemType == EQ::item::ItemTypeLockPick)
				{
					float modskill = sender->GetSkill(EQ::skills::SkillPickLock);

					// Lockpicks will be on the cursor and not equipped, so we need to manually get any skillmod they may have.
					if (lockpicks->GetItem()->SkillModType == EQ::skills::SkillPickLock)
					{
						modskill += modskill * (static_cast<float>(lockpicks->GetItem()->SkillModValue) / 100.0);
						if (modskill > HARD_SKILL_CAP)
						{
							modskill = HARD_SKILL_CAP;
						}
					}

					Log(Logs::General, Logs::Skills, "Client has lockpicks: skill=%f", modskill);

					if (GetLockpick() <= modskill)
					{
						if (!IsDoorOpen())
						{
							sender->CheckIncreaseSkill(EQ::skills::SkillPickLock, nullptr, zone->skill_difficulty[EQ::skills::SkillPickLock].difficulty);
						}
						sender->Message_StringID(CC_Blue, DOORS_SUCCESSFUL_PICK);
						return true;
					}
					else
					{
						sender->Message_StringID(CC_Blue, DOORS_INSUFFICIENT_SKILL);
						return false;
					}
				}
				else
				{
					sender->Message_StringID(CC_Blue, DOORS_NO_PICK);
					return false;
				}
			}
			else
			{
				sender->Message_StringID(CC_Blue, DOORS_CANT_PICK);
				return false;
			}
		}
		else
		{	// locked door and nothing to open it with
			// search for key on keyring
			uint32 keyring = 0;
			if (sender->KeyRingCheck(keyneeded))
				keyring = keyneeded;
			else if (sender->KeyRingCheck(altkey))
				keyring = altkey;

			if (keyring > 0)
			{
				key = keyring;
				return true;
			}
			else
			{
				sender->Message_StringID(CC_Blue, DOORS_LOCKED);
				return false;
			}
		}
	}

	return true;
}

void Doors::OpenDoor(Mob* sender, bool force)
{
	auto outapp = new EQApplicationPacket(OP_MoveDoor, sizeof(MoveDoor_Struct));
	MoveDoor_Struct* md = (MoveDoor_Struct*)outapp->pBuffer;
	md->doorid = door_id;
	md->action = invert_state == 0 ? OPEN_DOOR : OPEN_INVDOOR;
	entity_list.QueueClients(sender, outapp, false);

	if (GetTriggerDoorID() != 0)
	{
		Doors* triggerdoor = entity_list.FindDoor(GetTriggerDoorID());
		if (triggerdoor && (!triggerdoor->IsDoorOpen() || force))
		{
			md->doorid = triggerdoor->door_id;
			md->action = triggerdoor->invert_state == 0 ? OPEN_DOOR : OPEN_INVDOOR;
			entity_list.QueueClients(sender, outapp, false);

			triggerdoor->close_timer.Start(close_time * 1000);
			triggerdoor->SetOpenState(true);
		}
	}

	safe_delete(outapp);
}

void Doors::NPCOpen(NPC* sender, bool alt_mode)
{
	if(sender) {
		if(GetTriggerType() == 255 || (GetTriggerDoorID() > 0 && IsDoorOpen()) || GetLockpick() != 0 || GetKeyItem() != 0 || opentype == 59 || opentype == 58 || !sender->IsNPC()) { // this object isnt triggered or door is locked - NPCs should not open locked doors!
			return;
		}

		OpenDoor(sender);

		if(!alt_mode) { // original function
			if(!isopen) {
				close_timer.Start(close_time * 1000);
				isopen=true;
			}
			else {
				close_timer.Disable();
				isopen=false;
			}
		}
		else { // alternative function
			close_timer.Start();
			isopen=true;
		}
	}
}

void Doors::ForceOpen(Mob *sender, bool alt_mode)
{
	OpenDoor(sender, true);

	if(!alt_mode) { // original function
		if(!isopen) {
			close_timer.Start(close_time * 1000);
			isopen=true;
		}
		else {
			close_timer.Disable();
			isopen=false;
		}
	}
	else { // alternative function
		close_timer.Start();
		isopen=true;
	}
}

void Doors::ForceClose(Mob *sender, bool alt_mode)
{
	auto outapp = new EQApplicationPacket(OP_MoveDoor, sizeof(MoveDoor_Struct));
	MoveDoor_Struct* md=(MoveDoor_Struct*)outapp->pBuffer;
	md->doorid = door_id;
	md->action = invert_state == 0 ? CLOSE_DOOR : CLOSE_INVDOOR; // change from original (open to close)
	entity_list.QueueClients(sender,outapp,false);
	safe_delete(outapp);

	if(!alt_mode) { // original function
		if(!isopen) {
			close_timer.Start(close_time * 1000);
			isopen=true;
		}
		else {
			close_timer.Disable();
			isopen=false;
		}
	}
	else { // alternative function
		if(isopen)
			close_timer.Trigger();
	}
}

void Doors::ToggleState(Mob *sender)
{
	if(GetTriggerDoorID() > 0 || GetLockpick() != 0 || GetKeyItem() != 0 || opentype == 58 || opentype == 40) { // borrowed some NPCOpen criteria
		return;
	}

	auto outapp = new EQApplicationPacket(OP_MoveDoor, sizeof(MoveDoor_Struct));
	MoveDoor_Struct* md=(MoveDoor_Struct*)outapp->pBuffer;
	md->doorid = door_id;

	if(!isopen) {
		md->action = invert_state == 0 ? OPEN_DOOR : OPEN_INVDOOR;
		isopen=true;
	}
	else
	{
		md->action = invert_state == 0 ? CLOSE_DOOR : CLOSE_INVDOOR;
		isopen=false;
	}

	entity_list.QueueClients(sender,outapp,false);
	safe_delete(outapp);
}

void Doors::DumpDoor(){
	Log(Logs::General, Logs::Doors,
		"db_id:%i door_id:%i zone_name:%s door_name:%s %s",
		db_id, door_id, zone_name, door_name, to_string(m_Position).c_str());
	Log(Logs::General, Logs::Doors,
		"opentype:%i lockpick:%i keyitem:%i altkeyitem:%i nokeyring:%i trigger_door:%i trigger_type:%i door_param:%i open:%s lift:%i",
		opentype, lockpick, keyitem, altkeyitem, nokeyring, trigger_door, trigger_type, door_param, (isopen) ? "open":"closed", islift);
	Log(Logs::General, Logs::Doors,
		"dest_zone:%s destination:%s ",
		dest_zone, to_string(m_Destination).c_str());
}

int32 ZoneDatabase::GetDoorsCount(uint32* oMaxID, const char *zone_name) {

	std::string query = StringFormat("SELECT MAX(id), count(*) FROM doors "
                                    "WHERE zone = '%s'",
                                    zone_name);
    auto results = QueryDatabase(query);
    if (!results.Success()) {
		return -1;
    }

    if (results.RowCount() != 1)
        return -1;

    auto row = results.begin();

    if (!oMaxID)
        return atoi(row[1]);

    if (row[0])
        *oMaxID = atoi(row[0]);
    else
        *oMaxID = 0;

    return atoi(row[1]);

}

int32 ZoneDatabase::GetDoorsCountPlusOne(const char *zone_name) {

    std::string query = StringFormat("SELECT MAX(id) FROM doors "
                                    "WHERE zone = '%s'", zone_name);
    auto results = QueryDatabase(query);
    if (!results.Success()) {
		return -1;
    }

	if (results.RowCount() != 1)
        return -1;

    auto row = results.begin();

    if (!row[0])
        return 0;

    return atoi(row[0]) + 1;
}

int32 ZoneDatabase::GetDoorsDBCountPlusOne(const char *zone_name) {

	uint32 oMaxID = 0;

    std::string query = StringFormat("SELECT MAX(doorid) FROM doors "
                                    "WHERE zone = '%s'",
                                    zone_name);
	auto results = QueryDatabase(query);
	if (!results.Success()) {
		return -1;
	}

    if (results.RowCount() != 1)
        return -1;

    auto row = results.begin();

    if (!row[0])
        return 0;

    return atoi(row[0]) + 1;
}

bool ZoneDatabase::LoadDoors(int32 iDoorCount, Door *into, const char *zone_name) {
	LogInfo("Loading Doors from database...");

    std::string query = StringFormat("SELECT id, doorid, zone, name, pos_x, pos_y, pos_z, heading, "
                                    "opentype, lockpick, keyitem, nokeyring, triggerdoor, triggertype, "
                                    "dest_zone, dest_x, dest_y, dest_z, dest_heading, "
                                    "door_param, invert_state, incline, size, client_version_mask, altkeyitem, islift, "
									"close_time, can_open, guildzonedoor "
                                    "FROM doors WHERE zone = '%s' AND ((%.2f >= min_expansion AND %.2f < max_expansion) OR (min_expansion = 0 AND max_expansion = 0)) "
                                    "ORDER BY doorid asc", zone_name, RuleR(World, CurrentExpansion), RuleR(World, CurrentExpansion));
	auto results = QueryDatabase(query);
	if (!results.Success()){
		return false;
	}

    int32 rowIndex = 0;
    for(auto row = results.begin(); row != results.end(); ++row, ++rowIndex) {
        if(rowIndex >= iDoorCount) {
            std::cerr << "Error, Door Count of " << iDoorCount << " exceeded." << std::endl;
            break;
        }

        memset(&into[rowIndex], 0, sizeof(Door));

		into[rowIndex].db_id = atoi(row[0]);
		into[rowIndex].door_id = atoi(row[1]);

        strn0cpy(into[rowIndex].zone_name,row[2],32);
		strn0cpy(into[rowIndex].door_name,row[3],32);

		into[rowIndex].pos_x = (float)atof(row[4]);
		into[rowIndex].pos_y = (float)atof(row[5]);
		into[rowIndex].pos_z = (float)atof(row[6]);
		into[rowIndex].heading = (float)atof(row[7]);
		into[rowIndex].opentype = atoi(row[8]);
		into[rowIndex].lock_pick = atoi(row[9]);
		into[rowIndex].keyitem = atoi(row[10]);
		into[rowIndex].nokeyring = atoi(row[11]);
		into[rowIndex].trigger_door = atoi(row[12]);
		into[rowIndex].trigger_type = atoi(row[13]);

		strn0cpy(into[rowIndex].dest_zone, row[14], 32);

		into[rowIndex].dest_x = (float) atof(row[15]);
		into[rowIndex].dest_y = (float) atof(row[16]);
		into[rowIndex].dest_z = (float) atof(row[17]);
		into[rowIndex].dest_heading = (float) atof(row[18]);
		into[rowIndex].door_param=atoi(row[19]);
		into[rowIndex].invert_state=atoi(row[20]);
		into[rowIndex].incline=atoi(row[21]);
		into[rowIndex].size=atoi(row[22]);
		into[rowIndex].client_version_mask = (uint32)strtoul(row[23], nullptr, 10);
		into[rowIndex].altkeyitem = atoi(row[24]);
		into[rowIndex].islift = atobool(row[25]);
		into[rowIndex].close_time = atoi(row[26]);
		into[rowIndex].can_open = atobool(row[27]);
		into[rowIndex].guildzonedoor = atobool(row[28]);
    }

	return true;
}


void Doors::SetLocation(float x, float y, float z)
{
	entity_list.DespawnAllDoors();
    m_Position = glm::vec4(x, y, z, m_Position.w);
	entity_list.RespawnAllDoors();
}

void Doors::SetPosition(const glm::vec4& position) {
	entity_list.DespawnAllDoors();
	m_Position = position;
	entity_list.RespawnAllDoors();
}

void Doors::SetIncline(int in) {
	entity_list.DespawnAllDoors();
	incline = in;
	entity_list.RespawnAllDoors();
}

void Doors::SetOpenType(uint8 in) {
	entity_list.DespawnAllDoors();
	opentype = in;
	entity_list.RespawnAllDoors();
}

void Doors::SetDoorName(const char* name) {
	entity_list.DespawnAllDoors();
	memset(door_name, 0, sizeof(door_name));
	strncpy(door_name, name, sizeof(door_name));
	entity_list.RespawnAllDoors();
}

void Doors::SetSize(uint16 in) {
	entity_list.DespawnAllDoors();
	size = in;
	entity_list.RespawnAllDoors();
}

// Old world traps are doors.
Doors *EntityList::FindNearbyDoorTrap(Mob* searcher, float max_dist, float &door_curdist)
{
	float dist = 999999;
	Doors* current_trap = nullptr;

	float max_dist2 = max_dist*max_dist;

	for (auto it = door_list.begin(); it != door_list.end(); ++it)
	{
		Doors *cur = it->second;
		if (cur->GetOpenType() != 120 && cur->GetOpenType() != 125 && cur->GetOpenType() != 130 && cur->GetOpenType() != 140)
			continue;

		auto diff = searcher->GetPosition() - cur->GetPosition();
		float curdist = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

		if (curdist < max_dist2 && curdist < dist)
		{
			Log(Logs::General, Logs::Traps, "Door %d has opentype %d and is curdist %0.1f", cur->GetDoorID(), cur->GetOpenType(), curdist);
			dist = curdist;
			current_trap = cur;
		}
	}

	if (current_trap != nullptr)
	{
		Log(Logs::General, Logs::Traps, "Door %d is the closest trap.", current_trap->GetDoorID());
		door_curdist = dist;
	}
	else
		door_curdist = INVALID_INDEX;

	return current_trap;
}
