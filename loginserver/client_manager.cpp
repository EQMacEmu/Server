#include "client_manager.h"
#include "login_server.h"

#include "../common/eqemu_logsys.h"
#include "../common/file.h"
#include "../common/path_manager.h"
#include "../common/rdp/rdp_connection.h"
#include "../common/rdp/rdp_stream.h"

#include <cstdio>
#include <cstring>
#include <new>
#include <utility>

extern LoginServer server;
extern bool run_server;

void CheckOldOpcodeFile(const std::string& path)
{
	if (File::Exists(path)) {
		return;
	}

	auto f = fopen(path.c_str(), "w");
	if (f) {
		fprintf(f, "#EQEmu Public Login Server Opcodes\n");
		fprintf(f, "# Opcode values are the client's logical values. EQ application opcodes are\n");
		fprintf(f, "# serialized little-endian by the RDP stream.\n");
		fprintf(f, "OP_SessionReady=0x0059\n");
		fprintf(f, "OP_LoginOSX=0x008e\n");
		fprintf(f, "OP_LoginPC=0x0001\n");
		fprintf(f, "OP_ClientError=0x0002\n");
		fprintf(f, "OP_LoginDisconnect=0x0005\n");
		fprintf(f, "OP_ServerListRequest=0x0046\n");
		fprintf(f, "OP_PlayEverquestRequest=0x0047\n");
		fprintf(f, "OP_LoginUnknown1=0x0048\n");
		fprintf(f, "OP_LoginUnknown2=0x004A\n");
		fprintf(f, "OP_LoginAccepted=0x0004\n");
		fprintf(f, "OP_LoginComplete=0x0088\n");
		fprintf(f, "OP_ServerName=0x0049\n");
		fprintf(f, "OP_LoginBanner=0x0052\n");
		fclose(f);
	}
}

ClientManager::ClientManager()
{
	std::string opcode_file = server.config.GetVariableString("Old", "opcodes", "login_opcodes_oldver.conf");
	std::string opcode_path = fmt::format(
		"{}/{}",
		PathManager::Instance()->GetOpcodePath(),
		opcode_file
	);

	CheckOldOpcodeFile(opcode_path);

	if (!m_packet_translator.LoadOpcodes(opcode_path.c_str())) {
		LogError("ClientManager fatal error: couldn't load login opcodes from [{}]", opcode_path);
		run_server = false;
		return;
	}

	int result = m_rdp_runtime.Open();
	if (result != RDPLIB_OK) {
		LogError("ClientManager fatal error: couldn't open the RDP runtime, result [{}]", result);
		run_server = false;
		return;
	}

	uint16 mac_port = server.options.GetMacPort();
	result = m_rdp_endpoint.Open(m_rdp_runtime, mac_port);
	if (result != RDPLIB_OK) {
		LogError("ClientManager fatal error: couldn't open the Mac client RDP listener on port [{}], result [{}]", mac_port, result);
		run_server = false;
		return;
	}

	LogInfo("ClientManager listening for Mac RDP clients on port [{}]", mac_port);

	uint16 trilogy_port = server.options.GetTrilogyPort();
	if (trilogy_port != 0 && trilogy_port != mac_port) {
		result = m_rdp_endpoint_trilogy.Open(m_rdp_runtime, trilogy_port);
		if (result != RDPLIB_OK) {
			LogError("ClientManager warning: couldn't open the Trilogy client RDP listener on port [{}], result [{}]", trilogy_port, result);
		}
		else {
			LogInfo("ClientManager listening for Trilogy RDP clients on port [{}]", trilogy_port);
		}
	}
}

ClientManager::~ClientManager() = default;

void ClientManager::Process()
{
	int process_result = m_rdp_endpoint.Process();
	if (process_result < 0) {
		LogError("Unable to process the login RDP endpoint, result [{}]", process_result);
		run_server = false;
		return;
	}

	if (m_rdp_endpoint_trilogy.IsOpen()) {
		process_result = m_rdp_endpoint_trilogy.Process();
		if (process_result < 0) {
			LogError("Unable to process the Trilogy login RDP endpoint, result [{}]", process_result);
		}
	}

	AcceptClients();
	if (!run_server)
		return;

	ClientList::iterator iter = clients.begin();
	while (iter != clients.end()) {
		if ((*iter)->Process() == false) {
			LogInfo("Client disconnected from the login server, removing client");
			iter = RemoveClient(iter);
		}
		else {
			++iter;
		}
	}
}

