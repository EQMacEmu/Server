#include "../client.h"

void command_undeletechar(Client *c, const Seperator *sep)
{
	if (sep->arg[1][0] != 0)
	{
		if(!database.UnDeleteCharacter(sep->arg[1]))
		{
			c->Message(Chat::Red, "%s could not be undeleted. Check the spelling of their name.", sep->arg[1]);
		}
		else
		{
			c->Message(Chat::Green, "%s successfully undeleted!", sep->arg[1]);
		}
	}
	else
	{
		c->Message(Chat::White, "Usage: undeletechar [charname]");
	}
}

