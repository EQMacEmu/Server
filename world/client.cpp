#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include "../common/eq_packet.h"
#include "../common/rdp/rdp_stream.h"
#include "../common/misc.h"
#include "../common/rulesys.h"
#include "../common/emu_opcodes.h"
#include "../common/eq_packet_structs.h"
#include "../common/packet_dump.h"
#include "../common/inventory_profile.h"
#include "../common/races.h"
#include "../common/classes.h"
#include "../common/skills.h"
#include "../common/extprofile.h"
#include "../common/strings.h"
#include "../common/emu_versions.h"
#include "../common/random.h"
#include "../common/data_verification.h"
#include "../common/opcodemgr.h"
#include "../common/zone_store.h"
#include "../common/content/world_content_service.h"
#include "../common/skill_caps.h"
#include "../common/patches/patches.h"
#include "../common/deity.h"

#include "client.h"
#include "worlddb.h"
#include "world_config.h"
#include "login_server.h"
#include "login_server_list.h"
#include "zoneserver.h"
#include "zonelist.h"
#include "clientlist.h"
#include "wguild_mgr.h"
#include "char_create_data.h"
#include "../common/repositories/player_event_logs_repository.h"
#include "../common/events/player_event_logs.h"

#include <iostream>
#include <iomanip>
#include <memory>
#include <utility>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <limits.h>

//FatherNitwit: uncomment to enable my IP based authentication hack
//#define IPBASED_AUTH_HACK

// Disgrace: for windows compile
#ifdef _WINDOWS
	#include <winsock2.h>
	#include <windows.h>
#else
	
	#ifdef FREEBSD //Timothy Whitman - January 7, 2003
		#include <sys/types.h>
	#endif

	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

std::vector<RaceClassAllocation> character_create_allocations;
std::vector<RaceClassCombos> character_create_race_class_combos;

extern uint32 numclients;
extern volatile bool RunLoops;
extern volatile bool UCSServerAvailable_;

Client::Client(std::unique_ptr<RDPStream> stream)
	: autobootup_timeout(RuleI(World, ZoneAutobootTimeoutMS)),
	  initial_connection_timer(InitialConnectionTimeoutMs),
	  connect(1000),
	  m_session_disposition(SessionDisposition::ReleaseOnTransportEnd),
	  m_stream(std::move(stream))
{
	uint8 remote_address[4] = {};
	ip = 0;
	port = 0;
	if (m_stream == nullptr || m_stream->GetRemoteAddress(remote_address, port) != RDPLIB_OK)
	{
		LogError("Client was created without a usable RDP stream");
	}
	else
	{
		memcpy(&ip, remote_address, sizeof(ip));
	}

	autobootup_timeout.Disable();
	connect.Disable();
	seen_character_select = false;
	cle = 0;
	zone_id = 0;
	world_entrance_request_id = 0;
	char_name[0] = 0;
	char_id = 0;
	zone_waiting_for_bootup = 0;
	enter_world_triggered = false;
	m_ClientVersionBit = 0;
	memset(m_char_primary, 0, sizeof(m_char_primary));
	memset(m_char_secondary, 0, sizeof(m_char_secondary));
	memset(m_char_primary_color, 0, sizeof(m_char_primary_color));
	memset(m_char_secondary_color, 0, sizeof(m_char_secondary_color));
	numclients++;
}

bool Client::MatchesWorldEntranceRequest(uint32 request_id, uint32 account_id, uint32 character_id, uint32 requested_zone_id) const
{
	return request_id != 0 &&
		world_entrance_request_id == request_id &&
		cle != nullptr &&
		cle->AccountID() == account_id &&
		char_id == character_id &&
		zone_id == requested_zone_id;
}

Client::~Client()
{
	if (RunLoops && cle && cle->Online() != CLE_Status::Offline && m_session_disposition == SessionDisposition::ReleaseOnTransportEnd)
	{
		cle->SetOnline(CLE_Status::Offline);
	}

	numclients--;

	CloseStream(0);
}

void Client::SendLogServer()
{
	auto outapp = new EQApplicationPacket(OP_LogServer, sizeof(LogServer_Struct));
	LogServer_Struct *l=(LogServer_Struct *)outapp->pBuffer;

	if(RuleI(World, FVNoDropFlag) == 1 || RuleI(World, FVNoDropFlag) == 2 && GetAdmin() > RuleI(Character, MinStatusForNoDropExemptions))
		l->enable_FV = 1;

	l->enable_pvp = (RuleI(World, PVPSettings));

	l->auto_identify = 0;
	l->NameGen = 1;
	l->Gibberish = 1;
	l->test_server = 0;
	l->Locale = 0;
	l->ProfanityFilter = 0;

	const char *wsn=WorldConfig::get()->ShortName.c_str();
	memcpy(l->worldshortname,wsn,strlen(wsn));
	memcpy(l->loggingServerAddress, "127.0.0.1", 10);
	l->loggingServerPort = 9878;

	QueuePacket(outapp);
	safe_delete(outapp);
}

bool Client::SendEnterWorld(std::string name)
{
	char char_name[64] = { 0 };
	if (is_player_zoning && database.GetLiveChar(GetAccountID(), char_name)) {
		if(database.GetAccountIDByChar(char_name) != GetAccountID()) {
			return false;
		} else {
			LogInfo("Telling client to continue session.");
		}
	}

	auto outapp = new EQApplicationPacket(OP_EnterWorld, strlen(char_name) + 1);
	memcpy(outapp->pBuffer,char_name,strlen(char_name)+1);
	QueuePacket(outapp);
	safe_delete(outapp);
	return true;
}

void Client::SendExpansionInfo() {
	auto outapp = new EQApplicationPacket(OP_ExpansionInfo, sizeof(ExpansionInfo_Struct));
	ExpansionInfo_Struct *eis = (ExpansionInfo_Struct*)outapp->pBuffer;
	if (GetExpansion() > 15)
		eis->Expansions = 15;
	else
		eis->Expansions = GetExpansion();
	QueuePacket(outapp);
	safe_delete(outapp);
}

void Client::SendCharInfo() {

	if (cle) {
		cle->SetOnline(CLE_Status::CharSelect);
	}

	seen_character_select = true;

	// Send OP_SendCharInfo
	auto outapp = new EQApplicationPacket(OP_SendCharInfo, sizeof(CharacterSelect_Struct));
	CharacterSelect_Struct* cs = (CharacterSelect_Struct*)outapp->pBuffer;
	memset(cs, 0, sizeof(CharacterSelect_Struct));

	charcount = 0;
	database.GetCharSelectInfo(GetAccountID(), cs, m_ClientVersionBit, charcount, mule);

	for (int i = 0; i < 10; ++i) {
		m_char_primary[i] = cs->primary[i];
		m_char_secondary[i] = cs->secondary[i];
		m_char_primary_color[i] = cs->cs_colors[i].Slot[EQ::textures::weaponPrimary].Color;
		m_char_secondary_color[i] = cs->cs_colors[i].Slot[EQ::textures::weaponSecondary].Color;
	}

	LogInfo("SendCharInfo: AccountID [{}], charcount [{}]", GetAccountID(), charcount);
	for (int i = 0; i < 10; ++i) {
		LogInfo("  Slot [{}]: Name [{}], Level [{}], Class [{}], Race [{}], Zone [{}], Primary [{}], Secondary [{}]",
			i, cs->name[i], cs->level[i], cs->class_[i], cs->race[i], cs->zone[i], cs->primary[i], cs->secondary[i]);
	}

	QueuePacket(outapp);
	safe_delete(outapp);
}

