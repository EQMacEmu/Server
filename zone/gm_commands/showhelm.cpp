#include "../client.h"

void command_showhelm(Client *c, const Seperator *sep)
{
	bool state = atobool(sep->arg[1]);
	bool current_state = c->ShowHelm();
	bool all = strcasecmp(sep->arg[2], "all") == 0 ? true : false;

	if (sep->arg[1][0] != 0) 
	{
		c->SetShowHelm(state);
		if (!state && current_state)
		{
			c->WearChange(EQ::textures::armorHead, 0, 0, c);
			entity_list.HideHelms(c);
			if (all)
			{
				database.SaveAccountShowHelm(c->AccountID(), state);
			}
		}
		else if(state && !current_state)
		{
			c->SendWearChange(EQ::textures::armorHead, c);
			entity_list.SendHelms(c);
			if (all)
			{
				database.SaveAccountShowHelm(c->AccountID(), state);
			}
		}
		else
		{
			c->Message(CC_Default, "There was no change in your showhelm setting.");
			return;
		}
		
		c->Message(CC_Yellow, "You will %s display helms.", state ? "now" : "no longer");
	}
	else
		c->Message(CC_Default, "Usage: #showhelm on/off [all]");

	return;
}