void ClientManager::AcceptClients()
{
	AcceptClientsFromEndpoint(m_rdp_endpoint, intel);
	if (m_rdp_endpoint_trilogy.IsOpen()) {
		AcceptClientsFromEndpoint(m_rdp_endpoint_trilogy, trilogy);
	}
}

void ClientManager::AcceptClientsFromEndpoint(RDPEndpoint &endpoint, LSMacClientVersion default_version)
{
	int accept_result = RDPLIB_OK;
	for (uint32 accepted_connections = 0; accepted_connections < MaximumClientAcceptsPerTick; ++accepted_connections) {
		std::unique_ptr<RDPConnection> connection(endpoint.Accept(&accept_result));
		if (connection == nullptr)
			break;

		uint8 remote_address[4] = {};
		uint16 remote_port = 0;
		int setup_result = connection->GetRemoteAddress(remote_address, remote_port);
		if (setup_result != RDPLIB_OK) {
			LogError("Unable to read a new login RDP client's address, result [{}]", setup_result);
			connection->Close(0);
			continue;
		}

		std::unique_ptr<RDPStream> stream;
		try {
			stream.reset(new RDPStream(m_packet_translator, std::move(connection)));
		}
		catch (const std::bad_alloc &) {
			accept_result = RDPLIB_ERROR_OUT_OF_MEMORY;
			break;
		}

		setup_result = stream->EnableKeepalive();
		if (setup_result == RDPLIB_OK)
			setup_result = stream->SetDataRate();
		if (setup_result == RDPLIB_OK)
			setup_result = stream->SetSendBufferSize();

		if (setup_result != RDPLIB_OK) {
			LogError("Unable to configure a new login RDP client, result [{}]", setup_result);
			stream->Close(0);
			continue;
		}

		struct in_addr in = {};
		std::memcpy(&in.s_addr, remote_address, sizeof(in.s_addr));
		LogInfo("New login client connection from [{}]:[{}] (default_version={})", inet_ntoa(in), remote_port, (default_version == trilogy ? "Trilogy" : "Mac"));

		try {
			std::unique_ptr<Client> client(new Client(std::move(stream), default_version));
			clients.emplace_back(std::move(client));
		}
		catch (const std::bad_alloc &) {
			accept_result = RDPLIB_ERROR_OUT_OF_MEMORY;
			break;
		}
	}

	if (accept_result != RDPLIB_OK) {
		LogError("Unable to accept a login RDP client, result [{}]", accept_result);
		if (accept_result == RDPLIB_ERROR_OUT_OF_MEMORY)
			run_server = false;
	}
}

ClientManager::ClientList::iterator ClientManager::RemoveClient(ClientList::iterator client)
{
	return clients.erase(client);
}

void ClientManager::UpdateServerList()
{
	ClientList::iterator iter = clients.begin();
	while (iter != clients.end()) {
		(*iter)->SendServerListPacket();
		++iter;
	}
}

void ClientManager::RemoveExistingClient(unsigned int account_id)
{
	ClientList::iterator iter = clients.begin();
	while (iter != clients.end()) {
		if ((*iter)->GetAccountID() == account_id) {
			LogInfo("Client attempting to log in existing client already logged in, removing existing client");
			iter = RemoveClient(iter);
		}
		else {
			++iter;
		}
	}
}

Client *ClientManager::GetClient(unsigned int account_id)
{
	Client *cur = nullptr;
	int count = 0;
	ClientList::iterator iter = clients.begin();
	while (iter != clients.end()) {
		if ((*iter)->GetAccountID() == account_id) {
			cur = iter->get();
			count++;
		}
		++iter;
	}

	if (count > 1) {
		LogError("More than one client with a given account_id existed in the client list.");
	}
	return cur;
}