bool Client::HandleSendLoginInfoPacket(const EQApplicationPacket *app) {
	if (app->size != sizeof(LoginInfo_Struct)) {
		return false;
	}

	auto *login_info = (LoginInfo_Struct *)app->pBuffer;

	// Quagmire - max len for name is 18, pass 15
	char name[19] = {0};
	char password[16] = {0};
	strn0cpy(name, (char*)login_info->login_info,18);
	strn0cpy(password, (char*)&(login_info->login_info[strlen(name)+1]), 15);

	LogDebug("Receiving Login Info Packet from Client | name [{0}] password [{1}]", name, password);

	if (strlen(password) <= 1) {
		LogInfo("Login without a password");
		return false;
	}

	is_player_zoning = (login_info->zoning==1);

#ifdef IPBASED_AUTH_HACK
	struct in_addr tmpip;
	tmpip.s_addr = ip;
#endif
	uint32 id=0;
	if (strncasecmp(name, "LS#", 3) == 0)
		id=atoi(&name[3]);
	else if(database.GetAccountIDByName(name))
		id=database.GetAccountIDByName(name);
	else
		id=atoi(name);

	if (LoginServerList::Instance()->Connected() == false && !is_player_zoning) {
		LogInfo("Error: Login server login while not connected to login server.");
		return false;
	}

	ClientListEntry *authenticated_cle = ClientList::Instance()->CheckAuth(name, password);
	if (!authenticated_cle)
		authenticated_cle = ClientList::Instance()->CheckAuth(id, password);

	if (authenticated_cle)
	{
		if (authenticated_cle->Online() >= CLE_Status::Zoning)
			is_player_zoning = true;

		if (GetSessionLimit(authenticated_cle, is_player_zoning))
			return false;

		cle = authenticated_cle;
		initial_connection_timer.Disable();

		if(cle->Online() < CLE_Status::Online)
			cle->SetOnline();
		
		// Check for Trilogy or Mac client versions and assign the stream's packet translator.
		if (cle->GetMacClientVersion() == EQ::versions::ClientVersionBit::bit_Trilogy)
		{
			m_ClientVersion = EQ::versions::ClientVersion::Trilogy;
			m_ClientVersionBit = cle->GetMacClientVersion();
			if (m_stream) {
				m_stream->SetTranslator(Patches::GetTranslator(m_ClientVersion));
			}
		}
		else
		{
			if (cle->GetMacClientVersion() != EQ::versions::ClientVersionBit::bit_MacPC)
			{
				cle->SetMacClientVersion(login_info->macversion);
			}
			m_ClientVersionBit = cle->GetMacClientVersion();
			m_ClientVersion = EQ::versions::ClientVersion::Mac;
			if (m_stream) {
				m_stream->SetTranslator(Patches::GetTranslator(m_ClientVersion));
			}
		}

		LogInfo("ClientVersionBit is: [{}]", m_ClientVersionBit);
		LogInfo("Logged in. Mode= [{}]", is_player_zoning ? "(Zoning)" : "(CharSel)");
		LogInfo("LS Account #[{}]", cle->LSID());

		const WorldConfig *Config=WorldConfig::get();

		if(Config->UpdateStats){
			auto pack = new ServerPacket;
			pack->opcode = ServerOP_LSPlayerJoinWorld;
			pack->size = sizeof(ServerLSPlayerJoinWorld_Struct);
			pack->pBuffer = new uchar[pack->size];
			memset(pack->pBuffer,0,pack->size);
			ServerLSPlayerJoinWorld_Struct* join =(ServerLSPlayerJoinWorld_Struct*)pack->pBuffer;
			strcpy(join->key,GetLSKey());
			join->lsaccount_id = GetLSID();
			LoginServerList::Instance()->SendPacket(pack);
			safe_delete(pack);
		}

		expansion = 0;
		mule = false;
		database.GetAccountRestriction(cle->AccountID(), expansion, mule);

		if (m_ClientVersion == EQ::versions::ClientVersion::Mac) {
			SendLogServer();
		}
		SendApproveWorld();


		if (is_player_zoning)
		{
			uint32 tmpaccid = 0;
			database.GetLiveCharByLSID(id, char_name);
			char_id = database.GetCharacterInfo(char_name, &tmpaccid, &zone_id);
			if (char_id == 0 || tmpaccid != GetAccountID()) {
				LogInfo("Could not get CharInfo for '[{}]'", char_name);
				return false;
			}
			cle->SetChar(char_id, char_name);
			m_session_disposition = SessionDisposition::PreserveOnTransportEnd;
			if (!SendEnterWorld(cle->name()))
				return false;
		}

		if (!is_player_zoning) {
			if (!SendEnterWorld(cle->name()))
				return false;
			SendExpansionInfo();
			SendCharInfo();
			database.LoginIP(cle->AccountID(), long2ip(GetIP()));
		}
	}
	else {
		// TODO: Find out how to tell the client wrong username/password
		LogInfo("Bad/Expired session key '[{}]'",name);
		return false;
	}

	if (!cle)
		return true;

	cle->SetIP(GetIP());
	return true;
}

bool Client::HandleNameApprovalPacket(const EQApplicationPacket *app)
{
	bool valid_size = false;
    if (m_ClientVersion == EQ::versions::ClientVersion::Trilogy) {
        valid_size = (app->size == sizeof(NameApproval_Struct));
    }
    else {
        // Mac client sends 76 bytes where app->Size() == 78
        valid_size = (app->Size() == sizeof(NameApproval_Struct) || app->size == sizeof(NameApproval_Struct));
    }
    if (!valid_size) {
        LogError("Wrong size: HandleNameApprovalPacket, size= [{}], Size= [{}], expected [{}]", app->size, app->Size(), sizeof(NameApproval_Struct));
        return false;
    }

	if (GetAccountID() == 0) {
		LogInfo("Name approval request with no logged in account");
		return false;
	}

	auto na = (NameApproval_Struct *)app->pBuffer;

	strncpy(char_name, na->name, sizeof(char_name));

	const uint16 length   = strlen(na->name);
	const uint16 race_id  = na->race;
	const uint16 class_id = na->class_;

	if (!IsPlayerRace(race_id)) {
		LogInfo("Invalid Race ID.");
		return false;
	}

	if (!IsPlayerClass(class_id)) {
		LogInfo("Invalid Class ID.");
		return false;
	}

	LogInfo(
		"char_name [{}] race_id [{}] class_id [{}]",
		char_name, 
		GetRaceIDName(race_id), 
		GetClassIDName(class_id)
	);

	auto outapp = new EQApplicationPacket(OP_ApproveName, sizeof(NameApprovalReply_Struct));
	NameApprovalReply_Struct* nr = (NameApprovalReply_Struct *)outapp->pBuffer;

	bool is_valid = true;

	if (!EQ::ValueWithin(length, 4, 15)) { /* Name must be between 4 and 15 characters long, packet forged if this is true */
		is_valid = false;
	}
	else if (islower(char_name[0])) { /* Name must begin with an upper-case letter, can be sent with some tricking of the client */
		is_valid = false;
	}
	else if (strstr(char_name, " ")) { /* Name must not have any spaces, packet forged if this is true */
		is_valid = false;
	}
	else if (!database.CheckNameFilter(char_name)) { /* I would like to do this later, since it's likely more expensive, but oh well */
		is_valid = false;
	}
	else { /* Name must not contain any uppercase letters, can be sent with some tricking of the client */
		for (int i = 1; i < length; ++i) {
			if (isupper(char_name[i])) {
				is_valid = false;
				break;
			}
		}
	}

	if (is_valid) { /* Still not invalid, let's see if it's taken */
		is_valid = database.ReserveName(GetAccountID(), char_name);
	}

	nr->approval = is_valid ? 1 : 0;

	QueuePacket(outapp);
	safe_delete(outapp);

	if (!is_valid)
		memset(char_name, 0, sizeof(char_name));

	return true;
}

