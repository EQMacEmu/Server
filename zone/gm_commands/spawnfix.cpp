#include "../client.h"

void command_spawnfix(Client *c, const Seperator *sep)
{
	Mob *targetMob = c->GetTarget();
	if (!targetMob || !targetMob->IsNPC()) {
		c->Message(CC_Default, "Error: #spawnfix: Need an NPC target.");
		return;
	}

	Spawn2* s2 = targetMob->CastToNPC()->respawn2;

	if (!s2) {
		c->Message(CC_Default, "#spawnfix FAILED -- cannot determine which spawn entry in the database this mob came from.");
		return;
	}

	std::string query = StringFormat("UPDATE spawn2 SET x = '%f', y = '%f', z = '%f', heading = '%f' WHERE id = '%i'",
		c->GetX(), c->GetY(), c->GetZ(), c->GetHeading(), s2->GetID());
	auto results = database.QueryDatabase(query);
	if (!results.Success()) {
		c->Message(CC_Red, "Update failed! MySQL gave the following error:");
		c->Message(CC_Red, results.ErrorMessage().c_str());
		return;
	}

    c->Message(CC_Default, "Updating coordinates successful.");
    targetMob->Depop(false);
}

