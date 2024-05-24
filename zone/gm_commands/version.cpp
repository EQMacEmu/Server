#include "../client.h"

void command_version(Client *c, const Seperator *sep){
	c->Message(CC_Default, "Current version information.");
	c->Message(CC_Default, "	%s", CURRENT_VERSION);
	c->Message(CC_Default, "	Compiled on: %s at %s", COMPILE_DATE, COMPILE_TIME);
	c->Message(CC_Default, "	Last modified on: %s", LAST_MODIFIED);
}

