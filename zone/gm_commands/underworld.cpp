#include "../client.h"

void command_underworld(Client *c, const Seperator *sep)
{
	float z_coord = 0;
	if (sep->IsNumber(1))
	{
		z_coord = atof(sep->arg[1]);
	}
	else
	{
		std::string query = StringFormat("SELECT min(z) from spawn2 where zone = '%s'", zone->GetShortName());
		auto results = database.QueryDatabase(query);
		if (!results.Success() || results.RowCount() == 0)
		{
			c->Message(CC_Red, "This zone has no valid spawn2 entries.");
			return;
		}

		auto row = results.begin();
		z_coord = atof(row[0]);

		std::string query1 = StringFormat("SELECT min(z) from grid_entries where zoneid = %i", zone->GetZoneID());
		auto results1 = database.QueryDatabase(query1);

		if (results1.Success() && results1.RowCount() != 0)
		{
			auto row = results1.begin();
			if (atof(row[0]) < z_coord)
			{
				z_coord = atof(row[0]);
			}
		}
	}

	entity_list.ReportUnderworldNPCs(c, z_coord);
	return;
}

