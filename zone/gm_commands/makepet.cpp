#include "../client.h"

void command_makepet(Client *c, const Seperator *sep){
	if (sep->arg[1][0] == '\0')
		c->Message(CC_Default, "Usage: #makepet pet_type_name (will not survive across zones)");
	else
		c->MakePet(0, sep->arg[1]);
}

