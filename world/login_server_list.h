#ifndef LOGINSERVERLIST_H_
#define LOGINSERVERLIST_H_

#include "../common/servertalk.h"
#include "../common/linked_list.h"
#include "../common/timer.h"
#include "../common/queue.h"
#include "../common/eq_packet_structs.h"
#include "../common/mutex.h"

class LoginServer;

class LoginServerList {
public:
	LoginServerList();
	~LoginServerList();

	void	Add(const char*, uint16, const char*, const char*, uint8);

	bool	SendInfo();
	bool	SendNewInfo();
	bool	SendStatus();

	bool	SendPacket(ServerPacket* pack);
	bool	SendAccountUpdate(ServerPacket* pack);
	bool	Connected();
	bool	AllConnected();
	bool	CanUpdate();

protected:
	LinkedList<LoginServer*> list;
};

#endif /*LOGINSERVERLIST_H_*/
