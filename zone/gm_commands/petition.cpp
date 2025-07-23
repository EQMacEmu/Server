#include "../client.h"
#include "../guild_mgr.h"

void command_petition(Client *c, const Seperator *sep)
{
	int admin = c->Admin();
	std::string help0 = "Petition commands usage:";
	std::string help1 = "  #petition list - Lists all petitions.";
	std::string help2 = "  #petition view - #petition view (petition number).";
	std::string help3 = "  #petition info - #petition info (petition number).";
	std::string help4 = "  #petition update - #petition update (petition number) (Text).";
	std::string help5 = "      Adds GM comment Make sure you contain the comments in quotes.";
	std::string help6 = "  #petition delete - #petition delete. (petition number).";

	std::string help[] = { help0, help1, help2, help3, help4, help5, help6 };

	if (strcasecmp(sep->arg[1], "help") == 0)
	{
		int size = sizeof(help) / sizeof(std::string);
		for (int i = 0; i < size; i++)
		{
			c->Message(Chat::White, help[i].c_str());
		}
	}
	else if (strcasecmp(sep->arg[1], "list") == 0)
	{
		std::string query = "SELECT petid, charname, accountname, senttime FROM petitions ORDER BY petid";
		auto results = database.QueryDatabase(query);

		if (!results.Success() || results.RowCount() == 0)
		{
			c->Message(Chat::Red, "There was an error in your request: No petitions found!");
			return;
		}

		c->Message(Chat::Red, "ID : Character , Account , Time Sent");
		for (auto row = results.begin(); row != results.end(); ++row)
		{
			char *pet_time = row[3];
			time_t t = atoi(pet_time);
			struct tm *tm = localtime(&t);
			char date[80];
			strftime(date, 80, "%m/%d %I:%M%p", tm);

			c->Message(Chat::Yellow, " %s:	%s , %s , %s", row[0], row[1], row[2], date);
		}
	}
	else if (strcasecmp(sep->arg[1], "view") == 0)
	{
		if (sep->arg[2][0] == 0)
		{
			c->Message(Chat::White, "Usage: #petition view (petition number) Type #petition list for a list");
			return;
		}
		Log(Logs::Detail, Logs::Normal, "Petition viewed by %s, petition number:", c->GetName(), atoi(sep->arg[2]));
		c->Message(Chat::Red, "ID : Character , Account , Petition Text");
		std::string query = StringFormat("SELECT petid, charname, accountname, petitiontext FROM petitions WHERE petid = %i", atoi(sep->arg[2]));
		auto results = database.QueryDatabase(query);

		if (!results.Success() || results.RowCount() == 0)
		{
			c->Message(Chat::Red, "There was an error in your request: ID not found! Please check the Id and try again.");
			return;
		}

		auto row = results.begin();
		c->Message(Chat::Yellow, " %s: %s , %s , %s", row[0], row[1], row[2], row[3]);
	}
	else if (strcasecmp(sep->arg[1], "info") == 0)
	{
		if (sep->arg[2][0] == 0)
		{
			c->Message(Chat::White, "Usage: #petition info (petition number) Type #petition list for a list");
			return;
		}

		Log(Logs::General, Logs::Normal, "Petition information request from %s, petition number:", c->GetName(), atoi(sep->arg[2]));
		c->Message(Chat::Red, "ID : Character , Account , Zone , Class , Race , Level , Last GM , GM Comment");
		std::string query = StringFormat("SELECT petid, charname, accountname, zone, charclass, charrace, charlevel, lastgm, gmtext FROM petitions WHERE petid = %i", atoi(sep->arg[2]));
		auto results = database.QueryDatabase(query);

		if (!results.Success() || results.RowCount() == 0)
		{
			c->Message(Chat::Red, "There was an error in your request: ID not found! Please check the Id and try again.");
			return;
		}

		auto row = results.begin();
		c->Message(Chat::Yellow, "%s: %s %s %s %s %s %s %s %s", row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8]);
	}
	else if (strcasecmp(sep->arg[1], "update") == 0)
	{
		if (admin >= 90)
		{
			if (sep->arg[2][0] == 0 || sep->arg[3][0] == 0)
			{
				c->Message(Chat::White, "Usage: #petition update (petition number) (Text) Make sure you contain the comments in quotes. Type #petition list for a list");
				return;
			}

			Log(Logs::Detail, Logs::Normal, "Petition update request from %s, petition number:", c->GetName(), atoi(sep->arg[2]));
			std::string query = StringFormat("UPDATE `petitions` SET `lastgm` = '%s', `gmtext` = '%s' WHERE `petid` = %i;", c->GetName(), sep->arg[3], atoi(sep->arg[2]));
			auto results = database.QueryDatabase(query);

			if (!results.Success())
			{
				c->Message(Chat::Red, "There was an error in your request: ID not found! Please check the Id and try again.");
				return;
			}

			c->Message(Chat::Yellow, "%s, Updated petition comment to ( %s ) for petition: %i", c->GetName(), sep->arg[3], atoi(sep->arg[2]));
		}
		else
			c->Message(Chat::White, "Your access level is not high enough to use this command.");
	}
	else if (strcasecmp(sep->arg[1], "delete") == 0)
	{
		if (admin >= 90)
		{
			if (sep->arg[1][0] == 0 || sep->arg[2][0] == 0 || strcasecmp(sep->arg[2], "*") == 0)
			{
				c->Message(Chat::White, "Usage: #petition delete (petition number) Type #petition list for a list");
				return;
			}

			c->Message(Chat::Red, "Attempting to delete petition number: %i", atoi(sep->arg[2]));
			std::string query = StringFormat("DELETE FROM petitions WHERE petid = %i", atoi(sep->arg[2]));
			auto results = database.QueryDatabase(query);
			if (!results.Success())
				return;

			petition_list.ReadDatabase();
			Log(Logs::Detail, Logs::Normal, "Delete petition request from %s, petition number:", c->GetName(), atoi(sep->arg[2]));
		}
		else
			c->Message(Chat::White, "Your access level is not high enough to use this command.");
	}
	else
	{
		int size = sizeof(help) / sizeof(std::string);
		for (int i = 0; i < size; i++)
		{
			c->Message(Chat::White, help[i].c_str());
		}
	}
}

