#include "../client.h"

void command_goto(Client *c, const Seperator *sep)
{
	// goto function
	if (sep->arg[1][0] == '\0' && c->GetTarget())
		c->MovePC(zone->GetZoneID(), c->GetTarget()->GetX(), c->GetTarget()->GetY(), c->GetTarget()->GetZ(), c->GetTarget()->GetHeading(), 0, SummonPC);
	else if (!(sep->IsNumber(1) && sep->IsNumber(2) && sep->IsNumber(3)))
		c->Message(Chat::White, "Usage: #goto [x y z]");
	else
		c->MovePC(zone->GetZoneID(), atof(sep->arg[1]), atof(sep->arg[2]), atof(sep->arg[3]), 0.0f, 0, SummonPC);
}