bool Client::HandleGenerateRandomNamePacket(const EQApplicationPacket *app) {
	// creates up to a 10 char name
	char vowels[18]="aeiouyaeiouaeioe";
	char cons[48]="bcdfghjklmnpqrstvwxzybcdgklmnprstvwbcdgkpstrkd";
	char rndname[17]="\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	char paircons[33]="ngrkndstshthphsktrdrbrgrfrclcr";
	int rndnum= EQ::Random::Instance()->Int(0, 75),n=1;
	bool dlc=false;
	bool vwl=false;
	bool dbl=false;
	if (rndnum>63)
	{	// rndnum is 0 - 75 where 64-75 is cons pair, 17-63 is cons, 0-16 is vowel
		rndnum=(rndnum-61)*2;	// name can't start with "ng" "nd" or "rk"
		rndname[0]=paircons[rndnum];
		rndname[1]=paircons[rndnum+1];
		n=2;
	}
	else if (rndnum>16)
	{
		rndnum-=17;
		rndname[0]=cons[rndnum];
	}
	else
	{
		rndname[0]=vowels[rndnum];
		vwl=true;
	}
	int namlen= EQ::Random::Instance()->Int(5, 10);
	for (int i=n;i<namlen;i++)
	{
		dlc=false;
		if (vwl)	//last char was a vowel
		{			// so pick a cons or cons pair
			rndnum= EQ::Random::Instance()->Int(0, 62);
			if (rndnum>46)
			{	// pick a cons pair
				if (i>namlen-3)	// last 2 chars in name?
				{	// name can only end in cons pair "rk" "st" "sh" "th" "ph" "sk" "nd" or "ng"
					rndnum= EQ::Random::Instance()->Int(0, 7)*2;
				}
				else
				{	// pick any from the set
					rndnum=(rndnum-47)*2;
				}
				rndname[i]=paircons[rndnum];
				rndname[i+1]=paircons[rndnum+1];
				dlc=true;	// flag keeps second letter from being doubled below
				i+=1;
			}
			else
			{	// select a single cons
				rndname[i]=cons[rndnum];
			}
		}
		else
		{		// select a vowel
			rndname[i]=vowels[EQ::Random::Instance()->Int(0, 16)];
		}
		vwl=!vwl;
		if (!dbl && !dlc)
		{	// one chance at double letters in name
			if (!EQ::Random::Instance()->Int(0, i+9))	// chances decrease towards end of name
			{
				rndname[i+1]=rndname[i];
				dbl=true;
				i+=1;
			}
		}
	}

	rndname[0]=toupper(rndname[0]);
	NameGeneration_Struct* ngs = (NameGeneration_Struct*)app->pBuffer;
	memset(ngs->name,0,64);
	strcpy(ngs->name,rndname);

	QueuePacket(app);
	return true;
}

bool Client::HandleCharacterCreatePacket(const EQApplicationPacket *app) {
	if (GetAccountID() == 0) {
		LogInfo("Account ID not set; unable to create character.");
		return false;
	}
	else if (app->size != sizeof(CharCreate_Struct)) {
		LogInfo("Wrong size on OP_CharacterCreate. Got: [{}], Expected: [{}]",app->size,sizeof(CharCreate_Struct));
		DumpPacket(app);
		return false;
	}

	CharCreate_Struct *cc = (CharCreate_Struct*)app->pBuffer;
	if(OPCharCreate(char_name, cc) == false) {
		database.DeleteCharacter(char_name);
		auto outapp = new EQApplicationPacket(OP_ApproveName, 1);
		outapp->pBuffer[0] = 0;
		QueuePacket(outapp);
		safe_delete(outapp);
	}
	else
	{
		SendCharInfo();
	}

	return true;
}

bool Client::HandleEnterWorldPacket(const EQApplicationPacket *app)
{
	if (GetAccountID() == 0) {
		LogInfo( "Enter world with no logged in account");
		return false;
	}

	if (GetAdmin() < 0)	{
		LogInfo( "Account banned or suspended.");
		return false;
	}

	EnterWorld_Struct *ew = (EnterWorld_Struct *)app->pBuffer;
	strn0cpy(char_name, ew->name, 64);

	LogInfo("HandleEnterWorldPacket(): Received request for character {} from {}:{}", char_name, long2ip(this->GetIP()).c_str(), this->GetPort());

	//if (RuleI(World, MaxClientsPerIP) >= 0) {
	//	client_list.GetCLEIP(this->GetIP()); //Check current CLE Entry IPs against incoming connection
	//}
	if (GetSessionLimit(cle, is_player_zoning)) {
		LogInfo("HandleEnterWorldPacket(): Rejecting request for character {} from {}:{} because of GetSessionLimit()", char_name, long2ip(this->GetIP()).c_str(), this->GetPort());
		return false;
	}

	if (!mule && RuleI(World, MaxClientsPerIP) >= 0 && !ClientList::Instance()->CheckIPLimit(GetAccountID(), GetIP(), GetAdmin(), cle)) {
		LogInfo("HandleEnterWorldPacket(): Rejecting request for character {} from {}:{} because of CheckIPLimit()", char_name, long2ip(this->GetIP()).c_str(), this->GetPort());
		return false;
	}

	uint32 tmpaccid = 0;
	char_id = database.GetCharacterInfo(char_name, &tmpaccid, &zone_id);
	if (char_id == 0 || tmpaccid != GetAccountID()) {
		LogInfo( "Could not get CharInfo for '[{}]'", char_name);
		return false;
	}

	// Make sure this account owns this character
	if (tmpaccid != GetAccountID()) {
		LogInfo( "This account does not own the character named '[{}]'", char_name);
		return false;
	}

	if (zone_id == 0 || !ZoneName(zone_id)) {
		if (mule && WorldContentService::Instance()->IsTheShadowsOfLuclinEnabled()) {
			// This is to save people in an invalid zone, once it's removed from the DB
			database.MoveCharacterToZone(char_id, ZoneID("bazaar"));
			LogInfo("Zone [{}] not found, moving [{}] to Bazaar.", zone_id, char_name);
		}
		else {
			database.MoveCharacterToZone(char_id, ZoneID("arena"));
			LogInfo("Zone [{}] not found, moving [{}] to Arena.", zone_id, char_name);
		}
	}

	if (!is_player_zoning) {
		// A normal character select entry must not inherit group or raid data from an old session. Zoning reconnects preserve it.
		ClientList::Instance()->CharacterOffline(GetAccountID(), char_id, char_name);
	}
	else {
		uint32 groupid = database.GetGroupID(char_name);
		if (groupid > 0) {
			char* leader = 0;
			char leaderbuf[64] = { 0 };
			if ((leader = database.GetGroupLeaderForLogin(char_name, leaderbuf)) && strlen(leader) > 1) {
				auto outapp3 = new EQApplicationPacket(OP_GroupUpdate, sizeof(GroupJoin_Struct));
				GroupJoin_Struct* gj = (GroupJoin_Struct*)outapp3->pBuffer;
				gj->action = groupActMakeLeader;
				strcpy(gj->yourname, char_name);
				strcpy(gj->membername, leader);
				QueuePacket(outapp3);
				safe_delete(outapp3);
			}
		}
	}

	auto outapp = new EQApplicationPacket(OP_MOTD);
	std::string motd = RuleS(World, MOTD);
	if (!motd.empty()) {
		outapp->size = motd.length() + 1;
		outapp->pBuffer = new uchar[outapp->size];
		memset(outapp->pBuffer, 0, outapp->size);
		strcpy((char*)outapp->pBuffer, motd.c_str());
	}
	else if (database.GetVariable("MOTD", motd)) {
		outapp->size = motd.length() + 1;
		outapp->pBuffer = new uchar[outapp->size];
		memset(outapp->pBuffer, 0, outapp->size);
		strcpy((char*)outapp->pBuffer, motd.c_str());
	}
	else {
		// Null Message of the Day. :)
		outapp->size = 1;
		outapp->pBuffer = new uchar[outapp->size];
		outapp->pBuffer[0] = 0;
	}

	QueuePacket(outapp);
	safe_delete(outapp);

	// set mailkey - used for duration of character session
	int MailKey = EQ::Random::Instance()->Int(1, INT_MAX);

	database.SetMailKey(char_id, GetIP(), MailKey);
	if (UCSServerAvailable_) {
		const WorldConfig* Config = WorldConfig::get();
		std::string buffer;

		buffer = StringFormat("%s,%i,%s.%s,%08X",
			Config->ChatHost.c_str(),
			Config->ChatPort,
			Config->ShortName.c_str(),
			GetCharName(),
			MailKey
		);

		auto outapp = new EQApplicationPacket(OP_SetChatServer, (buffer.length() + 1));
		memcpy(outapp->pBuffer, buffer.c_str(), buffer.length());
		outapp->pBuffer[buffer.length()] = '\0';


		QueuePacket(outapp);
		safe_delete(outapp);
	}
	
	EnterWorld();

	return true;
}

