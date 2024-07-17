#include "../client.h"

void command_wpadd(Client *c, const Seperator *sep)
{
	int	type1=0,
		type2=0,
		pause=0;	// Defaults for a new grid

	Mob *t = c->GetTarget();
	if (t && t->IsNPC())
	{
		Spawn2* s2info = t->CastToNPC()->respawn2;

		if (s2info == nullptr)	// Can't figure out where this mob's spawn came from... maybe a dynamic mob created by #spawn
		{
			c->Message(Chat::White, "#wpadd FAILED -- Can't determine which spawn record in the database this mob came from!");
			return;
		}

		if (sep->arg[1][0])
		{
			if (atoi(sep->arg[1]) >= 0)
				pause = atoi(sep->arg[1]);
			else
			{
				c->Message(Chat::White, "Usage: #wpadd [pause] [-h]");
				return;
			}
		}
		auto position = c->GetPosition();
		if (strcmp("-h",sep->arg[2]) != 0)
			position.w = -1;

		uint32 tmp_grid = database.AddWPForSpawn(c, s2info->GetID(), position, pause, type1, type2, zone->GetZoneID());
		if (tmp_grid)
			t->CastToNPC()->SetGrid(tmp_grid);

		t->CastToNPC()->AssignWaypoints(t->CastToNPC()->GetGrid());
		c->Message(Chat::White, "Waypoint added. Use #wpinfo to see waypoints for this NPC (may need to #repop first).");
	}
	else
		c->Message(Chat::White, "You must target an NPC to use this.");
}

