#include "../client.h"

void command_delacct(Client *c, const Seperator *sep){
	if (sep->arg[1][0] == 0)
		c->Message(CC_Default, "Format: #delacct accountname");
	else
		if (database.DeleteAccount(sep->arg[1]))
			c->Message(CC_Default, "The account was deleted.");
		else
			c->Message(CC_Default, "Unable to delete account.");
}

