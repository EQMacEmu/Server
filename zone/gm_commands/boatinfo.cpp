#include "../client.h"

void command_boatinfo(Client *c, const Seperator *sep)
{
	if(!zone->IsBoatZone())
	{
		c->Message(CC_Default, "Zone is not a boat zone!");
	}
	else
	{
		entity_list.GetBoatInfo(c);
	}
}

