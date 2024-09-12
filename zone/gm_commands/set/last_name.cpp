#include "../../client.h"

void SetLastName(Client *c, const Seperator *sep)
{
	Client *t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t=c->GetTarget()->CastToClient();
	Log(Logs::General, Logs::Normal, "#lastname request from %s for %s", c->GetName(), t->GetName());

	if (strlen(sep->arg[2]) <= 70)
		t->ChangeLastName(sep->arg[2]);
	else
		c->Message(Chat::White, "Usage: #lastname <lastname> where <lastname> is less than 70 chars long");
}

