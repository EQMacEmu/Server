/*	EQEMu: Everquest Server Emulator
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
#ifndef EQEMU_CLIENT_H
#define EQEMU_CLIENT_H

#include "../common/global_define.h"
#include "../common/random.h"
#include "../common/types.h"
#include "login_types.h"
#include "salt.h"

#include <memory>
#include <string>

using namespace std;

class EQApplicationPacket;
class RDPStream;

/**
* Client class, controls a single client and it's
* connection to the login server.
*/
class Client
{
public:
	/**
	* Constructor takes ownership of the client stream.
	*/
	Client(std::unique_ptr<RDPStream> stream, LSMacClientVersion default_version = intel);

	/**
	* Destructor.
	*/
	~Client();

	/**
	* Processes the client's connection and does various actions.
	*/
	bool Process();

	/**
	* Sends our reply to session ready packet.
	*/
	void Handle_SessionReady();

	/**
* Verifies login and send a reply for Mac clients.
*/
	void Handle_Login(const char* data, unsigned int size, std::string client);

	/**
	* Not sure what this is, old clients need it to continue.
	*/
	void Handle_LoginComplete(const char* data, unsigned int size);

	/**
	* For all old clients, this disconnects them.
	*/
	void FatalError(const char* message);

	/**
	* Sends a packet to the requested server to see if the client is allowed or not.
	*/
	void Handle_Play(const char* data);

	/**
	* Sends a server list packet to the client.
	*/
	void SendServerListPacket();

	/**
	* sends a banner packet to the client
	*/
	void Handle_Banner(unsigned int size);

	/**
	* Sends the input packet to the client and clears our play response states.
	*/
	void SendPlayResponse(EQApplicationPacket *outapp);

	/**
	* Generates a random login key for the client during login.
	*/
	void GenerateKey();

	/**
	* Gets the account id of this client.
	*/
	unsigned int GetAccountID() const { return m_account_id; }

	/**
	* Gets the account name of this client.
	*/
	string GetAccountName() const { return m_account_name; }

	/**
	 * Returns a description for the client for logging
	 * @return std::string
	 */
	std::string GetClientDescription();

	/**
	* Gets the key generated at login for this client.
	*/
	string GetKey() const { return m_key; }

	uint32 GetIP() const { return m_ip; }
	uint16 GetPort() const { return m_port; }
	bool HasStream() const { return m_stream != nullptr; }

	/**
	* Gets the client version for this client.
	*/
	unsigned int GetMacClientVersion() const { return m_client_mac_version; }

	bool IsTrilogy() const { return m_is_trilogy || m_client_mac_version == trilogy; }
	void SetTrilogy(bool trilogy_client) { m_is_trilogy = trilogy_client; if (trilogy_client) m_client_mac_version = trilogy; }

private:
	void QueuePacket(const EQApplicationPacket *packet, bool reliable = true);
	void SendToStream(EQApplicationPacket **packet, bool reliable);
	void CloseStream();
	void CloseStream(uint32 linger_timeout_ms);

	Saltme                             m_salt;
	EQ::Random                         m_random;
	LSClientStatus                     m_client_status;
	LSMacClientVersion                 m_client_mac_version;
	bool                               m_is_trilogy;

	std::string  m_account_name;
	unsigned int m_account_id;
	bool         m_sent_session_info;
	std::string  m_key;
	uint32       m_ip;
	uint16       m_port;
	std::unique_ptr<RDPStream> m_stream;
};

#endif

