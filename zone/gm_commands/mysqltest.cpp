#include "../client.h"

void command_mysqltest(Client *c, const Seperator *sep)
{
	clock_t t = std::clock(); /* Function timer start */
	if (sep->IsNumber(1)){
		uint32 i = 0;
		t = std::clock();
		for (i = 0; i < atoi(sep->arg[1]); i++){
			std::string query = "SELECT * FROM `zone`";
			auto results = database.QueryDatabase(query);
		} 
	}
	Log(Logs::General, Logs::Debug, "MySQL Test... Took %f seconds", ((float)(std::clock() - t)) / CLOCKS_PER_SEC); 
}

