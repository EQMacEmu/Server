#include "../client.h"

void command_heal(Client *c, const Seperator *sep)
{
	auto target = c->GetTarget() ? c->GetTarget() : c;
	target->Heal();
	if (c != target) {
		c->Message(
			CC_Default,
			fmt::format(
				"Healed {} ({}) to full.",
				target->GetCleanName(),
				target->GetID()
			).c_str()
		);
	}
	else {
		c->Message(CC_Default, "Healed yourself to full.");
	}
}

