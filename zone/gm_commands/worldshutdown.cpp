#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;

void command_worldshutdown(Client *c, const Seperator *sep){
	// GM command to shutdown world server and all zone servers
	uint32 time = 0;
	uint32 interval = 0;
	if (worldserver.Connected()) {
		if (
			sep->IsNumber(1) &&
			sep->IsNumber(2) &&
			((time = std::stoi(sep->arg[1])) > 0) &&
			((interval = std::stoi(sep->arg[2])) > 0)
			) {
			int time_minutes = (time / 60);
			worldserver.SendEmoteMessage(0, AccountStatus::Player,
				CC_Default,
				fmt::format(
					"[SYSTEM] World will be shutting down in {} minutes.",
					time_minutes
				).c_str()
			);
			c->Message(
				CC_Default,
				fmt::format(
					"World will be shutting down in {} minutes, notifying every {} seconds",
					time_minutes,
					interval
				).c_str()
			);
			auto pack = new ServerPacket(ServerOP_ShutdownAll, sizeof(WorldShutDown_Struct));
			WorldShutDown_Struct* wsd = (WorldShutDown_Struct*)pack->pBuffer;
			wsd->time = time * 1000;
			wsd->interval = (interval * 1000);
			worldserver.SendPacket(pack);
			safe_delete(pack);
		}
		else if (!strcasecmp(sep->arg[1], "now")){
			worldserver.SendEmoteMessage(0, AccountStatus::Player, CC_Yellow, "[SYSTEM] World is shutting down now.");
			c->Message(CC_Default, "World is shutting down now.");
			auto pack = new ServerPacket;
			pack->opcode = ServerOP_ShutdownAll;
			pack->size = 0;
			worldserver.SendPacket(pack);
			safe_delete(pack);
		}
		else if (!strcasecmp(sep->arg[1], "disable")){
			c->Message(CC_Default, "World shutdown has been aborted.");
			auto pack = new ServerPacket(ServerOP_ShutdownAll, sizeof(WorldShutDown_Struct));
			WorldShutDown_Struct* wsd = (WorldShutDown_Struct*)pack->pBuffer;
			wsd->time = 0;
			wsd->interval = 0;
			worldserver.SendPacket(pack);
			safe_delete(pack);
		}
		else{
			c->Message(CC_Default, "#worldshutdown - Shuts down the server and all zones.");
			c->Message(CC_Default, "Usage: #worldshutdown now - Shuts down the server and all zones immediately.");
			c->Message(CC_Default, "Usage: #worldshutdown disable - Stops the server from a previously scheduled shut down.");
			c->Message(CC_Default, "Usage: #worldshutdown [timer] [interval] - Shuts down the server and all zones after [timer] seconds and notifies players every [interval] seconds.");
		}
	}
	else
		c->Message(CC_Default, "Error: World server disconnected");
}

