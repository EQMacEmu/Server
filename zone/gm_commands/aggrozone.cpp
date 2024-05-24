#include "../client.h"

void command_aggrozone(Client *c, const Seperator *sep){
	if (!c)
		return;

	Mob *m = c->GetTarget() ? c->GetTarget() : c->CastToMob();

	if (!m)
		return;

	int hate = atoi(sep->arg[1]); //should default to 0 if we don't enter anything
	bool use_ignore_dist = false;
	if(sep->IsNumber(2))
		use_ignore_dist = atoi(sep->arg[2]);
	entity_list.AggroZone(m, hate, use_ignore_dist);
	if (!c->GetTarget())
		c->Message(CC_Default, "Train to you! Last chance to go invulnerable...");
	else
		c->Message(CC_Default, "Train to %s! Watch them die!!!", c->GetTarget()->GetCleanName());
}

