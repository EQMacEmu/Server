#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;

void command_skilldifficulty(Client *c, const Seperator *sep)
{
	if (sep->argnum > 0) 
	{
		Client *t;

		if (c->GetTarget() && c->GetTarget()->IsClient())
			t = c->GetTarget()->CastToClient();
		else
			t = c;

		if (strcasecmp(sep->arg[1], "info") == 0)
		{
			for (int i = 0; i < EQ::skills::SkillCount; ++i)
			{
				int rawskillval = t->GetRawSkill(EQ::skills::SkillType(i));
				int skillval = t->GetSkill(EQ::skills::SkillType(i));
				int maxskill = t->GetMaxSkillAfterSpecializationRules(EQ::skills::SkillType(i), t->MaxSkill(EQ::skills::SkillType(i)));
				c->Message(CC_Yellow, "Skill: %s (%d) has difficulty: %0.2f", zone->skill_difficulty[i].name, i, zone->skill_difficulty[i].difficulty);
				if(maxskill > 0)
				{
					c->Message(CC_Green, "%s currently has %d (raw: %d) of %d towards this skill.", t->GetName(), skillval, rawskillval, maxskill);
				}
			}
		}
		else if (strcasecmp(sep->arg[1], "difficulty") == 0)
		{
			if(!sep->IsNumber(2) && !sep->IsNumber(3))
			{
				c->Message(CC_Red, "Please specify a valid skill and difficulty.");
				return;
			}
			else if(atoi(sep->arg[2]) > 74 || atoi(sep->arg[2]) < 0 || atof(sep->arg[3]) > 15 || atof(sep->arg[3]) < 1)
			{
				c->Message(CC_Red, "Please specify a skill between 0 and 74 and a difficulty between 1 and 15.");
				return;
			}
			else
			{
				uint16 skillid = atoi(sep->arg[2]);
				float difficulty = atof(sep->arg[3]);
				database.UpdateSkillDifficulty(skillid, difficulty);
				auto pack = new ServerPacket(ServerOP_ReloadSkills, 0);
				worldserver.SendPacket(pack);
				safe_delete(pack);
				c->Message(CC_Default, "Set skill %d to difficulty %0.2f and reloaded all zones.", skillid, difficulty);
			}

		}
		else if (strcasecmp(sep->arg[1], "reload") == 0)
		{
			auto pack = new ServerPacket(ServerOP_ReloadSkills, 0);
			worldserver.SendPacket(pack);
			safe_delete(pack);
			c->Message(CC_Default, "Reloaded skills in all zones.");
		}
		else if (strcasecmp(sep->arg[1], "values") == 0)
		{
			for (int i = 0; i < EQ::skills::SkillCount; ++i)
			{
				int rawskillval = t->GetRawSkill(EQ::skills::SkillType(i));
				int skillval = t->GetSkill(EQ::skills::SkillType(i));
				int maxskill = t->GetMaxSkillAfterSpecializationRules(EQ::skills::SkillType(i), t->MaxSkill(EQ::skills::SkillType(i)));
				if (skillval > 0)
				{
					uint16 type = CC_Green;
					if (maxskill < 1 || skillval > HARD_SKILL_CAP)
						type = CC_Red;
					else if (skillval > maxskill)
						type = CC_Yellow;

					c->Message(type, "%s currently has %d (raw: %d) of %d towards %s.", t->GetName(), skillval, rawskillval, maxskill, zone->skill_difficulty[i].name);
				}
			}
		}
	}
	else
	{
		c->Message(CC_Default, "Usage: #skills info - Provides information about target.");
		c->Message(CC_Default, "#skills difficulty [skillid] [difficulty] - Sets difficulty for selected skill.");
		c->Message(CC_Default, "#skills reload - Reloads skill difficulty in each zone.");
		c->Message(CC_Default, "#skills values - Displays target's skill values.");
	}
}

