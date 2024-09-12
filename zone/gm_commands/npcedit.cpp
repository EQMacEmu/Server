#include "../client.h"
#include "../../common/repositories/npc_types_repository.h"

void command_npcedit(Client *c, const Seperator *sep)
{
	if (!c->GetTarget() || !c->GetTarget()->IsNPC()) {
		c->Message(Chat::White, "You must target an NPC to use this command.");
		return;
	}

	int arguments = sep->argnum;

	bool is_help = (!arguments || !strcasecmp(sep->arg[1], "help"));

	if (is_help) {
		SendNPCEditSubCommands(c);
		return;
	}


	std::string sub_command = sep->arg[1];

	auto t = c->GetTarget()->CastToNPC();
	auto npc_id = t->GetNPCTypeID();
	auto npc_id_string = fmt::format(
		"NPC ID {}",
		Strings::Commify(std::to_string(npc_id))
	);

	auto n = NpcTypesRepository::FindOne(database, npc_id);
	if (!n.id) {
		c->Message(Chat::White, "You must target a valid NPC to use this command.");
		return;
	}

	std::string d;

	if (!strcasecmp(sep->arg[1], "name")) {
		std::string name = sep->argplus[2];
		if (!name.empty()) {
			n.name = name;
			d = fmt::format(
				"{} is now named '{}'.",
				npc_id_string,
				name
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit name [Name] - Sets an NPC's Name");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "lastname")) {
		std::string last_name = sep->argplus[2];
		n.lastname = last_name;
		d = fmt::format(
			"{} now has the lastname '{}'.",
			npc_id_string,
			last_name
		);
	}
	else if (!strcasecmp(sep->arg[1], "level")) {
		if (sep->IsNumber(2)) {
			auto level = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.level = level;
			d = fmt::format(
				"{} is now level {}.",
				npc_id_string,
				level
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit level [Level] - Sets an NPC's Level");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "maxlevel")) {
		if (sep->IsNumber(2)) {
			auto max_level = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.maxlevel = max_level;
			d = fmt::format(
				"{} is now level {}.",
				npc_id_string,
				max_level
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit maxlevel [Max Level] - Sets an NPC's Max Level");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "race")) {
		if (sep->IsNumber(2)) {
			auto race_id = static_cast<uint16_t>(std::stoul(sep->arg[2]));
			n.race = race_id;
			d = fmt::format(
				"{} is now a(n) {} ({}).",
				npc_id_string,
				GetRaceIDName(race_id),
				race_id
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit race [Race ID] - Sets an NPC's Race");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "class")) {
		if (sep->IsNumber(2)) {
			auto class_id = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.class_ = class_id;
			d = fmt::format(
				"{} is now a(n) {} ({}).",
				npc_id_string,
				GetClassIDName(class_id),
				class_id
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit class [Class ID] - Sets an NPC's Class");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "bodytype")) {
		if (sep->IsNumber(2)) {
			auto body_type_id = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			auto body_type_name = BodyType::GetName(body_type_id);
			n.bodytype = body_type_id;
			d = fmt::format(
				"{} is now using Body Type {}.",
				npc_id_string,
				(
					!body_type_name.empty() ?
					fmt::format(
						"{} ({})",
						body_type_name,
						body_type_id
					) :
					std::to_string(body_type_id)
					)
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit bodytype [Body Type ID] - Sets an NPC's Bodytype");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "hp")) {
		if (sep->IsNumber(2)) {
			auto hp = std::stoll(sep->arg[2]);
			n.hp = hp;
			d = fmt::format(
				"{} now has {} Health.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit hp [HP] - Sets an NPC's HP");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "mana")) {
		if (sep->IsNumber(2)) {
			auto mana = std::stoll(sep->arg[2]);
			n.mana = mana;
			d = fmt::format(
				"{} now has {} Mana.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit mana [Mana] - Sets an NPC's Mana");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "gender")) {
		if (sep->IsNumber(2)) {
			auto gender_id = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.gender = gender_id;
			d = fmt::format(
				"{} is now a {} ({}).",
				npc_id_string,
				gender_id,
				GetGenderName(gender_id)
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit gender [Gender ID] - Sets an NPC's Gender");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "texture")) {
		if (sep->IsNumber(2)) {
			auto texture = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.texture = texture;
			d = fmt::format(
				"{} is now using Texture {}.",
				npc_id_string,
				texture
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit texture [Texture] - Sets an NPC's Texture");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "helmtexture")) {
		if (sep->IsNumber(2)) {
			auto helmet_texture = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.helmtexture = helmet_texture;
			d = fmt::format(
				"{} is now using Helmet Texture {}.",
				npc_id_string,
				helmet_texture
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit helmtexture [Helmet Texture] - Sets an NPC's Helmet Texture");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "armtexture")) {
		if (sep->IsNumber(2)) {
			auto arm_texture = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.armtexture = arm_texture;
			d = fmt::format(
				"{} is now using Arm Texture {}.",
				npc_id_string,
				arm_texture
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit armtexture [Arm Texture] - Sets an NPC's Arm Texture");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "bracertexture")) {
		if (sep->IsNumber(2)) {
			auto bracer_texture = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.bracertexture = bracer_texture;
			d = fmt::format(
				"{} is now using Bracer Texture {}.",
				npc_id_string,
				bracer_texture
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit bracertexture [Bracer Texture] - Sets an NPC's Bracer Texture");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "handtexture")) {
		if (sep->IsNumber(2)) {
			auto hand_texture = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.handtexture = hand_texture;
			d = fmt::format(
				"{} is now using Hand Texture {}.",
				npc_id_string,
				hand_texture
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit handtexture [Hand Texture] - Sets an NPC's Hand Texture");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "legtexture")) {
		if (sep->IsNumber(2)) {
			auto leg_texture = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.legtexture = leg_texture;
			d = fmt::format(
				"{} is now using Leg Texture {}.",
				npc_id_string,
				leg_texture
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit legtexture [Leg Texture] - Sets an NPC's Leg Texture");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "feettexture")) {
		if (sep->IsNumber(2)) {
			auto feet_texture = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.feettexture = feet_texture;
			d = fmt::format(
				"{} is now using Feet Texture {}.",
				npc_id_string,
				feet_texture
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit feettexture [Feet Texture] - Sets an NPC's Feet Texture");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "size")) {
		if (sep->IsNumber(2)) {
			auto size = std::stof(sep->arg[2]);
			n.size = size;
			d = fmt::format(
				"{} is now Size {:.2f}.",
				npc_id_string,
				size
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit size [Size] - Sets an NPC's Size");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "hpregen")) {
		if (sep->IsNumber(2)) {
			auto hp_regen = std::stoll(sep->arg[2]);
			n.hp_regen_rate = hp_regen;
			d = fmt::format(
				"{} now regenerates {} Health per Tick.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit hpregen [HP Regen] - Sets an NPC's HP Regen Rate Per Tick");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "combathpregen")) {
		if (sep->IsNumber(2)) {
			auto combat_hp_regen = std::stoll(sep->arg[2]);
			n.combat_hp_regen = combat_hp_regen;
			d = fmt::format(
				"{} now regenerates {} Health per Tick in Combat.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit combathpregen [Combat HP Regen] - Sets an NPC's Combat HP Regen Rate Per Tick");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "manaregen")) {
		if (sep->IsNumber(2)) {
			auto mana_regen = std::stoll(sep->arg[2]);
			n.mana_regen_rate = mana_regen;
			d = fmt::format(
				"{} now regenerates {} Mana per Tick.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit manaregen [Mana Regen] - Sets an NPC's Mana Regen Rate Per Tick");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "combatmanaregen")) {
		if (sep->IsNumber(2)) {
			auto combat_mana_regen = std::stoll(sep->arg[2]);
			n.combat_mana_regen = combat_mana_regen;
			d = fmt::format(
				"{} now regenerates {} Mana per Tick in Combat.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit combatmanaregen [Combat Mana Regen] - Sets an NPC's Combat Mana Regen Rate Per Tick");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "loottable")) {
		if (sep->IsNumber(2)) {
			auto loottable_id = std::stoul(sep->arg[2]);
			n.loottable_id = loottable_id;
			d = fmt::format(
				"{} is now using Loottable ID {}.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit loottable [Loottable ID] - Sets an NPC's Loottable ID");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "merchantid")) {
		if (sep->IsNumber(2)) {
			auto merchant_id = std::stoul(sep->arg[2]);
			n.merchant_id = merchant_id;
			d = fmt::format(
				"{} is now using Merchant ID {}.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit merchantid [Merchant ID] - Sets an NPC's Merchant ID");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "npc_spells_effects_id")) {
		if (sep->IsNumber(2)) {
			auto spell_effects_id = std::stoul(sep->arg[2]);
			n.npc_spells_effects_id = spell_effects_id;
			d = fmt::format(
				"{} is now using Spells Effects ID {}.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit npc_spells_effects_id [Spell Effects ID] - Sets an NPC's Spell Effects ID"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "special_abilities")) {
		std::string special_abilities = sep->argplus[2];
		n.special_abilities = special_abilities;
		c->Message(
			Chat::Yellow,
			fmt::format(
				"{} is now using the following Special Abilities '{}'.",
				npc_id_string,
				special_abilities
			).c_str()
		);
	}
	else if (!strcasecmp(sep->arg[1], "spell")) {
		if (sep->IsNumber(2)) {
			auto spell_list_id = std::stoul(sep->arg[2]);
			n.npc_spells_id = spell_list_id;
			d = fmt::format(
				"{} is now using Spell List ID {}.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit spell [Spell List ID] - Sets an NPC's Spells List ID");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "faction")) {
		if (sep->IsNumber(2)) {
			auto faction_id = std::stoi(sep->arg[2]);
			auto faction_name = database.GetFactionName(faction_id);
			n.npc_faction_id = faction_id;
			d = fmt::format(
				"{} is now using Faction {}.",
				npc_id_string,
				(
					!faction_name.empty() ?
					fmt::format(
						"{} ({})",
						faction_name,
						faction_id
					) :
					Strings::Commify(sep->arg[2])
					)
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit faction [Faction ID] - Sets an NPC's Faction ID");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "damage")) {
		if (sep->IsNumber(2) && sep->IsNumber(3)) {
			auto minimum_damage = std::stoul(sep->arg[2]);
			auto maximum_damage = std::stoul(sep->arg[3]);
			n.mindmg = minimum_damage;
			n.maxdmg = maximum_damage;
			d = fmt::format(
				"{} now hits from {} to {} damage.",
				npc_id_string,
				Strings::Commify(sep->arg[2]),
				Strings::Commify(sep->arg[3])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit damage [Minimum] [Maximum] - Sets an NPC's Damage");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "aggroradius")) {
		if (sep->IsNumber(2)) {
			auto aggro_radius = std::stoul(sep->arg[2]);
			n.aggroradius = aggro_radius;
			d = fmt::format(
				"{} now has an Aggro Radius of {}.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit aggroradius [Radius] - Sets an NPC's Aggro Radius");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "assistradius")) {
		if (sep->IsNumber(2)) {
			auto assist_radius = std::stoul(sep->arg[2]);
			n.assistradius = assist_radius;
			d = fmt::format(
				"{} now has an Assist Radius of {}",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit assistradius [Radius] - Sets an NPC's Assist Radius");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "runspeed")) {
		if (sep->IsNumber(2)) {
			auto run_speed = std::stof(sep->arg[2]);
			n.runspeed = run_speed;
			d = fmt::format(
				"{} now runs at a Run Speed of {:.2f}.",
				npc_id_string,
				run_speed
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit runspeed [Run Speed] - Sets an NPC's Run Speed");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "agi")) {
		if (sep->IsNumber(2)) {
			auto agility = std::stoul(sep->arg[2]);
			n.AGI = agility;
			d = fmt::format(
				"{} now has {} Agility.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit agi [Agility] - Sets an NPC's Agility");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "cha")) {
		if (sep->IsNumber(2)) {
			auto charisma = std::stoul(sep->arg[2]);
			n.CHA = charisma;
			d = fmt::format(
				"{} now has {} Charisma.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit cha [Charisma] - Sets an NPC's Charisma");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "dex")) {
		if (sep->IsNumber(2)) {
			auto dexterity = std::stoul(sep->arg[2]);
			n.DEX = dexterity;
			d = fmt::format(
				"{} now has {} Dexterity.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit dex [Dexterity] - Sets an NPC's Dexterity");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "int")) {
		if (sep->IsNumber(2)) {
			auto intelligence = std::stoul(sep->arg[2]);
			n._INT = intelligence;
			d = fmt::format(
				"{} now has {} Intelligence.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit int [Intelligence] - Sets an NPC's Intelligence");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "sta")) {
		if (sep->IsNumber(2)) {
			auto stamina = std::stoul(sep->arg[2]);
			n.STA = stamina;
			d = fmt::format(
				"{} now has {} Stamina.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit sta [Stamina] - Sets an NPC's Stamina");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "str")) {
		if (sep->IsNumber(2)) {
			auto strength = std::stoul(sep->arg[2]);
			n.STR = strength;
			d = fmt::format(
				"{} now has {} Strength.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit str [Strength] - Sets an NPC's Strength");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "wis")) {
		if (sep->IsNumber(2)) {
			auto wisdom = std::stoul(sep->arg[2]);
			n.WIS = wisdom;
			d = fmt::format(
				"{} now has {} Wisdom.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit wis [Wisdom] - Sets an NPC's Wisdom");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "mr")) {
		if (sep->IsNumber(2)) {
			auto magic_resist = static_cast<int16_t>(std::stoul(sep->arg[2]));
			n.MR = magic_resist;
			d = fmt::format(
				"{} now has a Magic Resistance of {}.",
				npc_id_string,
				magic_resist
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit mr [Resistance] - Sets an NPC's Magic Resistance");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "dr")) {
		if (sep->IsNumber(2)) {
			auto disease_resist = static_cast<int16_t>(std::stoul(sep->arg[2]));
			n.DR = disease_resist;
			d = fmt::format(
				"{} now has a Disease Resistance of {}.",
				npc_id_string,
				disease_resist
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit dr [Resistance] - Sets an NPC's Disease Resistance");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "cr")) {
		if (sep->IsNumber(2)) {
			auto cold_resist = static_cast<int16_t>(std::stoul(sep->arg[2]));
			n.CR = cold_resist;
			d = fmt::format(
				"{} now has a Cold Resistance of {}.",
				npc_id_string,
				cold_resist
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit cr [Resistance] - Sets an NPC's Cold Resistance");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "fr")) {
		if (sep->IsNumber(2)) {
			auto fire_resist = static_cast<int16_t>(std::stoul(sep->arg[2]));
			n.FR = fire_resist;
			d = fmt::format(
				"{} now has a Fire Resistance of {}.",
				npc_id_string,
				fire_resist
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit fr [Resistance] - Sets an NPC's Fire Resistance");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "pr")) {
		if (sep->IsNumber(2)) {
			auto poison_resist = static_cast<int16_t>(std::stoul(sep->arg[2]));
			n.PR = poison_resist;
			d = fmt::format(
				"{} now has a Poison Resistance of {}.",
				npc_id_string,
				poison_resist
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit pr [Resistance] - Sets an NPC's Poison Resistance");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "seeinvis")) {
		if (sep->IsNumber(2)) {
			auto see_invisible = static_cast<int16_t>(std::stoul(sep->arg[2]));
			n.see_invis = see_invisible;
			d = fmt::format(
				"{} can {} See Invisible.",
				npc_id_string,
				see_invisible ? "now" : "no longer"
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit seeinvis [Flag] - Sets an NPC's See Invisible Flag [0 = Cannot See Invisible, 1 = Can See Invisible]"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "seeinvisundead")) {
		if (sep->IsNumber(2)) {
			auto see_invisible_undead = static_cast<int16_t>(std::stoul(sep->arg[2]));
			n.see_invis_undead = see_invisible_undead;
			d = fmt::format(
				"{} can {} See Invisible vs. Undead.",
				npc_id_string,
				see_invisible_undead ? "now" : "no longer"
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit seeinvisundead [Flag] - Sets an NPC's See Invisible vs. Undead Flag  [0 = Cannot See Invisible vs. Undead, 1 = Can See Invisible vs. Undead]"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "seesneak")) {
		if (sep->IsNumber(2)) {
			auto see_sneak = static_cast<int16_t>(std::stoul(sep->arg[2]));
			n.see_sneak = see_sneak;
			d = fmt::format(
				"{} can {} See Sneak.",
				npc_id_string,
				see_sneak ? "now" : "no longer"
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit seesneak [Flag] - Sets an NPC's See Sneak Flag  [0 = Cannot See Sneak, 1 = Can See Sneak]"
			);
			return;

		}
	}
	else if (!strcasecmp(sep->arg[1], "seeimprovedhide")) {
		if (sep->IsNumber(2)) {
			auto see_improved_hide = static_cast<int8>(std::stoi(sep->arg[2]));
			n.see_improved_hide = see_improved_hide;
			d = fmt::format(
				"{} can {} See Improved Hide.",
				npc_id_string,
				see_improved_hide ? "now" : "no longer"
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit seeimprovedhide [Flag] - Sets an NPC's See Improved Hide Flag [0 = Cannot See Improved Hide, 1 = Can See Improved Hide]"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "ac")) {
		if (sep->IsNumber(2)) {
			auto armor_class = static_cast<int16_t>(std::stoul(sep->arg[2]));
			n.AC = armor_class;
			d = fmt::format(
				"{} now has {} Armor Class.",
				npc_id_string,
				armor_class
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit ac [Armor Class] - Sets an NPC's Armor Class");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "atk")) {
		if (sep->IsNumber(2)) {
			auto attack = std::stoi(sep->arg[2]);
			n.ATK = attack;
			d = fmt::format(
				"{} now has {} Attack.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit atk [Attack] - Sets an NPC's Attack");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "accuracy")) {
		if (sep->IsNumber(2)) {
			auto accuracy = std::stoi(sep->arg[2]);
			n.Accuracy = accuracy;
			d = fmt::format(
				"{} now has {} Accuracy.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit accuracy [Accuracy] - Sets an NPC's Accuracy");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "qglobal")) {
		if (sep->IsNumber(2)) {
			auto use_qglobals = std::stoul(sep->arg[2]);
			n.qglobal = use_qglobals;
			d = fmt::format(
				"{} can {} use Quest Globals.",
				npc_id_string,
				use_qglobals ? "now" : "no longer"
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit qglobal [Flag] - Sets an NPC's Quest Global Flag [0 = Quest Globals Off, 1 = Quest Globals On]"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "npcaggro")) {
		if (sep->IsNumber(2)) {
			auto aggro_npcs = static_cast<int8_t>(std::stoul(sep->arg[2]));
			n.npc_aggro = aggro_npcs;
			d = fmt::format(
				"{} will {} aggro other NPCs that have a hostile faction.",
				npc_id_string,
				aggro_npcs ? "now" : "no longer"
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit npcaggro [Flag] - Sets an NPC's NPC Aggro Flag [0 = Aggro NPCs Off, 1 = Aggro NPCs On]"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "spawn_limit")) {
		if (sep->IsNumber(2)) {
			auto spawn_limit = static_cast<int8_t>(std::stoul(sep->arg[2]));
			n.spawn_limit = spawn_limit;
			d = fmt::format(
				"{} now has a Spawn Limit of {}.",
				npc_id_string,
				spawn_limit
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit spawn_limit [Limit] - Sets an NPC's Spawn Limit Counter");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "attackdelay")) {
		if (sep->IsNumber(2)) {
			auto attack_delay = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			n.attack_delay = attack_delay;
			d = fmt::format(
				"{} now has an Attack Delay of {}.",
				npc_id_string,
				attack_delay
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit attackdelay [Attack Delay] - Sets an NPC's Attack Delay");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "weapon")) {
		if (sep->IsNumber(2)) {
			auto     primary_model = std::stoul(sep->arg[2]);
			uint32_t secondary_model = sep->IsNumber(3) ? std::stoul(sep->arg[3]) : 0;
			n.d_melee_texture1 = primary_model;
			n.d_melee_texture2 = secondary_model;
			d = fmt::format(
				"{} will have Model {} set to their Primary and Model {} set to their Secondary on repop.",
				npc_id_string,
				Strings::Commify(sep->arg[2]),
				sep->IsNumber(3) ? Strings::Commify(sep->arg[3]) : 0
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit weapon [Primary Model] [Secondary Model] - Sets an NPC's Primary and Secondary Weapon Model"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "featuresave")) {
		d = fmt::format(
			"{} saved with all current body and facial feature settings.",
			npc_id
		);

		n.gender = t->GetGender();
		n.texture = t->GetTexture();
		n.helmtexture = t->GetHelmTexture();
		n.size = t->GetSize();
		n.face = t->GetLuclinFace();
		n.luclin_hairstyle = t->GetHairStyle();
		n.luclin_haircolor = t->GetHairColor();
		n.luclin_eyecolor = t->GetEyeColor1();
		n.luclin_eyecolor2 = t->GetEyeColor2();
		n.luclin_beardcolor = t->GetBeardColor();
		n.luclin_beard = t->GetBeard();
	}
	else if (!strcasecmp(sep->arg[1], "armortint_id")) {
		if (sep->IsNumber(2)) {
			auto armor_tint_id = std::stoul(sep->arg[2]);
			n.armortint_id = armor_tint_id;
			d = fmt::format(
				"{} is now using Armor Tint ID {}.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(Chat::White, "Usage: #npcedit armortint_id [Armor Tint ID] - Sets an NPC's Armor Tint ID");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "color")) {
		if (sep->IsNumber(2)) {
			auto  red = static_cast<uint8_t>(std::stoul(sep->arg[2]));
			uint8_t green = sep->IsNumber(3) ? static_cast<uint8_t>(std::stoul(sep->arg[3])) : 0;
			uint8_t blue = sep->IsNumber(4) ? static_cast<uint8_t>(std::stoul(sep->arg[4])) : 0;
			n.armortint_red = red;
			n.armortint_green = green;
			n.armortint_blue = blue;
			d = fmt::format(
				"{} now has {} Red, {} Green, and {} Blue tinting on their armor.",
				npc_id_string,
				red,
				green,
				blue
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit color [Red] [Green] [Blue] - Sets an NPC's Red, Green, and Blue armor tint"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "setanimation")) {
		if (sep->IsNumber(2)) {
			auto animation_id = std::stoul(sep->arg[2]);
			if (animation_id > EQ::constants::SpawnAnimations::Looting) {
				animation_id = EQ::constants::SpawnAnimations::Looting;
			}

			auto animation_name = EQ::constants::GetSpawnAnimationName(animation_id);
			d = fmt::format(
				"{} is now using Spawn Animation {} on Spawn Group ID {}.",
				npc_id_string,
				(
					!animation_name.empty() ?
					fmt::format(
						"{} ({})",
						animation_name,
						animation_id
					) :
					std::to_string(animation_id)
					),
				Strings::Commify(std::to_string(c->GetTarget()->CastToNPC()->GetSpawnGroupId()))
			);
			auto query = fmt::format(
				"UPDATE spawn2 SET animation = {} WHERE spawngroupID = {}",
				animation_id,
				c->GetTarget()->CastToNPC()->GetSpawnGroupId()
			);
			database.QueryDatabase(query);

			c->GetTarget()->SetAppearance(EmuAppearance(animation_id));
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit setanimation [Animation ID] - Sets an NPC's Animation on Spawn (Stored in spawn2 table)"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "scalerate")) {
		if (sep->IsNumber(2)) {
			auto scale_rate = std::stoi(sep->arg[2]);
			n.scalerate = scale_rate;
			d = fmt::format(
				"{} now has a Scaling Rate of {}%%.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit scalerate [Scale Rate] - Sets an NPC's Scaling Rate [50 = 50%, 100 = 100%, 200 = 200%]"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "spellscale")) {
		if (sep->IsNumber(2)) {
			auto spell_scale = std::stoul(sep->arg[2]);
			n.spellscale = static_cast<float>(spell_scale);
			d = fmt::format(
				"{} now has a Spell Scaling Rate of {}%%.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit spellscale [Scale Rate] - Sets an NPC's Spell Scaling Rate [50 = 50%, 100 = 100%, 200 = 200%]"
			);
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "healscale")) {
		if (sep->IsNumber(2)) {
			auto heal_scale = std::stoul(sep->arg[2]);
			n.healscale = static_cast<float>(heal_scale);
			d = fmt::format(
				"{} now has a Heal Scaling Rate of {}%%.",
				npc_id_string,
				Strings::Commify(sep->arg[2])
			);
		}
		else {
			c->Message(
				Chat::White,
				"Usage: #npcedit healscale [Scale Rate] - Sets an NPC's Heal Scaling Rate [50 = 50%, 100 = 100%, 200 = 200%]"
			);
			return;
		}
	}
	else {
		SendNPCEditSubCommands(c);
		return;
	}

	if (!NpcTypesRepository::UpdateOne(database, n)) {
		c->Message(
			Chat::White,
			fmt::format(
				"Failed to update {}.",
				npc_id_string
			).c_str()
		);
	}

	c->Message(Chat::White, d.c_str());
}

void SendNPCEditSubCommands(Client* c)
{
	c->Message(Chat::White, "Usage: #npcedit name [Name] - Sets an NPC's Name");
	c->Message(Chat::White, "Usage: #npcedit lastname [Last Name] - Sets an NPC's Last Name");
	c->Message(Chat::White, "Usage: #npcedit level [Level] - Sets an NPC's Level");
	c->Message(Chat::White, "Usage: #npcedit maxlevel [Max Level] - Sets an NPC's Maximum Level");
	c->Message(Chat::White, "Usage: #npcedit race [Race ID] - Sets an NPC's Race");
	c->Message(Chat::White, "Usage: #npcedit class [Class ID] - Sets an NPC's Class");
	c->Message(Chat::White, "Usage: #npcedit bodytype [Body Type ID] - Sets an NPC's Bodytype");
	c->Message(Chat::White, "Usage: #npcedit hp [HP] - Sets an NPC's HP");
	c->Message(Chat::White, "Usage: #npcedit mana [Mana] - Sets an NPC's Mana");
	c->Message(Chat::White, "Usage: #npcedit gender [Gender ID] - Sets an NPC's Gender");
	c->Message(Chat::White, "Usage: #npcedit texture [Texture] - Sets an NPC's Texture");
	c->Message(Chat::White, "Usage: #npcedit helmtexture [Helmet Texture] - Sets an NPC's Helmet Texture");
	c->Message(Chat::White, "Usage: #npcedit armtexture [Texture] - Sets an NPC's Arm Texture");
	c->Message(Chat::White, "Usage: #npcedit bracertexture [Texture] - Sets an NPC's Bracer Texture");
	c->Message(Chat::White, "Usage: #npcedit handtexture [Texture] - Sets an NPC's Hand Texture");
	c->Message(Chat::White, "Usage: #npcedit legtexture [Texture] - Sets an NPC's Leg Texture");
	c->Message(Chat::White, "Usage: #npcedit feettexture [Texture] - Sets an NPC's Feet Texture");
	c->Message(Chat::White, "Usage: #npcedit size [Size] - Sets an NPC's Size");
	c->Message(Chat::White, "Usage: #npcedit hpregen [HP Regen] - Sets an NPC's HP Regen Rate Per Tick");
	c->Message(Chat::White, "Usage: #npcedit combathpregen [Combat HP Regen] - Sets an NPC's Combat HP Regen Rate Per Tick");
	c->Message(Chat::White, "Usage: #npcedit manaregen [Mana Regen] - Sets an NPC's Mana Regen Rate Per Tick");
	c->Message(Chat::White, "Usage: #npcedit combatmanaregen [Combat Mana Regen] - Sets an NPC's Combat Mana Regen Rate Per Tick");
	c->Message(Chat::White, "Usage: #npcedit loottable [Loottable ID] - Sets an NPC's Loottable ID");
	c->Message(Chat::White, "Usage: #npcedit merchantid [Merchant ID] - Sets an NPC's Merchant ID");
	c->Message(Chat::White, "Usage: #npcedit spell [Spell List ID] - Sets an NPC's Spells List ID");
	c->Message(
		Chat::White,
		"Usage: #npcedit npc_spells_effects_id [Spell Effects ID] - Sets an NPC's Spell Effects ID"
	);
	c->Message(Chat::White, "Usage: #npcedit faction [Faction ID] - Sets an NPC's Faction ID");
	c->Message(Chat::White, "Usage: #npcedit special_abilities [Special Abilities] - Sets an NPC's Special Abilities");
	c->Message(Chat::White, "Usage: #npcedit damage [Minimum] [Maximum] - Sets an NPC's Damage");
	c->Message(Chat::White, "Usage: #npcedit aggroradius [Radius] - Sets an NPC's Aggro Radius");
	c->Message(Chat::White, "Usage: #npcedit assistradius [Radius] - Sets an NPC's Assist Radius");
	c->Message(Chat::White, "Usage: #npcedit runspeed [Run Speed] - Sets an NPC's Run Speed");
	c->Message(Chat::White, "Usage: #npcedit agi [Agility] - Sets an NPC's Agility");
	c->Message(Chat::White, "Usage: #npcedit cha [Charisma] - Sets an NPC's Charisma");
	c->Message(Chat::White, "Usage: #npcedit dex [Dexterity] - Sets an NPC's Dexterity");;
	c->Message(Chat::White, "Usage: #npcedit int [Intelligence] - Sets an NPC's Intelligence");
	c->Message(Chat::White, "Usage: #npcedit sta [Stamina] - Sets an NPC's Stamina");
	c->Message(Chat::White, "Usage: #npcedit str [Strength] - Sets an NPC's Strength");
	c->Message(Chat::White, "Usage: #npcedit wis {Widsom] - Sets an NPC's Wisdom");
	c->Message(Chat::White, "Usage: #npcedit mr [Resistance] - Sets an NPC's Magic Resistance");
	c->Message(Chat::White, "Usage: #npcedit pr [Resistance] - Sets an NPC's Poison Resistance");
	c->Message(Chat::White, "Usage: #npcedit dr [Resistance] - Sets an NPC's Disease Resistance");
	c->Message(Chat::White, "Usage: #npcedit fr [Resistance] - Sets an NPC's Fire Resistance");
	c->Message(Chat::White, "Usage: #npcedit cr [Resistance] - Sets an NPC's Cold Resistance");
	c->Message(
		Chat::White,
		"Usage: #npcedit seeinvis [Flag] - Sets an NPC's See Invisible Flag [0 = Cannot See Invisible, 1 = Can See Invisible]"
	);
	c->Message(
		Chat::White,
		"Usage: #npcedit seeinvisundead [Flag] - Sets an NPC's See Invisible vs. Undead Flag  [0 = Cannot See Invisible vs. Undead, 1 = Can See Invisible vs. Undead]"
	);
	c->Message(
		Chat::White, 
		"Usage: #npcedit seesneak [Flag] - Sets an NPC's ability to see through sneak Flag [0 = Cannot See Sneak, 1 = Can See Sneak] ");
	c->Message(
		Chat::White,
		"Usage: #npcedit seeimprovedhide [Flag] - Sets an NPC's See Improved Hide Flag [0 = Cannot See Improved Hide, 1 = Can See Improved Hide]"
	);
	c->Message(Chat::White, "Usage: #npcedit ac [Armor Class] - Sets an NPC's Armor Class");
	c->Message(Chat::White, "Usage: #npcedit atk [Attack] - Sets an NPC's Attack");
	c->Message(Chat::White, "Usage: #npcedit accuracy [Accuracy] - Sets an NPC's Accuracy");
	c->Message(
		Chat::White,
		"Usage: #npcedit npcaggro [Flag] - Sets an NPC's NPC Aggro Flag [0 = Aggro NPCs Off, 1 = Aggro NPCs On]"
	);
	c->Message(
		Chat::White,
		"Usage: #npcedit qglobal [Flag] - Sets an NPC's Quest Global Flag [0 = Quest Globals Off, 1 = Quest Globals On]"
	);
	c->Message(Chat::White, "Usage: #npcedit spawn_limit [Limit] - Sets an NPC's Spawn Limit Counter");
	c->Message(Chat::White, "Usage: #npcedit attackdelay [Attack Delay] - Sets an NPC's Attack Delay");
	c->Message(
		Chat::White,
		"Usage: #npcedit weapon [Primary Model] [Secondary Model] - Sets an NPC's Primary and Secondary Weapon Model"
	);
	c->Message(Chat::White, "Usage: #npcedit featuresave - Saves an NPC's current facial features to the database");
	c->Message(
		Chat::White,
		"Usage: #npcedit color [Red] [Green] [Blue] - Sets an NPC's Red, Green, and Blue armor tint"
	);
	c->Message(Chat::White, "Usage: #npcedit armortint_id [Armor Tint ID] - Sets an NPC's Armor Tint ID");
	c->Message(
		Chat::White,
		"Usage: #npcedit setanimation [Animation ID] - Sets an NPC's Animation on Spawn (Stored in spawn2 table)"
	);
	c->Message(
		Chat::White,
		"Usage: #npcedit scalerate [Scale Rate] - Sets an NPC's Scaling Rate [50 = 50%, 100 = 100%, 200 = 200%]"
	);
	c->Message(
		Chat::White,
		"Usage: #npcedit spellscale [Scale Rate] - Sets an NPC's Spell Scaling Rate [50 = 50%, 100 = 100%, 200 = 200%]"
	);
	c->Message(
		Chat::White,
		"Usage: #npcedit healscale [Scale Rate] - Sets an NPC's Heal Scaling Rate [50 = 50%, 100 = 100%, 200 = 200%]"
	);
}