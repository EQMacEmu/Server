#include "../client.h"

void command_showskills(Client *c, const Seperator *sep){
	Mob *t = c;

	if (c->GetTarget())
		t = c->GetTarget();

	c->Message(CC_Default, "Skills for %s", t->GetName());
	for (EQ::skills::SkillType i = EQ::skills::Skill1HBlunt; i <= EQ::skills::HIGHEST_SKILL; i = (EQ::skills::SkillType)(i + 1))
		c->Message(CC_Default, "Skill [%d] is at [%d]", i, t->GetSkill(i));
}

