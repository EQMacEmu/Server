#include "../client.h"

void command_push(Client *c, const Seperator *sep) {

	Mob* t;
	if (!c->GetTarget())
	{
		c->Message(CC_Default, "You need a target to push.");
		return;
	}
	else if (!c->GetTarget()->IsNPC() && !c->GetTarget()->IsClient())
	{
		c->Message(CC_Default, "That's an invalid target, nerd.");
		return;
	}
	else if (!sep->IsNumber(1))
	{
		c->Message(CC_Default, "Invalid number of arguments.\nUsage: #push [pushback] [pushup]");
		return;
	}
	else
	{
		bool success = false;
		float pushback = atof(sep->arg[1]);
		t = c->GetTarget();
		if (sep->IsNumber(2))
		{
			float pushup = atof(sep->arg[2]);
			if (t->DoKnockback(c, pushback, pushup))
			{
				success = true;
			}
		}
		else
		{
			if (t->IsNPC())
			{
				t->CastToNPC()->AddPush(c->GetHeading() * 2.0f, pushback);
				pushback = t->CastToNPC()->ApplyPushVector();
				if (pushback > 0.0f)
					success = true;
			}
			else
			{
				if (t->CombatPush(c, pushback))
				{
					success = true;
				}
			}
		}

		if (success)
		{
			c->Message(CC_Default, "%s was pushed for %0.1f!", t->GetCleanName(), pushback);
			return;
		}
		else
		{
			c->Message(CC_Default, "Pushed failed on %s. Coord check likely failed.", t->GetCleanName());
			return;
		}
	}
}

