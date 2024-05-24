#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;

void command_qtest(Client *c, const Seperator *sep){


	if (c && sep->arg[1][0])
	{
		if (c->GetTarget())
		{
			auto pack = new ServerPacket(ServerOP_Speech, sizeof(Server_Speech_Struct) + strlen(sep->arg[1]) + 1);
			Server_Speech_Struct* sem = (Server_Speech_Struct*)pack->pBuffer;
			strcpy(sem->message, sep->arg[1]);
			sem->minstatus = c->Admin();
			sem->type = 1;
			strncpy(sem->to, c->GetTarget()->GetCleanName(), 64);
			strncpy(sem->to, c->GetCleanName(), 64);
			sem->guilddbid = c->GuildID();
			worldserver.SendPacket(pack);
			safe_delete(pack);
		}
	}
}

