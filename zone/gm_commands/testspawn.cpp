#include "../client.h"

void command_testspawn(Client *c, const Seperator *sep){
	if (sep->IsNumber(1)) {
		auto outapp = new EQApplicationPacket(OP_NewSpawn, sizeof(NewSpawn_Struct));
		NewSpawn_Struct* ns = (NewSpawn_Struct*)outapp->pBuffer;
		c->FillSpawnStruct(ns, c);
		strcpy(ns->spawn.name, "Test");
		ns->spawn.spawnId = 1000;
		ns->spawn.NPC = 1;
		if (sep->IsHexNumber(2)) {
			if (strlen(sep->arg[2]) >= 3) // 0x00, 1 byte
				*(&((uint8*)&ns->spawn)[atoi(sep->arg[1])]) = hextoi(sep->arg[2]);
			else if (strlen(sep->arg[2]) >= 5) // 0x0000, 2 bytes
				*((uint16*)&(((uint8*)&ns->spawn)[atoi(sep->arg[1])])) = hextoi(sep->arg[2]);
			else if (strlen(sep->arg[2]) >= 9) // 0x0000, 2 bytes
				*((uint32*)&(((uint8*)&ns->spawn)[atoi(sep->arg[1])])) = hextoi(sep->arg[2]);
			else
				c->Message(Chat::White, "Error: unexpected hex string length");
		}
		else {
			strcpy((char*)(&((uint8*)&ns->spawn)[atoi(sep->arg[1])]), sep->argplus[2]);
		}
		EncryptSpawnPacket(outapp);
		c->FastQueuePacket(&outapp);
	}
	else
		c->Message(Chat::White, "Usage: #testspawn [memloc] [value] - spawns a NPC for you only, with the specified values set in the spawn struct");
}

