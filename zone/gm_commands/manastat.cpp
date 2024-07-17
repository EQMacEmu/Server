#include "../client.h"

void command_manastat(Client *c, const Seperator *sep){
	Mob *target = c->GetTarget() ? c->GetTarget() : c;

	c->Message(Chat::White, "Mana for %s:", target->GetName());
	c->Message(Chat::White, "  Current Mana: %d", target->GetMana());
	c->Message(Chat::White, "  Max Mana: %d", target->GetMaxMana());
}

