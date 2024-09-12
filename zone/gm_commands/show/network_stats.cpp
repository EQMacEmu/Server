#include "../../client.h"

void ShowNetworkStats(Client* c, const Seperator* sep)
{
	if (c)
	{
		if (c->GetTarget() && c->GetTarget()->IsClient())
		{
			c->Message(Chat::White, "Sent:");
			c->Message(Chat::White, "Total: %u, per second: %u", c->GetTarget()->CastToClient()->Connection()->GetBytesSent(),
				c->GetTarget()->CastToClient()->Connection()->GetBytesSentPerSecond());
			c->Message(Chat::White, "Recieved:");
			c->Message(Chat::White, "Total: %u, per second: %u", c->GetTarget()->CastToClient()->Connection()->GetBytesRecieved(),
				c->GetTarget()->CastToClient()->Connection()->GetBytesRecvPerSecond());

		}
		else
		{
			c->Message(Chat::White, "Sent:");
			c->Message(Chat::White, "Total: %u, per second: %u", c->Connection()->GetBytesSent(), c->Connection()->GetBytesSentPerSecond());
			c->Message(Chat::White, "Recieved:");
			c->Message(Chat::White, "Total: %u, per second: %u", c->Connection()->GetBytesRecieved(), c->Connection()->GetBytesRecvPerSecond());
		}
	}
}

