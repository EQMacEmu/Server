#include "../client.h"

void command_race(Client *c, const Seperator *sep){
	Mob *t = c->CastToMob();

	// Need to figure out max race for LoY/LDoN: going with upper bound of 500 now for testing
	if (sep->IsNumber(1) && atoi(sep->arg[1]) >= 0 && atoi(sep->arg[1]) <= 724) {
		if ((c->GetTarget()) && c->Admin() >= commandRaceOthers)
			t = c->GetTarget();
		t->SendIllusionPacket(atoi(sep->arg[1]));
	}
	else
		c->Message(Chat::White, "Usage: #race [0-724] (0 for back to normal)");
}

