#include "../client.h"

void command_permarace(Client *c, const Seperator *sep){
	Client *t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient())
		t = c->GetTarget()->CastToClient();

	if (sep->arg[1][0] == 0) {
		c->Message(CC_Default, "Usage: #permarace <racenum>");
		c->Message(CC_Default, "NOTE: Not all models are global. If a model is not global, it will appear as a human on character select and in zones without the model.");
	}
	else if (!t->IsClient())
		c->Message(CC_Default, "Target is not a client.");
	else {
		c->Message(CC_Default, "Setting %s's race - zone to take effect",t->GetName());
		Log(Logs::General, Logs::Normal, "Permanant race change request from %s for %s, requested race:%i", c->GetName(), t->GetName(), atoi(sep->arg[1]) );
		uint32 tmp = Mob::GetDefaultGender(atoi(sep->arg[1]), t->GetBaseGender());
		t->SetBaseRace(atoi(sep->arg[1]));
		t->SetBaseGender(tmp);
		t->Save();
		t->SendIllusionPacket(atoi(sep->arg[1]));
	}
}

