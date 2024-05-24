#include "../client.h"
#include "../water_map.h"

void command_bestz(Client *c, const Seperator *sep)
{
	float xcoord = c->GetX();
	float ycoord = c->GetY();
	if (sep->IsNumber(1) && sep->IsNumber(2))
	{
		xcoord = atof(sep->arg[1]);
		ycoord = atof(sep->arg[2]);
	}

	if (zone->zonemap == nullptr) {
		c->Message(CC_Default, "Map not loaded for this zone");
	}
	else {
		glm::vec3 me;
		me.x = xcoord;
		me.y = ycoord;
		me.z = c->GetZ();
		glm::vec3 hit;
		glm::vec3 bme(me);
		bme.z -= 500;

		float best_z = zone->zonemap->FindBestZ(me, &hit);

		if (best_z != BEST_Z_INVALID)
		{
			c->Message(CC_Default, "Z is %.3f at (%.3f, %.3f).", best_z, me.x, me.y);
		}
		else
		{
			c->Message(CC_Default, "Found no Z.");
		}
	}

	if (zone->watermap == nullptr) {
		c->Message(CC_Default, "Water Region Map not loaded for this zone");
	}
	else {
		WaterRegionType RegionType;
		float z;

		if (c->GetTarget()) {
			z = c->GetTarget()->GetZ();
			auto position = glm::vec3(c->GetTarget()->GetX(), c->GetTarget()->GetY(), z);
			RegionType = zone->watermap->ReturnRegionType(position);
			c->Message(CC_Default,"InWater returns %d", zone->watermap->InWater(position));
			c->Message(CC_Default,"InLava returns %d", zone->watermap->InLava(position));

		}
		else {
			z = c->GetZ();
			auto position = glm::vec3(c->GetX(), c->GetY(), z);
			RegionType = zone->watermap->ReturnRegionType(position);
			c->Message(CC_Default,"InWater returns %d", zone->watermap->InWater(position));
			c->Message(CC_Default,"InLava returns %d", zone->watermap->InLava(position));

		}

		switch (RegionType) {
		case RegionTypeNormal:	{ c->Message(CC_Default, "There is nothing special about the region you are in!"); break; }
		case RegionTypeWater:	{ c->Message(CC_Default, "You/your target are in Water."); break; }
		case RegionTypeLava:	{ c->Message(CC_Default, "You/your target are in Lava."); break; }
		case RegionTypeVWater:	{ c->Message(CC_Default, "You/your target are in VWater (Icy Water?)."); break; }
		case RegionTypePVP:	{ c->Message(CC_Default, "You/your target are in a pvp enabled area."); break; }
		case RegionTypeSlime:	{ c->Message(CC_Default, "You/your target are in slime."); break; }
		case RegionTypeIce:	{ c->Message(CC_Default, "You/your target are in ice."); break; }
		default: c->Message(CC_Default, "You/your target are in an unknown region type %d.", (int)RegionType);
		}
	}


}

