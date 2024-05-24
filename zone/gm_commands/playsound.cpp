#include "../client.h"

void command_playsound(Client* c, const Seperator* sep)
{
	uint16 soundnum = 50;
	if (sep->IsNumber(1))
	{
		soundnum = atoi(sep->arg[1]);
		if (soundnum > 3999)
		{
			c->Message(CC_Default, "Sound number out of range.");
			return;
		}
	}

	c->SendSound(soundnum);
}

