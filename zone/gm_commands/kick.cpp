#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;

void command_kick(Client *c, const Seperator *sep){
	int arguments = sep->argnum;
	if (!arguments) {
		c->Message(Chat::White, "Usage: #kick [Character Name]");
		return;
	}
	else {
		std::string character_name = sep->arg[1];
		auto client = entity_list.GetClientByName(character_name.c_str());

		if (!worldserver.Connected())
			c->Message(Chat::White, "The world server is currently disconnected.");
		else {
			auto pack = new ServerPacket(ServerOP_KickPlayer, sizeof(ServerKickPlayer_Struct));
			ServerKickPlayer_Struct* skp = (ServerKickPlayer_Struct*)pack->pBuffer;
			strcpy(skp->adminname, c->GetName());
			strcpy(skp->name, character_name.c_str());
			skp->adminrank = c->Admin();
			worldserver.SendPacket(pack);
			safe_delete(pack);
		}
		if (client) {
			if (client->Admin() <= c->Admin()) {
				client->Message(Chat::White, 
					fmt::format(
						"You have been kicked by {} ", 
						c->GetName()
					).c_str()
				);
				client->WorldKick();
				c->Message(Chat::White, 
					fmt::format(
					"{} has been kicked from the server.",
					character_name
					).c_str()
				);
			}
		}
	}
}

