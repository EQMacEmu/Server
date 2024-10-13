#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include "../common/misc_functions.h"
#include "ucs.h"
#include "world_config.h"
#include "queryserv.h" 


#include "../common/md5.h"
#include "../common/packet_dump.h"

extern QueryServConnection QSLink;

UCSConnection::UCSConnection()
{
	Stream = 0;
}

void UCSConnection::SetConnection(std::shared_ptr<EQ::Net::ServertalkServerConnection> inStream)
{
	if(Stream && Stream->Handle()) {
		Log(Logs::Detail, Logs::UCSServer, "Incoming UCS Connection while we were already connected to a UCS.");
		Stream->Handle()->Disconnect();
	}

	Stream = inStream;

	m_keepalive.reset(new EQ::Timer(5000, true, std::bind(&UCSConnection::OnKeepAlive, this, std::placeholders::_1)));
	Stream->OnMessage(std::bind(&UCSConnection::ProcessPacket, this, std::placeholders::_1, std::placeholders::_2));
}

void UCSConnection::ProcessPacket(uint16 opcode, EQ::Net::Packet& p)
{
	if (!Stream) {
		return;
	}

	ServerPacket tpack(opcode, p);
	ServerPacket* pack = &tpack;


	switch(pack->opcode)
	{
		case 0:
			break;

		case ServerOP_KeepAlive: {
			// ignore this
			break;
		}
		case ServerOP_ZAAuth: {
			Log(Logs::Detail, Logs::UCSServer, "Got authentication from UCS when they are already authenticated.");
			break;
		}
		default: {
			Log(Logs::Detail, Logs::UCSServer, "Unknown ServerOPcode from UCS 0x%04x, size %d", pack->opcode, pack->size);
			DumpPacket(pack->pBuffer, pack->size);
			break;
		}
	}
}

void UCSConnection::SendPacket(ServerPacket* pack)
{
	if(!Stream) {
		return;
	}

	Stream->SendPacket(pack);
}

void UCSConnection::SendMessage(const char *From, const char *Message)
{
	auto pack = new ServerPacket(ServerOP_UCSMessage, strlen(From) + strlen(Message) + 2);

	char *Buffer = (char *)pack->pBuffer;

	VARSTRUCT_ENCODE_STRING(Buffer, From);
	VARSTRUCT_ENCODE_STRING(Buffer, Message);

	SendPacket(pack);
	safe_delete(pack);
}

void UCSConnection::OnKeepAlive(EQ::Timer* t)
{
	if (!Stream) {
		return;
	}

	ServerPacket pack(ServerOP_KeepAlive, 0);
	Stream->SendPacket(&pack);
}
