#include "../client.h"
#include "../water_map.h"

void command_loc(Client *c, const Seperator *sep)
{
	Mob* target = c;
	if (c->GetTarget()) {
		target = c->GetTarget();
	}

	auto target_position = target->GetPosition();

	// client heading uses 0 - 511 if target is client.
	c->Message(CC_Default, fmt::format(" {} Location | XYZ: {:.2f}, {:.2f}, {:.2f} Heading: {:.2f} ", (c == target ? "Your" : fmt::format(" {} ({}) ", target->GetCleanName(), target->GetID())), target_position.x, target_position.y, target_position.z, (c == target || target->IsClient()) ? target_position.w * 2 : target_position.w).c_str());
		
	float newz = 0;
	if (!zone->zonemap)
	{
		c->Message(CC_Default, "Map not loaded for this zone.");
	}
	else
	{
		auto z = c->GetZ() + (c->GetSize() == 0.0 ? 6 : c->GetSize()) * HEAD_POSITION;
		auto me = glm::vec3(c->GetX(), c->GetY(), z);
		glm::vec3 hit;
		glm::vec3 bme(me);
		bme.z -= 500;

		auto newz = zone->zonemap->FindBestZ(me, &hit);
		if (newz != BEST_Z_INVALID)
		{
			me.z = target->SetBestZ(newz);
			c->Message(CC_Default, fmt::format("Best Z is {:.2f} ", newz).c_str());
		}
		else
		{
			c->Message(CC_Default, "Could not find Best Z.");
		}
	}

	if (!zone->watermap)
	{
		c->Message(CC_Default, "Water Map not loaded for this zone.");
	}
	else
	{
		auto position = glm::vec3(target->GetX(), target->GetY(), target->GetZ());
		auto region_type = zone->watermap->ReturnRegionType(position);
		auto position_string = fmt::format(" {} is", target->GetCleanName());
		
		switch (region_type) {
			case RegionTypeIce: {
				c->Message(CC_Default, fmt::format("{} in Ice.", position_string).c_str());
				break;
			}
			case RegionTypeLava: {
				c->Message(CC_Default, fmt::format("{} in Lava.", position_string).c_str());
				break;
			}
			case RegionTypeNormal: {
				c->Message(CC_Default, fmt::format("{} in a Normal Region.", position_string).c_str());
				break;
			}
			case RegionTypePVP: {
				c->Message(CC_Default, fmt::format("{} in a PvP Area.", position_string).c_str());
				break;
			}
			case RegionTypeSlime: {
				c->Message(CC_Default, fmt::format("{} in Slime.", position_string).c_str());
				break;
			}
			case RegionTypeVWater: {
				c->Message(CC_Default, fmt::format("{} in VWater (Icy Water?).", position_string).c_str());
				break;
			}
			case RegionTypeWater: {
				c->Message(CC_Default, fmt::format("{} in Water.", position_string).c_str());
				break;
			}
			default: {
				c->Message(CC_Default, fmt::format("{} in an Unknown Region.", position_string).c_str());
				break;
			}
		}

	}

	if(target->CanCastBindAffinity() && (zone->CanBind() || zone->IsCity() || zone->CanBindOthers()))
	{
		c->Message(CC_Default, fmt::format(" {} can bind here.", target->GetCleanName()).c_str());
	}
	else if(!target->CanCastBindAffinity() && (zone->IsCity() || zone->IsBindArea(target->GetX(), target->GetY(), target->GetZ())))
	{
		c->Message(CC_Default, fmt::format(" {} can be bound here.", target->GetCleanName()).c_str());
	}
	else
	{
		c->Message(CC_Default, fmt::format(" {} cannot bind here.", target->GetCleanName()).c_str());
	}
}

