#include "../client.h"
#include "../petitions.h"

void command_bug(Client *c, const Seperator *sep)
{
	int admin = c->Admin();
	std::string help0 = "Bug commands usage:";
	std::string help1 = "  #bug list - Lists all bugs.";
	std::string help2 = "  #bug view - #bug view (bug number).";
	std::string help3 = "  #bug delete - #bug delete. (bug number).";

	std::string help[] = { help0, help1, help2, help3 };

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
		std::string query = "SELECT id, name, zone, date FROM bugs ORDER BY id";
		auto results = database.QueryDatabase(query);

		if (!results.Success() || results.RowCount() == 0)
		{
			c->Message(Chat::Red, "There was an error in your request: No petitions found!");
			return;
		}

		c->Message(Chat::Red, "ID : Name , Zone , Time Sent");
		for (auto row = results.begin(); row != results.end(); ++row)
		{

			c->Message(Chat::Yellow, " %s:	%s , %s , %s", row[0], row[1], row[2], row[3]);
		}
	}
	else if (strcasecmp(sep->arg[1], "view") == 0)
	{
		if (sep->arg[2][0] == 0)
		{
			c->Message(Chat::White, "Usage: #bug view (bug number) Type #bug list for a list");
			return;
		}
		LogInfo("Bug viewed by [{}], bug number: [{}]", c->GetName(), atoi(sep->arg[2]));
		c->Message(Chat::Red, "ID : Name , Zone , x , y , z , Type , Flag , Target , Status , Client, Time Sent , Bug");
		std::string query = StringFormat("SELECT id, name, zone, x, y, z, type, flag, target, status, ui, date, bug FROM bugs WHERE id = %i", atoi(sep->arg[2]));
		auto results = database.QueryDatabase(query);

		if (!results.Success() || results.RowCount() == 0)
		{
			c->Message(Chat::Red, "There was an error in your request: ID not found! Please check the Id and try again.");
			return;
		}

		auto row = results.begin();

		c->Message(Chat::Yellow, " %s: %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s", row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11]);
		c->Message(Chat::Yellow, " %s ", row[12]);
	}
	else if (strcasecmp(sep->arg[1], "delete") == 0)
	{
		if (admin >= 90)
		{
			if (sep->arg[1][0] == 0 || sep->arg[2][0] == 0 || strcasecmp(sep->arg[2], "*") == 0)
			{
				c->Message(Chat::White, "Usage: #bug delete (bug number) Type #bug list for a list");
				return;
			}

			c->Message(Chat::Red, "Attempting to delete bug number: %i", atoi(sep->arg[2]));
			std::string query = StringFormat("DELETE FROM bugs WHERE id = %i", atoi(sep->arg[2]));
			auto results = database.QueryDatabase(query);
			if (!results.Success())
				return;

			PetitionList::Instance()->ReadDatabase();
			LogInfo("Delete bug request from [{}], bug number: [{}]", c->GetName(), atoi(sep->arg[2]));
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

