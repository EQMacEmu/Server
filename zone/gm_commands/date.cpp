#include "../client.h"

void command_date(Client *c, const Seperator *sep)
{
	const auto arguments = sep->argnum;
	if (
		arguments < 1 ||
		!sep->IsNumber(1) ||
		!sep->IsNumber(2) ||
		!sep->IsNumber(3)
		) {
		c->Message(Chat::White, "Usage: #date [Year] [Month] [Day] [Hour] [Minute]");
		c->Message(Chat::White, "Hour and Minute are optional");
		return;
	}

	TimeOfDay_Struct eqTime;
	zone->zone_time.getEQTimeOfDay(time(0), &eqTime);

	const uint16 year = Strings::ToUnsignedInt(sep->arg[1]);
	const uint8  month = Strings::ToUnsignedInt(sep->arg[2]);
	const uint8  day = Strings::ToUnsignedInt(sep->arg[3]);
	const uint8  hour = !sep->IsNumber(4) ? eqTime.hour : Strings::ToUnsignedInt(sep->arg[4]);
	const uint8  minute = !sep->IsNumber(5) ? eqTime.minute : Strings::ToUnsignedInt(sep->arg[5]);
	
	c->Message(Chat::White, fmt::format("Setting world time to {}-{}-{} {}:{}...", year, month, day, hour, minute).c_str());
	zone->SetDate(year, month, day, hour, minute);
}

