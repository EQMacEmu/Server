#include "../client.h"

void command_distance(Client *c, const Seperator *sep){
	if (c && c->GetTarget()) {
		Mob* target = c->GetTarget();

		c->Message(Chat::White, "Your target, %s, is %1.1f units from you.", c->GetTarget()->GetName(), Distance(c->GetPosition(), target->GetPosition()));
	}
}

