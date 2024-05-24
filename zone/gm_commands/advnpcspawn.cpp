#include "../client.h"
#include "../groups.h"

void command_advnpcspawn(Client* c, const Seperator* sep)
{
	int arguments = sep->argnum;
	if (!arguments) {
		c->Message(CC_Default, "Usage: #advnpcspawn addentry [Spawngroup ID] [NPC ID] [Spawn Chance] - Adds a new Spawngroup Entry");
		c->Message(CC_Default, "Usage: #advnpcspawn addspawn [Spawngroup ID] - Adds a new Spawngroup Entry from an existing Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn clearbox [Spawngroup ID] - Clears the roambox of a Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn deletespawn - Deletes a Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn editbox [Spawngroup ID] [Minimum X] [Maximum X] [Minimum Y] [Maximum Y] [Delay]  - Edit the roambox of a Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn editrespawn [Respawn Timer] [Variance] - Edit the Respawn Timer of a Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn makegroup [Spawn Group Name] [Spawn Limit] [Minimum X] [Maximum X] [Minimum Y] [Maximum Y] [Delay] - Makes a new Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn makenpc - Makes a new NPC");
		c->Message(CC_Default, "Usage: #advnpcspawn movespawn - Moves a Spawngroup to your current location");
		return;
	}

	std::string spawn_command = Strings::ToLower(sep->arg[1]);
	bool is_add_entry = spawn_command.find("addentry") != std::string::npos;
	bool is_add_spawn = spawn_command.find("addspawn") != std::string::npos;
	bool is_clear_box = spawn_command.find("clearbox") != std::string::npos;
	bool is_delete_spawn = spawn_command.find("deletespawn") != std::string::npos;
	bool is_edit_box = spawn_command.find("editgroup") != std::string::npos;
	bool is_edit_respawn = spawn_command.find("editrespawn") != std::string::npos;
	bool is_make_group = spawn_command.find("makegroup") != std::string::npos;
	bool is_make_npc = spawn_command.find("makenpc") != std::string::npos;
	bool is_move_spawn = spawn_command.find("movespawn") != std::string::npos;
	if (
		!is_add_entry &&
		!is_add_spawn &&
		!is_clear_box &&
		!is_delete_spawn &&
		!is_edit_box &&
		!is_edit_respawn &&
		!is_make_group &&
		!is_make_npc &&
		!is_move_spawn
		) {
		c->Message(CC_Default, "Usage: #advnpcspawn addentry [Spawngroup ID] [NPC ID] [Spawn Chance] - Adds a new Spawngroup Entry");
		c->Message(CC_Default, "Usage: #advnpcspawn addspawn [Spawngroup ID] - Adds a new Spawngroup Entry from an existing Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn clearbox [Spawngroup ID] - Clears the roambox of a Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn deletespawn - Deletes a Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn editbox [Spawngroup ID] [Minimum X] [Maximum X] [Minimum Y] [Maximum Y] [Delay]  - Edit the roambox of a Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn editrespawn [Respawn Timer] [Variance] - Edit the Respawn Timer of a Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn makegroup [Spawn Group Name] [Spawn Limit] [Minimum X] [Maximum X] [Minimum Y] [Maximum Y] [Delay] - Makes a new Spawngroup");
		c->Message(CC_Default, "Usage: #advnpcspawn makenpc - Makes a new NPC");
		c->Message(CC_Default, "Usage: #advnpcspawn movespawn - Moves a Spawngroup to your current location");
		return;
	}


	if (is_add_entry) {
		if (arguments < 4) {
			c->Message(CC_Default, "Usage: #advnpcspawn addentry [Spawngroup ID] [NPC ID] [Spawn Chance]");
			return;
		}

		auto spawngroup_id = std::stoi(sep->arg[2]);
		auto npc_id = std::stoi(sep->arg[2]);
		auto spawn_chance = std::stoi(sep->arg[2]);

		std::string query = fmt::format(
			SQL(
				INSERT INTO spawnentry(spawngroupID, npcID, chance)
				VALUES({}, {}, {})
			),
			spawngroup_id,
			npc_id,
			spawn_chance
		);
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(CC_Default, "Failed to add entry to Spawngroup.");
			return;
		}

		c->Message(
			CC_Default,
			fmt::format(
				"({}) added to Spawngroup {}, its spawn chance is {}%%.",
				npc_id,
				spawngroup_id,
				spawn_chance
			).c_str()
		);
		return;
	}
	else if (is_add_spawn) {
		database.NPCSpawnDB(
			NPCSpawnTypes::AddSpawnFromSpawngroup,
			zone->GetShortName(),
			c,
			0,
			std::stoi(sep->arg[2])
		);
		c->Message(
			CC_Default,
			fmt::format(
				"Spawn Added | Added spawn from Spawngroup ID {}.",
				std::stoi(sep->arg[2])
			).c_str()
		);
		return;
	}
	else if (is_clear_box) {
		if (arguments != 2 || !sep->IsNumber(2)) {
			c->Message(CC_Default, "Usage: #advnpcspawn clearbox [Spawngroup ID]");
			return;
		}

		std::string query = fmt::format(
			"UPDATE spawngroup SET min_x = 0, max_x = 0, min_y = 0, max_y = 0, delay = 0 WHERE id = {}",
			std::stoi(sep->arg[2])
		);
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(CC_Default, "Failed to clear Spawngroup box.");
			return;
		}

		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Roambox Cleared | Delay: 0 Distance: 0.00",
				std::stoi(sep->arg[2])
			).c_str()
		);
		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Roambox Cleared | Minimum X: 0.00 Maximum X: 0.00",
				std::stoi(sep->arg[2])
			).c_str()
		);
		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Roambox Cleared | Minimum Y: 0.00 Maximum Y: 0.00",
				std::stoi(sep->arg[2])
			).c_str()
		);
		return;
	}
	else if (is_delete_spawn) {
		if (!c->GetTarget() || !c->GetTarget()->IsNPC()) {
			c->Message(CC_Default, "You must target an NPC to use this command.");
			return;
		}

		NPC* target = c->GetTarget()->CastToNPC();
		Spawn2* spawn2 = target->respawn2;
		if (!spawn2) {
			c->Message(CC_Default, "Failed to delete spawn because NPC has no Spawn2.");
			return;
		}

		auto spawn2_id = spawn2->GetID();
		std::string query = fmt::format(
			"DELETE FROM spawn2 WHERE id = {}",
			spawn2_id
		);
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(CC_Default, "Failed to delete spawn.");
			return;
		}

		c->Message(
			CC_Default,
			fmt::format(
				"Spawn2 {} Deleted | Name: {} ({})",
				spawn2_id,
				target->GetCleanName(),
				target->GetID()
			).c_str()
		);
		target->Depop(false);
		return;
	}
	else if (is_edit_box) {
		if (
			arguments != 7 ||
			!sep->IsNumber(3) ||
			!sep->IsNumber(4) ||
			!sep->IsNumber(5) ||
			!sep->IsNumber(6) ||
			!sep->IsNumber(7)
			) {
			c->Message(CC_Default, "Usage: #advnpcspawn editbox [Spawngroup ID] [Minimum X] [Maximum X] [Minimum Y] [Maximum Y] [Delay]");
			return;
		}
		auto spawngroup_id = std::stoi(sep->arg[2]);
		auto minimum_x = std::stof(sep->arg[3]);
		auto maximum_x = std::stof(sep->arg[4]);
		auto minimum_y = std::stof(sep->arg[5]);
		auto maximum_y = std::stof(sep->arg[6]);
		auto delay = std::stoi(sep->arg[7]);

		std::string query = fmt::format(
			"UPDATE spawngroup SET min_x = {:.2f}, max_x = {:.2f}, max_y = {:.2f}, min_y = {:.2f}, delay = {} WHERE id = {}",
			minimum_x,
			maximum_x,
			minimum_y,
			maximum_y,
			delay,
			spawngroup_id
		);
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(CC_Default, "Failed to edit Spawngroup box.");
			return;
		}

		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Roambox Edited | Delay: {}",
				spawngroup_id,
				delay
			).c_str()
		);
		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Roambox Edited | Minimum X: {:.2f} Maximum X: {:.2f}",
				spawngroup_id,
				minimum_x,
				maximum_x
			).c_str()
		);
		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Roambox Edited | Minimum Y: {:.2f} Maximum Y: {:.2f}",
				spawngroup_id,
				minimum_y,
				maximum_y
			).c_str()
		);
		return;
	}
	else if (is_edit_respawn) {
		if (arguments < 2 || !sep->IsNumber(2)) {
			c->Message(CC_Default, "Usage: #advnpcspawn editrespawn [Respawn Timer] [Variance]");
			return;
		}

		if (!c->GetTarget() || !c->GetTarget()->IsNPC()) {
			c->Message(CC_Default, "You must target an NPC to use this command.");
			return;
		}

		NPC* target = c->GetTarget()->CastToNPC();
		Spawn2* spawn2 = target->respawn2;
		if (!spawn2) {
			c->Message(CC_Default, "Failed to edit respawn because NPC has no Spawn2.");
			return;
		}

		auto spawn2_id = spawn2->GetID();
		uint32 respawn_timer = std::stoi(sep->arg[2]);
		uint32 variance = (
			sep->IsNumber(3) ?
			std::stoi(sep->arg[3]) :
			spawn2->GetVariance()
			);
		std::string query = fmt::format(
			"UPDATE spawn2 SET respawntime = {}, variance = {} WHERE id = {}",
			respawn_timer,
			variance,
			spawn2_id
		);
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(CC_Default, "Failed to edit respawn.");
			return;
		}

		c->Message(
			CC_Default,
			fmt::format(
				"Spawn2 {} Respawn Modified | Name: {} ({}) Respawn Timer: {} Variance: {}",
				spawn2_id,
				target->GetCleanName(),
				target->GetID(),
				respawn_timer,
				variance
			).c_str()
		);
		spawn2->SetRespawnTimer(respawn_timer);
		spawn2->SetVariance(variance);
		return;
	}
	else if (is_make_group) {
		if (
			arguments != 8 ||
			!sep->IsNumber(3) ||
			!sep->IsNumber(4) ||
			!sep->IsNumber(5) ||
			!sep->IsNumber(6) ||
			!sep->IsNumber(7) ||
			!sep->IsNumber(8)
			) {
			c->Message(CC_Default, "Usage: #advncspawn makegroup [Spawn Group Name] [Spawn Limit] [Minimum X] [Maximum X] [Minimum Y] [Maximum Y] [Delay]");
			return;
		}
		std::string spawngroup_name = sep->arg[2];
		auto spawn_limit = std::stoi(sep->arg[3]);
		auto minimum_x = std::stof(sep->arg[4]);
		auto maximum_x = std::stof(sep->arg[5]);
		auto minimum_y = std::stof(sep->arg[6]);
		auto maximum_y = std::stof(sep->arg[7]);
		auto delay = std::stoi(sep->arg[8]);

		std::string query = fmt::format(
			"INSERT INTO spawngroup"
			"(name, spawn_limit, min_x, max_x, min_y, max_y, delay)"
			"VALUES ('{}', {}, {:.2f}, {:.2f}, {:.2f}, {:.2f}, {})",
			spawngroup_name,
			spawn_limit,
			minimum_x,
			maximum_x,
			minimum_y,
			maximum_y,
			delay
		);
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(CC_Default, "Failed to make Spawngroup.");
			return;
		}

		auto spawngroup_id = results.LastInsertedID();
		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Created | Name: {} Spawn Limit: {}",
				spawngroup_id,
				spawngroup_name,
				spawn_limit
			).c_str()
		);
		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Created | Delay: {}",
				spawngroup_id,
				delay
			).c_str()
		);
		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Created | Minimum X: {:.2f} Maximum X: {:.2f}",
				spawngroup_id,
				minimum_x,
				maximum_x
			).c_str()
		);
		c->Message(
			CC_Default,
			fmt::format(
				"Spawngroup {} Created | Minimum Y: {:.2f} Maximum Y: {:.2f}",
				spawngroup_id,
				minimum_y,
				maximum_y
			).c_str()
		);
		return;
	}
	else if (is_make_npc) {
		if (!c->GetTarget() || !c->GetTarget()->IsNPC()) {
			c->Message(CC_Default, "You must target an NPC to use this command.");
			return;
		}

		NPC* target = c->GetTarget()->CastToNPC();
		database.NPCSpawnDB(
			NPCSpawnTypes::CreateNewNPC,
			zone->GetShortName(),
			c,
			target
		);
		return;
	}
	else if (is_move_spawn) {
		if (!c->GetTarget() || !c->GetTarget()->IsNPC()) {
			c->Message(CC_Default, "You must target an NPC to use this command.");
			return;
		}

		NPC* target = c->GetTarget()->CastToNPC();
		Spawn2* spawn2 = target->respawn2;
		if (!spawn2) {
			c->Message(CC_Default, "Failed to move spawn because NPC has no Spawn2.");
			return;
		}

		auto client_position = c->GetPosition();
		auto spawn2_id = spawn2->GetID();
		std::string query = fmt::format(
			"UPDATE spawn2 SET x = {:.2f}, y = {:.2f}, z = {:.2f}, heading = {:.2f} WHERE id = {}",
			client_position.x,
			client_position.y,
			client_position.z,
			client_position.w,
			spawn2_id
		);
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(CC_Default, "Failed to move spawn.");
			return;
		}

		c->Message(
			CC_Default,
			fmt::format(
				"Spawn2 {} Moved | Name: {} ({})",
				spawn2_id,
				target->GetCleanName(),
				target->GetID()
			).c_str()
		);
		c->Message(
			CC_Default,
			fmt::format(
				"Spawn2 {} Moved | XYZ: {}, {}, {} Heading: {}",
				spawn2_id,
				client_position.x,
				client_position.y,
				client_position.z,
				client_position.w
			).c_str()
		);
		target->GMMove(
			client_position.x,
			client_position.y,
			client_position.z,
			client_position.w
		);
		return;
	}
}

