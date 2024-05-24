#include "../client.h"

void command_manastat(Client *c, const Seperator *sep){
	Mob *target = c->GetTarget() ? c->GetTarget() : c;

	c->Message(CC_Default, "Mana for %s:", target->GetName());
	c->Message(CC_Default, "  Current Mana: %d", target->GetMana());
	c->Message(CC_Default, "  Max Mana: %d", target->GetMaxMana());
}

