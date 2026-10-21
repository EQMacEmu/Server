/* EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2010 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#ifndef EQEMU_CLIENTMANAGER_H
#define EQEMU_CLIENTMANAGER_H

#include "../common/global_define.h"
#include "../common/eq_packet_translator.h"
#include "../common/rdp/rdp_endpoint.h"
#include "client.h"

#include <list>
#include <memory>

using namespace std;

/**
* Client manager class, holds all the client objects and does basic processing.
*/
class ClientManager
{
public:
	/**
	* Constructor, sets up the RDP listener and opcode translator.
	*/
	ClientManager();

	/**
	* Destructor, shuts down the clients, listener, translator, and RDP runtime.
	*/
	~ClientManager();

	/**
	* Processes every client in the internal list, removes them if necessary.
	*/
	void Process();

	/**
	* Sends a new server list to every client.
	*/
	void UpdateServerList();

	/**
	* Removes a client with a certain account id.
	*/
	void RemoveExistingClient(unsigned int account_id);

	/**
	* Gets a client (if exists) by their account id.
	*/
	Client *GetClient(unsigned int account_id);

private:
	using ClientList = std::list<std::unique_ptr<Client>>;

	static constexpr uint32 MaximumClientAcceptsPerTick = 5;

	void AcceptClients();
	void AcceptClientsFromEndpoint(RDPEndpoint &endpoint, LSMacClientVersion default_version);
	ClientList::iterator RemoveClient(ClientList::iterator client);

	RDPRuntime m_rdp_runtime;
	EQPacketTranslator m_packet_translator;
	RDPEndpoint m_rdp_endpoint;
	RDPEndpoint m_rdp_endpoint_trilogy;
	ClientList clients;
};

#endif

