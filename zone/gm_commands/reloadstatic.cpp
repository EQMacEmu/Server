#include "../client.h"

void command_reloadstatic(Client *c, const Seperator *sep){
	c->Message(CC_Default, "Reloading zone static data...");
	zone->ReloadStaticData();
}

