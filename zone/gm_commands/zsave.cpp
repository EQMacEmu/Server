#include "../client.h"

void command_zsave(Client *c, const Seperator *sep){
	if (zone->SaveZoneCFG())
		c->Message(CC_Red, "Zone header saved successfully.");
	else
		c->Message(CC_Red, "ERROR: Zone header data was NOT saved.");
}

