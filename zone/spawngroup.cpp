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
#include "../common/strings.h"
#include "../common/types.h"

#include "entity.h"
#include "spawngroup.h"
#include "zone.h"
#include "zonedb.h"
#include "../common/repositories/criteria/content_filter_criteria.h"

extern EntityList entity_list;
extern Zone* zone;

SpawnEntry::SpawnEntry( uint32 in_NPCType, int in_chance, uint8 in_npc_spawn_limit, uint8 in_mintime, uint8 in_maxtime ) {
	NPCType = in_NPCType;
	chance = in_chance;
	npc_spawn_limit = in_npc_spawn_limit;
	mintime = in_mintime;
	maxtime = in_maxtime;
}

SpawnGroup::SpawnGroup( uint32 in_id, char* name, int in_group_spawn_limit, float maxx, float minx, float maxy, float miny, int delay_in, int despawn_in, uint32 despawn_timer_in, int min_delay_in, bool wp_spawns_in ) {
	id = in_id;
	strn0cpy( name_, name, 120);
	group_spawn_limit = in_group_spawn_limit;
	roambox[0]=maxx;
	roambox[1]=minx;
	roambox[2]=maxy;
	roambox[3]=miny;

	if (maxx != 0 || minx != 0 || maxy != 0 || miny != 0)
		roamdist = true;
	else
		roamdist = false;
	min_delay=min_delay_in;
	delay=delay_in;
	despawn=despawn_in;
	despawn_timer=despawn_timer_in;
	skipped_spawn = 0;
	initial_loaded = 0;
	wp_spawns = wp_spawns_in;
}

uint32 SpawnGroup::GetNPCType(uint32& rtime, uint32 spawn2_id) 
{
	int npcType = 0;
	int totalchance = 0;

	if (group_spawn_limit > 0)
	{
		uint16 spawned_count = 0;
		if (!entity_list.LimitCheckGroup(id, group_spawn_limit, spawned_count))
			return(0);

		uint32 remaining_time_id = 0;
		uint8 waiting_to_spawn = zone->GetGroupActiveTimers(id, remaining_time_id);

		Spawn2 *sp = entity_list.GetSpawnByID(remaining_time_id);
		uint32 remaining_time = sp ? sp->timer.GetRemainingTime() : 0;

		if (waiting_to_spawn + spawned_count >= group_spawn_limit && remaining_time != 0)
		{
			// We are at the spawn limit, nothing will spawn this round.

			// We will only reach this point once per round if the spawn limit is 1, because all the spawnpoints will mass reset in Spawn2::Process() immediately following this.
			// For spawn limits greater than 1, we will reach this point once for every spawnpoint that has expired and then they will each reset one at a time. A mass timer reset will not occur here.
			// This is important because we may have multiple spawnpoints waiting to spawn and we need to keep track of their respective timers.
			Log(Logs::Moderate, Logs::Spawns, "Spawngroup %d: %d spawns are waiting to spawn, and %d spawns are up. Limit of %d prevents a new NPC from spawning. NPC will spawn in %d seconds", id, waiting_to_spawn, spawned_count, group_spawn_limit, static_cast<uint32>(remaining_time / 1000));
			rtime = group_spawn_limit == 1 ? remaining_time_id: 0;
			return 0;
		}
		else
		{			
			// We will be spawning something here if the spawn limit is 1. If it is higher than 1, then will force a new spawnpoint to be chosen and then that will immediately spawn.

			// It is possible for a dead mob to have its timer offset from the others in the group. Due to this, once its timer expires it will automatically be picked again 
			// because the other timers are still active. This will force us to pick another spawnpoint every time we are ready to spawn something, excluding the intial loading. 
			// Spawngroups with a limit of 1 are exempt, because we are always keeping all of their timers in sync.
			if (group_spawn_limit > 1 && initial_loaded == group_spawn_limit && skipped_spawn == 0 && spawn2_id != 0)
			{
				if (zone->GetAvailPointCount(this) > 1)
				{
					Log(Logs::General, Logs::Spawns, "Spawngroup %d: Spawnpoint %d Forcing another spawnpoint to be picked.", id, spawn2_id);
					skipped_spawn = spawn2_id;
					rtime = 1;
					return 0;
				}
			}

			Log(Logs::Moderate, Logs::Spawns, "Spawngroup %d: %d has passed limit checks.", id, spawn2_id);
		}
	}

	std::list<SpawnEntry*> possible;
	for (auto& it : list_) {
		auto se = it.get();

		if(!entity_list.LimitCheckType(se->NPCType, se->npc_spawn_limit))
			continue;

		if(se->mintime != 0 && se->maxtime != 0 && se->mintime <= 24 && se->maxtime <= 24)
		{
			if(!zone->zone_time.IsInbetweenTime(se->mintime, se->maxtime))
				continue;
		}

		totalchance += se->chance;
		possible.push_back(se);
	}

	if(totalchance == 0)
		return 0;


	int32 roll = 0;
	roll = zone->random.Int(0, totalchance-1);

	for (auto se : possible) {
		if (roll < se->chance) {
			npcType = se->NPCType;
			break;
		} else {
			roll -= se->chance;
		}
	}

	if (group_spawn_limit > 1)
	{
		skipped_spawn = 0;

		if (initial_loaded < group_spawn_limit)
		{
			++initial_loaded;
		}
	}

	return npcType;
}

