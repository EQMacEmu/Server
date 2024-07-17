#include "../client.h"

void command_setskillall(Client *c, const Seperator *sep){
	if (c->GetTarget() == 0)
		c->Message(Chat::White, "Error: #setallskill: No target.");
	else if (!c->GetTarget()->IsClient())
		c->Message(Chat::White, "Error: #setskill: Target must be a client.");
	else if (!sep->IsNumber(1) || atoi(sep->arg[1]) < 0) {
		c->Message(Chat::White, "Usage: #setskillall value ");
		c->Message(Chat::White, "       value = 0 to %d", HARD_SKILL_CAP);
	}
	else {
		if (c->Admin() >= commandSetSkillsOther || c->GetTarget() == c || c->GetTarget() == 0) {
			Log(Logs::General, Logs::Normal, "Set ALL skill request from %s, target:%s", c->GetName(), c->GetTarget()->GetName());
			uint16 level = atoi(sep->arg[1]) > HARD_SKILL_CAP ? HARD_SKILL_CAP : atoi(sep->arg[1]);

			for (EQ::skills::SkillType skill_num = EQ::skills::Skill1HBlunt; skill_num <= EQ::skills::HIGHEST_SKILL; skill_num = (EQ::skills::SkillType)(skill_num + 1))
			{
				Client* t = c->GetTarget()->CastToClient();
				if (t)
				{
					uint16 max_level = t->GetMaxSkillAfterSpecializationRules(skill_num, t->MaxSkill(skill_num));
					uint16 cap_level = level > max_level ? max_level : level;

					t->SetSkill(skill_num, cap_level);
				}
				else
				{
					c->Message(Chat::White, "Error: #setallskill: No target.");
					return;
				}
			}
		}
		else
			c->Message(Chat::White, "Error: Your status is not high enough to set anothers skills");
	}
}

