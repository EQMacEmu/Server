#include "../../client.h"

void SetEXP(Client* c, const Seperator* sep)
{
	Client* t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();

	if (sep->IsNumber(2)) {
		int exploss;
		t->GetExpLoss(nullptr, 0, exploss);
		uint32 currentXP = t->GetEXP();
		uint32 currentaaXP = t->GetAAXP();
		int input = atoi(sep->arg[2]);

		if (input > 9999999)
			c->Message(Chat::White, "Error: Value too high.");
		else if (input == -1)
		{
			uint32 newxp = currentXP - exploss;
			if (newxp < 1000)
				newxp = 1000;
			t->SetEXP(newxp, currentaaXP);
		}
		else if (input == 0)
		{
			uint32 newxp = currentXP + exploss;
			t->SetEXP(newxp, currentaaXP);
		}
		else if (input <= 100)
		{
			float percent = input / 100.0f;
			uint32 requiredxp = t->GetEXPForLevel(t->GetLevel() + 1) - t->GetEXPForLevel(t->GetLevel());
			float final_ = requiredxp * percent;
			uint32 newxp = (uint32)final_ + currentXP;
			t->SetEXP(newxp, currentaaXP);
		}
		else
		{
			uint32 newxp = currentXP + input;
			t->SetEXP(newxp, currentaaXP);
		}
	}
	else
		c->Message(Chat::White, "Usage: #setxp number or percentage. If 0, will 'rez' a single death. If -1 will subtract a single death.");
}

