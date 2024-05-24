#include "../client.h"

void command_giveplayerfaction(Client *c, const Seperator *sep)
{
	if (!sep->IsNumber(1) || atoi(sep->arg[2]) == 0)
	{
		c->Message(CC_Default, "Usage: #giveplayerfaction [factionid] [value]");
		return;
	}

	uint32 factionid = atoi(sep->arg[1]);
	int32 value = atoi(sep->arg[2]);
	Client* t;
	if (c->GetTarget() && c->GetTarget()->IsClient())
	{
		t = c->GetTarget()->CastToClient();
	}
	else
	{
		t = c;
	}

	char name[50];
	if (database.GetFactionName(factionid, name, sizeof(name)) == false)
		snprintf(name, sizeof(name), "Faction%i", factionid);

	t->SetFactionLevel2(t->CharacterID(), factionid, value, 0);
	c->Message(CC_Default, "%s was given %i points with %s.", t->GetName(), value, name);
}

