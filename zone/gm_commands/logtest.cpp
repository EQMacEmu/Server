#include "../client.h"

void command_logtest(Client *c, const Seperator *sep){
	clock_t t = std::clock(); /* Function timer start */
	if (sep->IsNumber(1)){
		uint32 i = 0;
		t = std::clock();
		for (i = 0; i < atoi(sep->arg[1]); i++){
			LogDebug("[{}] Test #2... Took [{}] seconds", i, ((float)(std::clock() - t)) / CLOCKS_PER_SEC);
		}
	}
}

