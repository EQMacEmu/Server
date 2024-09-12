#include "../../client.h"

void SetSkillAllMax(Client *c, const Seperator *sep)
{
	auto t = c;
	if (c->GetTarget() && c->GetTarget()->IsClient()) {
		t = c->GetTarget()->CastToClient();
	}

	for (int i = 0; i <= EQ::skills::HIGHEST_SKILL; ++i) {
		if (i >= EQ::skills::SkillSpecializeAbjure && i <= EQ::skills::SkillSpecializeEvocation) {
			t->SetSkill((EQ::skills::SkillType)i, 50);
		}
		else {
			int max_skill_level = database.GetSkillCap(t->GetClass(), (EQ::skills::SkillType)i, t->GetLevel());
			t->SetSkill((EQ::skills::SkillType)i, max_skill_level);
		}
	}
	
	c->Message(
		Chat::White,
		fmt::format(
			"Maxed skills for {}.",
			c->GetTargetDescription(t)
		).c_str()
	);
}
