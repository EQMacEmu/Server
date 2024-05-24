#include "../client.h"

void command_rewind(Client *c, const Seperator *sep){
	c->RewindCommand();
	return;
}

