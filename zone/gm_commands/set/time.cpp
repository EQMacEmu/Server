#include "../../client.h"

void SetTime(Client *c, const Seperator *sep)
{
	const auto arguments = sep->argnum;
	if (arguments < 2 || !sep->IsNumber(2)) {
		c->Message(Chat::White, "Usage: #set time [Hour] [Minute]");

		TimeOfDay_Struct world_time;
		zone->zone_time.getEQTimeOfDay(time(0), &world_time);

		auto time_string = fmt::format("{}:{}{} {}",
			((world_time.hour) % 12) == 0 ? 12 : ((world_time.hour) % 12),
			(world_time.minute < 10) ? "0" : "",
			world_time.minute,
			(world_time.hour >= 12 && world_time.hour < 24) ? "PM" : "AM"
		);

		c->Message(Chat::White, fmt::format("It is now {}.", time_string).c_str());

		return;
	}

	uint8 minutes = 0;
	uint8 hours = Strings::ToUnsignedInt(sep->arg[2]);

	if (hours > 24) {
		hours = 24;
	}


	if (sep->IsNumber(2)) {
		minutes = Strings::ToUnsignedInt(sep->arg[3]);

		if (minutes > 59) {
			minutes = 59;
		}
	}
		
	c->Message(Chat::White, fmt::format("Setting world time to {}:{} ...", hours, minutes).c_str());
		
	zone->SetTime(hours, minutes);
	
	LogInfo("{} :: Setting world time to {}:{} ...", c->GetCleanName(), hours, minutes);
}
