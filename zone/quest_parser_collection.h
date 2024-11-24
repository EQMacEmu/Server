/*  EQEMu:  Everquest Server Emulator
	Copyright (C) 2001-2006  EQEMu Development Team (http://eqemulator.net)

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

#ifndef _EQE_QUESTPARSERCOLLECTION_H
#define _EQE_QUESTPARSERCOLLECTION_H

#include "../common/types.h"

#include "encounter.h"
#include "beacon.h"
#include "client.h"
#include "corpse.h"
#include "doors.h"
#include "groups.h"
#include "mob.h"
#include "object.h"
#include "raids.h"
#include "trap.h"

#include "quest_interface.h"

#include "zone_config.h"

#include <list>
#include <map>

#define QuestFailedToLoad 0xFFFFFFFF
#define QuestUnloaded 0x00

extern const ZoneConfig *Config;
class Client;
class Mob;
class NPC;
class QuestInterface;

namespace EQ
{ 
	class Any; 
	class ItemInstance;
}

class QuestParserCollection {
public:
	QuestParserCollection();
	~QuestParserCollection();

	void RegisterQuestInterface(QuestInterface *qi, std::string ext);
	void ClearInterfaces();
	void AddVar(std::string name, std::string val);
	void Init();
	void ReloadQuests(bool reset_timers = true);
	void RemoveEncounter(const std::string& name);

	bool HasQuestSub(uint32 npcid, QuestEventID event_id);
	bool PlayerHasQuestSub(QuestEventID event_id);
	bool SpellHasQuestSub(uint32 spell_id, QuestEventID event_id);
	bool ItemHasQuestSub(EQ::ItemInstance* inst, QuestEventID event_id);

	int EventNPC(
		QuestEventID event_id,
		NPC* npc,
		Mob* init,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers = nullptr
	);

	int EventPlayer(
		QuestEventID event_id,
		Client* client,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers = nullptr
	);

	int EventItem(
		QuestEventID event_id,
		Client* client,
		EQ::ItemInstance* inst,
		Mob* mob,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers = nullptr
	);

	int EventSpell(
		QuestEventID event_id, 
		NPC* npc, 
		Client *client, 
		uint32 spell_id, 
		uint32 extra_data,
		std::vector<std::any> *extra_pointers = nullptr
	);

	int EventEncounter(
		QuestEventID event_id,
		std::string encounter_name,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers = nullptr
	);

	int EventMob(
		QuestEventID event_id,
		Mob* e,
		Mob* init,
		std::function<std::string()> lazy_data = []() { return ""; },
		uint32 extra_data = 0,
		std::vector<std::any>* extra_pointers = nullptr
	);

	void GetErrors(std::list<std::string> &quest_errors);

private:
	bool HasQuestSubLocal(uint32 npc_id, QuestEventID event_id);
	bool HasQuestSubGlobal(QuestEventID event_id);
	bool NPCHasEncounterSub(uint32 npc_id, QuestEventID event_id);
	bool PlayerHasQuestSubLocal(QuestEventID event_id);
	bool PlayerHasQuestSubGlobal(QuestEventID event_id);
	bool PlayerHasEncounterSub(QuestEventID event_id);
	bool SpellHasEncounterSub(uint32 spell_id, QuestEventID event_id);
	bool ItemHasEncounterSub(EQ::ItemInstance* inst, QuestEventID event_id);
	bool HasEncounterSub(QuestEventID event_id, const std::string& package_name);

	int EventNPCLocal(
		QuestEventID event_id,
		NPC* npc,
		Mob* init,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers
	);

	int EventNPCGlobal(
		QuestEventID event_id,
		NPC* npc,
		Mob* init,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers
	);

	int EventPlayerLocal(
		QuestEventID event_id,
		Client* client,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers
	);

	int EventPlayerGlobal(
		QuestEventID event_id,
		Client* client,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers
	);

	QuestInterface* GetQIByNPCQuest(uint32 npc_id, std::string& filename);
	QuestInterface* GetQIByGlobalNPCQuest(std::string& filename);
	QuestInterface* GetQIByPlayerQuest(std::string& filename);
	QuestInterface* GetQIByGlobalPlayerQuest(std::string& filename);
	QuestInterface* GetQIBySpellQuest(uint32 spell_id, std::string& filename);
	QuestInterface* GetQIByItemQuest(std::string item_script, std::string& filename);
	QuestInterface* GetQIByEncounterQuest(std::string encounter_name, std::string& filename);
	
	int DispatchEventNPC(
		QuestEventID event_id,
		NPC* npc,
		Mob* init,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers
	);

	int DispatchEventPlayer(
		QuestEventID event_id,
		Client* client,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers
	);

	int DispatchEventItem(
		QuestEventID event_id,
		Client* client,
		EQ::ItemInstance* inst,
		Mob* mob,
		std::string data,
		uint32 extra_data,
		std::vector<std::any>* extra_pointers
	);

	int DispatchEventSpell(
		QuestEventID event_id, 
		NPC* npc, 
		Client *client, 
		uint32 spell_id, 
		uint32 extra_data,
		std::vector<std::any> *extra_pointers
	);

	std::map<uint32, QuestInterface*> _interfaces;
	std::map<uint32, std::string>     _extensions;
	std::list<QuestInterface*>        _load_precedence;

	std::map<uint32, uint32>      _npc_quest_status;
	uint32                        _global_npc_quest_status;
	uint32                        _player_quest_status;
	uint32                        _global_player_quest_status;
	std::map<uint32, uint32>      _spell_quest_status;
	std::map<uint32, uint32>      _item_quest_status;
	std::map<std::string, uint32> _encounter_quest_status;
};

extern QuestParserCollection *parse;

#endif

