#include "../client.h"

void command_fillbuffs(Client *c, const Seperator *sep)
{
	Client *t;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();
	else
		t = c;

	t->BuffFadeAll();
	c->SpellFinished(3295, t);
	c->SpellFinished(278, t);
	c->SpellFinished(3441, t);
	c->SpellFinished(3451, t);
	c->SpellFinished(2519, t);
	c->SpellFinished(226, t);
	c->SpellFinished(227, t);
	c->SpellFinished(228, t);
	c->SpellFinished(3360, t);
	c->SpellFinished(80, t);
	c->SpellFinished(86, t);
	c->SpellFinished(3439, t);
	c->SpellFinished(3450, t);
	c->SpellFinished(3453, t);
	c->SpellFinished(1709, t);

}

