#include "../client.h"

void command_time(Client *c, const Seperator *sep)
{
	const auto arguments = sep->argnum;
	if (arguments < 1 || !sep->IsNumber(1)) {
		c->Message(Chat::White, "To set the Time: #time HH [MM]");

		TimeOfDay_Struct eqTime;
		zone->zone_time.getEQTimeOfDay(time(0), &eqTime);

		auto time_string = fmt::format("{}:{}{} {}",
			((eqTime.hour) % 12) == 0 ? 12 : ((eqTime.hour) % 12),
			(eqTime.minute < 10) ? "0" : "",
			eqTime.minute,
			(eqTime.hour >= 12 && eqTime.hour < 24) ? "PM" : "AM"
		);

		c->Message(Chat::White, fmt::format("It is now {}.", time_string).c_str());

		return;
	}

	uint8 minutes = 0;
	uint8 hours = Strings::ToUnsignedInt(sep->arg[1]);

	if (hours > 24) {
		hours = 24;
	}


	if (sep->IsNumber(2)) {
		minutes = Strings::ToUnsignedInt(sep->arg[2]);

		if (minutes > 59) {
			minutes = 59;
		}
	}
		
	c->Message(Chat::White, fmt::format("Setting world time to {}:{} ...", hours, minutes).c_str());
		
	zone->SetTime(hours, minutes);
	
	LogInfo("{} :: Setting world time to {}:{} ...", c->GetCleanName(), hours, minutes);
}

