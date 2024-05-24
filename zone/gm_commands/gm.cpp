#include "../client.h"

void command_gm(Client *c, const Seperator *sep){
	bool state = atobool(sep->arg[1]);
	Client *t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();

	if (sep->arg[1][0] != 0) {
		t->SetGM(state);
		c->Message(CC_Default, "%s is %s a GM.", t->GetName(), state ? "now" : "no longer");
	}
	else
		c->Message(CC_Default, "Usage: #gm [on/off]");
}

