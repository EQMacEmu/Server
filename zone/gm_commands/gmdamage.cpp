#include "../client.h"

void command_gmdamage(Client *c, const Seperator *sep)
{
	if (c->GetTarget() == 0)
		c->Message(CC_Default, "Error: #gmamage: No Target.");
	else if (!sep->IsNumber(1)) {
		c->Message(CC_Default, "Usage: #gmdamage [dmg] [skipaggro] [spell]");
	}
	else {
		int32 nkdmg = atoi(sep->arg[1]);
		bool skipaggro = false;
		if (sep->IsNumber(2))
			skipaggro = atoi(sep->arg[2]) > 0 ? true : false;

		bool spell = false;
		if (sep->IsNumber(3))
			spell = atoi(sep->arg[3]) > 0 ? true : false;

		uint16 spell_id = SPELL_UNKNOWN;
		EQ::skills::SkillType attack_skill = EQ::skills::SkillHandtoHand;
		if (spell)
		{
			spell_id = 383;
			attack_skill = EQ::skills::SkillEvocation;
		}

		if (nkdmg > 2100000000)
			c->Message(CC_Default, "Enter a value less then 2,100,000,000.");
		else
			c->GetTarget()->DamageCommand(c, nkdmg, skipaggro, spell_id, attack_skill);
	}
}

