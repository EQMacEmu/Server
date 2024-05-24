#include "../client.h"

void command_setskill(Client *c, const Seperator *sep){
	if (c->GetTarget() == nullptr) {
		c->Message(CC_Default, "Error: #setskill: No target.");
	}
	else if (!c->GetTarget()->IsClient()) {
		c->Message(CC_Default, "Error: #setskill: Target must be a client.");
	}
	else if (
		!sep->IsNumber(1) || atoi(sep->arg[1]) < 0 || atoi(sep->arg[1]) > EQ::skills::HIGHEST_SKILL ||
		!sep->IsNumber(2) || atoi(sep->arg[2]) < 0
		)
	{
		c->Message(CC_Default, "Usage: #setskill skill x ");
		c->Message(CC_Default, "       skill = 0 to %d", EQ::skills::HIGHEST_SKILL);
		c->Message(CC_Default, "       x = 0 to %d", HARD_SKILL_CAP);
	}
	else {
		Log(Logs::General, Logs::Normal, "Set skill request from %s, target:%s skill_id:%i value:%i", c->GetName(), c->GetTarget()->GetName(), atoi(sep->arg[1]), atoi(sep->arg[2]) );
		int skill_num = atoi(sep->arg[1]);
		uint16 skill_value = atoi(sep->arg[2]) > HARD_SKILL_CAP ? HARD_SKILL_CAP : atoi(sep->arg[2]);
		if (skill_num < EQ::skills::HIGHEST_SKILL)
		{
			Client* t = c->GetTarget()->CastToClient();
			if (t)
			{
				t->SetSkill((EQ::skills::SkillType)skill_num, skill_value);
			}
			else
			{
				c->Message(CC_Default, "Error: #setskill: No target.");
			}
		}
	}
}

