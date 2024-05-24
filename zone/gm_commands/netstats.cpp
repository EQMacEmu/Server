#include "../client.h"

void command_netstats(Client *c, const Seperator *sep){
	if (c)
	{
		if (c->GetTarget() && c->GetTarget()->IsClient())
		{
			c->Message(CC_Default, "Sent:");
			c->Message(CC_Default, "Total: %u, per second: %u", c->GetTarget()->CastToClient()->Connection()->GetBytesSent(),
				c->GetTarget()->CastToClient()->Connection()->GetBytesSentPerSecond());
			c->Message(CC_Default, "Recieved:");
			c->Message(CC_Default, "Total: %u, per second: %u", c->GetTarget()->CastToClient()->Connection()->GetBytesRecieved(),
				c->GetTarget()->CastToClient()->Connection()->GetBytesRecvPerSecond());

		}
		else
		{
			c->Message(CC_Default, "Sent:");
			c->Message(CC_Default, "Total: %u, per second: %u", c->Connection()->GetBytesSent(), c->Connection()->GetBytesSentPerSecond());
			c->Message(CC_Default, "Recieved:");
			c->Message(CC_Default, "Total: %u, per second: %u", c->Connection()->GetBytesRecieved(), c->Connection()->GetBytesRecvPerSecond());
		}
	}
}