bool Client::HandleDeleteCharacterPacket(const EQApplicationPacket *app) {

	uint32 char_acct_id = database.GetAccountIDByChar((char*)app->pBuffer);
	uint32 level = database.GetLevelByChar((char*)app->pBuffer);
	if(char_acct_id == GetAccountID()) {
		if(level >= 30) {
			database.MarkCharacterDeleted((char *)app->pBuffer);
			LogInfo("Backing up and marking as deleted character: [{}]", (char *)app->pBuffer);
		}
		else {
			database.DeleteCharacter((char *)app->pBuffer);
			LogInfo("Delete character: [{}]", (char *)app->pBuffer);

		}
		SendCharInfo();
	}

	return true;
}

bool Client::HandleChecksumPacket(const EQApplicationPacket *app)
{
	if(GetClientVersionBit() > EQ::versions::ClientVersionBit::bit_MacPC || GetAdmin() >= 80)
		return true;

	if (app->size != sizeof(Checksum_Struct)) 
	{
		LogInfo( "Checksum packet is BAD!");
		return false;
	}

	Checksum_Struct *cs=(Checksum_Struct *)app->pBuffer;
	uint64 checksum = cs->checksum;

	if(GetClientVersionBit() == EQ::versions::ClientVersionBit::bit_MacPC)
	{
		//Pristine spell file. 
		if(checksum == 8148330249184697) {
			LogInfo( "Original Spell Checksum is GOOD!");
		}
		//Hobart's updated file.
		else if(checksum == 8148329455921329) {
			LogInfo( "Updated Spell Checksum is GOOD!");
		}
		else if(checksum == 29639760219562021) {
			LogInfo( "Exe Checksum is GOOD!");
		}
		else {
			LogInfo( "Checksum is BAD!");
			return false;
		}
	}

	return true;
}

bool Client::HandleWearChangePacket(const EQApplicationPacket *app)
{
	if (app->size != sizeof(WearChange_Struct))
	{
		return false;
	}

	auto wc = (WearChange_Struct *)app->pBuffer;
	if (wc->wear_slot_id == 7 || wc->wear_slot_id == 8)
	{
		int char_idx = wc->material - 1;
		if (char_idx >= 0 && char_idx < 10)
		{
			uint32 idfile = (wc->wear_slot_id == 7) ? m_char_primary[char_idx] : m_char_secondary[char_idx];
			uint32 color = (wc->wear_slot_id == 7) ? m_char_primary_color[char_idx] : m_char_secondary_color[char_idx];

			auto outapp = new EQApplicationPacket(OP_WearChange, sizeof(WearChange_Struct));
			auto out_wc = (WearChange_Struct *)outapp->pBuffer;
			memset(out_wc, 0, sizeof(WearChange_Struct));
			out_wc->wear_slot_id = wc->wear_slot_id;
			out_wc->material = idfile;
			out_wc->color.Color = color;

			QueuePacket(outapp);
			safe_delete(outapp);

			LogInfo("Handled character select OP_WearChange: char slot [{}] wear_slot [{}] idfile [{}] color [{:#x}]",
				char_idx, wc->wear_slot_id, idfile, color);
		}
	}

	return true;
}

bool Client::HandlePacket(const EQApplicationPacket *app) {

	EmuOpcode opcode = app->GetOpcode();

	LogInfo(
		"HandlePacket: [{}] [{:#06x}] Size [{}]",
		OpcodeManager::EmuToName(app->GetOpcode()),
		app->GetProtocolOpcode(),
		app->Size()
	);

	LogPacketClientServer(
		"[{}] [{:#06x}] Size [{}] {}",
		OpcodeManager::EmuToName(app->GetOpcode()),
		app->GetProtocolOpcode(),
		app->Size(),
		(LogSys.IsLogEnabled(Logs::Detail, Logs::PacketClientServer) ? DumpPacketToString(app) : "")
	);

	// Voidd: Anti-GM Account hack, Checks source ip against valid GM Account IP Addresses
	if (RuleB(World, GMAccountIPList) && this->GetAdmin() >= (RuleI(World, MinGMAntiHackStatus))) {
		if(!database.CheckGMIPs(long2ip(this->GetIP()), this->GetAccountID())) {
			LogInfo("GM Account not permited from source address [{}] and accountid [{}]", long2ip(this->GetIP()).c_str(), this->GetAccountID());
			return false;
		}
	}

	if (GetAccountID() == 0 && opcode != OP_SendLoginInfo) {
		// Got a packet other than OP_SendLoginInfo when not logged in
		LogInfo("Expecting OP_SendLoginInfo, got [{}]", OpcodeNames[opcode]);
		return false;
	}

	switch(opcode)
	{
		case OP_SendLoginInfo:
		{
			return HandleSendLoginInfoPacket(app);
		}
		case OP_ApproveName: //Name approval
		{
			return HandleNameApprovalPacket(app);
		}
		case OP_RandomNameGenerator:
		{
			return HandleGenerateRandomNamePacket(app);
		}
		case OP_CharacterCreate: //Char create
		{
			return HandleCharacterCreatePacket(app);
		}
		case OP_EnterWorld: // Enter world
		{
			return HandleEnterWorldPacket(app);
		}
		case OP_DeleteCharacter:
		{
			return HandleDeleteCharacterPacket(app);
		}
		case OP_GuildsList:
		{
			SendGuildList();
			return true;
		}
		case OP_WorldLogout:
		{
			if (m_ClientVersion == EQ::versions::ClientVersion::Trilogy) {
				return true;
			}
			return false;
		}
		
		case OP_WearChange:
		{
			if (m_ClientVersion == EQ::versions::ClientVersion::Trilogy) {
				return HandleWearChangePacket(app);
			}
			return true;
		}
		case OP_ZoneChange:
		case OP_LoginUnknown1:
		case OP_LoginUnknown2:
		case OP_LoginComplete:
		case OP_ApproveWorld:
		{
			// Essentially we are just 'eating' these packets, indicating
			// they are handled.
			return true;
		}
		case OP_ChecksumExe:
		case OP_ChecksumSpell:
		{
			if(HandleChecksumPacket(app))
			{
				return true;
			}
			else
			{
				LogInfo("Checksum failed for account: [{}]. Closing connection.", this->GetAccountID());
				return false;
			}
		}
		default:
		{
			LogNetcode("Received unknown EQApplicationPacket");
			return true;
		}
	}
	return true;
}

