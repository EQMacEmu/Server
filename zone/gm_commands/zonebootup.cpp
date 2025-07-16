#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;

void command_zonebootup(Client *c, const Seperator *sep)
{
	if (!worldserver.Connected())
	{
		c->Message(Chat::White, "Error: World server disconnected");
	}
	else if (sep->arg[1][0] == 0)
	{
		auto pack = new ServerPacket(ServerOP_BootDownZones, sizeof(ServerDownZoneBoot_struct));
		ServerDownZoneBoot_struct* s = (ServerDownZoneBoot_struct *)pack->pBuffer;
		strcpy(s->adminname, c->GetName());
		worldserver.SendPacket(pack);
		safe_delete(pack);
		c->Message(Chat::White, "Zonebootup completed.");
	}
	else 
	{
		auto pack = new ServerPacket(ServerOP_ZoneBootup, sizeof(ServerZoneStateChange_struct));
		ServerZoneStateChange_struct* s = (ServerZoneStateChange_struct *)pack->pBuffer;
		s->zone_server_id = atoi(sep->arg[2]);
		strcpy(s->admin_name, c->GetName());
		s->zone_id = ZoneID(sep->arg[1]);
		s->is_static = true;
		worldserver.SendPacket(pack);
		safe_delete(pack);
		c->Message(Chat::White, "Zonebootup completed.");
	}
}

