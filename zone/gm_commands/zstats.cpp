#include "../client.h"

void command_zstats(Client *c, const Seperator *sep){
	c->Message(CC_Default, "Zone Header Data:");
	c->Message(CC_Default, "Sky Type: %i", zone->newzone_data.sky);
	c->Message(CC_Default, "Fog Colour: Red: %i; Blue: %i; Green %i", zone->newzone_data.fog_red[0], zone->newzone_data.fog_green[0], zone->newzone_data.fog_blue[0]);
	c->Message(CC_Default, "Safe Coords: %f, %f, %f", zone->newzone_data.safe_x, zone->newzone_data.safe_y, zone->newzone_data.safe_z);
	c->Message(CC_Default, "Underworld Coords: %f", zone->newzone_data.underworld);
	c->Message(CC_Default, "Clip Plane: %f - %f", zone->newzone_data.minclip, zone->newzone_data.maxclip);
}

