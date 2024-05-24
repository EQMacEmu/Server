#include "../client.h"
#include "../groups.h"
#include "../raids.h"

void command_setaaxp(Client *c, const Seperator *sep){
	Client *t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();

	if (sep->IsNumber(1)) {
		t->SetEXP(t->GetEXP(), atoi(sep->arg[1]), false);
	}
	else
		c->Message(CC_Default, "Usage: #setaaxp <new AA XP value> (<new Group AA XP value> <new Raid XP value>)");
}

