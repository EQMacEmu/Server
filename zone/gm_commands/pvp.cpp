#include "../client.h"

void command_pvp(Client *c, const Seperator *sep){
	bool state = atobool(sep->arg[1]);
	Client *t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();

	if (sep->arg[1][0] != 0) {
		t->SetPVP(state);
		c->Message(CC_Default, "%s now follows the ways of %s.", t->GetName(), state ? "discord" : "order");
	}
	else
		c->Message(CC_Default, "Usage: #pvp [on/off]");
}

