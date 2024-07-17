#include "../client.h"

void command_falltest(Client *c, const Seperator *sep)
{
	float zmod;
	if(c)
	{
		if (!sep->IsNumber(1)) {
			c->Message(Chat::White, "Invalid number of arguments.\nUsage: #falltest [+z]");
			return;
		}
		else
		{
			zmod = c->GetZ() + atof(sep->arg[1]);
			c->MovePC(zone->GetZoneID(), c->GetX(), c->GetY(), zmod, c->GetHeading());
			c->Message(Chat::White, "Moving to X: %0.2f Y: %0.2f Z: %0.2f", c->GetX(), c->GetY(), zmod);
		}
	}
}