void SpawnGroup::AddSpawnEntry(std::unique_ptr<SpawnEntry>& newEntry) {
	list_.push_back(std::move(newEntry));
}

SpawnGroup::~SpawnGroup() 
{
	list_.clear();
}

SpawnGroupList::~SpawnGroupList() {

	m_spawn_groups.clear();
}

void SpawnGroupList::AddSpawnGroup(std::unique_ptr<SpawnGroup>& new_group)
{
	if(new_group == nullptr)
		return;
	m_spawn_groups[new_group->id] = std::move(new_group);
}

SpawnGroup* SpawnGroupList::GetSpawnGroup(uint32 in_id)
{
	if (m_spawn_groups.count(in_id) != 1) {
		return nullptr;
	}

	return (m_spawn_groups[in_id].get());
}

bool SpawnGroupList::RemoveSpawnGroup(uint32 in_id) 
{
	if (m_spawn_groups.count(in_id) != 1) {
		return (false);
	}

	m_spawn_groups.erase(in_id);

	return (true);
}

bool ZoneDatabase::LoadSpawnGroups(const char* zone_name, SpawnGroupList* spawn_group_list) {

	std::string query = fmt::format( 
		SQL(
			SELECT 
			DISTINCT(spawngroupID),
				spawngroup.name, 
				spawngroup.spawn_limit, 
				spawngroup.max_x, 
				spawngroup.min_x,
				spawngroup.max_y,
				spawngroup.min_y,
				spawngroup.delay,
				spawngroup.despawn,
				spawngroup.despawn_timer,
				spawngroup.mindelay,
				spawngroup.wp_spawns
			FROM
				spawn2,
				spawngroup 
			WHERE 
				spawn2.spawngroupID = spawngroup.ID
				AND 
				zone = '{}'
				{}
		), 
		zone_name, 
		ContentFilterCriteria::apply()
	);

    auto results = QueryDatabase(query);
    if (!results.Success()) {
		return false;
    }

    for (auto row = results.begin(); row != results.end(); ++row) {
        auto new_spawn_group = std::make_unique<SpawnGroup>(
			atoi(row[0]), 
			row[1],
			atoi(row[2]),
			atof(row[3]),
			atof(row[4]),
			atof(row[5]),
			atof(row[6]),
			atof(row[7]),
			atoi(row[8]),
			atoi(row[9]),
			atoi(row[10]),
			atoi(row[11])
		);

        spawn_group_list->AddSpawnGroup(new_spawn_group);
    }

	query = fmt::format(
		SQL(
			SELECT 
			DISTINCT 
				spawnentry.spawngroupID,
				npcid,
				chance,
				npc_types.spawn_limit 
				AS sl, mintime, maxtime
			FROM
				spawnentry,
				spawn2,
				npc_types
			WHERE 
				spawnentry.npcID=npc_types.id
				AND
				spawnentry.spawngroupID = spawn2.spawngroupID
				AND
				zone = '{}'
				{}
			), 
			zone_name,
			ContentFilterCriteria::apply("spawnentry")
	);

    results = QueryDatabase(query);
    if (!results.Success()) {
        Log(Logs::General, Logs::Error, "Error2 in PopulateZoneLists query '%'", query.c_str());
		return false;
    }

    for (auto row = results.begin(); row != results.end(); ++row) {
        auto newSpawnEntry = std::make_unique<SpawnEntry>( 
			atoi(row[1]),
			atoi(row[2]),
			row[3]?atoi(row[3]):0,
			atoi(row[4]),
			atoi(row[5])
		);

		SpawnGroup *spawn_group = spawn_group_list->GetSpawnGroup(atoi(row[0]));

		if (!spawn_group) {
            continue;
		}

		spawn_group->AddSpawnEntry(newSpawnEntry);
    }

	return true;
}

