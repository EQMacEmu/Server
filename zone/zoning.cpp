/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2005 EQEMu Development Team (http://eqemulator.net)

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
#include "../common/packet_dump_file.h"
#include "../common/rulesys.h"
#include "../common/strings.h"

#include "queryserv.h"
#include "quest_parser_collection.h"
#include "string_ids.h"
#include "worldserver.h"
#include "zone.h"

#include "../common/repositories/zone_repository.h"
#include "../common/content/world_content_service.h"
#include "../common/events/player_event_logs.h"

extern QueryServ* QServ;
extern WorldServer worldserver;
extern Zone* zone;


void Client::Handle_OP_ZoneChange(const EQApplicationPacket *app) {
	zoning = true;
	if (app->size != sizeof(ZoneChange_Struct)) {
		LogError("Wrong size: OP_ZoneChange, size=[{}], expected [{}]", app->size, sizeof(ZoneChange_Struct));
		DumpPacket(app);
		return;
	}

	LogInfo("Zone request from [{}]", GetName());
	
	ZoneChange_Struct* zc=(ZoneChange_Struct*)app->pBuffer;

	uint16 target_zone_id = 0;
	ZonePoint* zone_point = nullptr;
	//figure out where they are going.
	//we should never trust the client's logic, however, the client's information coupled with the server's information can help us determine locations they should be going to.
	//try to figure it out for them.

	if(zc->zoneID == 0) {
		//client dosent know where they are going...
		//try to figure it out for them.

		switch(zone_mode) {
			case EvacToSafeCoords:
			case ZoneToSafeCoords:
				//going to safe coords, but client dosent know where?
				//assume it is this zone for now.
				target_zone_id = zone->GetZoneID();
				break;
			case GMSummon:
			case ZoneSolicited: //we told the client to zone somewhere, so we know where they are going.
				target_zone_id = zonesummon_id;
				break;
			case GateToBindPoint:
			case ZoneToBindPoint:
				target_zone_id = m_pp.binds[0].zoneId;
				break;
			case ZoneUnsolicited: //client came up with this on its own.
				zone_point = zone->GetClosestZonePointWithoutZone(GetX(), GetY(), GetZ(), this, ZONEPOINT_NOZONE_RANGE);

				if(zone_point) {
					//we found a zone point, which is a reasonable distance away
					//assume that is the one were going with.
					target_zone_id = zone_point->target_zone_id;
				} 
				else {
					//unable to find a zone point... is there anything else
					//that can be a valid un-zolicited zone request?

					Message(Chat::Red, "Invalid unsolicited zone request.");
					LogError("Zoning [{}]: Invalid unsolicited zone request to zone id [{}]", GetName(), target_zone_id);
					if (GetBindZoneID() == target_zone_id) {
						cheat_manager.CheatDetected(MQGate, glm::vec3(GetX(), GetY(), GetZ()));
					}
					else {
						cheat_manager.CheatDetected(MQZone, glm::vec3(GetX(), GetY(), GetZ()));
					}
					SendZoneCancel(zc);
					return;
				}
				break;
			default:
				break;
		};
	}
	else {
		if (zone_mode == EvacToSafeCoords && zonesummon_id > 0) {
			target_zone_id = zonesummon_id;
		}
		else {
			target_zone_id = zc->zoneID;
		}

		//if we are zoning to a specific zone unsolicied,
		//then until otherwise determined, they must be zoning
		//on a zone line.
		if(zone_mode == ZoneUnsolicited)
		{
			if (target_zone_id == zone->GetZoneID())
			{
				SendZoneCancel(zc);
				return;
			}

			zone_point = zone->GetClosestZonePoint(glm::vec3(GetPosition()), target_zone_id, this, ZONEPOINT_ZONE_RANGE);
			//if we didnt get a zone point, or its to a different zone,
			//then we assume this is invalid.
			if(!zone_point || zone_point->target_zone_id != target_zone_id) {
				LogError("Zoning [{}]: Invalid unsolicited zone request to zone id [{}]", GetName(), target_zone_id);
				if (GetBindZoneID() == target_zone_id) {
					cheat_manager.CheatDetected(MQGate, glm::vec3(GetX(), GetY(), GetZ()));
				}
				else {
					cheat_manager.CheatDetected(MQZone, glm::vec3(GetX(), GetY(), GetZ()));
				}
				SendZoneCancel(zc);
				return;
			}
		}
	}

	/* Check for Valid Zone */
	const char *target_zone_name = ZoneName(target_zone_id);
	if(target_zone_name == nullptr) {
		//invalid zone...
		Message(Chat::Red, "Invalid target zone ID.");
		LogError("Zoning [{}]: Unable to get zone name for zone id '[{}]'.", GetName(), target_zone_id);
		SendZoneCancel(zc);
		return;
	}

	/* Load up the Safe Coordinates, restrictions and verify the zone name*/
	auto zone_data = GetZone(ZoneID(target_zone_name));
	if (!zone_data) {
		Message(Chat::Red, "Invalid target zone while getting safe points.");
		LogError("Zoning [{}]: Unable to get safe coordinates for zone '[{}]'.", GetName(), target_zone_name);
		SendZoneCancel(zc);
		return;
	}

	float safe_x, safe_y, safe_z, safe_heading;
	int16 min_status = AccountStatus::Player;
	uint8 min_level = 0;

	LogInfo("[Handle_OP_ZoneChange] Loaded zone flag [{}]", zone_data->flag_needed);

	safe_x = zone_data->safe_x;
	safe_y = zone_data->safe_y;
	safe_z = zone_data->safe_z;
	min_status = zone_data->min_status;
	min_level = zone_data->min_level;

	if (target_zone_id == Zones::AIRPLANE)
		BuffFadeAll(true);

	std::string export_string = fmt::format(
		"{} {}",
		zone->GetZoneID(),
		target_zone_id
	);
	if (parse->EventPlayer(EVENT_ZONE, this, export_string, 0) != 0) {
		SendZoneCancel(zc);
		return;
	}

	if (PlayerEventLogs::Instance()->IsEventEnabled(PlayerEvent::ZONING)) {
		auto e = PlayerEvent::ZoningEvent{};
		e.from_zone_long_name = zone->GetLongName();
		e.from_zone_short_name = zone->GetShortName();
		e.from_zone_id = zone->GetZoneID();
		e.to_zone_long_name = ZoneLongName(target_zone_id);
		e.to_zone_short_name = ZoneName(target_zone_id);
		e.to_zone_id = target_zone_id;

		RecordPlayerEventLog(PlayerEvent::ZONING, e);
	}

	//handle circumvention of zone restrictions
	//we need the value when creating the outgoing packet as well.
	uint8 ignorerestrictions = zonesummon_ignorerestrictions;
	zonesummon_ignorerestrictions = 0;

	float dest_x=0, dest_y=0, dest_z=0, dest_h=0;
	switch(zone_mode) {
	case EvacToSafeCoords:
	case ZoneToSafeCoords:
		LogInfo(
			"Zoning [{}] to safe coords ([{}],[{}],[{}],[{}]) in [{}] ([{}])", 
			GetName(), 
			safe_x, 
			safe_y, 
			safe_z, 
			safe_heading, 
			target_zone_name, 
			target_zone_id
		);
		dest_x = safe_x;
		dest_y = safe_y;
		dest_z = safe_z;
		dest_h = safe_heading;
		break;
	case GMSummon:
		dest_x = m_ZoneSummonLocation.x;
		dest_y = m_ZoneSummonLocation.y;
		dest_z = m_ZoneSummonLocation.z;
		dest_h = m_ZoneSummonLocation.w;
		ignorerestrictions = 1;
		break;
	case GateToBindPoint:
		dest_x = m_pp.binds[0].x;
		dest_y = m_pp.binds[0].y;
		dest_z = m_pp.binds[0].z;
		dest_h = m_pp.binds[0].heading;
		break;
	case ZoneToBindPoint:
		dest_x = m_pp.binds[0].x;
		dest_y = m_pp.binds[0].y;
		dest_z = m_pp.binds[0].z;
		dest_h = m_pp.binds[0].heading;
		ignorerestrictions = 1;	//can always get to our bind point? seems exploitable
		break;
	case ZoneSolicited: //we told the client to zone somewhere, so we know where they are going.
		//recycle zonesummon variables
		dest_x = m_ZoneSummonLocation.x;
		dest_y = m_ZoneSummonLocation.y;
		dest_z = m_ZoneSummonLocation.z;
		dest_h = m_ZoneSummonLocation.w;
		break;
	case ZoneUnsolicited: //client came up with this on its own.
		//client requested a zoning... what are the cases when this could happen?

		//Handle zone point case:
		if(zone_point != nullptr) {
			//they are zoning using a valid zone point, figure out coords

			//999999 is a placeholder for 'same as where they were from'
			//The client compile shows 1044 difference when zoning freport <-> nro.  this fixes server side when using 999999.
			if (zone_point->target_x == 999999) {  
				if (zone->GetZoneID() == Zones::FREPORTE && zone_point->target_zone_id == Zones::NRO) {
					dest_x = GetX() + 1044;
				}
				else if (zone->GetZoneID() == Zones::NRO && zone_point->target_zone_id == Zones::FREPORTE) {
					dest_x = GetX() - 1044;
				}
				else {
					dest_x = GetX();
				}
			}
			else
				dest_x = zone_point->target_x;
			if (zone_point->target_y == 999999)
				dest_y = GetY();
			else
				dest_y = zone_point->target_y;
			if(zone_point->target_z == 999999)
				dest_z=GetZ()+5;
			else
				dest_z = zone_point->target_z;
			if(zone_point->target_heading == 999)
				dest_h = GetHeading()*2.0f;  // client heading uses 0 - 512
			else
				dest_h = zone_point->target_heading;

			break;
		}

		//for now, there are no other cases...

		//could not find a valid reason for them to be zoning, stop it.
		cheat_manager.CheatDetected(MQZoneUnknownDest, glm::vec3(0.0, 0.0, 0.0));
		LogError("Zoning [{}]: Invalid unsolicited zone request to zone id [{}]", GetName(), target_zone_id);
		if (GetBindZoneID() == target_zone_id) {
			cheat_manager.CheatDetected(MQGate, glm::vec3(GetX(), GetY(), GetZ()));
		}
		else {
			cheat_manager.CheatDetected(MQZone, glm::vec3(GetX(), GetY(), GetZ()));
		}
		SendZoneCancel(zc);
		return;
	default:
		break;
	};

	//OK, now we should know where were going...

	//Check some rules first.
	auto zoning_message = ZoningMessage::ZoneSuccess;

	//not sure when we would use ZONE_ERROR_NOTREADY
	
	//enforce min status and level
	if (!ignorerestrictions && (Admin() < min_status || GetLevel() < zone_data->min_level)) {
		zoning_message = ZoningMessage::ZoneNoExperience;
	}

	if(!ignorerestrictions && !zone_data->flag_needed.empty()) {
		//the flag needed string is not empty, meaning a flag is required.
		if(Admin() < minStatusToIgnoreZoneFlags && !HasZoneFlag(target_zone_id)) {
			LogInfo(
				"Client [{}] does not have the proper flag to enter [{}] ({})",
				GetCleanName(),
				ZoneName(target_zone_id),
				target_zone_id
			);
			Message(Chat::Red, "You do not have the flag to enter %s.", target_zone_name);
			zoning_message = ZoningMessage::ZoneNoExperience;
		}
	}

	if (Admin() < minStatusToIgnoreZoneFlags && IsMule() && (target_zone_id != Zones::BAZAAR && target_zone_id != Zones::NEXUS && target_zone_id != Zones::POKNOWLEDGE)) {
		zoning_message = ZoningMessage::ZoneNoExperience;
		LogCharacterDetail("[CLIENT] Character is a mule and cannot leave Bazaar/Nexus/PoK!");
	}

	// Expansion checks and routing
	if ((WorldContentService::Instance()->GetCurrentExpansion() >= Expansion::Classic && !GetGM())) {
		bool meets_zone_expansion_check = false;

		auto zones = ZoneRepository::GetWhere(
			database,
			fmt::format(
				"expansion <= {} AND short_name = '{}'",
				(WorldContentService::Instance()->GetCurrentExpansion()),
				target_zone_name
			)
		);
	
		meets_zone_expansion_check = !zones.empty(); 
		
		LogInfo(
			"Checking zone request [{}] for expansion [{}] ({}) success [{}]",
			target_zone_name,
			(WorldContentService::Instance()->GetCurrentExpansion()),
			WorldContentService::Instance()->GetCurrentExpansionName(),
			!zones.empty() ? "true" : "false"
		);

		if (!meets_zone_expansion_check) {
			zoning_message = ZoningMessage::ZoneNoExpansion;
		}
	}

	if (WorldContentService::Instance()->GetCurrentExpansion() >= Expansion::Classic && GetGM()) {
		LogInfo("[{}] Bypassing Expansion zone checks because GM status is set", GetCleanName());
	}

	if(zoning_message == ZoningMessage::ZoneSuccess) {
		//we have successfully zoned
		DoZoneSuccess(zc, target_zone_id, dest_x, dest_y, dest_z, dest_h, ignorerestrictions);
		UpdateZoneChangeCount(target_zone_id);
	} else {
		LogError("Zoning [{}]: Rules prevent this char from zoning into [{}]", GetName(), target_zone_name);
		SendZoneError(zc, zoning_message);
	}
}

void Client::SendZoneCancel(ZoneChange_Struct *zc) {
	//effectively zone them right back to where they were
	//unless we find a better way to stop the zoning process.
	cheat_manager.SetExemptStatus(Port, true);
	auto outapp = new EQApplicationPacket(OP_ZoneChange, sizeof(ZoneChange_Struct));
	ZoneChange_Struct *zc2 = (ZoneChange_Struct*)outapp->pBuffer;
	strcpy(zc2->char_name, zc->char_name);
	zc2->zoneID = zone->GetZoneID();
	zc2->success = 1;
	outapp->priority = 6;
	FastQueuePacket(&outapp);

	//reset to unsolicited.
	zone_mode = ZoneUnsolicited;
	// reset since we're not zoning anymore
	zoning = false;
	// remove save position lock
	m_lock_save_position = false;
}

void Client::SendZoneError(ZoneChange_Struct *zc, int8 err)
{
	LogError("Zone [{}] is not available because target wasn't found or character insufficent level", zc->zoneID);

	cheat_manager.SetExemptStatus(Port, true);

	auto outapp = new EQApplicationPacket(OP_ZoneChange, sizeof(ZoneChange_Struct));
	ZoneChange_Struct *zc2 = (ZoneChange_Struct*)outapp->pBuffer;
	strcpy(zc2->char_name, zc->char_name);
	zc2->zoneID = zc->zoneID;
	zc2->success = err;
	memset(zc2->error, 0xff, sizeof(zc2->error));
	outapp->priority = 6;
	FastQueuePacket(&outapp);

	//reset to unsolicited.
	zone_mode = ZoneUnsolicited;
	// reset since we're not zoning anymore
	zoning = false;
	// remove save position lock
	m_lock_save_position = false;
}

void Client::DoZoneSuccess(ZoneChange_Struct *zc, uint16 zone_id, float dest_x, float dest_y, float dest_z, float dest_h, int8 ignore_r) {
	//this is called once the client is fully allowed to zone here
	//it takes care of all the activities which occur when a client zones out

	SendLogoutPackets();

	// fade charmed pets
	Mob* mypet = GetPet();
	if (mypet && mypet->IsCharmedPet())
		FadePetCharmBuff();

	/* Dont clear aggro until the zone is successful */
	entity_list.RemoveFromHateLists(this);
	m_client_npc_aggro_scan_timer.Reset(); // prevent mobs from immediately reaggroing before player is actually gone

	EndShield();		// warrior /shield

	// depop pet
	DepopPet();

	LogInfo("Zoning [{}] to: [{}] ([{}]) x = [{}], y = [{}], z = [{}]", m_pp.name, ZoneName(zone_id), zone_id, dest_x, dest_y, dest_z);

	//set the player's coordinates in the new zone so they have them
	//when they zone into it
	m_Position.x = dest_x; //these coordinates will now be saved when ~client is called
	m_Position.y = dest_y;
	m_Position.z = dest_z;
	m_Position.w = dest_h / 2.0f; // fix for zone heading
	m_pp.heading = dest_h;
	m_pp.zone_id = zone_id;

	//Force a save so its waiting for them when they zone
	Save(2);

	m_lock_save_position = true;

	// vesuvias - zoneing to another zone so we need to the let the world server
	//handle things with the client for a while
	SetZoningState();
	auto pack = new ServerPacket(ServerOP_ZoneToZoneRequest, sizeof(ZoneToZone_Struct));
	ZoneToZone_Struct* ztz = (ZoneToZone_Struct*) pack->pBuffer;
	ztz->response = 0;
	ztz->current_zone_id = zone->GetZoneID();
	ztz->requested_zone_id = zone_id;
	ztz->admin = admin;
	ztz->ignorerestrictions = ignore_r;
	strcpy(ztz->name, GetName());
	ztz->guild_id = GuildID();
	worldserver.SendPacket(pack);
	safe_delete(pack);

	//reset to unsolicited.
	zone_mode = ZoneUnsolicited;
	m_ZoneSummonLocation = glm::vec4();
	zonesummon_id = 0;
	zonesummon_ignorerestrictions = 0;
}

void Client::MovePC(const char* zonename, float x, float y, float z, float heading, uint8 ignorerestrictions, ZoneMode zm) {
	ProcessMovePC(ZoneID(zonename), x, y, z, heading, ignorerestrictions, zm);
}

//designed for in zone moving
void Client::MovePC(float x, float y, float z, float heading, uint8 ignorerestrictions, ZoneMode zm) {
	ProcessMovePC(zone->GetZoneID(), x, y, z, heading, ignorerestrictions, zm);
}

void Client::MovePC(uint32 zoneID, float x, float y, float z, float heading, uint8 ignorerestrictions, ZoneMode zm) {
	if (IsAIControlled())
		StopNavigation();
	if (currently_fleeing)
		currently_fleeing = false;
	ProcessMovePC(zoneID, x, y, z, heading, ignorerestrictions, zm);
}

void Client::ProcessMovePC(uint32 zoneID, float x, float y, float z, float heading, uint8 ignorerestrictions, ZoneMode zm)
{
	// From what I have read, dragged corpses should stay with the player for Intra-zone summons etc, but we can implement that later.
	ClearDraggedCorpses();

	if(zoneID == 0)
		zoneID = zone->GetZoneID();

	if(zoneID == zone->GetZoneID())
	{
		if(GetPetID() != 0 && GetGM()) 
		{
			//if they have a pet, are a GM, and they are staying in zone, move with them
			Mob *p = GetPet();
			if(p != nullptr){
				p->SetPetOrder(SPO_Follow);
				p->GMMove(x+15, y, z);	//so it dosent have to run across the map.
			}
		}
	}

	switch(zm) {
		case GateToBindPoint:
			ZonePC(zoneID, x, y, z, heading, ignorerestrictions, zm);
			break;
		case EvacToSafeCoords:
		case ZoneToSafeCoords:
			ZonePC(zoneID, x, y, z, heading, ignorerestrictions, zm);
			break;
		case GMSummon:
			if (!GetGM())
				Message(Chat::Yellow, "You have been summoned by a GM!");
			ZonePC(zoneID, x, y, z, heading, ignorerestrictions, zm);
			break;
		case ZoneToBindPoint:
			ZonePC(zoneID, x, y, z, heading, ignorerestrictions, zm);
			break;
		case ZoneSolicited:
			ZonePC(zoneID, x, y, z, heading, ignorerestrictions, zm);
			break;
		case SummonPC:
			if(!GetGM())
				Message_StringID(Chat::Yellow, StringID::BEEN_SUMMONED);
			ZonePC(zoneID, x, y, z, heading, ignorerestrictions, zm);
			break;
		case Rewind:
			Message(Chat::Yellow, "Rewinding to previous location.");
			ZonePC(zoneID, x, y, z, heading, ignorerestrictions, zm);
			break;
		default:
			LogError("Client::ProcessMovePC received a reguest to perform an unsupported client zone operation.");
			break;
	}
}

void Client::ZonePC(uint32 zoneID, float x, float y, float z, float heading, uint8 ignorerestrictions, ZoneMode zm) {

	bool ReadyToZone = true;
	int iZoneNameLength = 0;
	const char*	pShortZoneName = nullptr;
	char* pZoneName = nullptr;

	pShortZoneName = ZoneName(zoneID);
	auto zd = GetZone(zoneID);
	if (zd) {
		pZoneName = strcpy(new char[strlen(zd->long_name.c_str()) + 1], zd->long_name.c_str());
	}

	cheat_manager.SetExemptStatus(Port, true);

	if(!pZoneName) {
		Message(Chat::Red, "Invalid zone number specified");
		safe_delete_array(pZoneName);
		return;
	}
	iZoneNameLength = strlen(pZoneName);
	glm::vec4 safePoint;

	switch(zm) {
		case EvacToSafeCoords:
		case ZoneToSafeCoords:
			safePoint = zone->GetSafePoint();
			x = safePoint.x;
			y = safePoint.y;
			z = safePoint.z;
			heading = safePoint.w;
			break;
		case GMSummon:
			m_Position = glm::vec4(x, y, z, heading);
			m_ZoneSummonLocation = m_Position;
			zonesummon_id = zoneID;
			zonesummon_ignorerestrictions = 1;
			break;
		case ZoneSolicited:
			m_ZoneSummonLocation = glm::vec4(x,y,z,heading);
			zonesummon_id = zoneID;
			zonesummon_ignorerestrictions = ignorerestrictions;
			break;
		case GateToBindPoint:
			x = m_Position.x = m_pp.binds[0].x;
			y = m_Position.y = m_pp.binds[0].y;
			z = m_Position.z = m_pp.binds[0].z;
			heading = m_pp.binds[0].heading;
			m_Position.w = heading * 0.5f;
			break;
		case ZoneToBindPoint:
			x = m_Position.x = m_pp.binds[0].x;
			y = m_Position.y = m_pp.binds[0].y;
			z = m_Position.z = m_pp.binds[0].z;
			heading = m_pp.binds[0].heading;
			m_Position.w = heading * 0.5f;

			zonesummon_ignorerestrictions = 1;
			LogInfo(
				"Player [{}] has died and will be zoned to bind point in zone: [{}] at LOC x=[{}], y=[{}], z=[{}], heading=[{}]", 
				GetName(), 
				pZoneName, 
				m_pp.binds[0].x, 
				m_pp.binds[0].y, 
				m_pp.binds[0].z, 
				m_pp.binds[0].heading
			);
			break;
		case SummonPC:
			m_ZoneSummonLocation = glm::vec4(x, y, z, heading);
			m_Position = m_ZoneSummonLocation;
			break;
		case Rewind:
			LogInfo(
				"[{}] has requested a /rewind from [{}], [{}], [{}], to [{}], [{}], [{}] in [{}]", 
				GetName(), 
				m_Position.x, 
				m_Position.y, 
				m_Position.z, 
				m_RewindLocation.x, 
				m_RewindLocation.y, 
				m_RewindLocation.z, 
				zone->GetShortName()
			);
			m_ZoneSummonLocation = glm::vec4(x, y, z, heading);
			m_Position = m_ZoneSummonLocation;
			break;
		default:
			LogError("Client::ZonePC() received a request to perform an unsupported client zone operation.");
			ReadyToZone = false;
			break;
	}

	if (ReadyToZone)
	{
		//if client is looting, we need to send an end loot
		if (IsLooting())
		{
			Entity* entity = entity_list.GetID(entity_id_being_looted);
			if (entity == 0)
			{
				Corpse::SendLootReqErrorPacket(this);
			}
			else if (!entity->IsCorpse())
			{
				Corpse::SendLootReqErrorPacket(this);
			}
			else
			{
				Corpse::SendEndLootErrorPacket(this);
				entity->CastToCorpse()->EndLoot(this, nullptr);
			}
			SetLooting(0);
		}
		if(Trader)
		{
			Trader_EndTrader();
		}

		zone_mode = zm;

		if(zm == ZoneSolicited || zm == ZoneToSafeCoords) {
			LogEQMacDetail("Zoning packet about to be sent (ZS/ZTS). We are headed to zone: [{}], at [{}], [{}], [{}]", zoneID, x, y, z);
			auto outapp = new EQApplicationPacket(OP_RequestClientZoneChange, sizeof(RequestClientZoneChange_Struct));
			RequestClientZoneChange_Struct* gmg = (RequestClientZoneChange_Struct*) outapp->pBuffer;

			gmg->zone_id = zoneID;

			gmg->x = x;
			gmg->y = y;
			gmg->z = z;
			gmg->heading = heading;
			gmg->type = 0x01;				//an observed value, not sure of meaning

			outapp->priority = 6;
			FastQueuePacket(&outapp);	
		}
		else if (zm == ZoneToBindPoint) {
			//TODO: Find a better packet that works with EQMac on death. Sending OP_RequestClientZoneChange here usually does not zone the
			//player correctly (it starts the zoning process, then disconnect.) OP_GMGoto seems to work 90% of the time. It's a hack, but it works...
			LogEQMacDetail("Zoning packet about to be sent (ZTB). We are headed to zone: [{}], at [{}], [{}], [{}]", zoneID, x, y, z);
			auto outapp = new EQApplicationPacket(OP_GMGoto, sizeof(GMGoto_Struct));
			GMGoto_Struct* gmg = (GMGoto_Struct*) outapp->pBuffer;
	
			strcpy(gmg->charname,this->name);
			strcpy(gmg->gmname,this->name);
			gmg->zoneID = zoneID;
			gmg->x = x;
			gmg->y = y;
			gmg->z = z;
			outapp->priority = 6;
			FastQueuePacket(&outapp);
		}
		else if (zm == GateToBindPoint) {			

			// we hide the real zoneid we want to evac/succor to here
			zonesummon_id = zoneID;
			LogEQMacDetail("Zoning packet about to be sent (GTB). We are headed to zone: [{}], at [{}], [{}], [{}]", zoneID, x, y, z);
			if(zoneID == GetZoneID()) {
				//Not doing inter-zone for same zone gates. Client is supposed to handle these, based on PP info it is fed.
				//properly handle proximities
				entity_list.ProcessMove(this, glm::vec3(m_Position));
				m_Proximity = glm::vec3(m_Position);
				//send out updates to people in zone.
				SendPosition(true);
			}

			auto outapp = new EQApplicationPacket(OP_RequestClientZoneChange, sizeof(RequestClientZoneChange_Struct));
			RequestClientZoneChange_Struct* gmg = (RequestClientZoneChange_Struct*)outapp->pBuffer;

			gmg->zone_id = zoneID;
			gmg->x = x;
			gmg->y = y;
			gmg->z = z;
			gmg->heading = heading;
			gmg->type = 0x01;	//an observed value, not sure of meaning
			outapp->priority = 6;
			FastQueuePacket(&outapp);
		}
		else if(zm == EvacToSafeCoords)
		{
			LogEQMacDetail("Zoning packet about to be sent (ETSC). We are headed to zone: [{}], at [{}], [{}], [{}]", zoneID, x, y, z);
			auto outapp = new EQApplicationPacket(OP_RequestClientZoneChange, sizeof(RequestClientZoneChange_Struct));
			RequestClientZoneChange_Struct* gmg = (RequestClientZoneChange_Struct*) outapp->pBuffer;

			if (this->GetZoneID() == Zones::QEYNOS) {
				gmg->zone_id = Zones::QEYNOS2;
			}
			else {
				gmg->zone_id = Zones::QEYNOS;
			}

			gmg->x = x;
			gmg->y = y;
			gmg->z = z;
			gmg->heading = heading;
			gmg->type = 0x01;				// '0x01' was an observed value for the type field, not sure of meaning

			// we hide the real zoneid we want to evac/succor to here
			zonesummon_id = zoneID;

			outapp->priority = 6;
			FastQueuePacket(&outapp);
		}
		else {
			if(zoneID == GetZoneID()) {
				LogEQMacDetail("Zoning packet about to be sent (GOTO). We are headed to zone: [{}], at [{}], [{}], [{}]", zoneID, x, y, z);
				//properly handle proximities
				entity_list.ProcessMove(this, glm::vec3(m_Position));
				m_Proximity = glm::vec3(m_Position);

				//send out updates to people in zone.
				SendPosition(true);
			}

			auto outapp = new EQApplicationPacket(OP_RequestClientZoneChange, sizeof(RequestClientZoneChange_Struct));
			RequestClientZoneChange_Struct* gmg = (RequestClientZoneChange_Struct*) outapp->pBuffer;

			gmg->zone_id = zoneID;
			gmg->x = x;
			gmg->y = y;
			gmg->z = z;
			gmg->heading = heading * 2.0f; // this doubling is necessary because we are storing halved headings for players/mobs and this packet expects the normal 512 range heading
			gmg->type = 0x01;	//an observed value, not sure of meaning
			outapp->priority = 6;
			FastQueuePacket(&outapp);
		}

		LogEQMacDetail("Player [{}] has requested a zoning to LOC x=[{}], y=[{}], z=[{}], heading=[{}] in zoneid=[{}]", GetName(), x, y, z, heading, zoneID);
		//Clear zonesummon variables if we're zoning to our own zone
		//Client wont generate a zone change packet to the server in this case so
		//They aren't needed and it keeps behavior on next zone attempt from being undefined.
		if(zoneID == zone->GetZoneID())
		{
			if(zm != EvacToSafeCoords && zm != ZoneToSafeCoords && zm != ZoneToBindPoint)
			{
				m_ZoneSummonLocation = glm::vec4();
				zonesummon_id = 0;
				zonesummon_ignorerestrictions = 0;
				zone_mode = ZoneUnsolicited;
			}
		}
	}

	safe_delete_array(pZoneName);
}


void Client::GoToSafeCoords(uint16 zone_id) {
	if(zone_id == 0)
		zone_id = zone->GetZoneID();

	MovePC(zone_id, 0.0f, 0.0f, 0.0f, 0.0f, 0, ZoneToSafeCoords);
}


void Mob::Gate() {
	GoToBind();
}

void Client::Gate() 
{
	Mob::Gate();
}

void NPC::Gate() {
	entity_list.FilteredMessageClose_StringID(this, true, RuleI(Range,SpellMessages), Chat::SpellCrit, FilterSpellCrits, StringID::GATES, GetCleanName());
	
	if (GetHPRatio() < 25.0f)
	{
		SetHP(GetMaxHP() / 4);
	}

	Mob::Gate();
}

void Client::SetBindPoint(int to_zone, const glm::vec3& location) {
	if (to_zone == -1) {
		m_pp.binds[0].zoneId = zone->GetZoneID();
		m_pp.binds[0].x = m_Position.x;
		m_pp.binds[0].y = m_Position.y;
		m_pp.binds[0].z = m_Position.z;
		m_pp.binds[0].heading = m_Position.w * 2.0f;
	}
	else {
		m_pp.binds[0].zoneId = to_zone;
		m_pp.binds[0].x = location.x;
		m_pp.binds[0].y = location.y;
		m_pp.binds[0].z = location.z;
		m_pp.binds[0].heading = m_Position.w * 2.0f;
	}
	database.SaveCharacterBinds(this);
}

void Client::SetBindPoint2(int to_zone, const glm::vec4& location) {
	if (to_zone == -1) {
		m_pp.binds[0].zoneId = zone->GetZoneID();
		m_pp.binds[0].x = m_Position.x;
		m_pp.binds[0].y = m_Position.y;
		m_pp.binds[0].z = m_Position.z;
		m_pp.binds[0].heading = m_Position.w * 2.0f;
	}
	else {
		m_pp.binds[0].zoneId = to_zone;
		m_pp.binds[0].x = location.x;
		m_pp.binds[0].y = location.y;
		m_pp.binds[0].z = location.z;
		m_pp.binds[0].heading = location.w * 2.0f;
	}
	database.SaveCharacterBinds(this);
}

void Client::GoToBind(uint8 bindnum) {
	// if the bind number is invalid, use the primary bind
	if(bindnum > 4)
		bindnum = 0;

	// move the client, which will zone them if needed.
	// ignore restrictions on the zone request..?
	if(bindnum == 0)
		MovePC(m_pp.binds[0].zoneId, 0.0f, 0.0f, 0.0f, 0.0f, 1, GateToBindPoint);
	else
		MovePC(m_pp.binds[bindnum].zoneId, m_pp.binds[bindnum].x, m_pp.binds[bindnum].y, m_pp.binds[bindnum].z, m_pp.binds[bindnum].heading, 1);
}

void Client::GoToDeath() {
	//Client will request a zone in EQMac era clients, but let's make sure they get there:
	zone_mode = ZoneToBindPoint;
	MovePC(m_pp.binds[0].zoneId, 0.0f, 0.0f, 0.0f, 0.0f, 1, ZoneToBindPoint);
}

void Client::SetZoneFlag(uint32 zone_id) {
	if(HasZoneFlag(zone_id))
		return;

	ClearZoneFlag(zone_id);

	ZoneFlags_Struct* zfs = new ZoneFlags_Struct;
	zfs->zoneid = zone_id;
	ZoneFlags.Insert(zfs);

	std::string query = StringFormat("INSERT INTO character_zone_flags (id,zoneID) VALUES(%d,%d)", CharacterID(), zone_id);
	auto results = database.QueryDatabase(query);
	if(!results.Success())
		LogError("MySQL Error while trying to set zone flag for [{}]: [{}]", GetName(), results.ErrorMessage().c_str());
}

void Client::ClearZoneFlag(uint32 zone_id) {
	if(!HasZoneFlag(zone_id))
		return;

	LinkedListIterator<ZoneFlags_Struct*> iterator(ZoneFlags);
	iterator.Reset();
	while (iterator.MoreElements())
	{
		ZoneFlags_Struct* zfs = iterator.GetData();
		if (zfs->zoneid == zone_id)
		{
			iterator.RemoveCurrent(true);
		}
		iterator.Advance();
	}

	std::string query = StringFormat("DELETE FROM character_zone_flags WHERE id=%d AND zoneID=%d", CharacterID(), zone_id);
	auto results = database.QueryDatabase(query);
	if(!results.Success())
		LogError("MySQL Error while trying to clear zone flag for [{}]: [{}]", GetName(), results.ErrorMessage().c_str());

}

void Client::LoadZoneFlags(LinkedList<ZoneFlags_Struct*>* ZoneFlags) 
{
	ZoneFlags->Clear();
	std::string query = StringFormat("SELECT zoneID from character_zone_flags WHERE id=%d order by zoneID", CharacterID());
	auto results = database.QueryDatabase(query);
    if (!results.Success()) {
        LogError("MySQL Error while trying to load zone flags for [{}]: [{}]", GetName(), results.ErrorMessage().c_str());
        return;
    }

	for(auto row = results.begin(); row != results.end(); ++row)
	{
		ZoneFlags_Struct* zfs = new ZoneFlags_Struct;
		zfs->zoneid = atoi(row[0]);
		ZoneFlags->Insert(zfs);
	}
}

bool Client::HasZoneFlag(uint32 zone_id) {

	if(GetGM())
		return true;

	LinkedListIterator<ZoneFlags_Struct*> iterator(ZoneFlags);
	iterator.Reset();
	while (iterator.MoreElements())
	{
		ZoneFlags_Struct* zfs = iterator.GetData();
		if (zfs->zoneid == zone_id)
		{
			return true;
		}
		iterator.Advance();
	}
	return false;
}

void Client::SendZoneFlagInfo(Client *to) {
	if(ZoneFlags.Count() == 0) {
		to->Message(Chat::White, "%s has no zone flags.", GetName());
		return;
	}

	to->Message(Chat::White, "Flags for %s:", GetName());
	char empty[1] = { '\0' };
	LinkedListIterator<ZoneFlags_Struct*> iterator(ZoneFlags);
	iterator.Reset();
	while (iterator.MoreElements())
	{
		ZoneFlags_Struct* zfs = iterator.GetData();
		uint32 zoneid = zfs->zoneid;

		const char *zone_short_name = ZoneName(zoneid);
		std::string zone_long_name = ZoneLongName(zoneid);

		char flag_name[128];

		auto z = GetZone(zoneid);
		if (!z) {
			strcpy(flag_name, "(ERROR GETTING NAME)");
		}

		to->Message(Chat::White, "Has Flag %s for zone %s (%d,%s)", flag_name, zone_long_name, zoneid, zone_short_name);

		iterator.Advance();
	}
}

bool Client::CanBeInZone(uint32 zoneid)
{
	//check some critial rules to see if this char needs to be booted from the zone
	//only enforce rules here which are serious enough to warrant being kicked from
	//the zone

	if(Admin() >= RuleI(GM, MinStatusToZoneAnywhere))
		return true;

	// If zoneid is 0, then we are just checking the current zone. In that case the player has already been allowed 
	// to zone, and we're checking if we should boot them to bazaar.
	const char *target_zone_name = zoneid > 0 ? ZoneName(zoneid) : zone->GetShortName();
	uint32 target_zone_id = zoneid > 0 ? zoneid : zone->GetZoneID();

	float safe_x, safe_y, safe_z, safe_heading;
	int16 min_status = 0;
	uint8 min_level = 0;

	auto z = GetZone(ZoneID(target_zone_name));
	if(!z) {
		return false;
	}

	safe_x = z->safe_x;
	safe_y = z->safe_y;
	safe_z = z->safe_z;
	safe_heading = z->safe_heading;
	min_status = z->min_status;
	min_level = z->min_level;

	if(GetLevel() < min_level) {
		LogCharacterDetail("Character does not meet min level requirement ([{}] < [{}])!", GetLevel(), min_level);
		return false;
	}
	if(Admin() < min_status) {
		LogCharacterDetail("Character does not meet min status requirement ([{}] < [{}])!", Admin(), min_status);
		return false;
	}

	if(!z->flag_needed.empty()) {
		//the flag needed string is not empty, meaning a flag is required.
		if(Admin() < minStatusToIgnoreZoneFlags && !HasZoneFlag(target_zone_id)) {
			LogInfo("Character [{}] does not have the flag to be in this zone [{}]!", GetCleanName(), z->flag_needed);
			return false;
		}
	}

	if (Admin() < minStatusToIgnoreZoneFlags && IsMule() && 
		(target_zone_id != Zones::BAZAAR && target_zone_id != Zones::NEXUS && target_zone_id != Zones::POKNOWLEDGE))
	{
		LogCharacterDetail("Character is a mule and cannot leave Bazaar/Nexus/PoK!");
		Message(Chat::Red, "Trader accounts may not leave Bazaar, Plane of Knowledge, or Nexus!");
		return false;
	}

	return true;
}

void Client::UpdateZoneChangeCount(uint32 zoneID)
{
	if(zoneID != GetZoneID() && !ignore_zone_count)
	{
		++m_pp.zone_change_count;
		ignore_zone_count = true;
	}

}

