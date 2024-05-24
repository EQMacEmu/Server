#include "../client.h"

void command_stun(Client *c, const Seperator *sep)
{
	int arguments = sep->argnum;
	if (!arguments || !sep->IsNumber(1)) {
		c->Message(CC_Default, "Usage: #stun [Duration]");
		return;
	}

	Mob* target = c;
	int duration = static_cast<int>(std::min(std::stoll(sep->arg[1]), (long long)2000000000));

	if (duration < 0) {
		duration = 0;
	}

	if (c->GetTarget()) {
		target = c->GetTarget();
		if (target->IsClient()) {
			target->CastToClient()->Stun(duration, c);
		}
		else if (target->IsNPC()) {
			target->CastToNPC()->Stun(duration, c);
		}
	}
	else {
		c->Stun(duration, c);
	}

	std::string stun_message = (
		duration ?
		fmt::format(
			"You stunned {} for {}.",
			(
				c == target ?
				"yourself" :
				fmt::format(
					"{} ({})",
					target->GetCleanName(),
					target->GetID()
				)
				),
			Strings::MillisecondsToTime(duration)
		) :
		fmt::format(
			"You unstunned {}.",
			(
				c == target ?
				"yourself" :
				fmt::format(
					"{} ({})",
					target->GetCleanName(),
					target->GetID()
				)
				)
		)
		);
	c->Message(
		CC_Default,
		stun_message.c_str()
	);
}

