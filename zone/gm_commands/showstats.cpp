#include "../client.h"

void command_showstats(Client *c, const Seperator *sep)
{
	if (sep->IsNumber(1) && atoi(sep->arg[1]) == 1) 
	{
		if (c->GetTarget() != 0 && c->GetTarget()->IsClient())
		{
			c->GetTarget()->CastToClient()->SendQuickStats(c);
		}
		else if(c->GetTarget() != 0 && c->GetTarget()->IsNPC())
		{
			c->GetTarget()->CastToNPC()->ShowQuickStats(c);
		}
		else
		{
			c->SendQuickStats(c);
		}
	}
	else if (c->GetTarget() != 0)
	{
		c->GetTarget()->ShowStats(c);
	}
	else
	{
		c->ShowStats(c);
	}
}

