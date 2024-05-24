#include "../client.h"

void command_damagetotals(Client *c, const Seperator *sep)
{
	Mob* target = c->GetTarget();
	if (!target || !target->IsNPC())
	{
		c->Message(CC_Default, "Please target a NPC to use this command on.");
	}
	else
	{
		target->ReportDmgTotals(c);
	}	
}

