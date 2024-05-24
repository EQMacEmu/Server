#include "../client.h"

void command_grid(Client* c, const Seperator* sep) {
	if (strcasecmp("max", sep->arg[1]) == 0)
	{
		c->Message(CC_Default, "Highest grid ID in this zone: %d", database.GetHighestGrid(zone->GetZoneID()));
	}
	else if (strcasecmp("add", sep->arg[1]) == 0)
	{
		database.ModifyGrid(c, false, atoi(sep->arg[2]), atoi(sep->arg[3]), atoi(sep->arg[4]), zone->GetZoneID());
	}
	else if (strcasecmp("show", sep->arg[1]) == 0) 
	{

		Mob* target = c->GetTarget();

		if (!target || !target->IsNPC()) 
		{
			c->Message(0, "You need a NPC target!");
			return;
		}

		std::string query = StringFormat (
			"SELECT `x`, `y`, `z`, `heading`, `number` "
			"FROM `grid_entries` "
			"WHERE `zoneid` = %u and `gridid` = %i "
			"ORDER BY `number`",
			zone->GetZoneID(),
			target->CastToNPC()->GetGrid()
		);

		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(0, "Error querying database.");
			c->Message(0, query.c_str());
		}

		if (results.RowCount() == 0) {
			c->Message(0, "No grid found");
			return;
		}

		/**
		 * Spawn grid nodes
		 */
		std::map<std::vector<float>, int32> zoffset;

		for (auto row = results.begin(); row != results.end(); ++row) {
			glm::vec4 node_position = glm::vec4(atof(row[0]), atof(row[1]), atof(row[2]), atof(row[3]));
			std::vector<float> node_loc{
					node_position.x, node_position.y, node_position.z
			};

			// If we already have a node at this location, set the z offset
			// higher from the existing one so we can see it.  Adjust so if
			// there is another at the same spot we adjust again.
			auto search = zoffset.find(node_loc);
			if (search != zoffset.end()) {
				search->second = search->second + 3;
			}
			else {
				zoffset[node_loc] = 0.0;
			}

			node_position.z += zoffset[node_loc];

			NPC::SpawnGridNodeNPC(node_position, atoi(row[4]), zoffset[node_loc]);
		}
	}
	else if (strcasecmp("delete", sep->arg[1]) == 0)
	{
		database.ModifyGrid(c, true, atoi(sep->arg[2]), 0, 0, zone->GetZoneID());
	}
	else
	{
		c->Message(CC_Default, "Usage: #grid add/delete grid_num wandertype pausetype");
		c->Message(CC_Default, "Usage: #grid max - displays the highest grid ID used in this zone (for add)");
	}
}

