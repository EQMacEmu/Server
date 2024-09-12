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
#ifndef SPAWNGROUP_H
#define SPAWNGROUP_H

#include "../common/types.h"

#include <map>
#include <list>
#include <memory>

class SpawnEntry {
public:
	SpawnEntry(uint32 in_NPCType, int in_chance, uint8 in_npc_spawn_limit, uint8 in_mintime, uint8 in_maxtime);
	~SpawnEntry() {}
	uint32 NPCType;
	int chance;
	uint8 mintime;
	uint8 maxtime;

	//this is a cached value from npc_types, for speed
	uint8 npc_spawn_limit; //max # of this entry which can be spawned in this zone
};

class SpawnGroup {
public:
	SpawnGroup(
		uint32 in_id, 
		char* name, 
		int in_group_spawn_limit, 
		float maxx, 
		float minx, 
		float maxy, 
		float miny, 
		int delay_in, 
		int despawn_in, 
		uint32 despawn_timer_in, 
		int min_delay_in, 
		bool wp_spawns_in 
	);

	~SpawnGroup();
	uint32 GetNPCType(uint32& rtime, uint32 spawn2_id = 0);
	void AddSpawnEntry(std::unique_ptr<SpawnEntry>& newEntry);
	uint32 id;
	bool wp_spawns;			// if true, spawn NPCs at a random waypoint location (if spawnpoint has a grid) instead of the spawnpoint's loc
	bool roamdist;
	float roambox[4];
	int min_delay;
	int delay;
	int despawn;
	uint32 despawn_timer;

	// These variables are only used when we have a spawn limit on the spawngroup
	uint8 group_spawn_limit; //max # of this entry which can be spawned by this group
	uint32 skipped_spawn;
	uint8 initial_loaded;

private:
	char name_[120];
	std::list<std::unique_ptr<SpawnEntry>> list_;
};

class SpawnGroupList {
public:
	SpawnGroupList() {}
	~SpawnGroupList();

	void AddSpawnGroup(std::unique_ptr<SpawnGroup>& new_group);
	SpawnGroup* GetSpawnGroup(uint32 id);
	void ClearSpawnGroups();
	void ReloadSpawnGroups();
private:
	std::map<uint32, std::unique_ptr<SpawnGroup>> m_spawn_groups;
};

#endif
