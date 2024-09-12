#include "../../client.h"

void SetAAPoints(Client* c, const Seperator* sep)
{
	const auto arguments = sep->argnum;
	if (arguments < 2 || !sep->IsNumber(2)) {
		c->Message(Chat::White, "Usage: #set aa_points [aa|group|raid] [Amount]");
		return;
	}

	auto t = c;

	if (c->GetTarget() && c->GetTarget()->IsClient()) {
		t = c->GetTarget()->CastToClient();
	}

	const uint32       aa_points = Strings::ToUnsignedInt(sep->arg[2]);

	t->GetPP().aapoints = aa_points;
	t->GetPP().expAA = 0;
	t->SendAAStats();

	c->Message(
		Chat::White,
		fmt::format(
			"{} now {} {} AA Point{}.",
			c->GetTargetDescription(t, TargetDescriptionType::UCYou),
			c == t ? "have" : "has",
			Strings::Commify(aa_points),
			aa_points != 1 ? "s" : ""
		).c_str()
	);
}
