#include "../../client.h"

void SetTimeZone(Client *c, const Seperator *sep)
{
	const auto arguments = sep->argnum;
	if (arguments < 2 || sep->IsNumber(2)) {
		c->Message(Chat::White, "Usage: #timezone HH [MM]");
		c->Message(Chat::White, fmt::format("Current timezone is: {}h {}m", zone->zone_time.getEQTimeZoneHr(), zone->zone_time.getEQTimeZoneMin()).c_str());
		return;
	}

	uint8 minutes = 0;
	uint8 hours = Strings::ToUnsignedInt(sep->arg[2]);

	if (hours > 24) {
		hours = 24;
	}

	if (!sep->IsNumber(3)) {
		minutes = Strings::ToUnsignedInt(sep->arg[3]);

		if (minutes > 59) {
			minutes = 59;
		}
	}

	c->Message(Chat::White, fmt::format("Setting timezone to {} h {} m", hours, minutes).c_str());
	const int new_timezone = ((hours * 60) + minutes);
	zone->zone_time.setEQTimeZone(new_timezone);
	database.SetZoneTZ(zone->GetZoneID(), new_timezone);

	// Update all clients with new TZ.
	auto outapp = new EQApplicationPacket(OP_TimeOfDay, sizeof(TimeOfDay_Struct));

	auto tod = (TimeOfDay_Struct*)outapp->pBuffer;
	zone->zone_time.getEQTimeOfDay(time(0), tod);

	entity_list.QueueClients(c, outapp);
	safe_delete(outapp);
}

