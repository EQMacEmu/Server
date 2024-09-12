#include "../../client.h"

void SetLevel(Client *c, const Seperator *sep)
{
	uint16 level = atoi(sep->arg[2]);
	uint16 current_level = c->GetLevel();
	bool target = false;

	if (c->GetTarget() && ((level <= 0) ||
		((level > RuleI(Character, MaxLevel)) && (c->Admin() < commandLevelAboveCap)) ||
		(!c->GetTarget()->IsNPC() && ((c->Admin() < commandLevelNPCAboveCap) && (level > RuleI(Character, MaxLevel))))) ||
		(c->Admin() < RuleI(GM, MinStatusToLevelTarget) && level > RuleI(Character, MaxLevel)))
	{
		c->Message(Chat::White, "Error: #Level: Invalid Level");
		return;
	}
	else if (c->GetTarget())
	{
		current_level = c->GetTarget()->GetLevel();
		c->GetTarget()->SetLevel(level, true);
		target = true;
	}
	else if (c->GetTarget() && c->Admin() < RuleI(GM, MinStatusToLevelTarget) && level <= RuleI(Character, MaxLevel) && level > 0) {
		c->Message(Chat::White, "Your status level only supports self use of this command.");
		c->SetLevel(level, true);
	}
	else
	{
		c->Message(Chat::White, "No valid target selected, using command on self.");
		c->SetLevel(level, true);
	}

	if (level < current_level)
	{
		Mob* tar = !target ? c : c->GetTarget();
		if (tar->IsClient())
			c->Message(Chat::Yellow, "%s must zone before their client will see the lowered level.", tar->GetName());
	}
}

