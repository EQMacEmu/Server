#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;

void command_zonelock(Client *c, const Seperator *sep){
	auto pack = new ServerPacket(ServerOP_LockZone, sizeof(ServerLockZone_Struct));
	ServerLockZone_Struct* lock_zone = (ServerLockZone_Struct*)pack->pBuffer;
	strn0cpy(lock_zone->adminname, c->GetName(), sizeof(lock_zone->adminname));
	if (strcasecmp(sep->arg[1], "list") == 0) {
		lock_zone->op = ServerLockType::List;
		worldserver.SendPacket(pack);
	}
	else if (strcasecmp(sep->arg[1], "lock") == 0 && c->Admin() >= commandLockZones) {
		uint16 tmp = database.GetZoneID(sep->arg[2]);
		if (tmp) {
			lock_zone->op = ServerLockType::Lock;
			lock_zone->zoneID = tmp;
			worldserver.SendPacket(pack);
		}
		else
			c->Message(CC_Default, "Usage: #zonelock lock [zonename]");
	}
	else if (strcasecmp(sep->arg[1], "unlock") == 0 && c->Admin() >= commandLockZones) {
		uint16 tmp = database.GetZoneID(sep->arg[2]);
		if (tmp) {
			lock_zone->op = ServerLockType::Unlock;
			lock_zone->zoneID = tmp;
			worldserver.SendPacket(pack);
		}
		else
			c->Message(CC_Default, "Usage: #zonelock unlock [zonename]");
	}
	else {
		c->Message(CC_Default, "#zonelock sub-commands");
		c->Message(CC_Default, "  list");
		if (c->Admin() >= commandLockZones)
		{
			c->Message(CC_Default, "  lock [zonename]");
			c->Message(CC_Default, "  unlock [zonename]");
		}
	}
	safe_delete(pack);
}

