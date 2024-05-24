#include "../client.h"
#include "../../common/patches/patches.h"

void command_opcode(Client *c, const Seperator *sep){
	if (!strcasecmp(sep->arg[1], "reload")) {
		ReloadAllPatches();
		c->Message(CC_Default, "Opcodes for all patches have been reloaded");
	}
}

