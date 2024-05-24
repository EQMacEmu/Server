#include "../client.h"

void command_showbonusstats(Client *c, const Seperator *sep){
	if (c->GetTarget() == 0)
		c->Message(CC_Default, "ERROR: No target!");
	else if (!c->GetTarget()->IsMob() && !c->GetTarget()->IsClient())
		c->Message(CC_Default, "ERROR: Target is not a Mob or Player!");
	else {
		bool bAll = false;
		if (sep->arg[1][0] == '\0' || strcasecmp(sep->arg[1], "all") == 0)
			bAll = true;
		if (bAll || (strcasecmp(sep->arg[1], "item") == 0)) {
			c->Message(CC_Default, "Target Item Bonuses:");
			c->Message(CC_Default, "  Accuracy: %i%%   Divine Save: %i%%", c->GetTarget()->GetItemBonuses().Accuracy, c->GetTarget()->GetItemBonuses().DivineSaveChance);
			c->Message(CC_Default, "  Flurry: %i%%     HitChance: %i%%", c->GetTarget()->GetItemBonuses().FlurryChance, c->GetTarget()->GetItemBonuses().HitChance / 15);
		}
		if (bAll || (strcasecmp(sep->arg[1], "spell") == 0)) {
			c->Message(CC_Default, "  Target Spell Bonuses:");
			c->Message(CC_Default, "  Accuracy: %i%%   Divine Save: %i%%", c->GetTarget()->GetSpellBonuses().Accuracy, c->GetTarget()->GetSpellBonuses().DivineSaveChance);
			c->Message(CC_Default, "  Flurry: %i%%     HitChance: %i%% ", c->GetTarget()->GetSpellBonuses().FlurryChance, c->GetTarget()->GetSpellBonuses().HitChance / 15);
		}
		c->Message(CC_Default, "  Effective Casting Level: %i", c->GetTarget()->GetCasterLevel(0));
	}
}

