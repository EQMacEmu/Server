#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;

void command_motd(Client *c, const Seperator *sep)
{
	auto pack = new ServerPacket(ServerOP_Motd, sizeof(ServerMotd_Struct));
	auto mss = (ServerMotd_Struct*)pack->pBuffer;
	strn0cpy(mss->myname, c->GetName(), sizeof(mss->myname));
	strn0cpy(mss->motd, sep->argplus[1], sizeof(mss->motd));
	c->Message(Chat::White, "MoTD is set.");
	worldserver.SendPacket(pack);
	safe_delete(pack);
}

