#include "../../client.h"

void ShowNPCGlobalLoot(Client* c, const Seperator* sep)
{
	if (!c->GetTarget() || !c->GetTarget()->IsNPC()) {
		c->Message(0, "You must target an NPC to use this command.");
		return;
	}

	auto target = c->GetTarget()->CastToNPC();

	c->Message(
		Chat::White,
		fmt::format(
			"Global loot for {} ({}).",
			target->GetCleanName(),
			target->GetNPCTypeID()
		).c_str()
	);

	zone->ShowNPCGlobalLoot(c, target);
}

