#include "../client.h"

void command_size(Client *c, const Seperator *sep){
	Mob *target = c->GetTarget();
	if (!sep->IsNumber(1))
	{
		// ChangeSize just sends the AT_Size apperance packet, which doesn't support float.
		// All attempts to workaround this using the illusion packet have failed. 
		c->Message(Chat::White, "Usage: #size [0 - 255] This command does not support decimals.");
	}
	else 
	{
		float newsize = atof(sep->arg[1]);
		if (newsize > 255)
		{
			c->Message(Chat::White, "Error: #size: Size can not be greater than 255.");
			return;
		}
		else if (newsize < 0)
		{
			c->Message(Chat::White, "Error: #size: Size can not be less than 0.");
			return;
		}
		else if (!target)
		{
			target = c;
		}
		else 
		{
			target->ChangeSize(newsize);
			c->Message(Chat::White, "%s is now size %0.1f", target->GetName(), atof(sep->arg[1]));
		}
	}
}

