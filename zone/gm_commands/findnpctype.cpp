#include "../client.h"

void command_findnpctype(Client *c, const Seperator *sep)
{
	if (sep->arg[1][0] == 0) {
		c->Message(CC_Default, "Usage: #findnpctype [search criteria]");
		return;
	}

	std::string query;

	int id = atoi((const char *)sep->arg[1]);
	if (id == 0) // If id evaluates to 0, then search as if user entered a string.
		query = StringFormat("SELECT id, name FROM npc_types WHERE name LIKE '%%%s%%' ORDER by id", sep->arg[1]);
	else // Otherwise, look for just that npc id.
		query = StringFormat("SELECT id, name FROM npc_types WHERE id = %i", id);

	auto results = database.QueryDatabase(query);
	if (!results.Success()) {
		c->Message(CC_Default, "Error querying database.");
		c->Message(CC_Default, query.c_str());
	}

	if (results.RowCount() == 0) // No matches found.
		c->Message(CC_Default, "No matches found for %s.", sep->arg[1]);

	// If query runs successfully.
	int count = 0;
	const int maxrows = 20;

	// Process each row returned.
	for (auto row = results.begin(); row != results.end(); ++row) {
		// Limit to returning maxrows rows.
		if (++count > maxrows) {
			c->Message(CC_Default, "%i npc types shown. Too many results.", maxrows);
			break;
		}

		c->Message(CC_Default, "  %s: %s", row[0], row[1]);
	}

	// If we did not hit the maxrows limit.
	if (count <= maxrows)
		c->Message(CC_Default, "Query complete. %i rows shown.", count);

}

