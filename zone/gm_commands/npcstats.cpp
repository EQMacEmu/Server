#include "../client.h"

void command_npcstats(Client *c, const Seperator *sep){
	if (c->GetTarget() == 0)
		c->Message(Chat::White, "ERROR: No target!");
	else if (!c->GetTarget()->IsNPC())
		c->Message(Chat::White, "ERROR: Target is not a NPC!");
	else {
		c->GetTarget()->CastToNPC()->ShowQuickStats(c);
	}
}

