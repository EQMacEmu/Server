#include "../client.h"

void command_findzone(Client *c, const Seperator *sep)
{
	if (sep->arg[1][0] == 0) {
		c->Message(CC_Default, "Usage: #findzone [search criteria]");
		return;
	}

	std::string query;
	int id = atoi((const char *)sep->arg[1]);
	if (id == 0) { // If id evaluates to 0, then search as if user entered a string.
		auto escName = new char[strlen(sep->arg[1]) * 2 + 1];
		database.DoEscapeString(escName, sep->arg[1], strlen(sep->arg[1]));

		query = StringFormat("SELECT zoneidnumber, short_name, long_name FROM zone "
			"WHERE long_name RLIKE '%s'", escName);
		safe_delete_array(escName);
	}
	else // Otherwise, look for just that zoneidnumber.
		query = StringFormat("SELECT zoneidnumber, short_name, long_name FROM zone "
		"WHERE zoneidnumber = %i", id);

	auto results = database.QueryDatabase(query);
	if (!results.Success()) {
		c->Message(CC_Default, "Error querying database.");
		c->Message(CC_Default, query.c_str());
		return;
	}

	int count = 0;
	const int maxrows = 20;

	for (auto row = results.begin(); row != results.end(); ++row) {
		if (++count > maxrows) {
			c->Message(CC_Default, "%i zones shown. Too many results.", maxrows);
			break;
		}

		c->Message(CC_Default, "  %s: %s, %s", row[0], row[1], row[2]);
	}

	if (count <= maxrows)
		c->Message(CC_Default, "Query complete. %i rows shown.", count);
	else if (count == 0)
		c->Message(CC_Default, "No matches found for %s.", sep->arg[1]);
}

