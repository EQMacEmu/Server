#include "../client.h"

void command_resetboat(Client* c, const Seperator *sep){
	if (c->GetTarget() != 0 && c->GetTarget()->IsClient()){
		c->GetTarget()->CastToClient()->SetBoatID(0);
		c->GetTarget()->CastToClient()->SetBoatName("");
		c->Message(Chat::Red, "Successfully removed %s from a boat in their PP.", c->GetTarget()->GetName());
	}
	else
		c->Message(Chat::White, "Usage: Target a client and use #resetboat to remove any boat in their Profile.");
}

