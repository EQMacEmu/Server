#include "../client.h"

void command_attackentity(Client *c, const Seperator *sep) {
	if (c->GetTarget() && c->GetTarget()->IsNPC() && atoi(sep->arg[1]) > 0) {
		Mob* sictar = entity_list.GetMob(atoi(sep->arg[1]));
		if (sictar)
			c->GetTarget()->CastToNPC()->AddToHateList(sictar, 1, 0);
		else
			c->Message(CC_Default, "Error: Entity %d not found", atoi(sep->arg[1]));
	}
	else
		c->Message(CC_Default, "Usage: (needs NPC targeted) #attackentity entityid");
}

