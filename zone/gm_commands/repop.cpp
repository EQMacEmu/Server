#include "../client.h"

void command_repop(Client *c, const Seperator *sep)
{
	bool is_force = false;
	if (sep->arg[1] && !strcasecmp(sep->arg[1], "force")) {
		is_force = true;
	}

	if (!is_force && c->GetTarget() && c->GetTarget()->IsNPC()) {
		c->GetTarget()->CastToNPC()->ForceRepop();
		c->Message(
			Chat::White, 
			fmt::format(
				"Repopping {}", 
				c->GetTarget()->GetName()).c_str()
		);
	}
	else {
		if (is_force) {
			zone->ClearSpawnTimers();
			c->Message(Chat::White, "Zone depopped, forcefully repopping now.");
		}
		else {
			c->Message(Chat::White, "Zone depopped, repopping now.");
		}

		zone->Repop();
	}
}
