#include "../client.h"

void command_repop(Client *c, const Seperator *sep)
{
	bool force = false;
	if (sep->arg[1] && strcasecmp(sep->arg[1], "force") == 0) {
		force = true;
	}

	if (!force && c->GetTarget() && c->GetTarget()->IsNPC())
	{
		c->GetTarget()->CastToNPC()->ForceRepop();
		c->Message(Chat::White, "Repopping %s", c->GetTarget()->GetName());
	}
	else
	{
		int timearg = 1;
		if (force) {
			timearg++;

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
			c->Message(Chat::White, "Zone depop: Force resetting spawn timers.");
		}

		c->Message(Chat::White, "Zone depoped. Repoping now.");
		zone->Repop();
		return;
	}
}

