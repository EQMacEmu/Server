#include "../client.h"

void command_synctod(Client *c, const Seperator *sep)
{
	c->Message(Chat::White, "Updating Time/Date for all clients in zone...");
	auto outapp = new EQApplicationPacket(OP_TimeOfDay, sizeof(TimeOfDay_Struct));
	TimeOfDay_Struct* tod = (TimeOfDay_Struct*)outapp->pBuffer;
	zone->zone_time.getEQTimeOfDay(time(0), tod);
	entity_list.QueueClients(c, outapp);
	safe_delete(outapp);
}

