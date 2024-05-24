#include "../client.h"

void command_showspellslist(Client *c, const Seperator *sep){
	Mob *target = c->GetTarget();

	if (!target) {
		c->Message(CC_Default, "Must target an NPC.");
		return;
	}

	if (!target->IsNPC()) {
		c->Message(CC_Default, "%s is not an NPC.", target->GetName());
		return;
	}

	target->CastToNPC()->AISpellsList(c);

	return;
}