bool ZoneDatabase::LoadSpawnGroupsByID(int spawngroupid, SpawnGroupList* spawn_group_list) {


	std::string query = StringFormat("SELECT DISTINCT(spawngroup.id), spawngroup.name, spawngroup.spawn_limit, "
                                    "spawngroup.max_x, spawngroup.min_x, "
                                    "spawngroup.max_y, spawngroup.min_y, spawngroup.delay, "
                                    "spawngroup.despawn, spawngroup.despawn_timer, spawngroup.mindelay, spawngroup.wp_spawns "
                                    "FROM spawngroup WHERE spawngroup.ID = '%i'", spawngroupid);
    auto results = QueryDatabase(query);
    if (!results.Success()) {
        Log(Logs::General, Logs::Error, "Error2 in PopulateZoneLists query %s", query.c_str());
		return false;
    }

    for (auto row = results.begin(); row != results.end(); ++row) {
        auto newSpawnGroup = std::make_unique<SpawnGroup>(atoi(row[0]), row[1], atoi(row[2]), atof(row[3]), atof(row[4]), atof(row[5]), atof(row[6]), atof(row[7]), atoi(row[8]), atoi(row[9]), atoi(row[10]), atoi(row[11]));
        spawn_group_list->AddSpawnGroup(newSpawnGroup);
    }

	query = StringFormat("SELECT DISTINCT(spawnentry.spawngroupID), spawnentry.npcid, "
                        "spawnentry.chance, spawngroup.spawn_limit, spawnentry.mintime, spawnentry.maxtime FROM spawnentry, spawngroup "
                        "WHERE spawnentry.spawngroupID = '%i' AND spawngroup.spawn_limit = '0' "
                        "ORDER BY chance", spawngroupid);
    results = QueryDatabase(query);
	if (!results.Success()) {
        Log(Logs::General, Logs::Error, "Error3 in PopulateZoneLists query '%s'", query.c_str());
		return false;
	}

    for(auto row = results.begin(); row != results.end(); ++row) {
        auto newSpawnEntry = std::make_unique<SpawnEntry>( atoi(row[1]), atoi(row[2]), row[3]?atoi(row[3]):0, atoi(row[4]), atoi(row[5]));
        SpawnGroup *sg = spawn_group_list->GetSpawnGroup(atoi(row[0]));
        if (!sg) {
            continue;
        }

        sg->AddSpawnEntry(newSpawnEntry);
    }

	return true;
}
