#include "../client.h"

void command_pf(Client *c, const Seperator *sep){
	if (c->GetTarget())
	{
		Mob *who = c->GetTarget();
		c->Message(CC_Default, "POS: (%.2f, %.2f, %.2f)", who->GetX(), who->GetY(), who->GetZ());
		c->Message(CC_Default, "WP: %s (%d/%d)", to_string(who->GetCurrentWayPoint()).c_str(), who->IsNPC()?who->CastToNPC()->GetMaxWp():-1);
		c->Message(CC_Default, "TAR: (%.2f, %.2f, %.2f)", who->GetTarX(), who->GetTarY(), who->GetTarZ());
		c->Message(CC_Default, "TARV: (%.2f, %.2f, %.2f)", who->GetTarVX(), who->GetTarVY(), who->GetTarVZ());
		c->Message(CC_Default, "|TV|=%.2f index=%d", who->GetTarVector(), who->GetTarNDX());
		c->Message(CC_Default, "pause=%d RAspeed=%d", who->GetCWPP(), who->GetRunAnimSpeed());
	}
	else {
		c->Message(CC_Default, "ERROR: target required");
	}
}

