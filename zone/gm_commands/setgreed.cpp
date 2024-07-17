#include "../client.h"

void command_setgreed(Client *c, const Seperator *sep)
{
	if (!sep->IsNumber(1))
	{
		c->Message(Chat::White, "Usage: #setgreed [greed]");
		return;
	}

	uint32 value = atoi(sep->arg[1]);
	if (c->GetTarget() && c->GetTarget()->IsNPC())
	{
		c->GetTarget()->CastToNPC()->shop_count = value;
		c->Message(Chat::White, "Set %s greed to %d (%d percent)", c->GetTarget()->GetName(), value, c->GetTarget()->CastToNPC()->GetGreedPercent());
	}
	else
	{
		c->Message(Chat::White, "Please select a NPC target to set greed.");
		return;
	}
		
}

