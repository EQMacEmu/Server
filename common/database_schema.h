/**
 * EQEmulator: Everquest Server Emulator
 * Copyright (C) 2001-2019 EQEmulator Development Team (https://github.com/EQEmu/Server)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY except by those people which sell it, which
 * are required to give you total support for your newly bought product;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef EQEMU_DATABASE_SCHEMA_H
#define EQEMU_DATABASE_SCHEMA_H

#include <vector>
#include <map>
#include <string>

namespace DatabaseSchema {

	/**
 * Character-specific tables
 *
 * Does not included related meta-data tables such as 'guilds', 'accounts'
 * @return
 */
	static std::map<std::string, std::string> GetCharacterTables()
	{
		return {
			{"character_alternate_abilities",  "id"},
			{"character_bind",                 "id"},
			{"character_buffs",                "character_id"},
			{"character_corpses",              "id"},
			{"character_currency",             "id"},
			{"character_data",                 "id"},
			{"character_faction_values",       "char_id"},
			{"character_inventory",            "charid"},
			{"character_keyring",              "char_id"},
			{"character_languages",            "id"},
			{"character_memmed_spells",        "id"},
			{"character_pet_buffs",            "char_id"},
			{"character_pet_info",             "char_id"},
			{"character_pet_inventory",        "char_id"},
			{"character_skills",               "id"},
			{"character_spells",               "id"},
			{"character_timers",               "char_id"},
			{"character_zone_flags",           "charID"},
			{"data_buckets",                   "character_id"},
			{"friends",                        "charid"},
			{"guild_members",                  "char_id"},
			{"guilds",                         "id"},
			{"mail",                           "charid"},
			{"player_titlesets",               "char_id"},
			{"quest_globals",                  "charid"},
			{"trader",                         "char_id"}
		};
	}

	/**
	 * Gets player tables
	 *
	 * @return
	 */
	static std::vector<std::string> GetPlayerTables()
	{
		return {
			"account",
			"account_ip",
			"account_flags",
			"character_alternate_abilities",
			"character_consent",
			"character_bind",
			"character_buffs",
			"character_consent",
			"character_corpse_items",
			"character_corpse_items_backup",
			"character_corpses",
			"character_corpses_backup",
			"character_currency",
			"character_data",
			"character_faction_values",
			"character_inventory",
			"character_keyring",
			"character_languages",
			"character_lookup",
			"character_memmed_spells",
			"character_pet_buffs",
			"character_pet_info",
			"character_pet_inventory",
			"character_skills",
			"character_soulmarks",
			"character_spells",
			"character_timers",
			"character_zone_flags",
			"data_buckets",
			"discovered_items",
			"friends",
			"guilds",
			"guild_ranks",
			"guild_members",
			"mail",
			"petitions",
			"player_titlesets",
			"quest_globals",
			"spell_buckets",
			"spell_globals",
			"titles",
			"trader",
		};
	}

	/**
	 * Gets content tables
	 *
	 * @return
	 */
	static std::vector<std::string> GetContentTables()
	{
		return {
			"aa_actions",
			"aa_effects",
			"altadv_vars",
			"blocked_spells",
			"books",
			"char_create_combinations",
			"char_create_point_allocations",
			"damageshieldtypes",
			"doors",
			"faction_list",
			"faction_list_mod",
			"fishing",
			"forage",
			"global_loot",
			"goallists",
			"graveyard",
			"grid",
			"grid_entries",
			"ground_spawns",
			"horses",
			"items",
			"keyring_data",
			"lootdrop",
			"lootdrop_entries",
			"loottable",
			"loottable_entries",
			"merchantlist",
			"npc_emotes",
			"npc_faction",
			"npc_faction_entries",
			"npc_spells",
			"npc_spells_entries",
			"npc_spells_effects",
			"npc_spells_effects_entries",
			"npc_types",
			"npc_types_metadata",
			"npc_types_tint",
			"object",
			"pets",
			"races",
			"skill_caps",
			"skill_difficulty",
			"spawn2",
			"spawn_conditions",
			"spawnentry",
			"spawngroup",
			"spells_en",
			"spells_new",
			"start_zones",
			"starting_items",
			"tradeskill_recipe",
			"tradeskill_recipe_entries",
			"traps",
			"zone",
			"zone_points"
		};
	}

	/**
	 * Gets server tables
	 *
	 * @return
	 */
	static std::vector<std::string> GetServerTables()
	{
		return {
			
			"chatchannels",
			"command_settings",
			"command_subsettings",
			"content_flags",
			"eqtime",
			"launcher",
			"launcher_zones",
			"level_exp_mods",
			"logsys_categories",
			"name_filter",
			"profanity_list",
			"spawn_condition_values",
			"spawn_events",
			"rule_sets",
			"rule_values",
			"titles",
			"variables",
		};
	}

	/**
	* Gets state tables
	 * Tables that keep track of server state
	 *
	 * @return
	 */

	static std::vector<std::string> GetStateTables()
	{
		return {
			"banned_ips",
			"bugs",
			"commands_log",
			"discord_webhooks",
			"gm_ips",
			"group_id",
			"group_leaders",
			"group_members",
			"group_ranks",
			"item_tick",
			"mb_messages",
			"merchantlist_temp",
			"object_contents",
			"proximities",
			"raid_details",
			"raid_members",
			"reports",
			"respawn_times",
			"saylink",
			"server_scheduled_events",
			"player_event_aa_purchase",
			"player_event_killed_npc",
			"player_event_killed_named_npc",
			"player_event_killed_raid_npc",
			"player_event_log_settings",
			"player_event_logs",
			"player_event_loot_items",
			"player_event_merchant_purchase",
			"player_event_merchant_sell",
			"player_event_npc_handin",
			"player_event_npc_handin_entries",
			"player_event_speech",
			"player_event_trade",
			"player_event_trade_entries",
			"webdata_character",
			"webdata_servers"
		};
	}

	/**
	* Gets login tables
	 *
	 * @return
	 */
	static std::vector<std::string> GetLoginTables()
	{
		return {
			"login_accounts",
			//"login_api_tokens",
			"login_server_admins",
			"login_server_list_types",
			"login_world_servers",
		};
	}

	/**
	* Gets login tables
	*
	 * @return
	 */
	static std::vector<std::string> GetVersionTables()
	{
		return {
			"client_version"//,
			//"db_version"
		};
	}
}





#endif //EQEMU_DATABASE_SCHEMA_H