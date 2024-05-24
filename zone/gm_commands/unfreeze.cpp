#include "../client.h"

void command_unfreeze(Client *c, const Seperator *sep){
	if (c->GetTarget() != 0)
		c->GetTarget()->SendAppearancePacket(AppearanceType::Animation, Animation::Standing);
	else
		c->Message(CC_Default, "ERROR: Unfreeze requires a target.");
}

