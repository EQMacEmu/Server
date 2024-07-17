#include "../client.h"

void command_setaapts(Client *c, const Seperator *sep){
	Client *t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();

	if (sep->arg[1][0] == '\0')
		c->Message(Chat::White, "Usage: #setaapts <new AA points value>");
	else if (atoi(sep->arg[1]) <= 0 || atoi(sep->arg[1]) > 170)
		c->Message(Chat::White, "You must have a number greater than 0 for points and no more than 170.");
	else {
		t->SetEXP(t->GetEXP(), t->GetEXPForLevel(t->GetLevel(), true)*atoi(sep->arg[1]), false);
		t->SendAAStats();
		t->SendAATable();
	}
}

