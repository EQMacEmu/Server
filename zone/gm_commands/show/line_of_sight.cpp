#include "../../client.h"
#include "../../water_map.h"

void ShowLineOfSight(Client* c, const Seperator* sep)
{
	if (!c->GetTarget() || c->GetTarget() == c) {
		c->Message(Chat::White, "You must have a target to use this command.");
		return;
	}

	const auto t = c->GetTarget();

	const bool has_los = c->CheckLosFN(t);
	const bool has_los_region = c->CheckRegion(t, false);
	const bool has_los_to = c->GetTarget()->CheckLosFN(c);

	c->Message(
		Chat::White,
		fmt::format(
			"You {}have line of sight to {}.",
			has_los ? "" : "do not ",
			c->GetTargetDescription(t)
		).c_str()
	);

	c->Message(
		Chat::White,
		fmt::format(
			"You are in {}region than {}.",
			has_los_region ? "the same " : "a different ",
			c->GetTargetDescription(t)
		).c_str()
	);

	c->Message(
		Chat::White,
		fmt::format(
			"{} {}LOS to you",
			c->GetTargetDescription(t),
			has_los_to ? "has " : "does not have "
		).c_str()
	);
}

