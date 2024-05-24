#include "../client.h"

void command_npcstats(Client *c, const Seperator *sep){
	if (c->GetTarget() == 0)
		c->Message(CC_Default, "ERROR: No target!");
	else if (!c->GetTarget()->IsNPC())
		c->Message(CC_Default, "ERROR: Target is not a NPC!");
	else {
		c->GetTarget()->CastToNPC()->ShowQuickStats(c);
	}
}

