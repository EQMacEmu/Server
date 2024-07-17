#include "../client.h"

void command_manaburn(Client *c, const Seperator *sep){
	Mob* target = c->GetTarget();

	if (c->GetTarget() == 0)
		c->Message(Chat::White, "#Manaburn needs a target.");
	else {
		int cur_level = c->GetAA(MANA_BURN);//ManaBurn ID
		if (DistanceSquared(c->GetPosition(), target->GetPosition()) > 200)
			c->Message(Chat::White, "You are too far away from your target.");
		else {
			if (cur_level == 1) {
				if (c->IsAttackAllowed(target))
				{
					c->SetMana(0);
					int nukedmg = (c->GetMana()) * 2;
					if (nukedmg>0)
					{
						target->Damage(c, nukedmg, 2751, EQ::skills::SkillAbjuration/*hackish*/);
						c->Message(Chat::LightBlue, "You unleash an enormous blast of magical energies.");
					}
					Log(Logs::General, Logs::Normal, "Manaburn request from %s, damage: %d", c->GetName(), nukedmg);
				}
			}
			else
				c->Message(Chat::White, "You have not learned this skill.");
		}
	}
}

