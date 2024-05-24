#include "../client.h"

void command_reloadtraps(Client *c, const Seperator *sep)
{
	entity_list.UpdateAllTraps(true, true);
	c->Message(CC_Default, "Traps reloaded for %s.", zone->GetShortName());
}