bool Client::Process()
{
	if (initial_connection_timer.Check(false))
	{
		LogInfo("Client timed out before sending an initial world login packet");
		FinishTransport();
		return false;
	}

	bool keep_client = true;
	bool transport_ended = m_stream == nullptr;
	uint32 disconnect_reason = 0;

	while (keep_client && m_stream != nullptr)
	{
		EQApplicationPacket *app = nullptr;
		RDPStream::ReceiveResult receive_result = m_stream->Receive(&app, &disconnect_reason);
		if (receive_result == RDPStream::NoData)
			break;

		if (receive_result == RDPStream::PacketReceived)
		{
			keep_client = HandlePacket(app);
			delete app;
			continue;
		}

		transport_ended = true;
		if (receive_result == RDPStream::PeerClosed)
		{
			LogInfo("World client [{}] closed its RDP connection (PeerClosed)", GetAccountName());
		}
		else
		{
			LogInfo(
				"World client [{}] lost its RDP connection, receive_result [{}], disconnect_reason [{:#010x}]",
				GetAccountName(),
				static_cast<int>(receive_result),
				disconnect_reason
			);
		}
		break;
	}

	if (!keep_client || transport_ended)
	{
		FinishTransport();
		return false;
	}

	if (autobootup_timeout.Check())
	{
		LogInfo("Zone bootup timer expired, bootup failed or too slow.");
		TellClientZoneUnavailable();
	}

	if (connect.Check())
	{
		//SendGuildList();// Send OPCode: OP_GuildsList
		SendApproveWorld();
		connect.Disable();
	}

	if (cle)
		cle->KeepAlive();

	return true;
}

void Client::FinishTransport()
{
	if (WorldConfig::get()->UpdateStats && cle != nullptr)
	{
		auto pack = new ServerPacket;
		pack->opcode = ServerOP_LSPlayerLeftWorld;
		pack->size = sizeof(ServerLSPlayerLeftWorld_Struct);
		pack->pBuffer = new uchar[pack->size];
		memset(pack->pBuffer, 0, pack->size);
		auto logout = reinterpret_cast<ServerLSPlayerLeftWorld_Struct *>(pack->pBuffer);
		strcpy(logout->key, GetLSKey());
		logout->lsaccount_id = GetLSID();
		LoginServerList::Instance()->SendPacket(pack);
		safe_delete(pack);
	}

	if (RunLoops && cle != nullptr && cle->Online() != CLE_Status::Offline && m_session_disposition == SessionDisposition::ReleaseOnTransportEnd)
		cle->SetOnline(CLE_Status::Offline);

	CloseStream();
	LogInfo("Client disconnected (world transport ended)");
}

void Client::CloseStream()
{
	CloseStream(RDPStream::DefaultLingerTimeout);
}

void Client::CloseStream(uint32 linger_timeout_ms)
{
	if (m_stream == nullptr)
		return;

	m_stream->Close(linger_timeout_ms);
	m_stream.reset();
}

void Client::EnterWorld(bool TryBootup) {
	world_entrance_request_id = 0;

	if (zone_id == 0)
		return;

	ZoneServer* zone_server = nullptr;
	zone_server = ZSList::Instance()->FindByZoneID(zone_id);

	const char *zone_name = ZoneName(zone_id, true);
	if (zone_server) {
		if (false == enter_world_triggered) {
			//Drop any clients we own in other zones.
			ZSList::Instance()->DropClient(GetLSID(), zone_server);
			// warn the zone we're coming
			zone_server->IncomingClient(this);
			//tell the server not to trigger this multiple times before we get a zone unavailable
			enter_world_triggered = true;
		}
	}
	else
	{
		if (TryBootup && !RuleB(World, DontBootDynamics)) {
			LogInfo("Attempting autobootup of [{}] [{}]", zone_name, zone_id);
			autobootup_timeout.Start();
			zone_waiting_for_bootup = ZSList::Instance()->TriggerBootup(zone_id);
			if (zone_waiting_for_bootup == 0) {
				LogInfo("No zoneserver available to boot up.");
				TellClientZoneUnavailable();
			}
			return;
		}
		else {
			LogInfo("Requested zone [{}] is not running.", zone_name);
			TellClientZoneUnavailable();
			return;
		}
	}
	zone_waiting_for_bootup = 0;

	if (GetAdmin() < 80 && ZSList::Instance()->IsZoneLocked(zone_id)) {
		LogInfo("Enter world failed. Zone is locked.");
		TellClientZoneUnavailable();
		return;
	}
	if (!cle) {
		TellClientZoneUnavailable();
		return;
	}

	cle->SetChar(char_id, char_name);

	database.CharacterJoin(char_id, char_name);
	database.UpdateLiveChar(char_name, GetAccountID());

	LogInfo(
		"({}) [{}] [{}] (Zone ID [{}]) ",
		char_name,
		(seen_character_select ? "Zoning from character select" : "Zoning to"),
		zone_name,
		zone_id
	);

	if (seen_character_select) {
		auto pack = new ServerPacket;
		pack->opcode = ServerOP_AcceptWorldEntrance;
		pack->size = sizeof(WorldToZone_Struct);
		pack->pBuffer = new uchar[pack->size];
		memset(pack->pBuffer, 0, pack->size);
		WorldToZone_Struct* wtz = (WorldToZone_Struct*) pack->pBuffer;
		world_entrance_request_id = ClientList::Instance()->GetNextWorldEntranceRequestID();
		wtz->account_id = GetAccountID();
		wtz->request_id = world_entrance_request_id;
		wtz->character_id = char_id;
		wtz->zone_id = zone_id;
		wtz->response = 0;
		zone_server->SendPacket(pack);
		delete pack;
	}
	else {	// if they havent seen character select screen, we can assume this is a zone
			// to zone movement, which should be preauthorized before they leave the previous zone
		Clearance(1);
	}
}

void Client::Clearance(int8 response)
{
	world_entrance_request_id = 0;

	ZoneServer* zs = nullptr;
	zs = ZSList::Instance()->FindByZoneID(zone_id);

	if(zs == 0 || response == -1 || response == 0)
	{
		if (zs == 0)
		{
			LogInfo("Unable to find zoneserver in Client::Clearance!!");
		} else {
			LogInfo( "Invalid response [{}] in Client::Clearance", response);
		}

		TellClientZoneUnavailable();
		return;
	}

	if (zs->GetCAddress() == nullptr) {
		LogInfo( "Unable to do zs->GetCAddress() in Client::Clearance!!");
		TellClientZoneUnavailable();
		return;
	}

	if (zone_id == 0) {
		LogInfo( "zoneID is nullptr in Client::Clearance!!");
		TellClientZoneUnavailable();
		return;
	}

	const char* zonename = ZoneName(zone_id);
	if (zonename == 0) {
		LogInfo( "zonename is nullptr in Client::Clearance!!");
		TellClientZoneUnavailable();
		return;
	}

	// Send zone server IP data
	auto outapp = new EQApplicationPacket(OP_ZoneServerInfo, sizeof(ZoneServerInfo_Struct));
	ZoneServerInfo_Struct* zsi = (ZoneServerInfo_Struct*)outapp->pBuffer;

	std::string zs_addr;
	if(cle && cle->IsLocalClient()) {
		const char *local_addr = zs->GetCLocalAddress();

		if(local_addr[0]) {
			zs_addr = local_addr;
		} else {
			zs_addr = zs->GetIP();

			if (zs_addr.empty()) {
				zs_addr = WorldConfig::get()->LocalAddress;
			}

			if (zs_addr == "127.0.0.1")
			{
				LogInfo( "Local zone address was [{}], setting local address to: [{}]", zs_addr, WorldConfig::get()->LocalAddress.c_str());
				zs_addr = WorldConfig::get()->LocalAddress.c_str();
			} else {
				LogInfo( "Local zone address [{}]", zs_addr);
			}
		}

	} else {
		const char *addr = zs->GetCAddress();
		if(addr[0]) {
			zs_addr = addr;
		} else {
			zs_addr = WorldConfig::get()->WorldAddress.c_str();
		}
	}

	strcpy(zsi->ip, zs_addr.c_str());
	zsi->port =ntohs(zs->GetCPort());
	LogInfo("Sending client to zone [{}] ([{}] at [{}]:[{}]", zonename, zone_id, zsi->ip, zsi->port);
	QueuePacket(outapp);
	safe_delete(outapp);

	// solar: The client will tear down the connection without a clean close as soon as it sees OP_ZoneServerInfo from us.
	// This causes our side to keep retransmitting to a deaf peer.  We can't just tear it down immediately here,
	// we need to give RDP time to send it out and possibly retransmit if it's lost, so we set a linger period.
	// During this linger period RDP will keep trying to retransmit and will provoke ICMP unrechables from the peer.
	// We have ICMP handling in rdplib which will close the transport and stop the retransmits.  If the ICMP doesn't
	// make it, rdplib will keep trying to retransmit until either the linger period expires, or the transport is
	// detected dead after 30 seconds.
	CloseStream(30000);

	if (cle) {
		autobootup_timeout.Disable();
		cle->SetOnline(CLE_Status::Zoning);
		m_session_disposition = SessionDisposition::PreserveOnTransportEnd;
	}
}

