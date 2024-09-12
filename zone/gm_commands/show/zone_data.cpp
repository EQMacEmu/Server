#include "../../client.h"

void ShowZoneData(Client* c, const Seperator* sep)
{
	c->Message(Chat::White, "Zone Header Data:");
	c->Message(Chat::White, "Sky Type: %i", zone->newzone_data.sky);
	c->Message(Chat::White, "Fog Colour: Red: %i; Blue: %i; Green %i", zone->newzone_data.fog_red[0], zone->newzone_data.fog_green[0], zone->newzone_data.fog_blue[0]);
	c->Message(Chat::White, "Safe Coords: %f, %f, %f", zone->newzone_data.safe_x, zone->newzone_data.safe_y, zone->newzone_data.safe_z);
	c->Message(Chat::White, "Underworld Coords: %f", zone->newzone_data.underworld);
	c->Message(Chat::White, "Clip Plane: %f - %f", zone->newzone_data.minclip, zone->newzone_data.maxclip);
}
