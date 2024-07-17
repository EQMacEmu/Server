#include "../client.h"
#include "../water_map.h"

void command_checklos(Client *c, const Seperator *sep){
	if (c->GetTarget())
	{
		//		if(c->CheckLos(c->GetTarget()))
		if (c->CheckLosFN(c->GetTarget()))
			c->Message(Chat::White, "You have LOS to %s", c->GetTarget()->GetName());
		else
			c->Message(Chat::White, "You do not have LOS to %s", c->GetTarget()->GetName());
		if (c->CheckRegion(c->GetTarget(), false))
			c->Message(Chat::White, "You are in the same region as %s", c->GetTarget()->GetName());
		else
		{
			c->Message(Chat::White, "You are in a different region than %s", c->GetTarget()->GetName());
			auto position = glm::vec3(c->GetX(), c->GetY(), c->GetZ());
			auto other_position = glm::vec3(c->GetTarget()->GetX(), c->GetTarget()->GetY(), c->GetTarget()->GetZ());
			c->Message(Chat::White,"Your region: %d Target region: %d", zone->watermap->ReturnRegionType(position), zone->watermap->ReturnRegionType(other_position));
		}
		if (c->GetTarget()->CheckLosFN(c))
			c->Message(Chat::White, " %s has LOS to you.", c->GetTarget()->GetName());
		else
			c->Message(Chat::White, " %s does not have LOS to you.", c->GetTarget()->GetName());
		c->Message(Chat::White, " %s is at heading %.3f pitch %.3f", c->GetTarget()->GetName(), c->CalculateHeadingToTarget(c->GetTarget()->GetX(), c->GetTarget()->GetY()), c->CalculatePitchToTarget(c->GetTarget()->GetPosition()));
	}
	else
	{
		c->Message(Chat::White, "ERROR: Target required");
	}
}

