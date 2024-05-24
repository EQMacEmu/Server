#include "../client.h"

void command_invul(Client *c, const Seperator *sep)
{
	int arguments = sep->argnum;
	if (!arguments) {
		c->Message(CC_Default, "Usage: #invul [On|Off]");
		return;
	}

	bool invul_flag = atobool(sep->arg[1]);
	Client* target = c;
	if (c->GetTarget() && c->GetTarget()->IsClient()) {
		target = c->GetTarget()->CastToClient();
	}

	target->SetInvul(invul_flag);
	uint32 account = target->AccountID();
	database.SetGMInvul(account, invul_flag);
	c->Message(
		CC_Default,
		fmt::format(
			"{} {} now {}.",
			c == target ? "You" : target->GetCleanName(),
			c == target ? "are" : "is",
			invul_flag ? "invulnerable" : "vulnerable"
		).c_str()
	);
}

