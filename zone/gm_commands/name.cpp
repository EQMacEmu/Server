#include "../client.h"

void command_name(Client *c, const Seperator *sep){
	Client *target;

	if ((strlen(sep->arg[1]) == 0) || (!(c->GetTarget() && c->GetTarget()->IsClient())))
		c->Message(Chat::White, "Usage: #name newname (requires player target)");
	else
	{
		target = c->GetTarget()->CastToClient();
		std::string oldname = target->GetName();
		oldname[0] = toupper(oldname[0]);

		if (target->ChangeFirstName(sep->arg[1], c->GetName()))
		{
			c->Message(Chat::White, "Successfully renamed %s to %s", oldname.c_str(), sep->arg[1]);
			// until we get the name packet working right this will work
			c->Message(Chat::White, "Sending player to char select.");
			target->Kick();
		}
		else
			c->Message(Chat::Red, "ERROR: Unable to rename %s. Check that the new name '%s' isn't already taken.", oldname.c_str(), sep->arg[2]);
	}
}

