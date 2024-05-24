#include "../client.h"

void command_npcspawn(Client *c, const Seperator *sep){
	int arguments = sep->argnum;
	if (!arguments) {
		c->Message(CC_Default, "Command Syntax: #npcspawn [Add|Create|Delete|Remove|Update]");
		return;
	}

	if (!(c->GetTarget() && c->GetTarget()->IsNPC())) {
		c->Message(CC_Default, "You must target an NPC to use this command.");
		return;
	}

	NPC* target = c->GetTarget()->CastToNPC();
	std::string spawn_type = Strings::ToLower(sep->arg[1]);
	uint32 extra = 0;
	bool is_add = spawn_type.find("add") != std::string::npos;
	bool is_create = spawn_type.find("create") != std::string::npos;
	bool is_delete = spawn_type.find("delete") != std::string::npos;
	bool is_remove = spawn_type.find("remove") != std::string::npos;
	bool is_update = spawn_type.find("update") != std::string::npos;
	if (!is_add && !is_create && !is_delete && !is_remove && !is_update) {
		c->Message(CC_Default, "Command Syntax: #npcspawn [Add|Create|Delete|Remove|Update]");
		return;
	}

	if (is_add || is_create) {
		extra = (sep->IsNumber(2) ? (is_add ? std::stoi(sep->arg[2]) : 1) : (is_add ? 1200 : 0)); // Default to 1200 for Add, 0 for Create if not set
		database.NPCSpawnDB(is_add ? NPCSpawnTypes::AddNewSpawngroup : NPCSpawnTypes::CreateNewSpawn, zone->GetShortName(), c, target, extra);
		c->Message(CC_Default, fmt::format("Spawn {} | Name: {} ({})", is_add ? "Added" : "Created", target->GetCleanName(), target->GetID()).c_str());
	}
	else if (is_delete || is_remove || is_update) {
		uint8 spawn_update_type = (is_delete ? NPCSpawnTypes::DeleteSpawn : (is_remove ? NPCSpawnTypes::RemoveSpawn : NPCSpawnTypes::UpdateAppearance));
		std::string spawn_message = (is_delete ? "Deleted" : (is_remove ? "Removed" : "Updated"));
		database.NPCSpawnDB(spawn_update_type, zone->GetShortName(), c, target);
		c->Message(CC_Default, fmt::format("Spawn {} | Name: {} ({})", spawn_message, target->GetCleanName(), target->GetID()).c_str());
		if (is_delete || is_remove)
			target->Depop(false);
	}
}

