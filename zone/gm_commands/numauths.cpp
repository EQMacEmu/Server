#include "../client.h"

void command_numauths(Client *c, const Seperator *sep){
	c->Message(Chat::White, "NumAuths: %i", zone->CountAuth());
	c->Message(Chat::White, "Your WID: %i", c->GetWID());
}

