#include "../client.h"

void command_showzonegloballoot(Client *c, const Seperator *sep)
{
	c->Message(
		CC_Default,
		fmt::format(
			"Global loot for {} ({}).",
			zone->GetLongName(),
			zone->GetZoneID()
		).c_str()
	);
	zone->ShowZoneGlobalLoot(c);
}

