#include "../client.h"

void command_doanim(Client *c, const Seperator *sep){
	DoAnimation animation = static_cast<DoAnimation>(atoi(sep->arg[1]));
	if (!sep->IsNumber(1))
		c->Message(Chat::White, "Usage: #DoAnim [number]");
	else
		if (c->Admin() >= commandDoAnimOthers)
			if (c->GetTarget() == 0)
				c->Message(Chat::White, "Error: You need a target.");
			else
				c->GetTarget()->DoAnim(animation, atoi(sep->arg[2]));
		else
			c->DoAnim(animation, atoi(sep->arg[2]));
}

