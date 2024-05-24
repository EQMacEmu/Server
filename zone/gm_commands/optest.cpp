#include "../client.h"

void command_optest(Client *c, const Seperator *sep){

	if(c)
	{
		int arg = atoi(sep->arg[1]);
		if (arg == 1)
		{
			auto app = new EQApplicationPacket(OP_FreezeClientControl, 65);
			strcpy((char *)app->pBuffer, c->GetName());
			c->QueuePacket(app);
			safe_delete(app);
		}
		else
		{
			auto app = new EQApplicationPacket(OP_UnfreezeClientControl, 65);
			strcpy((char *)app->pBuffer, c->GetName());
			c->QueuePacket(app);
			safe_delete(app);
		}
	}

}
