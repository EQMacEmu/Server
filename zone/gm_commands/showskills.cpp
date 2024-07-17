#include "../client.h"

void command_showskills(Client *c, const Seperator *sep){
	Mob *t = c;

	if (c->GetTarget())
		t = c->GetTarget();

	c->Message(Chat::White, "Skills for %s", t->GetName());
	for (EQ::skills::SkillType i = EQ::skills::Skill1HBlunt; i <= EQ::skills::HIGHEST_SKILL; i = (EQ::skills::SkillType)(i + 1))
		c->Message(Chat::White, "Skill [%d] is at [%d]", i, t->GetSkill(i));
}

