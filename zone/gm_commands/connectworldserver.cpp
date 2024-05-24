#include "../client.h"
#include "../worldserver.h"
extern WorldServer worldserver;

void command_connectworldserver(Client *c, const Seperator *sep){
	if (worldserver.Connected())
		c->Message(CC_Default, "Error: Already connected to world server");
	else
	{
		c->Message(CC_Default, "Attempting to connect to world server...");
		worldserver.AsyncConnect();
	}
}

