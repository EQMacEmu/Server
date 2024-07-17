#include "../client.h"

void command_zonespawn(Client *c, const Seperator *sep){
	c->Message(Chat::White, "This command is not yet implemented.");
	return;

	/* this was kept from client.cpp verbatim (it was commented out) */
	//	if (target && target->IsNPC()) {
	//		Message(Chat::White, "Inside main if.");
	//		if (strcasecmp(sep->arg[1], "add")==0) {
	//			Message(Chat::White, "Inside add if.");
	//			database.DBSpawn(1, StaticGetZoneName(this->GetPP().current_zone), target->CastToNPC());
	//		}
	//		else if (strcasecmp(sep->arg[1], "update")==0) {
	//			database.DBSpawn(2, StaticGetZoneName(this->GetPP().current_zone), target->CastToNPC());
	//		}
	//		else if (strcasecmp(sep->arg[1], "remove")==0) {
	//			if (strcasecmp(sep->arg[2], "all")==0) {
	//				database.DBSpawn(4, StaticGetZoneName(this->GetPP().current_zone));
	//			}
	//			else {
	//				if (database.DBSpawn(3, StaticGetZoneName(this->GetPP().current_zone), target->CastToNPC())) {
	//					Message(Chat::White, "#zonespawn: %s removed successfully!", target->GetName());
	//					target->CastToNPC()->Death(target, target->GetHP());
	//				}
	//			}
	//		}
	//		else
	//			Message(Chat::White, "Error: #dbspawn: Invalid command. (Note: EDIT and REMOVE are NOT in yet.)");
	//		if (target->CastToNPC()->GetNPCTypeID() > 0) {
	//			Message(Chat::White, "Spawn is type %i", target->CastToNPC()->GetNPCTypeID());
	//		}
	//	}
	//	else if(!target || !target->IsNPC())
	//		Message(Chat::White, "Error: #zonespawn: You must have a NPC targeted!");
	//	else
	//		Message(Chat::White, "Usage: #zonespawn [add|edit|remove|remove all]");
}

