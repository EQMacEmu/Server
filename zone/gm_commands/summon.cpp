#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;
#include "../corpse.h"

void command_summon(Client *c, const Seperator *sep){
	Mob *t;

	if (sep->arg[1][0] != 0)		// arg specified
	{
		Client* client = entity_list.GetClientByName(sep->arg[1]);
		if (client != 0)	// found player in zone
			t = client->CastToMob();
		else
		{
			if (!worldserver.Connected())
				c->Message(CC_Default, "Error: World server disconnected.");
			else
			{ // player is in another zone
				//Taking this command out until we test the factor of 8 in ServerOP_ZonePlayer
				//c->Message(CC_Default, "Summoning player from another zone not yet implemented.");
				//return;

				auto pack = new ServerPacket(ServerOP_ZonePlayer, sizeof(ServerZonePlayer_Struct));
				ServerZonePlayer_Struct* szp = (ServerZonePlayer_Struct*)pack->pBuffer;
				strcpy(szp->adminname, c->GetName());
				szp->adminrank = c->Admin();
				szp->ignorerestrictions = 2;
				strcpy(szp->name, sep->arg[1]);
				strcpy(szp->zone, zone->GetShortName());
				szp->x_pos = c->GetX(); // May need to add a factor of 8 in here..
				szp->y_pos = c->GetY();
				szp->z_pos = c->GetZ();
				worldserver.SendPacket(pack);
				safe_delete(pack);
			}
			return;
		}
	}
	else if (c->GetTarget())		// have target
		t = c->GetTarget();
	else
	{
		/*if(c->Admin() < 150)
		c->Message(CC_Default, "You need a NPC/corpse target for this command");
		else*/
		c->Message(CC_Default, "Usage: #summon [charname] Either target or charname is required");
		return;
	}

	if (!t)
		return;

	if (t->IsNPC())
	{ // npc target
		c->Message(CC_Default, "Summoning NPC %s to %1.1f, %1.1f, %1.1f", t->GetName(), c->GetX(), c->GetY(), c->GetZ());
		t->CastToNPC()->GMMove(c->GetX(), c->GetY(), c->GetZ(), c->GetHeading());
		t->CastToNPC()->SaveGuardSpot();
	}
	else if (t->IsCorpse())
	{ // corpse target
		c->Message(CC_Default, "Summoning corpse %s to %1.1f, %1.1f, %1.1f", t->GetName(), c->GetX(), c->GetY(), c->GetZ());
		t->CastToCorpse()->GMMove(c->GetX(), c->GetY(), c->GetZ(), c->GetHeading());
	}
	else if (t->IsClient())
	{
		//If #hideme is on, prevent being summoned by a lower GM.
		if(t->CastToClient()->GetAnon() == 1 && t->CastToClient()->Admin() > c->Admin())
		{
			c->Message(CC_Red, "You cannot summon a GM with a higher status than you.");
			return;
		}

		c->Message(CC_Default, "Summoning player %s to %1.1f, %1.1f, %1.1f", t->GetName(), c->GetX(), c->GetY(), c->GetZ());
		t->CastToClient()->MovePC(zone->GetZoneID(), c->GetX(), c->GetY(), c->GetZ(), c->GetHeading(), 2, GMSummon);
	}
}

