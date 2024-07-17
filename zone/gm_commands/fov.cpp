#include "../client.h"

void command_fov(Client *c, const Seperator *sep){
	if (c->GetTarget())
		if (c->BehindMob(c->GetTarget(), c->GetX(), c->GetY()))
			c->Message(Chat::White, "You are behind mob %s, it is looking to %d", c->GetTarget()->GetName(), c->GetTarget()->GetHeading());
		else
			c->Message(Chat::White, "You are NOT behind mob %s, it is looking to %d", c->GetTarget()->GetName(), c->GetTarget()->GetHeading());
	else
		c->Message(Chat::White, "I Need a target!");
}

