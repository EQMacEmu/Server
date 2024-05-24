#include "../client.h"

void command_setgreed(Client *c, const Seperator *sep)
{
	if (!sep->IsNumber(1))
	{
		c->Message(CC_Default, "Usage: #setgreed [greed]");
		return;
	}

	uint32 value = atoi(sep->arg[1]);
	if (c->GetTarget() && c->GetTarget()->IsNPC())
	{
		c->GetTarget()->CastToNPC()->shop_count = value;
		c->Message(CC_Default, "Set %s greed to %d (%d percent)", c->GetTarget()->GetName(), value, c->GetTarget()->CastToNPC()->GetGreedPercent());
	}
	else
	{
		c->Message(CC_Default, "Please select a NPC target to set greed.");
		return;
	}
		
}

