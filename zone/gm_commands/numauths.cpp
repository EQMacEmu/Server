#include "../client.h"

void command_numauths(Client *c, const Seperator *sep){
	c->Message(CC_Default, "NumAuths: %i", zone->CountAuth());
	c->Message(CC_Default, "Your WID: %i", c->GetWID());
}