void Client::TellClientZoneUnavailable() {
	auto outapp = new EQApplicationPacket(OP_ZoneUnavail, sizeof(ZoneUnavail_Struct));
	ZoneUnavail_Struct* ua = (ZoneUnavail_Struct*)outapp->pBuffer;
	const char* zonename = ZoneName(zone_id);
	if (zonename)
		strcpy(ua->zonename, zonename);
	QueuePacket(outapp);
	delete outapp;

	zone_id = 0;
	world_entrance_request_id = 0;
	m_session_disposition = SessionDisposition::ReleaseOnTransportEnd;
	zone_waiting_for_bootup = 0;
	enter_world_triggered = false;
	autobootup_timeout.Disable();
}

void Client::SendToStream(EQApplicationPacket **packet, bool reliable)
{
	if (packet == nullptr || *packet == nullptr)
		return;

	EmuOpcode opcode = (*packet)->GetOpcode();
	if (m_stream == nullptr)
	{
		delete *packet;
		*packet = nullptr;
		return;
	}

	int result = m_stream->Send(packet, reliable);
	LogInfo("SendToStream: opcode [{}] ({:#06x}), reliable [{}] -> result [{}]", OpcodeManager::EmuToName(opcode), opcode, reliable, result);
}

void Client::QueuePacket(const EQApplicationPacket* app, bool ack_req) {
	if (app == nullptr)
		return;

	LogNetcodeDetail("Sending EQApplicationPacket OpCode {:#04x}", app->GetOpcode());

	EQApplicationPacket *copy = app->Copy();
	SendToStream(&copy, ack_req);
}

void Client::SendGuildList() 
{
	auto outapp = new EQApplicationPacket(OP_GuildsList);

	//ask the guild manager to build us a nice guild list packet
	outapp->pBuffer = guild_mgr.MakeOldGuildList(outapp->size);
	if(outapp->pBuffer == nullptr) {
		LogGuilds("Unable to make guild list!");
		safe_delete(outapp);
		return;
	}

	LogGuilds("Sending OP_GuildsList of length [{}]", outapp->size);

	SendToStream(&outapp, true);
}

void Client::SendApproveWorld()
{
	auto outapp = new EQApplicationPacket(OP_ApproveWorld, sizeof(ApproveWorld_Struct));
	ApproveWorld_Struct* aw = (ApproveWorld_Struct*)outapp->pBuffer;
	aw->name;
	aw->response = 0;
	QueuePacket(outapp);
	safe_delete(outapp);
}

bool Client::OPCharCreate(char *name, CharCreate_Struct *cc)
{
	if (!RuleB(Character, CanCreate)) {
		return false;
	}

	uint8 limit = mule ? RuleI(World, MuleToonLimit) : 8; //Mule accounts are limited by their rule, everybody else gets 8.
	if(charcount >= limit) {
		LogInfo("[{}] already has [{}] characters. OPCharCreate returning false.", name, charcount);
		return false;
	}

	PlayerProfile_Struct pp;
	EQ::InventoryProfile inv;

	time_t bday = time(nullptr);
	uint32 i;
	in_addr in;

	const uint32 stats_sum = (
		cc->AGI +
		cc->CHA +
		cc->DEX +
		cc->INT +
		cc->STA +
		cc->STR +
		cc->WIS
	);

	in.s_addr = GetIP();

	LogInfo(
		"Character creation request from [{}] LS [{}] ([{}]:[{}]) : ", 
		GetCLE()->LSName(), 
		GetCLE()->LSID(), 
		inet_ntoa(in), 
		GetPort()
	);
	LogInfo("Name: [{}]", name);
	LogInfo("Race: [{}]  Class: [{}]  Gender: [{}]  Deity: [{}]  Start zone: [{}]",
		cc->race,
		cc->class_,
		cc->gender,
		cc->deity,
		cc->zone_id
	);
	LogInfo(
		"AGI [{}] CHA [{}] DEX [{}] INT [{}] STA [{}] STR [{}] WIS [{}] Total [{}]",
		cc->AGI,
		cc->CHA,
		cc->DEX,
		cc->INT,
		cc->STA,
		cc->STR,
		cc->WIS,
		stats_sum
	);
	LogInfo("Face: [{}]  Eye colors: [{}] [{}]", cc->luclinface, cc->eyecolor1, cc->eyecolor2);
	LogInfo("Hairstyle: [{}]  Haircolor: [{}]", cc->hairstyle, cc->haircolor);
	LogInfo("Beard: [{}]  Beardcolor: [{}]", cc->beard, cc->beardcolor);

	/* Validate the char creation struct */
		if(!Client::CheckCharCreateInfo(cc)) {
			LogInfo("CheckCharCreateInfo did not validate the request (bad race/class/stats)");
			return false;
		}
	/* Convert incoming cc_s to the new PlayerProfile_Struct */
	memset(&pp, 0, sizeof(PlayerProfile_Struct));	// start building the profile

	strn0cpy(pp.name, name, sizeof(pp.name));

	if (cc->luclinface == 0 && cc->oldface > 0)
		cc->luclinface = cc->oldface;

	pp.race         = cc->race;
	pp.class_       = cc->class_;
	pp.gender       = cc->gender;
	pp.deity        = cc->deity;
	pp.STR          = cc->STR;
	pp.STA          = cc->STA;
	pp.AGI          = cc->AGI;
	pp.DEX          = cc->DEX;
	pp.WIS          = cc->WIS;
	pp.INT          = cc->INT;
	pp.CHA          = cc->CHA;
	pp.face         = cc->oldface;
	pp.luclinface	= cc->luclinface;
	pp.eyecolor1    = cc->eyecolor1;
	pp.eyecolor2    = cc->eyecolor2;
	pp.hairstyle    = cc->hairstyle;
	pp.haircolor    = cc->haircolor;
	pp.beard        = cc->beard;
	pp.beardcolor   = cc->beardcolor;
	pp.birthday     = bday;
	pp.lastlogin    = bday;
	pp.level        = 1;
	pp.points       = 5;
	pp.cur_hp       = 1000; // 1k hp during dev only
	pp.hunger_level = 4500;
	pp.thirst_level = 4500;
	pp.fatigue      = 0;

	/* Set Racial and Class specific language and skills */
	SetRacialLanguages(&pp);
	SetRaceStartingSkills(&pp);
	SetClassStartingSkills(&pp);
	SetClassLanguages(&pp);

	for (i = 0; i < MAX_PP_REF_SPELLBOOK; i++)
		pp.spell_book[i] = 0xFFFF;

	for(i = 0; i < MAX_PP_REF_MEMSPELL; i++)
		pp.mem_spells[i] = 0xFFFF;

	for(i = 0; i < BUFF_COUNT; i++)
		pp.buffs[i].spellid = 0xFFFF;

	/* If server is PVP by default, make all character set to it. */
	pp.pvp = database.GetServerType() == 1 ? 1 : 0;

	// if there's a startzone variable put them in there
	std::string startzone;
	if(database.GetVariable("startzone", startzone))
	{
		LogInfo("Found 'startzone' variable setting: [{}]", startzone);
		pp.zone_id = ZoneID(startzone.c_str());
		if (pp.zone_id) {
			auto z = GetZone(pp.zone_id);
			if (z) {
				pp.x = z->safe_x;
				pp.y = z->safe_y;
				pp.z = z->safe_z;
			}
		}
		else {
			LogInfo("Error getting zone id for '[{}]'", startzone);
		}
	}
	else	// otherwise use normal starting zone logic
	{
		bool ValidStartZone = false;

		ValidStartZone = database.GetStartZone(&pp, cc, mule);

		if(!ValidStartZone)
			return false;
	}

	/* just in case  */
	if (!pp.zone_id) {
		pp.zone_id = 1;
		pp.x = pp.y = pp.z = -1;
	}

	if (pp.x == 0.0f && pp.y == 0.0f && pp.z == 0.0f) {
		auto z = GetZone(pp.zone_id);
		if (z) {
			pp.x = z->safe_x;
			pp.y = z->safe_y;
			pp.z = z->safe_z;
			pp.heading = z->safe_heading;
		}
	}

	if (!pp.binds[0].zoneId)
	{
		pp.binds[0].zoneId = pp.zone_id;
		pp.binds[0].x = pp.x;
		pp.binds[0].y = pp.y;
		pp.binds[0].z = pp.z;
		pp.binds[0].heading = pp.heading;
	}

	// set starting city location to the initial bind point
	pp.binds[4] = pp.binds[0];

	LogInfo("Character [{}] starting location: zone_id={}, x={:.2f}, y={:.2f}, z={:.2f}, heading={:.2f}",
		pp.name, pp.zone_id, pp.x, pp.y, pp.z, pp.heading);
	LogInfo("Character [{}] bind location: zone_id={}, x={:.2f}, y={:.2f}, z={:.2f}",
		pp.name, pp.binds[0].zoneId, pp.binds[0].x, pp.binds[0].y, pp.binds[0].z);

	/* Starting Items inventory */
	database.SetStartingItems(&pp, &inv, pp.race, pp.class_, pp.deity, pp.zone_id, pp.name, GetAdmin());

	// now we give the pp and the inv we made to StoreCharacter
	// to see if we can store it
	if (!database.StoreCharacter(GetAccountID(), &pp, &inv)) {
		LogInfo("Character creation failed: [{}]", pp.name);
		return false;
	}
	LogInfo("Character creation successful: [{}]", pp.name);
	++charcount;
	return true;
}

