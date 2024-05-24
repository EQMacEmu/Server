#include "../client.h"

void command_disablerecipe(Client *c, const Seperator *sep){
	uint32 recipe_id = 0;
	bool success = false;
	if (c) {
		if (sep->argnum == 1) {
			recipe_id = atoi(sep->arg[1]);
		}
		else {
			c->Message(CC_Default, "Invalid number of arguments.\nUsage: #disablerecipe recipe_id");
			return;
		}
		if (recipe_id > 0) {
			success = database.DisableRecipe(recipe_id);
			if (success) {
				c->Message(CC_Default, "Recipe disabled.");
			}
			else {
				c->Message(CC_Default, "Recipe not disabled.");
			}
		}
		else {
			c->Message(CC_Default, "Invalid recipe id.\nUsage: #disablerecipe recipe_id");
		}
	}
}

