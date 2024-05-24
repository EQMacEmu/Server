#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;

void command_reloadworld(Client *c, const Seperator *sep){
	//c->Message(CC_Default, "Reloading quest cache, reloading rules, and repopping zones worldwide.");
	auto pack = new ServerPacket(ServerOP_ReloadWorld, sizeof(ReloadWorld_Struct));
	ReloadWorld_Struct* RW = (ReloadWorld_Struct*) pack->pBuffer;
	RW->Option = 0; //Keep it, maybe we'll use it in the future.
	worldserver.SendPacket(pack);
	safe_delete(pack);
	if (!worldserver.SendChannelMessage(c, 0, ChatChannel_Broadcast, 0, 0, 100, "Reloading quest cache, reloading rules, merchants, emotes, and repopping zones worldwide."))
		c->Message(CC_Default, "Error: World server disconnected");

	c->Message(CC_Yellow, "You broadcast, Reloading quest cache, reloading rules, merchants, emotes, and repopping zones worldwide.");
}

