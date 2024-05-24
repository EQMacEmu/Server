#include "../client.h"

void command_showfilters(Client *c, const Seperator *sep) {

	if (c)
	{
		for (int i = 0; i < 17; i++)
		{
			c->Message(CC_Yellow, "ServerFilter (%i) = %i", i, (int)c->GetFilter(static_cast<eqFilterType>(i)));
		}
	}
}

