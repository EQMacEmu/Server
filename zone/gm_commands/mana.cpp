#include "../client.h"

void command_mana(Client *c, const Seperator *sep)
{
	auto target = c->GetTarget() ? c->GetTarget() : c;
	if (target->IsClient()) {
		target->CastToClient()->SetMana(target->CastToClient()->CalcMaxMana());
	}
	else {
		target->SetMana(target->CalcMaxMana());
	}

	if (c != target) {
		c->Message(
			CC_Default,
			fmt::format(
				"Set {} ({}) to full Mana.",
				target->GetCleanName(),
				target->GetID()
			).c_str()
		);
	}
	else {
		c->Message(CC_Default, "Restored your Mana to full.");
	}
}

