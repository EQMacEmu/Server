#include "../client.h"

void command_chattest(Client *c, const Seperator *sep)
{
	if(!sep->IsNumber(1))
		c->Message(CC_Red, "Please specify a valid number to send as the message color. (This message is red, btw.)");
	else
	{
		int default_ = 10;
		if(sep->IsNumber(2))
			default_ = atoi(sep->arg[2]);

		uint16 base = atoi(sep->arg[1]);
		for(uint8 i = 0; i < default_; ++i)
		{
			uint16 color = base + i;
			c->Message(color, "All work and no play makes Jack a dull boy. (%i)", color);
		}
	}
}

