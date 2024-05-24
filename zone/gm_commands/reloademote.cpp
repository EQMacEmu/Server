#include "../client.h"

void command_reloademote(Client *c, const Seperator *sep){
	zone->LoadNPCEmotes(&zone->NPCEmoteList);
	c->Message(CC_Default, "NPC emotes reloaded.");
}

