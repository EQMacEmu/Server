#include "../client.h"

void command_doanim(Client *c, const Seperator *sep){
	DoAnimation animation = static_cast<DoAnimation>(atoi(sep->arg[1]));
	float speed = 1.0f;
	if(sep->IsNumber(2))
		speed = atof(sep->arg[2]);

	if (!sep->IsNumber(1))
		c->Message(Chat::White, "Usage: #DoAnim [number] [speed - default 1.0]");
	else
		if (c->Admin() >= commandDoAnimOthers)
			if (c->GetTarget() == 0)
				c->Message(Chat::White, "Error: You need a target.");
			else
				c->GetTarget()->DoAnim(animation, speed);
		else
			c->DoAnim(animation, speed);
}
