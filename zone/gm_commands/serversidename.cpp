#include "../client.h"

void command_serversidename(Client *c, const Seperator *sep){
	if (c->GetTarget())
		c->Message(Chat::White, c->GetTarget()->GetName());
	else
		c->Message(Chat::White, "Error: no target");
}

