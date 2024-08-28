#include "../client.h"
#include "../../common/patches/patches.h"

void command_reload(Client *c, const Seperator *sep)
{
	std::string command = sep->arg[0] ? sep->arg[0] : "";
	const auto  arguments = sep->argnum;
	if (!arguments && Strings::Contains(command, "#reload")) {
		c->SendReloadCommandMessages();
		return;
	}

	bool is_logs_reload_alias = sep->arg[0] && Strings::Contains(command, "#rl");
	bool is_opcodes_reload_alias = sep->arg[0] && Strings::Contains(command, "#opcode");
	bool is_rq_alias = sep->arg[0] && Strings::Contains(command, "#rq");
	bool is_aa = !strcasecmp(sep->arg[1], "aa");
	bool is_blocked_spells = !strcasecmp(sep->arg[1], "blocked_spells");
	bool is_commands = !strcasecmp(sep->arg[1], "commands");
	bool is_content_flags = !strcasecmp(sep->arg[1], "content_flags");
	bool is_doors = !strcasecmp(sep->arg[1], "doors");
	bool is_ground_spawns = !strcasecmp(sep->arg[1], "ground_spawns");
	bool is_level_mods = !strcasecmp(sep->arg[1], "level_mods");
	bool is_logs = !strcasecmp(sep->arg[1], "logs") || is_logs_reload_alias;
	bool is_loot = !strcasecmp(sep->arg[1], "loot");
	bool is_keyrings = !strcasecmp(sep->arg[1], "keyrings");
	bool is_merchants = !strcasecmp(sep->arg[1], "merchants");
	bool is_npc_emotes = !strcasecmp(sep->arg[1], "npc_emotes");
	bool is_npc_spells = !strcasecmp(sep->arg[1], "npc_spells");
	bool is_objects = !strcasecmp(sep->arg[1], "objects");
	bool is_opcodes = !strcasecmp(sep->arg[1], "opcodes") || is_opcodes_reload_alias;
	bool is_quest = !strcasecmp(sep->arg[1], "quest") || (is_rq_alias);
	bool is_rules = !strcasecmp(sep->arg[1], "rules");
	bool is_static = !strcasecmp(sep->arg[1], "static");
	bool is_titles = !strcasecmp(sep->arg[1], "titles");
	bool is_traps = !strcasecmp(sep->arg[1], "traps");
	bool is_variables = !strcasecmp(sep->arg[1], "variables");
	bool is_world = !strcasecmp(sep->arg[1], "world");
	bool is_zone = !strcasecmp(sep->arg[1], "zone");
	bool is_zone_points = !strcasecmp(sep->arg[1], "zone_points");

	if (
		!is_aa &&
		!is_blocked_spells &&
		!is_commands &&
		!is_content_flags &&
		!is_doors &&
		!is_ground_spawns &&
		!is_level_mods &&
		!is_logs &&
		!is_loot &&
		!is_keyrings &&
		!is_merchants &&
		!is_npc_emotes &&
		!is_npc_spells &&
		!is_objects &&
		!is_opcodes &&
		!is_quest &&
		!is_rules &&
		!is_static &&
		!is_titles &&
		!is_traps &&
		!is_variables &&
		!is_world &&
		!is_zone &&
		!is_zone_points
	) {
		c->SendReloadCommandMessages();
		return;
	}

	auto pack = new ServerPacket;

	if (is_aa) {
		c->Message(Chat::White, "Attempting to reload Alternate Advancement Data globally.");	
		pack = new ServerPacket(ServerOP_ReloadAAData, 0);
	} else if (is_blocked_spells) {
		c->Message(Chat::White, "Attempting to reload Blocked Spells globally.");	
		pack = new ServerPacket(ServerOP_ReloadBlockedSpells, 0);
	} else if (is_commands) {
		c->Message(Chat::White, "Attempting to reload Commands globally.");
		pack = new ServerPacket(ServerOP_ReloadCommands, 0);
	} else if (is_content_flags) {		
		c->Message(Chat::White, "Attempting to reload Content Flags globally.");
		pack = new ServerPacket(ServerOP_ReloadContentFlags, 0);
	} else if (is_doors) {
		c->Message(Chat::White, "Attempting to reload Doors globally.");	
		pack = new ServerPacket(ServerOP_ReloadDoors, 0);
	} else if (is_ground_spawns) {
		c->Message(Chat::White, "Attempting to reload Ground Spawns globally.");	
		pack = new ServerPacket(ServerOP_ReloadGroundSpawns, 0);
	} else if (is_level_mods) {
		if (!RuleB(Zone, LevelBasedEXPMods)) {
			c->Message(Chat::White, "Level Based Experience Modifiers are disabled.");
			return;
		}
		c->Message(Chat::White, "Attempting to reload Level Based Experience Modifiers globally.");
		pack = new ServerPacket(ServerOP_ReloadLevelEXPMods, 0);
	} else if (is_logs) {
		c->Message(Chat::White, "Attempting to reload Log Settings globally.");
		pack = new ServerPacket(ServerOP_ReloadLogs, 0);
	} else if (is_loot) {
		c->Message(Chat::White, "Attempting to reload Loot globally.");
		pack = new ServerPacket(ServerOP_ReloadLoot, 0);
	} else if (is_keyrings) {
		c->Message(Chat::White, "Attempting to reload Key Rings globally.");
		pack = new ServerPacket(ServerOP_ReloadKeyRings, 0);
	} else if (is_merchants) {
		c->Message(Chat::White, "Attempting to reload Merchants globally.");
		pack = new ServerPacket(ServerOP_ReloadMerchants, 0);
	} else if (is_npc_emotes) {
		c->Message(Chat::White, "Attempting to reload NPC Emotes globally.");
		pack = new ServerPacket(ServerOP_ReloadNPCEmotes, 0);
	} else if (is_npc_spells) {
		c->Message(Chat::White, "Attempting to reload NPC Spells globally.");
		pack = new ServerPacket(ServerOP_ReloadNPCSpells, 0);
	} else if (is_objects) {
		c->Message(Chat::White, "Attempting to reload Objects globally.");
		pack = new ServerPacket(ServerOP_ReloadObjects, 0);
	} else if (is_opcodes) {
		c->Message(Chat::White, "Attempting to reload Opcodes globally.");
		pack = new ServerPacket(ServerOP_ReloadOpcodes, 0);
	} else if (is_quest) {
		const auto stop_timers = sep->IsNumber(2) ? Strings::ToBool(sep->arg[2]) : false;

		c->Message(
			Chat::Yellow,
			fmt::format(
				"Quests reloaded{} for {}.",
				stop_timers ? " and timers stopped" : "",
				zone->GetZoneDescription()
			).c_str()
		);

		entity_list.ClearAreas();
		parse->ReloadQuests(stop_timers);
	} else if (is_rules) {
		c->Message(Chat::White, "Attempting to reload Rules globally.");
		pack = new ServerPacket(ServerOP_ReloadRules, 0);
	} else if (is_static) {		
		c->Message(Chat::White, "Attempting to reload Static Zone Data globally.");
		pack = new ServerPacket(ServerOP_ReloadStaticZoneData, 0);
	} else if (is_titles) {		
		c->Message(Chat::White, "Attempting to reload Titles globally.");
		pack = new ServerPacket(ServerOP_ReloadTitles, 0);
	} else if (is_traps) {
		if (arguments < 2 || !sep->IsNumber(2)) {
			entity_list.UpdateAllTraps(true, true);
			c->Message(
				Chat::Yellow,
				fmt::format(
					"Traps reloaded for {}.",
					zone->GetZoneDescription()
				).c_str()
			);
			return;
		}

		const auto global = Strings::ToBool(sep->arg[2]);

		if (!global) {
			entity_list.UpdateAllTraps(true, true);
			c->Message(
				Chat::Yellow,
				fmt::format(
					"Traps reloaded for {}.",
					zone->GetZoneDescription()
				).c_str()
			);
			return;
		}

		c->Message(Chat::White, "Attempting to reload Traps globally.");
		pack = new ServerPacket(ServerOP_ReloadTraps, 0);
	} else if (is_variables) {
		c->Message(Chat::White, "Attempting to reload Variables globally.");
		pack = new ServerPacket(ServerOP_ReloadVariables, 0);
	} else if (is_world) {
		uint8 global_repop = ReloadWorld::NoRepop;

		if (sep->IsNumber(2)) {
			global_repop = static_cast<uint8>(std::stoul(sep->arg[2]));

			if (global_repop > ReloadWorld::ForceRepop) {
				global_repop = ReloadWorld::ForceRepop;
			}
		}

		c->Message(
			Chat::White,
			fmt::format(
				"Attempting to reload Quests {}worldwide.",
				(
					global_repop ?
					(
						global_repop == ReloadWorld::Repop ?
						"and repop NPCs " :
						"and forcefully repop NPCs "
					) :
					""
				)
			).c_str()
		);

		pack = new ServerPacket(ServerOP_ReloadWorld, sizeof(ReloadWorld_Struct));
		auto RW  = (ReloadWorld_Struct *) pack->pBuffer;
		RW->global_repop = global_repop;
	} else if (is_zone) {
		c->Message(Chat::White, "Attempting to reloading Zone data globally.");
		pack = new ServerPacket(ServerOP_ReloadZoneData, sizeof(NewZone_Struct));
	} else if (is_zone_points) {		
		c->Message(Chat::White, "Attempting to reloading Zone Points globally.");
		pack = new ServerPacket(ServerOP_ReloadZonePoints, 0);
	}
	
	if (pack->opcode) {
		worldserver.SendPacket(pack);
	}

	safe_delete(pack);
}