#include "../../client.h"

void SetRace(Client *c, const Seperator *sep)
{
	const auto arguments = sep->argnum;
	if (arguments < 2 || !sep->IsNumber(2)) {
		c->Message(
			Chat::White,
			fmt::format(
				"Usage: #set race [0-{}] (0 for back to normal)",
				Race::Portal
			).c_str()
		);

		return;
	}

	Mob* t = c;
	if (c->GetTarget()) {
		t = c->GetTarget();
	}

	const uint16 race_id = Strings::ToUnsignedInt(sep->arg[2]);

	if (
		!EQ::ValueWithin(race_id, Race::Doug, Race::Portal)) 
	{
		c->Message(
			Chat::White,
			fmt::format(
				"Usage: #race [0-{}] (0 for back to normal)",
				Race::Portal
			).c_str()
		);

		return;
	}

	t->SendIllusionPacket(race_id);

	c->Message(
		Chat::White,
		fmt::format(
			"{} {} now temporarily a(n) {} ({}).",
			c->GetTargetDescription(t, TargetDescriptionType::UCYou),
			c == t ? "are" : "is",
			GetRaceIDName(race_id),
			race_id
		).c_str()
	);
}
