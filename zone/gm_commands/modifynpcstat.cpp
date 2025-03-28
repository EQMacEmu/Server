#include "../client.h"

void command_modifynpcstat(Client *c, const Seperator *sep)
{
	auto arguments = sep->argnum;
	if (!arguments) {
		c->Message(Chat::White, "Usage: #modifynpcstat [Stat] [Value]");
		ListModifyNPCStatMap(c);
		return;
	}

	if (!c->GetTarget() || !c->GetTarget()->IsNPC()) {
		c->Message(Chat::White, "You must target an NPC to use this command.");
		return;
	}

	auto target = c->GetTarget()->CastToNPC();

	std::string stat = sep->arg[1];
	std::string value = sep->arg[2] ? sep->arg[2] : "";

	auto stat_description = GetModifyNPCStatDescription(stat);
	if (!stat_description.length()) {
		c->Message(
			Chat::White,
			fmt::format(
				"Stat '{}' does not exist.",
				stat
			).c_str()
		);
		return;
	}

	target->ModifyNPCStat(stat, value);

	c->Message(
		Chat::White,
		fmt::format(
			"Stat Modified | Target: {}",
			c->GetTargetDescription(target)
		).c_str()
	);

	c->Message(
		Chat::White,
		fmt::format(
			"Stat Modified | Stat: {} ({}) Value: {}",
			GetModifyNPCStatDescription(stat),
			stat,
			value
		).c_str()
	);
}

std::map<std::string, std::string> GetModifyNPCStatMap()
{
	std::map<std::string, std::string> identifiers_map = {
		{ "ac", "Armor Class" },
		{ "accuracy", "Accuracy" },
		{ "aggro", "Aggro" },
		{ "agi", "Agility" },
		{ "assist", "Assist" },
		{ "atk", "Attack" },
		{ "attack_count", "Attack Count" },
		{ "attack_delay", "Attack Delay" },
		{ "cha", "Charisma" },
		{ "combat_hp_regen", "HP Regen in Combat" },
		{ "combat_mana_regen", "Mana Regen in Combat"},
		{ "cr", "Cold Resist" },
		{ "dex", "Dexterity" },
		{ "dr", "Disease Resist" },
		{ "fr", "Fire Resist" },
		{ "healscale", "Heal Scale" },
		{ "hp_regen", "HP Regen" },
		{ "int", "Intelligence" },
		{ "_int", "Intelligence" },
		{ "level", "Level" },
		{ "loottable_id", "Loottable ID" },
		{ "mana_regen", "Mana Regen" },
		{ "max_hit", "Maximum Damage" },
		{ "min_hit", "Minimum Damage" },
		{ "max_hp", "Maximum Hitpoints"},
		{ "max_mana", "Maximum Mana"},
		{ "mr", "Magic Resist" },
		{ "npc_spells_id", "NPC Spells ID" },
		{ "pr", "Poison Resist" },
		{ "runspeed", "Run Speed" },
		{ "see_invis", "See Invisible" },
		{ "see_invis_undead", "See Invisible vs. Undead" },
		{ "see_sneak", "See Sneak" },
		{ "see_improved_hide", "See Improved Hide" },
		{ "slow_mitigation", "Slow Mitigation" },
		{ "special_abilities", "Special Abilities" },
		{ "spellscale", "Spell Scale" },
		{ "sta", "Stamina" },
		{ "str", "Strength" },
		{ "wis", "Wisdom" }
	};

	return identifiers_map;
}

std::string GetModifyNPCStatDescription(std::string stat)
{
	if (GetModifyNPCStatMap().find(stat) != GetModifyNPCStatMap().end()) {
		return GetModifyNPCStatMap().find(stat)->second;
	}

	return std::string();
}

void ListModifyNPCStatMap(Client* c)
{
	for (const auto& s : GetModifyNPCStatMap()) {
		c->Message(
			Chat::White,
			fmt::format(
				"Usage: #modifynpcstat {} [Value] - Modifies an NPC's {}",
				s.first,
				s.second
			).c_str()
		);
	}
}
