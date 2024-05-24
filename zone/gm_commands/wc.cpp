#include "../client.h"

void command_wc(Client *c, const Seperator *sep){
	if (sep->argnum < 2)
	{
		c->Message(CC_Default, "Usage: #wc slot material [color] [unknown06]");
	}
	else if (c->GetTarget() == nullptr) {
		c->Message(CC_Red, "You must have a target to do a wear change.");
	}
	else
	{
		uint8 wearslot = atoi(sep->arg[1]);
		uint16 texture = atoi(sep->arg[2]);
		uint32 color = 0;
		
		if (sep->argnum > 2)
		{
			color = atoi(sep->arg[3]);
		}

		c->GetTarget()->WearChange(wearslot, texture, color);
	}
}

