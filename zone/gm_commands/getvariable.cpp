#include "../client.h"

void command_getvariable(Client *c, const Seperator *sep){
	std::string tmp;
	if (database.GetVariable(sep->argplus[1], tmp))
		c->Message(CC_Default, "%s = %s",  sep->argplus[1], tmp.c_str());
	else
		c->Message(CC_Default, "GetVariable(%s) returned false", sep->argplus[1]);
}

