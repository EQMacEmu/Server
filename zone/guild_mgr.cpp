/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2006 EQEMu Development Team (http://eqemulator.net)

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

#include "../common/servertalk.h"
#include "../common/strings.h"

#include "client.h"
#include "entity.h"
#include "guild_mgr.h"
#include "worldserver.h"
#include "zonedb.h"

ZoneGuildManager guild_mgr;

extern WorldServer worldserver;
extern volatile bool is_zone_loaded;

void ZoneGuildManager::SendGuildRefresh(uint32 guild_id, bool name, bool motd, bool rank, bool relation) {
	Log(Logs::Detail, Logs::Guilds, "Sending guild refresh for %d to world, changes: name=%d, motd=%d, rank=d, relation=%d", guild_id, name, motd, rank, relation);
	auto pack = new ServerPacket(ServerOP_RefreshGuild, sizeof(ServerGuildRefresh_Struct));
	ServerGuildRefresh_Struct *s = (ServerGuildRefresh_Struct *) pack->pBuffer;
	s->guild_id = guild_id;
	s->name_change = name;
	s->motd_change = motd;
	s->rank_change = rank;
	s->relation_change = relation;
	worldserver.SendPacket(pack);
	safe_delete(pack);
}

void ZoneGuildManager::SendCharRefresh(uint32 old_guild_id, uint32 guild_id, uint32 charid) {
	if(guild_id == 0) {
		Log(Logs::Detail, Logs::Guilds, "Guild lookup for char %d when sending char refresh.", charid);

		CharGuildInfo gci;
		if(!GetCharInfo(charid, gci)) {
			guild_id = GUILD_NONE;
		} else {
			guild_id = gci.guild_id;
		}
	}

	Log(Logs::Detail, Logs::Guilds, "Sending char refresh for %d from guild %d to world", charid, guild_id);

	auto pack = new ServerPacket(ServerOP_GuildCharRefresh, sizeof(ServerGuildCharRefresh_Struct));
	ServerGuildCharRefresh_Struct *s = (ServerGuildCharRefresh_Struct *) pack->pBuffer;
	s->guild_id = guild_id;
	s->old_guild_id = old_guild_id;
	s->char_id = charid;
	worldserver.SendPacket(pack);
	safe_delete(pack);
}

void ZoneGuildManager::SendRankUpdate(uint32 CharID)
{
	CharGuildInfo gci;

	if(!GetCharInfo(CharID, gci))
		return;

	auto pack = new ServerPacket(ServerOP_GuildRankUpdate, sizeof(ServerGuildRankUpdate_Struct));

	ServerGuildRankUpdate_Struct *sgrus = (ServerGuildRankUpdate_Struct*)pack->pBuffer;

	sgrus->GuildID = gci.guild_id;
	strn0cpy(sgrus->MemberName, gci.char_name.c_str(), sizeof(sgrus->MemberName));
	sgrus->Rank = gci.rank;
	sgrus->Banker = gci.banker + (gci.alt * 2);

	worldserver.SendPacket(pack);

	safe_delete(pack);
}

void ZoneGuildManager::SendGuildDelete(uint32 guild_id) {
	Log(Logs::Detail, Logs::Guilds, "Sending guild delete for guild %d to world", guild_id);
	auto pack = new ServerPacket(ServerOP_DeleteGuild, sizeof(ServerGuildID_Struct));
	ServerGuildID_Struct *s = (ServerGuildID_Struct *) pack->pBuffer;
	s->guild_id = guild_id;
	worldserver.SendPacket(pack);
	safe_delete(pack);
}

//makes a guild member list packet (internal format), returns ownership of the buffer.
uint8 *ZoneGuildManager::MakeGuildMembers(uint32 guild_id, const char *prefix_name, uint32 &length) {
	uint8 *retbuffer;

	//hack because we dont have the "remove from guild" packet right now.
	if(guild_id == GUILD_NONE) {
		length = sizeof(Internal_GuildMembers_Struct);
		retbuffer = new uint8[length];
		Internal_GuildMembers_Struct *gms = (Internal_GuildMembers_Struct *) retbuffer;
		strcpy(gms->player_name, prefix_name);
		gms->count = 0;
		gms->name_length = 0;
		gms->note_length = 0;
		return(retbuffer);
	}

	std::vector<CharGuildInfo *> members;
	if(!GetEntireGuild(guild_id, members))
		return(nullptr);

	//figure out the actual packet length.
	uint32 fixed_length = sizeof(Internal_GuildMembers_Struct) + members.size()*sizeof(Internal_GuildMemberEntry_Struct);
	std::vector<CharGuildInfo *>::iterator cur, end;
	CharGuildInfo *ci;
	cur = members.begin();
	end = members.end();
	uint32 name_len = 0;
	uint32 note_len = 0;
	for(; cur != end; ++cur) {
		ci = *cur;
		name_len += ci->char_name.length();
		note_len += ci->public_note.length();
	}

	//calc total length.
	length = fixed_length + name_len + note_len + members.size()*2;	//string data + null terminators

	//make our nice buffer
	retbuffer = new uint8[length];

	Internal_GuildMembers_Struct *gms = (Internal_GuildMembers_Struct *) retbuffer;

	//fill in the global header
	strcpy(gms->player_name, prefix_name);
	gms->count = members.size();
	gms->name_length = name_len;
	gms->note_length = note_len;

	char *name_buf = (char *) ( retbuffer + fixed_length );
	char *note_buf = (char *) ( name_buf + name_len + members.size() );

	//fill in each member's entry.
	Internal_GuildMemberEntry_Struct *e = gms->member;

	cur = members.begin();
	end = members.end();
	for(; cur != end; ++cur) {
		ci = *cur;

		//the order we set things here must match the struct

//nice helper macro
#define SlideStructString(field, str) \
		strcpy(field, str.c_str()); \
		field += str.length() + 1
#define PutField(field) \
		e->field = ci->field

		SlideStructString( name_buf, ci->char_name );
		PutField(level);
		e->banker = ci->banker + (ci->alt * 2);	// low bit is banker flag, next bit is 'alt' flag.
		PutField(class_);
		PutField(rank);
		PutField(time_last_on);
		SlideStructString( note_buf, ci->public_note );
		e->zone_id = 0;	// Flag them as offline (zoneid 0) as world will update us with their online status afterwards.
#undef SlideStructString
#undef PutFieldN

		delete *cur;

		e++;
	}

	return(retbuffer);
}

void ZoneGuildManager::ListGuilds(Client *c) const {
	c->Message(CC_Default, "Listing guilds on the server:");
	char leadername[64];
	std::map<uint32, GuildInfo *>::const_iterator cur, end;
	cur = m_guilds.begin();
	end = m_guilds.end();
	int r = 0;
	for(; cur != end; ++cur) {
		leadername[0] = '\0';
		database.GetCharName(cur->second->leader_char_id, leadername);
		if (leadername[0] == '\0')
			c->Message(CC_Default, "  Guild #%i <%s>", cur->first, cur->second->name.c_str());
		else
			c->Message(CC_Default, "  Guild #%i <%s> Leader: %s", cur->first, cur->second->name.c_str(), leadername);
		r++;
	}
	c->Message(CC_Default, "%i guilds listed.", r);
}


void ZoneGuildManager::DescribeGuild(Client *c, uint32 guild_id) const {
	std::map<uint32, GuildInfo *>::const_iterator res;
	res = m_guilds.find(guild_id);
	if(res == m_guilds.end()) {
		c->Message(CC_Default, "Guild %d not found.", guild_id);
		return;
	}

	const GuildInfo *info = res->second;

	c->Message(CC_Default, "Guild info DB# %i <%s>", guild_id, info->name.c_str());

	char leadername[64];
	database.GetCharName(info->leader_char_id, leadername);
	c->Message(CC_Default, "Guild Leader: %s", leadername);

	char permbuffer[256];
	uint8 i;
	for (i = 0; i <= GUILD_MAX_RANK; i++) {
		char *permptr = permbuffer;
		uint8 r;
		for(r = 0; r < _MaxGuildAction; r++)
			permptr += sprintf(permptr, "  %s: %c", GuildActionNames[r], info->ranks[i].permissions[r]?'Y':'N');

		c->Message(CC_Default, "Rank %i: %s", i, info->ranks[i].name.c_str());
		c->Message(CC_Default, "Permissions: %s", permbuffer);
	}

}

//in theory, we could get a pile of unused entries in this array, but only if
//we had a malicious client sending controlled packets, plus its like 10 bytes per entry.
void ZoneGuildManager::RecordInvite(uint32 char_id, uint32 guild_id, uint8 rank) {
	m_inviteQueue[char_id] = std::pair<uint32, uint8>(guild_id, rank);
}

bool ZoneGuildManager::VerifyAndClearInvite(uint32 char_id, uint32 guild_id, uint8 rank) {
	std::map<uint32, std::pair<uint32, uint8> >::iterator res;
	res = m_inviteQueue.find(char_id);
	if(res == m_inviteQueue.end())
		return(false);	//no entry...
	bool valid = false;
	if(res->second.first == guild_id && res->second.second == rank) {
		valid = true;
	}
	m_inviteQueue.erase(res);
	return(valid);
}

void ZoneGuildManager::ProcessWorldPacket(ServerPacket *pack) {
	switch(pack->opcode) {
	case ServerOP_RefreshGuild: {
		if(pack->size != sizeof(ServerGuildRefresh_Struct)) {
			Log(Logs::General, Logs::Error, "Received ServerOP_RefreshGuild of incorrect size %d, expected %d", pack->size, sizeof(ServerGuildRefresh_Struct));
			return;
		}
		ServerGuildRefresh_Struct *s = (ServerGuildRefresh_Struct *) pack->pBuffer;

		Log(Logs::Detail, Logs::Guilds, "Received guild refresh from world for %d, changes: name=%d, motd=%d, rank=%d, relation=%d", s->guild_id, s->name_change, s->motd_change, s->rank_change, s->relation_change);

		//reload all the guild details from the database.
		RefreshGuild(s->guild_id);

		if(s->motd_change) {
			//resend guild MOTD to all guild members in this zone.
			entity_list.SendGuildMOTD(s->guild_id);
		}

		if(s->name_change) {
			//until we figure out the guild update packet, we resend the whole guild list.
			entity_list.SendGuildList();
		}

		if(s->rank_change) {
			//we need to send spawn appearance packets for all members of this guild in the zone, to everybody.
			entity_list.SendGuildSpawnAppearance(s->guild_id);
		}

		if(s->relation_change) {
			//unknown until we implement guild relations.
		}

		break;
	}

	case ServerOP_GuildCharRefresh: {
		if(pack->size != sizeof(ServerGuildCharRefresh_Struct)) {
			Log(Logs::General, Logs::Error, "Received ServerOP_RefreshGuild of incorrect size %d, expected %d", pack->size, sizeof(ServerGuildCharRefresh_Struct));
			return;
		}
		ServerGuildCharRefresh_Struct *s = (ServerGuildCharRefresh_Struct *) pack->pBuffer;

		Log(Logs::Detail, Logs::Guilds, "Received guild member refresh from world for char %d from guild %d", s->char_id, s->guild_id);

		Client *c = entity_list.GetClientByCharID(s->char_id);

		if(c != nullptr) {
			//this reloads the char's guild info from the database and sends appearance updates
			c->RefreshGuildInfo();
		}

		if(c != nullptr && s->guild_id != GUILD_NONE) {
			//char is in zone, and has changed into a new guild, send MOTD.
			c->SendGuildMOTD();
		}
		break;
	}

	case ServerOP_DeleteGuild: {
		if(pack->size != sizeof(ServerGuildID_Struct)) {
			Log(Logs::General, Logs::Error, "Received ServerOP_DeleteGuild of incorrect size %d, expected %d", pack->size, sizeof(ServerGuildID_Struct));
			return;
		}
		ServerGuildID_Struct *s = (ServerGuildID_Struct *) pack->pBuffer;

		Log(Logs::Detail, Logs::Guilds, "Received guild delete from world for guild %d", s->guild_id);

		//clear all the guild tags.
		entity_list.RefreshAllGuildInfo(s->guild_id);

		//remove the guild data from the local guild manager
		guild_mgr.LocalDeleteGuild(s->guild_id);

		//if we stop forcing guild list to send on guild create, we need to do this:
		//in the case that we delete a guild and add a new one.
		//entity_list.SendGuildList();

		break;
	}

	case ServerOP_OnlineGuildMembersResponse:
		if (is_zone_loaded)
		{
			char *Buffer = (char *)pack->pBuffer;

			uint32 FromID = VARSTRUCT_DECODE_TYPE(uint32, Buffer);
			uint32 Count = VARSTRUCT_DECODE_TYPE(uint32, Buffer);
			Client *c = entity_list.GetClientByCharID(FromID);

			if (!c || !c->IsInAGuild())
			{
				Log(Logs::Detail, Logs::Guilds,"Invalid Client or not in guild. ID=%i", FromID);
				break;
			}
			Log(Logs::Detail, Logs::Guilds,"Processing ServerOP_OnlineGuildMembersResponse");
		}
		break;
	}
}

void ZoneGuildManager::RequestOnlineGuildMembers(uint32 FromID, uint32 GuildID)
{
	auto pack =
	    new ServerPacket(ServerOP_RequestOnlineGuildMembers, sizeof(ServerRequestOnlineGuildMembers_Struct));
	ServerRequestOnlineGuildMembers_Struct *srogm = (ServerRequestOnlineGuildMembers_Struct*)pack->pBuffer;

	srogm->FromID = FromID;
	srogm->GuildID = GuildID;
	worldserver.SendPacket(pack);

	safe_delete(pack);
}

void ZoneGuildManager::ProcessApproval()
{
	LinkedListIterator<GuildApproval*> iterator(list);

	iterator.Reset();
	while(iterator.MoreElements())
	{
		if(!iterator.GetData()->ProcessApproval())
			iterator.RemoveCurrent();
		iterator.Advance();
	}
}

void ZoneGuildManager::AddGuildApproval(const char* guildname,Client* owner)
{
	auto tmp = new GuildApproval(guildname, owner, GetFreeID());
	list.Insert(tmp);
}

void ZoneGuildManager::AddMemberApproval(uint32 refid,Client* name)
{
	GuildApproval* tmp = FindGuildByIDApproval(refid);
	if(tmp != 0)
	{
		if(!tmp->AddMemberApproval(name))
			name->Message(CC_Default,"Unable to add to list.");
		else
		{
			name->Message(CC_Default,"Added to list.");
		}
	}
	else
		name->Message(CC_Default,"Unable to find guild reference id.");
}

ZoneGuildManager::~ZoneGuildManager()
{
	ClearGuilds();
}

void ZoneGuildManager::ClearGuildsApproval()
{
	list.Clear();
}

GuildApproval* ZoneGuildManager::FindGuildByIDApproval(uint32 refid)
{
	LinkedListIterator<GuildApproval*> iterator(list);

	iterator.Reset();
	while(iterator.MoreElements())
	{
		if(iterator.GetData()->GetID() == refid)
			return iterator.GetData();
		iterator.Advance();
	}
	return 0;
}

GuildApproval* ZoneGuildManager::FindGuildByOwnerApproval(Client* owner)
{
	LinkedListIterator<GuildApproval*> iterator(list);

	iterator.Reset();
	while(iterator.MoreElements())
	{
		if(iterator.GetData()->GetOwner() == owner)
			return iterator.GetData();
		iterator.Advance();
	}
	return 0;
}

/*================== GUILD APPROVAL ========================*/

bool GuildApproval::ProcessApproval()
{
	if(owner && owner->GuildID() != 0)
	{
		owner->Message(CC_Default,"You are already in a guild! Guild request deleted.");
		return false;
	}
	if(deletion_timer->Check() || !owner)
	{
		if(owner)
			owner->Message(CC_Default,"You took too long! Your guild request has been deleted.");
		return false;
	}

	return true;
}

GuildApproval::GuildApproval(const char* guildname, Client* owner,uint32 id)
{
	std::string founders;
	database.GetVariable("GuildCreation", founders);
	uint8 tmp = atoi(founders.c_str());
	deletion_timer = new Timer(1800000);
	strcpy(guild,guildname);
	this->owner = owner;
	this->refid = id;
	if(owner)
		owner->Message(CC_Default,"You can now start getting your guild approved, tell your %i members to #guildapprove %i, you have 30 minutes to create your guild.",tmp,GetID());
	for(int i=0;i<tmp;i++)
		members[i] = 0;
}

GuildApproval::~GuildApproval()
{
	safe_delete(deletion_timer);
}

bool GuildApproval::AddMemberApproval(Client* addition)
{
	std::string founders;
	database.GetVariable("GuildCreation", founders);
	uint8 tmp = atoi(founders.c_str());
	for(int i=0;i<tmp;i++)
	{
		if(members[i] && members[i] == addition)
			return false;
	}

	for(int i=0;i<tmp;i++)
	{
		if(!members[i])
		{
			members[i] = addition;
			int z=0;
			for(int i=0;i<tmp;i++)
			{
				if(members[i])
					z++;
			}
			if(z==tmp)
				GuildApproved();

			return true;
		}
	}
	return false;
}

void GuildApproval::ApprovedMembers(Client* requestee)
{
	std::string founders;
	database.GetVariable("GuildCreation", founders);
	uint8 tmp = atoi(founders.c_str());
	for(int i=0;i<tmp;i++)
	{
		if(members[i])
			requestee->Message(CC_Default,"%i: %s",i,members[i]->GetName());
	}
}

void GuildApproval::GuildApproved()
{
	char petitext[PBUFFER] = "A new guild was founded! Guildname: ";
	char gmembers[MBUFFER] = " ";

	if(!owner)
		return;
	std::string founders;
	database.GetVariable("GuildCreation", founders);
	uint8 tmp = atoi(founders.c_str());
	uint32 tmpeq = guild_mgr.CreateGuild(guild, owner->CharacterID());
	guild_mgr.SetGuild(owner->CharacterID(),tmpeq,2);
	owner->SendAppearancePacket(AppearanceType::GuildID,true,false);
	for(int i=0;i<tmp;i++)
	{
		if(members[i])
			{
			owner->Message(CC_Default, "%s",members[i]->GetName());
			owner->Message(CC_Default, "%i",members[i]->CharacterID());
			guild_mgr.SetGuild(members[i]->CharacterID(),tmpeq,0);
			size_t len = MBUFFER - strlen(gmembers)+1;
			strncat(gmembers," ",len);
			strncat(gmembers,members[i]->GetName(),len);
			}
	}
	size_t len = PBUFFER - strlen(petitext)+1;
	strncat(petitext,guild,len);
	strncat(petitext," Leader: ",len);
	strncat(petitext,owner->CastToClient()->GetName(),len);
	strncat(petitext," Members:",len);
	strncat(petitext,gmembers,len);
	auto pet = new Petition(owner->CastToClient()->CharacterID());
	pet->SetAName(owner->CastToClient()->AccountName());
	pet->SetClass(owner->CastToClient()->GetClass());
	pet->SetLevel(owner->CastToClient()->GetLevel());
	pet->SetCName(owner->CastToClient()->GetName());
	pet->SetRace(owner->CastToClient()->GetRace());
	pet->SetLastGM("");
	pet->SetCName(owner->CastToClient()->GetName()); //aza77 is this really 2 times needed ??
	pet->SetPetitionText(petitext);
	pet->SetZone(zone->GetZoneID());
	pet->SetUrgency(0);
	petition_list.AddPetition(pet);
	database.InsertPetitionToDB(pet);
	petition_list.UpdateGMQueue();
	petition_list.UpdateZoneListQueue();
	worldserver.SendEmoteMessage(0, 0, 80, 15, "%s has made a petition. #%i", owner->CastToClient()->GetName(), pet->GetID());
	auto pack = new ServerPacket;
	pack->opcode = ServerOP_RefreshGuild;
	pack->size = tmp;
	pack->pBuffer = new uchar[pack->size];
	memcpy(pack->pBuffer, &tmpeq, 4);
	worldserver.SendPacket(pack);
	safe_delete(pack);
	owner->Message(CC_Default, "Your guild was created.");
	owner = 0;
}

