#include "../client.h"

void command_ai(Client *c, const Seperator *sep)
{
	int arguments = sep->argnum;
	if (!arguments) {
		c->Message(Chat::White, "Usage: #ai consider [Mob Name] - Show how an NPC considers to a mob");
		c->Message(Chat::White, "Usage: #ai faction [Faction ID] - Set an NPC's Faction ID");
		c->Message(Chat::White, "Usage: #ai guard - Save an NPC's guard spot to their current location");
		c->Message(Chat::White, "Usage: #ai roambox [Min X] [Max X] [Min Y] [Max Y] [Delay] [Minimum Delay] - Set an NPC's roambox using X and Y coordinates");
		c->Message(Chat::White, "Usage: #ai spells [Spell List ID] - Set an NPC's Spell List ID");
		return;
	}

	if (!c->GetTarget() || !c->GetTarget()->IsNPC()) {
		c->Message(Chat::White, "You must target an NPC to use this command.");
		return;
	}

	auto target = c->GetTarget()->CastToNPC();

	bool is_consider = !strcasecmp(sep->arg[1], "consider");
	bool is_faction = !strcasecmp(sep->arg[1], "faction");
	bool is_guard = !strcasecmp(sep->arg[1], "guard");
	bool is_roambox = !strcasecmp(sep->arg[1], "roambox");
	bool is_spells = !strcasecmp(sep->arg[1], "spells");

	if (
		!is_consider &&
		!is_faction &&
		!is_guard &&
		!is_roambox &&
		!is_spells
		) {
		c->Message(Chat::White, "Usage: #ai consider [Mob Name] - Show how an NPC considers to a mob");
		c->Message(Chat::White, "Usage: #ai faction [Faction ID] - Set an NPC's Faction ID");
		c->Message(Chat::White, "Usage: #ai guard - Save an NPC's guard spot to their current location");
		c->Message(Chat::White, "Usage: #ai roambox [Min X] [Max X] [Min Y] [Max Y] [Delay] [Minimum Delay] - Set an NPC's roambox using X and Y coordinates");
		c->Message(Chat::White, "Usage: #ai spells [Spell List ID] - Set an NPC's Spell List ID");
		return;
	}

	if (is_consider) {
		if (arguments == 2) {
			auto mob_name = sep->arg[2];
			auto mob_to_consider = entity_list.GetMob(mob_name);
			if (mob_to_consider) {
				auto consider_level = static_cast<uint8>(mob_to_consider->GetReverseFactionCon(target));
				c->Message(
					Chat::White,
					fmt::format(
						"{} ({}) considers {} ({}) as {} ({}).",
						target->GetCleanName(),
						target->GetID(),
						mob_to_consider->GetCleanName(),
						mob_to_consider->GetID(),
						EQ::constants::GetConsiderLevelName(consider_level),
						consider_level
					).c_str()
				);
			}
			else
				c->Message(Chat::White, fmt::format("Error: {} not found.", mob_name).c_str());
		}
		else {
			c->Message(Chat::White, "Usage: #ai consider [Mob Name] - Show how an NPC considers a mob");
		}
	}
	else if (is_faction) {
		if (sep->IsNumber(2)) {
			auto faction_id = std::stoi(sep->arg[2]);
			auto faction_name = database.GetFactionName(faction_id);
			target->SetNPCFactionID(faction_id);
			c->Message(
				Chat::White,
				fmt::format(
					"{} ({}) is now on Faction {}.",
					target->GetCleanName(),
					target->GetID(),
					(
						faction_name.empty() ?
						std::to_string(faction_id) :
						fmt::format(
							"{} ({})",
							faction_name,
							faction_id
						)
						)
				).c_str()
			);
		}
		else {
			c->Message(Chat::White, "Usage: #ai faction [Faction ID] - Set an NPC's Faction ID");
		}
	}
	else if (is_guard) {
		auto target_position = target->GetPosition();

		target->SaveGuardSpot();

		c->Message(
			Chat::White,
			fmt::format(
				"{} ({}) now has a guard spot of {:.2f}, {:.2f}, {:.2f} with a heading of {:.2f}.",
				target->GetCleanName(),
				target->GetID(),
				target_position.x,
				target_position.y,
				target_position.z,
				target_position.w
			).c_str()
		);
	}
	else if (is_roambox) {
		if (target->IsAIControlled()) {
			if (
				arguments >= 5 &&
				arguments <= 7 &&
				sep->IsNumber(2) &&
				sep->IsNumber(3) &&
				sep->IsNumber(4) &&
				sep->IsNumber(5)
				) {
				auto min_x = std::stof(sep->arg[2]);
				auto max_x = std::stof(sep->arg[3]);
				auto min_y = std::stof(sep->arg[4]);
				auto max_y = std::stof(sep->arg[5]);

				uint32 delay = 2500;
				uint32 minimum_delay = 2500;

				if (sep->IsNumber(6)) {
					delay = std::stoul(sep->arg[6]);
				}

				if (sep->IsNumber(7)) {
					minimum_delay = std::stoul(sep->arg[7]);
				}

				target->CastToNPC()->AI_SetRoambox(
					max_x,
					min_x,
					max_y,
					min_y,
					delay,
					minimum_delay
				);

				c->Message(
					Chat::White,
					fmt::format(
						"{} ({}) now has a roambox from {}, {} to {}, {} with {} and {}.",
						target->GetCleanName(),
						target->GetID(),
						min_x,
						min_y,
						max_x,
						max_y,
						(
							delay ?
							fmt::format(
								"a delay of {} ({})",
								Strings::MillisecondsToTime(delay),
								delay
							) :
							"no delay"
							),
						(
							minimum_delay ?
							fmt::format(
								"a minimum delay of {} ({})",
								Strings::MillisecondsToTime(minimum_delay),
								minimum_delay
							) :
							"no minimum delay"
							)
					).c_str()
				);
			}
			else {
				c->Message(Chat::White, "Usage: #ai roambox [Min X] [Max X] [Min Y] [Max Y] [Delay] [Minimum Delay] - Set an NPC's roambox using X and Y coordinates");
			}
		}
		else {
			c->Message(Chat::White, "You must target an NPC with AI.");
		}
	}
	else if (is_spells) {
		if (sep->IsNumber(2)) {
			auto spell_list_id = std::stoul(sep->arg[2]);
			if (spell_list_id >= 0) {
				target->CastToNPC()->AI_AddNPCSpells(spell_list_id);

				c->Message(
					Chat::White,
					fmt::format(
						"{} ({}) is now using Spell List {}.",
						target->GetCleanName(),
						target->GetID(),
						spell_list_id
					).c_str()
				);
			}
			else {
				c->Message(Chat::White, "Spell List ID must be greater than or equal to 0.");
			}
		}
	}
}

