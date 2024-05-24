#include "../client.h"

void command_hatelist(Client *c, const Seperator *sep){
	Mob *target = c->GetTarget();
	if (target == nullptr) {
		c->Message(CC_Default, "Error: you must have a target.");
		return;
	}

	c->Message(CC_Default, "Display hate list for %s..", target->GetName());
	target->PrintHateListToClient(c);
}

