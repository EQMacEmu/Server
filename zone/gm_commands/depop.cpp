#include "../client.h"
#include "../corpse.h"

void command_depop(Client *c, const Seperator *sep){
	if (c->GetTarget() == 0 || !(c->GetTarget()->IsNPC() || c->GetTarget()->IsNPCCorpse()))
		c->Message(CC_Default, "You must have a NPC target for this command. (maybe you meant #depopzone?)");
	else {
		c->Message(CC_Default, "Depoping '%s'.", c->GetTarget()->GetName());
		c->GetTarget()->Depop();
	}
}

