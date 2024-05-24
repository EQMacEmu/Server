#include "../client.h"

void command_repopclose(Client *c, const Seperator *sep)
{
	int repop_distance = 500;

	if (sep->arg[1] && strcasecmp(sep->arg[1], "force") == 0) {

		LinkedListIterator<Spawn2*> iterator(zone->spawn2_list);
		iterator.Reset();
		while (iterator.MoreElements()) {
			std::string query = StringFormat(
				"DELETE FROM respawn_times WHERE id = %lu",
				(unsigned long)iterator.GetData()->GetID()
			);
			auto results = database.QueryDatabase(query);
			iterator.Advance();
		}
		c->Message(0, "Zone depop: Force resetting spawn timers.");
	}
	if (sep->IsNumber(1)) {
		repop_distance = atoi(sep->arg[1]);
	}

	c->Message(0, "Zone depoped. Repopping NPC's within %i distance units", repop_distance);
	zone->RepopClose(c->GetPosition(), repop_distance);
}

