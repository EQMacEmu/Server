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
/*
Below are the blob structures for zone state dumping to the database
-Quagmire

create table zone_state_dump (zonename varchar(16) not null primary key, spawn2_count int unsigned not null default 0,
npc_count int unsigned not null default 0, npcloot_count int unsigned not null default 0, gmspawntype_count int unsigned not null default 0,
spawn2 mediumblob, npcs mediumblob, npc_loot mediumblob, gmspawntype mediumblob, time timestamp(14));
*/

#ifndef ZONEDUMP_H
#define ZONEDUMP_H
#include "../common/faction.h"
#include "../common/eq_packet_structs.h"
#include "../common/inventory_profile.h"

#pragma pack(1)

struct NPCType
{
	char	name[64];
	char	lastname[70]; 
	int32	cur_hp;
	int32	max_hp; 
	float	size;
	float	runspeed;
	float	walkspeed;
	uint8	gender;
	uint16	race;
	uint8	class_;
	uint8	bodytype;	// added for targettype support
	uint8	deity;		//not loaded from DB
	uint8	level;
	uint32	npc_id;
	uint8	texture;
	uint8	helmtexture;
	uint32	loottable_id;
	uint32	npc_spells_id;
	uint32	npc_spells_effects_id;
	int32	npc_faction_id;
	uint32	merchanttype;
	uint8	light;
	uint32	AC;
	uint32	Mana;	//not loaded from DB
	uint32	ATK;	//not loaded from DB
	uint32	STR;
	uint32	STA;
	uint32	DEX;
	uint32	AGI;
	uint32	INT;
	uint32	WIS;
	uint32	CHA;
	int32	MR;
	int32	FR;
	int32	CR;
	int32	PR;
	int32	DR;
	uint8	haircolor;
	uint8	beardcolor;
	uint8	eyecolor1;			// the eyecolors always seem to be the same, maybe left and right eye?
	uint8	eyecolor2;
	uint8	hairstyle;
	uint8	luclinface;			//
	uint8	beard;				//
	EQ::TintProfile	armor_tint;
	uint32	min_dmg;
	uint32	max_dmg;
	int16	attack_count;
	char	special_abilities[512];
	uint16	d_melee_texture1;
	uint16	d_melee_texture2;
	uint8	prim_melee_type;
	uint8	sec_melee_type;
	uint8	ranged_type;
	int32	hp_regen;
	int32	mana_regen;
	int32	aggroradius; // added for AI improvement - neotokyo
	int32	assistradius; // assist radius, defaults to aggroradis if not set
	uint8	see_invis;			// See Invis flag added
	bool	see_invis_undead;	// See Invis vs. Undead flag added
	uint8	see_sneak;
	uint8	see_improved_hide;
	bool	qglobal;
	bool	npc_aggro;
	uint8	spawn_limit;	//only this many may be in zone at a time (0=no limit)
	uint8	mount_color;	//only used by horse class
	uint8	attack_delay;	//delay between attacks in 10ths of a second
	int		accuracy_rating;	//10 = 1% accuracy
	int16	slow_mitigation;	
	uint8	maxlevel;
	uint32	scalerate;
	bool	private_corpse;
	bool	unique_spawn_by_name;
	bool	underwater;
	uint32	emoteid;
	float	spellscale;
	float	healscale;
	bool	raid_target;
	uint8 	probability;
	uint32  combat_hp_regen;
	uint32  combat_mana_regen;
	bool	aggro_pc;
	uint8	armtexture;
	uint8	bracertexture;
	uint8	handtexture;
	uint8	legtexture;
	uint8	feettexture;
	uint8	chesttexture;
	float	ignore_distance;
	bool	ignore_despawn;
	int16	avoidance;
	uint16	exp_pct;
	uint8	greed;
	bool	engage_notice;
	int8	stuck_behavior;
	int8	flymode;
	bool	skip_global_loot;
	bool	rare_spawn;
};

struct PlayerCorpse_Struct {
	uint32	crc;
	bool	locked;
	uint32	itemcount;
	uint32	exp;
	uint32	gmexp;
	float	size;
	uint8	level;
	uint32	race;
	uint8	gender;
	uint8	class_;
	uint8	deity;
	uint8	texture;
	uint8	helmtexture;
	uint32	copper;
	uint32	silver;
	uint32	gold;
	uint32	plat;
	EQ::TintProfile item_tint;
	uint8 haircolor;
	uint8 beardcolor;
	uint8 eyecolor1;
	uint8 eyecolor2;
	uint8 hairstyle;
	uint8 face;
	uint8 beard;
	uint8 killedby;
	bool  rezzable;
	uint32	rez_time;
	uint32 time_of_death;
	ServerLootItem_Struct	items[0];
	//std::list<player_lootitem::ServerLootItem_Struct*> items;
};

#pragma pack()

#endif
