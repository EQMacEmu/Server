#include "../client.h"

void command_cleartimers(Client *c, const Seperator *sep)
{
	Client *t;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();
	else
		t = c;

	uint8 type = 0;
	if(sep->IsNumber(1))
	{
		type = atoi(sep->arg[1]);
		t->ClearPTimers(type);
		c->Message(Chat::White, "Cleared PTimer %d on %s", type, t->GetName());
	}
	else
	{
		int x = 0;
		if (strcasecmp(sep->arg[1], "help") == 0)
		{
			c->Message(Chat::White, "PTimer list: (1 Unused) (2 Surname) (3 FD) (4 Sneak) (5 Hide) (6 Taunt) (7 InstallDoubt)");
			c->Message(Chat::White, "(8 Fishing) (9 Foraging) (10 Mend) (11 Tracking) (12 SenseTraps) (13 DisarmTraps)");
			c->Message(Chat::White, "(14-16 Discs) (17 Combat Ability) (18 Begging/PP) (19 Sense Heading) (20 Bind Wound)");
			c->Message(Chat::White, "(21 Apply Posion) (22 Disarm) (23 PEQZone)");
			c->Message(Chat::White, "(1000-2999 AAs) (3001-4999 AA Effects) (5000-9678 Spells)");
			c->Message(Chat::White, "(5087 Lay Hands) (5088 Harm Touch)");
		}
		else if(strcasecmp(sep->arg[1], "all") == 0)
		{
			t->ClearPTimers(0);
			t->SendAATimers();
			t->ResetAllSkills();
			c->Message(Chat::White, "Cleared all timers on %s", t->GetName());
		}
		else if(strcasecmp(sep->arg[1], "skills") == 0)
		{
			t->ResetAllSkills();
			c->Message(Chat::White, "Cleared all skill timers on %s", t->GetName());
		}
		else if(strcasecmp(sep->arg[1], "disc") == 0)
		{
			for (x = pTimerDisciplineReuseStart; x <= pTimerDisciplineReuseEnd; ++x)
			{
				t->ClearPTimers(x);
			}
			c->Message(Chat::White, "Cleared all disc timers on %s", t->GetName());
		}
		else if(strcasecmp(sep->arg[1], "aa") == 0)
		{
			for (x = pTimerAAStart; x <= pTimerAAEffectEnd; ++x)
			{
				t->ClearPTimers(x);
			}
			t->SendAATimers();
			c->Message(Chat::White, "Cleared all AA timers on %s", t->GetName());
		}
		else if(strcasecmp(sep->arg[1], "spells") == 0)
		{
			for (x = pTimerSpellStart; x <= pTimerSpellStart + 4678; ++x)
			{
				t->ClearPTimers(x);
			}
			c->Message(Chat::White, "Cleared all spell timers on %s", t->GetName());
		}
		else
		{
			c->Message(Chat::White, "Usage: #cleartimers [type]/help/all/skills/disc/aa/spells");
		}

	}
	t->Save();
}

