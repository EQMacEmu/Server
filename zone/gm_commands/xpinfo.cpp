#include "../client.h"

void command_xpinfo(Client *c, const Seperator *sep){

	Client *t;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();
	else
		t = c;

	uint16 level = t->GetLevel();
	uint32 totalrequiredxp = t->GetEXPForLevel(level + 1);
	uint32 currentxp = t->GetEXP();
	float xpforlevel = totalrequiredxp - currentxp;
	float totalxpforlevel = totalrequiredxp - t->GetEXPForLevel(level);
	float xp_percent = 100.0 - ((xpforlevel/totalxpforlevel) * 100.0);

	int exploss;
	t->GetExpLoss(nullptr, 0, exploss);
	float loss_percent = (exploss/totalxpforlevel) * 100.0;

	float maxaa = t->GetEXPForLevel(0, true);
	uint32 currentaaxp = t->GetAAXP();
	float aa_percent = (currentaaxp/maxaa) * 100.0;

	c->Message(CC_Yellow, "%s has %d of %d required XP.", t->GetName(), currentxp, totalrequiredxp);
	c->Message(CC_Yellow, "They need %0.1f more to get to %d. They are %0.2f percent towards this level.", xpforlevel, level+1, xp_percent);
	c->Message(CC_Yellow, "Their XP loss at this level is %d which is %0.2f percent of their current level.", exploss, loss_percent);
	c->Message(CC_Yellow, "They have %d of %0.1f towards an AA point. They are %0.2f percent towards this point.", currentaaxp, maxaa, aa_percent);
}