static bool IsSameStartingCityZone(uint32 zone_a, uint32 zone_b) {
	if (zone_a == zone_b)
		return true;
	// Freeport: freportn (8), freportw (9), freporte (10)
	if ((zone_a >= 8 && zone_a <= 10) && (zone_b >= 8 && zone_b <= 10))
		return true;
	// Qeynos: qeynos (1), qeynos2 (2)
	if ((zone_a == 1 || zone_a == 2) && (zone_b == 1 || zone_b == 2))
		return true;
	// Erudin: erudnint (23), erudnext (24)
	if ((zone_a == 23 || zone_a == 24) && (zone_b == 23 || zone_b == 24))
		return true;
	// Neriak: neriaka (40), neriakb (41), neriakc (42), neriakd (43)
	if ((zone_a >= 40 && zone_a <= 43) && (zone_b >= 40 && zone_b <= 43))
		return true;
	// Kaladim: kaladima (60), kaladimb (67)
	if ((zone_a == 60 || zone_a == 67) && (zone_b == 60 || zone_b == 67))
		return true;
	// Felwithe: felwithea (61), felwitheb (62)
	if ((zone_a == 61 || zone_a == 62) && (zone_b == 61 || zone_b == 62))
		return true;

	return false;
}

// returns true if the request is ok, false if there's an error
bool Client::CheckCharCreateInfo(CharCreate_Struct *cc)
{
	if (!cc)
		return false;

	LogInfo( "Validating char creation info...");

	//int currentExpansions = GetExpansion(); // Get expansion value from account table

	int csexp = WorldContentService::Instance()->GetCurrentExpansion(); // this is the chronological expansion number or -1 to indicate ALL but we need a bit mask to compare against
	int currentExpansions = (1 << (csexp >= 0 ? csexp : 30)) - 1; // Get expansion mask based on CurrentExpansion rule for TLP
	if (m_ClientVersion == EQ::versions::ClientVersion::Trilogy && cc->deity == Deity::Agnostic1) {
		cc->deity = Deity::Agnostic2;
	}

	RaceClassCombos class_combo;
	bool found = false;
	int combos = character_create_race_class_combos.size();
	for (int i = 0; i < combos; ++i) {
		bool zone_match = false;
		if (m_ClientVersion == EQ::versions::ClientVersion::Trilogy) {
			zone_match = (cc->zone_id == 0 ||
				IsSameStartingCityZone(character_create_race_class_combos[i].Zone, cc->zone_id));
		}
		else {
			zone_match = (character_create_race_class_combos[i].Zone == cc->zone_id);
		}

		if (character_create_race_class_combos[i].Class == cc->class_ &&
				character_create_race_class_combos[i].Race == cc->race &&
				character_create_race_class_combos[i].Deity == cc->deity &&
				zone_match &&
			((currentExpansions & character_create_race_class_combos[i].ExpansionRequired) == character_create_race_class_combos[i].ExpansionRequired || 
				character_create_race_class_combos[i].ExpansionRequired == 0)) {
			class_combo = character_create_race_class_combos[i];
			if (m_ClientVersion == EQ::versions::ClientVersion::Trilogy) {
				cc->zone_id = class_combo.Zone;
			}
			found = true;
			break;
		}
	}

	if (!found) {
		LogInfo( "Could not find class/race/deity/start_zone combination");
		return false;
	}

	uint32 allocs = character_create_allocations.size();
	RaceClassAllocation allocation = {0};
	found = false;
	for (int i = 0; i < allocs; ++i) {
		if (character_create_allocations[i].Index == class_combo.AllocationIndex) {
			allocation = character_create_allocations[i];
			found = true;
			break;
		}
	}

	if (!found) {
		LogInfo( "Could not find starting stats for selected character combo, cannot verify stats");
		return false;
	}

	uint32 max_stats = allocation.DefaultPointAllocation[0] +
		allocation.DefaultPointAllocation[1] +
		allocation.DefaultPointAllocation[2] +
		allocation.DefaultPointAllocation[3] +
		allocation.DefaultPointAllocation[4] +
		allocation.DefaultPointAllocation[5] +
		allocation.DefaultPointAllocation[6];

	if (cc->STR > allocation.BaseStats[0] + max_stats || cc->STR < allocation.BaseStats[0]) {
		LogInfo( "Strength out of range");
		return false;
	}

	if (cc->DEX > allocation.BaseStats[1] + max_stats || cc->DEX < allocation.BaseStats[1]) {
		LogInfo( "Dexterity out of range");
		return false;
	}

	if (cc->AGI > allocation.BaseStats[2] + max_stats || cc->AGI < allocation.BaseStats[2]) {
		LogInfo( "Agility out of range");
		return false;
	}

	if (cc->STA > allocation.BaseStats[3] + max_stats || cc->STA < allocation.BaseStats[3]) {
		LogInfo( "Stamina out of range");
		return false;
	}

	if (cc->INT > allocation.BaseStats[4] + max_stats || cc->INT < allocation.BaseStats[4]) {
		LogInfo( "Intelligence out of range");
		return false;
	}

	if (cc->WIS > allocation.BaseStats[5] + max_stats || cc->WIS < allocation.BaseStats[5]) {
		LogInfo( "Wisdom out of range");
		return false;
	}

	if (cc->CHA > allocation.BaseStats[6] + max_stats || cc->CHA < allocation.BaseStats[6]) {
		LogInfo( "Charisma out of range");
		return false;
	}

	uint32 current_stats = 0;
	current_stats += cc->STR - allocation.BaseStats[0];
	current_stats += cc->DEX - allocation.BaseStats[1];
	current_stats += cc->AGI - allocation.BaseStats[2];
	current_stats += cc->STA - allocation.BaseStats[3];
	current_stats += cc->INT - allocation.BaseStats[4];
	current_stats += cc->WIS - allocation.BaseStats[5];
	current_stats += cc->CHA - allocation.BaseStats[6];
	if (current_stats > max_stats) {
		LogInfo( "Current Stats > Maximum Stats");
		return false;
	}

	return true;
}

