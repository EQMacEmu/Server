#include "../client.h"

void command_serversidename(Client *c, const Seperator *sep){
	if (c->GetTarget())
		c->Message(CC_Default, c->GetTarget()->GetName());
	else
		c->Message(CC_Default, "Error: no target");
}

