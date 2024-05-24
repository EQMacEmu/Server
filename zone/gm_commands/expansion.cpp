#include "../client.h"

void command_expansion(Client *c, const Seperator *sep)
{
	if (sep->arg[1][0] != 0)
	{
		uint8 toggle = 0;
		if (sep->IsNumber(2))
			toggle = atoi(sep->arg[2]);

		if (!database.SetExpansion(sep->arg[1], toggle))
		{
			c->Message(CC_Red, "%s could not be toggled. Check the spelling of their account name.", sep->arg[1]);
		}
		else
		{
			c->Message(CC_Green, "%s has their expansion set to %d!", sep->arg[1], toggle);
		}
	}
	else
	{
		c->Message(CC_Default, "Usage: expansion [accountname] [expansion]");
		c->Message(CC_Default, "Expansions: 0 - Classic 1 - Kunark 2 - Velious 4 - Luclin 8 - PoP");
		c->Message(CC_Default, "Added them together to set which expansions the account will have.");
		c->Message(CC_Default, "Classic cannot be disabled.");
	}
}

