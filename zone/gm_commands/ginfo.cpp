#include "../client.h"
#include "../groups.h"
#include "../raids.h"
#include "../raids.h"

void command_ginfo(Client *c, const Seperator *sep){
	Client *t;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();
	else
		t = c;

	Group *g = t->GetGroup();
	if (!g)
	{
		Raid* r = t->GetRaid();
		if (r)
		{
			uint32 t_group_id = r->GetGroup(t);
			c->Message(Chat::White, "Client %s is in a raid.  Raid ID: %u;  Raid leader: %s;  Raid count: %u;  Raid group ID: %u",
				t->GetName(), r->GetID(), r->leadername, r->RaidCount(), t_group_id);

			if (t->IsRaidGrouped())
			{
				for (int i = 0; i < MAX_RAID_MEMBERS; i++)
				{
					Client* rmember = r->members[i].member;
					if (r->members[i].GroupNumber == t_group_id)
					{
						if (rmember)
						{
							c->Message(Chat::White, "- name: %s (%s)%s%s HP: %u / %u MP: %u / %u", rmember->GetName(), GetClassIDName(rmember->GetClass()),
								r->members[i].IsGroupLeader ? " (group leader)" : "", r->members[i].IsLooter ? " (looter)" : "",
								rmember->GetHP(), rmember->GetMaxHP(), rmember->GetMana(), rmember->GetMaxMana());
						}
						else if (r->members[i].membername[0] != '\0')
						{
							c->Message(Chat::White, "- name: %s (%s)%s%s (not in zone)", r->members[i].membername, GetClassIDName(r->members[i]._class),
								r->members[i].IsGroupLeader ? " (group leader)" : "", r->members[i].IsLooter ? " (looter)" : "");
						}
					}
				}
			}
			else
				c->Message(Chat::White, "Client is not in a raid group");
		}
		else
			c->Message(Chat::White, "Client %s is not in a group or raid", t->GetName());

		return;
	}

	c->Message(Chat::White, "Player %s is in group ID %u with %u members.  Group leader is: %s", t->GetName(), g->GetID(), g->GroupCount(), g->GetLeaderName());

	uint32 r;
	for (r = 0; r < MAX_GROUP_MEMBERS; r++) {
		if (g->members[r] == nullptr) {
			if (g->membername[r][0] == '\0')
				continue;
			c->Message(Chat::White, "- Zoned Member: %s", g->membername[r]);
		}
		else {
			c->Message(Chat::White, "- In-Zone Member: %s (%s) level: %u HP: %u / %u MP: %u / %u", g->membername[r], GetClassIDName(g->members[r]->GetClass()), 
				g->members[r]->GetLevel(), g->members[r]->GetHP(), g->members[r]->GetMaxHP(), g->members[r]->GetMana(), g->members[r]->GetMaxMana());
		}
	}
}

