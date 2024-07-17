#include "../client.h"

void command_showspellslist(Client *c, const Seperator *sep){
	Mob *target = c->GetTarget();

	if (!target) {
		c->Message(Chat::White, "Must target an NPC.");
		return;
	}

	if (!target->IsNPC()) {
		c->Message(Chat::White, "%s is not an NPC.", target->GetName());
		return;
	}

	target->CastToNPC()->AISpellsList(c);

	return;
}

