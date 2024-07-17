#include "../client.h"

void command_viewplayerfaction(Client *c, const Seperator *sep)
{
	NPC* npc = nullptr;
	uint32 factionid = 0;
	if (c->GetTarget() && c->GetTarget()->IsNPC())
	{
		npc = c->GetTarget()->CastToNPC();

		if (npc)
		{
			factionid = npc->GetPrimaryFaction();
		}
		else
		{
			if (!sep->IsNumber(1))
			{
				c->Message(Chat::Red, "Invalid target.");
				return;
			}
		}
	}
	else if (!sep->IsNumber(1))
	{
		c->Message(Chat::White, "Usage: #viewplayerfaction [factionid 1-5999]");
		return;
	}

	if (sep->IsNumber(1) && npc == nullptr)
	{
		if (atoi(sep->arg[1]) > 0 && atoi(sep->arg[1]) < 6000)
			factionid = atoi(sep->arg[1]);
		else
		{
			c->Message(Chat::White, "Usage: #viewplayerfaction [factionid 1-5999]");
			return;
		}
	}

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

	int32 personal = t->GetCharacterFactionLevel(factionid);
	int32 modified = t->GetModCharacterFactionLevel(factionid, true);
	int32 illusioned = t->GetModCharacterFactionLevel(factionid, false);
	int16 min_cap = database.MinFactionCap(factionid);
	int16 max_cap = database.MaxFactionCap(factionid);
	int16 modified_min_cap = min_cap + (modified - personal);
	int16 modified_max_cap = max_cap + (modified - personal);
	
	c->Message(Chat::White, "%s has %d personal and %d modified faction with '%s' (%d)  personal cap: %d to %d; modified cap: %d to %d",
		t->GetName(), personal, modified, name, factionid, min_cap, max_cap, modified_min_cap, modified_max_cap);
	if (illusioned != modified)
		c->Message(Chat::White, "Illusion is active and fooling this faction.  Illusioned faction value is %d", illusioned);

	if (npc != nullptr)
	{
		modified = t->GetFactionValue(npc);
		c->Message(Chat::White, "Effective faction for '%s' is %d and includes extra modifiers such as invis and aggro", npc->GetName(), modified);
	}
}

