#include "../../client.h"

void SetBindPoint(Client *c, const Seperator *sep)
{
	auto t = c;
	if (c->GetTarget() && c->GetTarget()->IsClient()) {
		t = c->GetTarget()->CastToClient();
	}
	
	t->SetBindPoint();

	c->Message(
		Chat::White,
		fmt::format(
			"Set Bind Point for {} | Zone: {}",
			c->GetTargetDescription(t, TargetDescriptionType::UCSelf),
			zone->GetZoneDescription()
		).c_str()
	);

	c->Message(
		Chat::White,
		fmt::format(
			"Set Bind Point for {} | XYZH: {:.2f}, {:.2f}, {:.2f}, {:.2f}",
			c->GetTargetDescription(t, TargetDescriptionType::UCSelf),
			t->GetX(),
			t->GetY(),
			t->GetZ(),
			t->GetHeading()
		).c_str()
	);
}

