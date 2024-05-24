#include "../client.h"

void command_setfaction(Client *c, const Seperator *sep){
	if ((sep->arg[1][0] == 0 || strcasecmp(sep->arg[1], "*") == 0) || ((c->GetTarget() == 0) || (c->GetTarget()->IsClient()))) {
		c->Message(CC_Default, "Usage: #setfaction [faction number]");
		return;
	}
	
	uint32 faction_id = atoi(sep->argplus[1]);
	auto npcTypeID = c->GetTarget()->CastToNPC()->GetNPCTypeID();
	c->Message(CC_Yellow, "Setting NPC %u to faction %i", npcTypeID, faction_id);
	
	std::string query = StringFormat("UPDATE npc_types SET npc_faction_id = %i WHERE id = %i",
		faction_id, npcTypeID);
	database.QueryDatabase(query);
}

