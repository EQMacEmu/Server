#include "../client.h"

void command_sendop(Client *c, const Seperator *sep){

	int RezSpell = 0;

	if (sep->arg[1][0]) {
		RezSpell = atoi(sep->arg[1]);
	}

	auto outapp = new EQApplicationPacket(OP_RezzRequest, sizeof(Resurrect_Struct));
	Resurrect_Struct *rs = (Resurrect_Struct*)outapp->pBuffer;

	strcpy(rs->your_name, c->GetName());
	strcpy(rs->rezzer_name, "A Cleric01");
	rs->spellid = RezSpell;
	DumpPacket(outapp);
	c->QueuePacket(outapp);
	safe_delete(outapp);
	return;
}

