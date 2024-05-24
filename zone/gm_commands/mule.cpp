#include "../client.h"

void command_mule(Client *c, const Seperator *sep)
{
	if (sep->arg[1][0] != 0)
	{
		uint8 toggle = 0;
		if(sep->IsNumber(2))
			toggle = atoi(sep->arg[2]);

		if(toggle >= 1)
			toggle = 1;
		else
			toggle = 0;

		if(!database.SetMule(sep->arg[1], toggle))
		{
			c->Message(CC_Red, "%s could not be toggled. Check the spelling of their account name.", sep->arg[1]);
		}
		else
		{
			c->Message(CC_Green, "%s is %s a mule!", sep->arg[1], toggle == 0 ? "no longer" : "now");
		}
	}
	else
	{
		c->Message(CC_Default, "Usage: mule [accountname] [0/1]");
	}
}

