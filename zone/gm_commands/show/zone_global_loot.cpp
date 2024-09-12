#include "../../client.h"

void ShowZoneGlobalLoot(Client* c, const Seperator* sep)
{
	c->Message(
		Chat::White,
		fmt::format(
			"Global loot for {} ({}).",
			zone->GetLongName(),
			zone->GetZoneID()
		).c_str()
	);
	zone->ShowZoneGlobalLoot(c);
}

