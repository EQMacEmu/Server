#include "../client.h"

void command_listnpcs(Client *c, const Seperator *sep){
	c->Message(0, "Deprecated, use the #list command (#list npcs <search>)");
}

