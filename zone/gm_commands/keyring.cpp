#include "../client.h"

void command_keyring(Client *c, const Seperator *sep)
{
	Client *t;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();
	else
		t = c;

	t->KeyRingList(c);
}

