#include "../client.h"

void command_freeze(Client *c, const Seperator *sep){
	if (c->GetTarget() != 0)
		c->GetTarget()->SendAppearancePacket(AppearanceType::Animation, Animation::Freeze);
	else
		c->Message(CC_Default, "ERROR: Freeze requires a target.");
}

