#include "../client.h"

void command_maxallskills(Client *c, const Seperator *sep){
	if (c)
	{
		for (int i = 0; i <= EQ::skills::HIGHEST_SKILL; ++i)
		{
			if (i >= EQ::skills::SkillSpecializeAbjure && i <= EQ::skills::SkillSpecializeEvocation)
			{
				c->SetSkill((EQ::skills::SkillType)i, 50);
			}
			else
			{
				int max_skill_level = database.GetSkillCap(c->GetClass(), (EQ::skills::SkillType)i, c->GetLevel());
				c->SetSkill((EQ::skills::SkillType)i, max_skill_level);
			}
		}
	}
}