void Client::SetClassStartingSkills(PlayerProfile_Struct *pp)
{
	for (uint32 i = 0; i <= EQ::skills::HIGHEST_SKILL; ++i) {
		if (pp->skills[i] == 0) {
			// Skip specialized, tradeskills (fishing excluded), Alcohol Tolerance, and Bind Wound
			if (EQ::skills::IsSpecializedSkill((EQ::skills::SkillType)i) ||
					(EQ::skills::IsTradeskill((EQ::skills::SkillType)i) && i != EQ::skills::SkillFishing) ||
					i == EQ::skills::SkillAlcoholTolerance || i == EQ::skills::SkillBindWound)
				continue;

			pp->skills[i] = 0;
		}
	}
}

void Client::SetRaceStartingSkills( PlayerProfile_Struct *pp )
{
	switch( pp->race ) {
	case Race::Barbarian:
	case Race::Erudite:
	case Race::HalfElf:
	case Race::HighElf:
	case Race::Human:
	case Race::Ogre:
	case Race::Troll:
		{
			// No Race Specific Skills
			break;
		}
	case Race::Dwarf:
		{
			pp->skills[EQ::skills::SkillSenseHeading] = 50; //Even if we set this to 0, Intel client sets this to 50 anyway. Confirmed this is correct for era.
			break;
		}
	case Race::DarkElf:
		{
			pp->skills[EQ::skills::SkillHide] = 50;
			break;
		}
	case Race::Gnome:
		{
			pp->skills[EQ::skills::SkillTinkering] = 50;
			break;
		}
	case Race::Halfling:
		{
			pp->skills[EQ::skills::SkillHide] = 50;
			pp->skills[EQ::skills::SkillSneak] = 50;
			break;
		}
	case Race::Iksar:
		{
			pp->skills[EQ::skills::SkillForage] = 50;
			pp->skills[EQ::skills::SkillSwimming] = 100;
			break;
		}
	case Race::WoodElf:
		{
			pp->skills[EQ::skills::SkillForage] = 50;
			pp->skills[EQ::skills::SkillHide] = 50;
			break;
		}
	case Race::VahShir:
		{
			pp->skills[EQ::skills::SkillSafeFall] = 50;
			pp->skills[EQ::skills::SkillSneak] = 50;
			break;
		}
	}
}

void Client::SetRacialLanguages( PlayerProfile_Struct *pp )
{
	switch( pp->race ) {
		case Race::Human: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			break;
		}
		case Race::Barbarian: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::Barbarian] = Language::MaxValue;
			break;
		}
		case Race::Erudite: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::Erudian] = Language::MaxValue;
			break;
		}
		case Race::WoodElf: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::Elvish] = Language::MaxValue;
			break;
		}
		case Race::HighElf : {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::DarkElvish] = 25;
			pp->languages[Language::ElderElvish] = 25;
			pp->languages[Language::Elvish] = Language::MaxValue;
			break;
		}
		case Race::DarkElf: {
		   pp->languages[Language::CommonTongue] = Language::MaxValue;
		   pp->languages[Language::DarkElvish] = Language::MaxValue;
		   pp->languages[Language::DarkSpeech] = Language::MaxValue;
		   pp->languages[Language::ElderElvish] = Language::MaxValue;
		   pp->languages[Language::Elvish] = 25;
			break;
		}
		case Race::HalfElf: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::Elvish] = Language::MaxValue;
			break;
		}
		case Race::Dwarf: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::Dwarvish] = Language::MaxValue;
			pp->languages[Language::Gnomish] = 25;
			break;
		}
		case Race::Troll: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::DarkSpeech] = Language::MaxValue;
			pp->languages[Language::Troll] = Language::MaxValue;
			break;
		}
		case Race::Ogre: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::DarkSpeech] = Language::MaxValue;
			pp->languages[Language::Ogre] = Language::MaxValue;
			break;
		}
		case Race::Halfling: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::Halfling] = Language::MaxValue;
			break;
		}
		case Race::Gnome: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::Dwarvish] = 25;
			pp->languages[Language::Gnomish] = Language::MaxValue;
			break;
		}
		case Race::Iksar: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::DarkSpeech] = Language::MaxValue;
			pp->languages[Language::Lizardman] = Language::MaxValue;
			break;
		}
		case Race::VahShir: {
			pp->languages[Language::CommonTongue] = Language::MaxValue;
			pp->languages[Language::CombineTongue] = Language::MaxValue;
			pp->languages[Language::Erudian] = 32;
			pp->languages[Language::VahShir] = Language::MaxValue;
			break;
		}
	}
}

void Client::SetClassLanguages(PlayerProfile_Struct *pp)
{
	// we only need to handle one class, but custom server might want to do more
	switch(pp->class_) {
		case Class::Rogue:
			pp->languages[Language::ThievesCant] = Language::MaxValue;
			break;
		default:
			break;
	}
}

bool Client::GetSessionLimit(ClientListEntry *client_entry, bool continuing_zone_transfer)
{
	if (!client_entry)
		return false;

	if (RuleI(World, AccountSessionLimit) >= 0 && client_entry->Admin() < (RuleI(World, ExemptAccountLimitStatus)) && (RuleI(World, ExemptAccountLimitStatus) != -1))
	{
		bool entry_active = client_entry->Online() >= CLE_Status::Zoning;
		bool may_continue = continuing_zone_transfer && entry_active;
		if (ClientList::Instance()->CheckAccountActive(client_entry->AccountID(), client_entry) || (entry_active && !may_continue))
		{
			LogInfo("Account [{}] attempted to login with an active player in the world.", client_entry->AccountID());
			return true;
		}
		else
			return false;
	}

	return false;
}

void Client::RecordPossibleHack(const std::string &message)
{
	if (PlayerEventLogs::Instance()->IsEventEnabled(PlayerEvent::POSSIBLE_HACK)) {
		auto event = PlayerEvent::PossibleHackEvent{ .message = message };
		std::stringstream ss;
		{
			cereal::JSONOutputArchiveSingleLine ar(ss);
			event.serialize(ar);
		}
		auto e = PlayerEventLogsRepository::NewEntity();
		e.character_id = char_id;
		e.account_id = GetCLE() ? GetAccountID() : 0;
		e.event_type_id = PlayerEvent::POSSIBLE_HACK;
		e.event_type_name = PlayerEvent::EventName[PlayerEvent::POSSIBLE_HACK];
		e.event_data = ss.str();
		e.created_at = std::time(nullptr);
		PlayerEventLogsRepository::InsertOne(database, e);
	}
}
