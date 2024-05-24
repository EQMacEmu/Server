#include "../client.h"

void command_showtraderitems(Client *c, const Seperator *sep)
{
	Client *t;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();
	else
		t = c;

	c->DisplayTraderInventory(t);
}

