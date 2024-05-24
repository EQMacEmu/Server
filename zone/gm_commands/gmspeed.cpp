#include "../client.h"

void command_gmspeed(Client *c, const Seperator *sep)
{
	bool   state = atobool(sep->arg[1]);
	Client* t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient()) {
		t = c->GetTarget()->CastToClient();
	}

	if (sep->arg[1][0] != 0) {
		database.SetGMSpeed(t->AccountID(), state ? 1 : 0);
		c->Message(CC_Default, fmt::format("Turning GMSpeed {} for {} (zone to take effect)", state ? "On" : "Off", t->GetName()).c_str());
	}
	else {
		c->Message(CC_Default, "Usage: #gmspeed [on/off]");
	}
}

