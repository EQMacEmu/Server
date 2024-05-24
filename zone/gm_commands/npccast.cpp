#include "../client.h"

void command_npccast(Client *c, const Seperator *sep){
	if (c->GetTarget() && c->GetTarget()->IsNPC() && !sep->IsNumber(1) && sep->arg[1] != 0 && sep->IsNumber(2)) {
		Mob* spelltar = entity_list.GetMob(sep->arg[1]);
		if (spelltar)
			c->GetTarget()->CastSpell(atoi(sep->arg[2]), spelltar->GetID());
		else
			c->Message(CC_Default, "Error: %s not found", sep->arg[1]);
	}
	else if (c->GetTarget() && c->GetTarget()->IsNPC() && sep->IsNumber(1) && sep->IsNumber(2)) {
		Mob* spelltar = entity_list.GetMob(atoi(sep->arg[1]));
		if (spelltar)
			c->GetTarget()->CastSpell(atoi(sep->arg[2]), spelltar->GetID());
		else
			c->Message(CC_Default, "Error: target ID %i not found", atoi(sep->arg[1]));
	}
	else
		c->Message(CC_Default, "Usage: (needs NPC targeted) #npccast targetname/entityid spellid");
}

