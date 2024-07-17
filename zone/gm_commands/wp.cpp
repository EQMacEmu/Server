#include "../client.h"

void command_wp(Client *c, const Seperator *sep){
	int wp = atoi(sep->arg[4]);

	if (strcasecmp("add", sep->arg[1]) == 0) {
		if (wp == 0) //default to highest if it's left blank, or we enter 0
			wp = database.GetHighestWaypoint(zone->GetZoneID(), atoi(sep->arg[2])) + 1;
		if (strcasecmp("-h",sep->arg[5]) == 0) {
			database.AddWP(c, atoi(sep->arg[2]),wp, c->GetPosition(), atoi(sep->arg[3]),zone->GetZoneID());
		}
		else {
            auto position = c->GetPosition();
            position.w = -1;
			database.AddWP(c, atoi(sep->arg[2]),wp, position, atoi(sep->arg[3]),zone->GetZoneID());
		}
	}
	else if (strcasecmp("delete", sep->arg[1]) == 0)
		database.DeleteWaypoint(c, atoi(sep->arg[2]), wp, zone->GetZoneID());
	else
		c->Message(Chat::White, "Usage: #wp add/delete grid_num pause wp_num [-h]");
}

