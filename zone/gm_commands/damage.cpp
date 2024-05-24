#include "../client.h"

void command_damage(Client *c, const Seperator *sep)
{
	if (c->GetTarget() == 0)
		c->Message(CC_Default, "Error: #Damage: No Target.");
	else if (!sep->IsNumber(1)) {
		c->Message(CC_Default, "Usage: #damage [dmg]");
	}
	else {			
		int32 nkdmg = atoi(sep->arg[1]);
		if (nkdmg > 2100000000)
			c->Message(CC_Default, "Enter a value less then 2,100,000,000.");
		else if (c->GetTarget() != c)
			c->GetTarget()->Damage(c, nkdmg, SPELL_UNKNOWN, EQ::skills::SkillHandtoHand);
		else
			c->GetTarget()->DamageCommand(c, nkdmg);
	}
}

