#include "../../client.h"

void ShowContentFlags(Client *c, const Seperator *sep)
{
	Client *t = c;
	if (c->GetTarget() && c->GetTarget()->IsClient()) {
		t = c->GetTarget()->CastToClient();
	}
	
	for (auto &f : ContentFlagsRepository::All(database)) {
		c->Message(
			Chat::White,
			fmt::format(
				"id: [{}] | flag name: [{}] | enabled: [{}]", 
				f.id, 
				f.flag_name, 
				f.enabled ? "yes" : "no"
			).c_str()
		);
	}
}
