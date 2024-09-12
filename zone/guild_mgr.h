#ifndef GUILD_MGR_H_
#define GUILD_MGR_H_

#include "../common/types.h"
#include "../common/guild_base.h"
#include <map>
#include <list>
#include "../zone/petitions.h"

extern PetitionList petition_list;
//extern GuildRanks_Struct guilds[512];
//extern ZoneDatabase database;

#define PBUFFER 50
#define MBUFFER 50

#define GUILD_BANK_MAIN_AREA_SIZE	200
#define GUILD_BANK_DEPOSIT_AREA_SIZE	20
class Client;
class ServerPacket;

class GuildApproval
{
public:
	GuildApproval(const char* guildname,Client* owner,uint32 id);
	~GuildApproval();
	bool	ProcessApproval();
	bool	AddMemberApproval(Client* addition);
	uint32	GetID() { return refid; }
	Client*	GetOwner() { return owner; }
	void	GuildApproved();
	void	ApprovedMembers(Client* requestee);
private:
	Timer* deletion_timer;
	char guild[16];
	Client* owner;
	Client* members[6];
	uint32 refid;
};

class ZoneGuildManager : public BaseGuildManager {
public:
	~ZoneGuildManager(void);

	void	AddGuildApproval(const char* guildname, Client* owner);
	void	AddMemberApproval(uint32 refid,Client* name);
	void	ClearGuildsApproval();
	GuildApproval* FindGuildByIDApproval(uint32 refid);
	GuildApproval* FindGuildByOwnerApproval(Client* owner);
	void	ProcessApproval();
	uint32	GetFreeID() { return id+1; }
	//called by worldserver when it receives a message from world.
	void ProcessWorldPacket(ServerPacket *pack);

	void ListGuilds(Client* c, std::string search_criteria = std::string()) const;
	void ListGuilds(Client* c, uint32 guild_id = 0) const;
	void DescribeGuild(Client *c, uint32 guild_id) const;

	uint8 *MakeGuildMembers(uint32 guild_id, const char *prefix_name, uint32 &length);	//make a guild member list packet, returns ownership of the buffer.

	void RecordInvite(uint32 char_id, uint32 guild_id, uint8 rank);
	bool VerifyAndClearInvite(uint32 char_id, uint32 guild_id, uint8 rank);
	void RequestOnlineGuildMembers(uint32 FromID, uint32 GuildID);

protected:
	virtual void SendGuildRefresh(uint32 guild_id, bool name, bool motd, bool rank, bool relation);
	virtual void SendCharRefresh(uint32 old_guild_id, uint32 guild_id, uint32 charid);
	virtual void SendRankUpdate(uint32 CharID);
	virtual void SendGuildDelete(uint32 guild_id);

	std::map<uint32, std::pair<uint32, uint8> > m_inviteQueue;	//map from char ID to guild,rank

private:
	LinkedList<GuildApproval*> list;
	uint32 id;

};

extern ZoneGuildManager guild_mgr;


#endif /*GUILD_MGR_H_*/

