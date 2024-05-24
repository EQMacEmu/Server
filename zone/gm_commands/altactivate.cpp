#include "../client.h"

void command_altactivate(Client *c, const Seperator *sep){
	if (sep->arg[1][0] == '\0'){
		c->Message(CC_Default, "Invalid argument, usage:");
		c->Message(CC_Default, "#altactivate list - lists the AA ID numbers that are available to you");
		c->Message(CC_Default, "#altactivate time [argument] - returns the time left until you can use the AA with the ID that matches the argument.");
		c->Message(CC_Default, "#altactivate [argument] - activates the AA with the ID that matches the argument.");
		return;
	}
	if (!strcasecmp(sep->arg[1], "help")){
		c->Message(CC_Default, "Usage:");
		c->Message(CC_Default, "#altactivate list - lists the AA ID numbers that are available to you");
		c->Message(CC_Default, "#altactivate time [argument] - returns the time left until you can use the AA with the ID that matches the argument.");
		c->Message(CC_Default, "#altactivate [argument] - activates the AA with the ID that matches the argument.");
		return;
	}
	if (!strcasecmp(sep->arg[1], "list")){
		c->Message(CC_Default, "You have access to the following AA Abilities:");
		int x, val;
		SendAA_Struct* saa = nullptr;
		for (x = 0; x < aaHighestID; x++){
			if (AA_Actions[x][0].spell_id || AA_Actions[x][0].action){ //if there's an action or spell associated we assume it's a valid
				val = 0;					//and assume if they don't have a value for the first rank then it isn't valid for any rank
				saa = nullptr;
				val = c->GetAA(x);
				if (val){
					saa = zone->FindAA(x, false);
					c->Message(CC_Default, "%d: %s %d", x, saa->name, val);
				}
			}
		}
	}
	else if (!strcasecmp(sep->arg[1], "time")){
		int ability = atoi(sep->arg[2]);
		if (c->GetAA(ability)){
			int remain = c->GetPTimers().GetRemainingTime(pTimerAAStart + ability);
			if (remain)
				c->Message(CC_Default, "You may use that ability in %d minutes and %d seconds.", (remain / 60), (remain % 60));
			else
				c->Message(CC_Default, "You may use that ability now.");
		}
		else{
			c->Message(CC_Default, "You do not have access to that ability.");
		}
	}
	else
	{
		c->ActivateAA((aaID)atoi(sep->arg[1]));
	}
}

