#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include "ucs.h"
#include "world_config.h"
#include "queryserv.h" 


#include "../common/md5.h"
#include "../common/emu_tcp_connection.h"
#include "../common/packet_dump.h"

extern QueryServConnection QSLink;

UCSConnection::UCSConnection()
{
	Stream = 0;
	authenticated = false;
}

void UCSConnection::SetConnection(EmuTCPConnection *inStream)
{
	if(Stream)
	{
		Log(Logs::Detail, Logs::UCSServer, "Incoming UCS Connection while we were already connected to a UCS.");
		Stream->Disconnect();
	}

	Stream = inStream;

	m_keepalive.reset(new EQ::Timer(5000, true, std::bind(&UCSConnection::OnKeepAlive, this, std::placeholders::_1)));
	authenticated = false;
}

bool UCSConnection::Process()
{
	if (!Stream || !Stream->Connected())
		return false;

	ServerPacket *pack = 0;

	while((pack = Stream->PopPacket()))
	{
		if (!authenticated)
		{
			if (WorldConfig::get()->SharedKey.length() > 0)
			{
				if (pack->opcode == ServerOP_ZAAuth && pack->size == 16)
				{
					uint8 tmppass[16];

					MD5::Generate((const uchar*) WorldConfig::get()->SharedKey.c_str(), WorldConfig::get()->SharedKey.length(), tmppass);

					if (memcmp(pack->pBuffer, tmppass, 16) == 0)
						authenticated = true;
					else
					{
						struct in_addr in;
						in.s_addr = GetIP();
						Log(Logs::Detail, Logs::UCSServer, "UCS authorization failed.");
						auto pack = new ServerPacket(ServerOP_ZAAuthFailed);
						SendPacket(pack);
						delete pack;
						Disconnect();
						return false;
					}
				}
				else
				{
					struct in_addr in;
					in.s_addr = GetIP();
					Log(Logs::Detail, Logs::UCSServer, "UCS authorization failed.");
					auto pack = new ServerPacket(ServerOP_ZAAuthFailed);
					SendPacket(pack);
					delete pack;
					Disconnect();
					return false;
				}
			}
			else
			{
				Log(Logs::Detail, Logs::UCSServer,"**WARNING** You have not configured a world shared key in your config file. You should add a <key>STRING</key> element to your <world> element to prevent unauthroized zone access.");
				authenticated = true;
			}
			delete pack;
			continue;
		}
		switch(pack->opcode)
		{
			case 0:
				break;

			case ServerOP_KeepAlive:
			{
				// ignore this
				break;
			}
			case ServerOP_ZAAuth:
			{
				Log(Logs::Detail, Logs::UCSServer, "Got authentication from UCS when they are already authenticated.");
				break;
			}
			default:
			{
				Log(Logs::Detail, Logs::UCSServer, "Unknown ServerOPcode from UCS 0x%04x, size %d", pack->opcode, pack->size);
				DumpPacket(pack->pBuffer, pack->size);
				break;
			}
		}

		delete pack;
	}
	return(true);
}

bool UCSConnection::SendPacket(ServerPacket* pack)
{
	if(!Stream)
		return false;

	return Stream->SendPacket(pack);
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
