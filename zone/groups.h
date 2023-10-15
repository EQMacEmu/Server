/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2002 EQEMu Development Team (http://eqemu.org)

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
#ifndef GROUPS_H
#define GROUPS_H

#include "../common/eq_packet_structs.h"
#include "../common/types.h"

#include "mob.h"

class Client;
class EQApplicationPacket;
class Mob;

class GroupIDConsumer {
public:
	GroupIDConsumer() { id = 0; }
	GroupIDConsumer(uint32 gid) { id = gid; }
	inline const uint32 GetID()	const { return id; }

protected:
	friend class EntityList;
	//use of this function is highly discouraged
	inline void SetID(uint32 set_id) { id = set_id; }
private:
	uint32 id;
};

class Group : public GroupIDConsumer {
public:
	Group(Mob* leader);
	Group(uint32 gid);
	~Group();

	bool	AddMember(Mob* newmember, const char* NewMemberName = nullptr, uint32 CharacterID = 0);
	void	AddMember(const char* NewMemberName);
	void	SendUpdate(uint32 type,Mob* member);
	bool	DelMemberOOZ(const char *Name, bool checkleader);
	bool	DelMember(Mob* oldmember);
	void	DisbandGroup(bool alt_msg = false, uint32 msg = 0);
	bool	IsGroupMember(Mob* client);
	bool	IsGroupMember(const char *Name);
	bool	Process();
	bool	IsGroup()			{ return true; }
	void	SendGroupJoinOOZ(Mob* NewMember);
	void	CastGroupSpell(Mob* caster,uint16 spellid, bool isrecourse=false, int recourse_level=-1);
	void	SplitExp(uint32 exp, Mob* killed_mob);
	bool	ProcessGroupSplit(Mob* killed_mob, struct GroupExpSplit_Struct& gs, bool isgreen);
	void	GiveGroupSplitExp(Mob* killed_mob, uint8 maxlevel, int16 weighted_levels, int conlevel, float groupexp, int8 close_count);
	void	GroupMessage(Mob* sender,uint8 language,uint8 lang_skill,const char* message);
	void	GroupMessage_StringID(Mob* sender, uint32 type, uint32 string_id, const char* message,const char* message2=0,const char* message3=0,const char* message4=0,const char* message5=0,const char* message6=0,const char* message7=0,const char* message8=0,const char* message9=0, uint32 distance = 0);
	int32	GetTotalGroupDamage(Mob* other);
	void	SplitMoney(uint32 copper, uint32 silver, uint32 gold, uint32 platinum, Client *splitter = nullptr, bool share = false);
	inline	void SetLeader(Mob* newleader) { leader = newleader; if(newleader != nullptr) strncpy(leadername, newleader->GetName(), 64); };
	inline	void SetLeaderName(const char* leader) { strncpy(leadername, leader, 64); };
	inline	Mob* GetLeader(){ return leader; };
	char*	GetLeaderName() { return leadername; };
	void	SetOldLeaderName(const char* oldleader) { strcpy(oldleadername, oldleader); }
	char*	GetOldLeaderName() { return oldleadername; }
	void	SendHPPacketsTo(Mob* newmember);
	void	SendHPPacketsFrom(Mob* newmember);
	bool	UpdatePlayer(Mob* update);
	void	MemberZoned(Mob* removemob);
	inline	bool IsLeader(Mob* leadertest) { return leadertest==leader; };
	uint8	GroupCount();
	uint32	GetHighestLevel();
	uint32	GetHighestLevel2();
	uint32	GetLowestLevel();
	void	QueuePacket(const EQApplicationPacket *app, bool ack_req = true);
	void	TeleportGroup(Mob* sender, uint32 zoneID, float x, float y, float z, float heading);
	uint16	GetAvgLevel();
	bool	LearnMembers();
	void	VerifyGroup();
	void	BalanceHP(int32 penalty, float range = 0, Mob* caster = nullptr, int32 limit = 0);
	void	BalanceMana(int32 penalty, float range = 0, Mob* caster = nullptr, int32 limit = 0);
	void	HealGroup(uint32 heal_amt, Mob* caster, float range = 0);
	int8	GetNumberNeedingHealedInGroup(int8 hpr, bool includePets);
	void	ChangeLeader(Mob* newleader);
	void	ChangeLeaderByName(std::string name);
	const char *GetClientNameByIndex(uint8 index);
	void	SetLevels();
	bool	HasOOZMember(std::string& member);

	Mob* members[MAX_GROUP_MEMBERS];
	char	membername[MAX_GROUP_MEMBERS][64];
	char	leadername[64];
	char	oldleadername[64]; // Keeps the previous leader name, so when the entity is destroyed we can still transfer leadership.
	bool	disbandcheck;
	bool	castspell;
	uint8	maxlevel;
	uint8	minlevel;

private:
	Mob*	leader;
};

#endif
