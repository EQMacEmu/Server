#include "../client.h"

void command_kill(Client *c, const Seperator *sep)
{
	auto target = c->GetTarget();
	if (!target) 
	{
		c->Message(CC_Default, "You must have a target to use this command.");
		return;
	}

	if (!target->IsClient() || target->CastToClient()->Admin() <= c->Admin())
	{
		if (c != target)
		{
			c->Message(CC_Default, fmt::format("Killing {} .", target->GetCleanName()).c_str());

		}
		target->Kill();
	}
}

