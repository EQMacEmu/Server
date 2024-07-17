#include "../client.h"

void command_starve(Client *c, const Seperator *sep){

	Client *t;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();
	else
		t = c;

	t->SetConsumption(0,0);
	t->SendStaminaUpdate();
	c->Message(Chat::White, "Target starved.");

}

