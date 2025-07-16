#ifndef ZONELIST_H_
#define ZONELIST_H_

#include "../common/types.h"
#include "../common/eqtime.h"
#include "../common/timer.h"
#include "../common/event/timer.h"
#include "../common/server_reload_types.h"
#include <vector>
#include <memory>
#include <deque>
#include <mutex>

class WorldTCPConnection;
class ServerPacket;
class ZoneServer;

class ZSList
{
public:
	enum { MaxLockedZones = 10 };

	static void ShowUpTime(WorldTCPConnection* con, const char* adminname = 0);

	ZSList();
	~ZSList();

	void Init();
	bool IsZoneLocked(uint16 iZoneID);
	bool SendPacket(ServerPacket *pack);
	bool SendPacket(uint32 zoneid, ServerPacket *pack);
	bool SetLockedZone(uint16 iZoneID, bool iLock);

	EQTime worldclock;

	inline uint32 GetNextID() { return NextID++; }
	int GetZoneCount();

	Timer *reminder;
	Timer *shutdowntimer;

	uint16 GetAvailableZonePort();
	uint32 GetAvailableZoneID();
	uint32 TriggerBootup(uint32 iZoneID);

	void Add(ZoneServer *zoneserver);
	void GetZoneIDList(std::vector<uint32> &zones);
	void KillAll();
	void ListLockedZones(const char *to, WorldTCPConnection *connection);
	void NextGroupIDs(uint32 &start, uint32 &end);
	void Process();
	void RebootZone(const char *ip1, uint16 port, const char *ip2, uint32 skipid, uint32 zoneid = 0);
	void Remove(const std::string& uuid);
	void SendChannelMessage(const char *from, const char *to, uint8 chan_num, uint8 language, uint8 lang_skill, const char *message, ...);
	void SendChannelMessageRaw(const char *from, const char *to, uint8 chan_num, uint8 language, uint8 lang_skill, const char *message);
	void SendEmoteMessage(const char *to, uint32 to_guilddbid, int16 to_minstatus, uint32 type, const char *message, ...);
	void SendEmoteMessageRaw(const char *to, uint32 to_guilddbid, int16 to_minstatus, uint32 type, const char *message);
	void SendLSZones();
	void SendTimeSync();
	void SendZoneCountConsole(const char *to, int16 admin, WorldTCPConnection *connection, bool zonepop = false);
	void SendZoneStatus(const char *to, int16 admin, WorldTCPConnection *connection);
	void SOPZoneBootup(const char *adminname, uint32 ZoneServerID, const char *zonename, bool iMakeStatic = false);
	void UpdateUCSServerAvailable(bool ucss_available = true);
	void WorldShutDown(uint32 time, uint32 interval);
	void DropClient(uint32 lsid, ZoneServer* ignore_zoneserver);

	ZoneServer *FindByID(uint32 ZoneID);
	ZoneServer *FindByName(const char *zonename);
	ZoneServer *FindByPort(uint16 port);
	ZoneServer *FindByZoneID(uint32 ZoneID);

	const std::list<std::unique_ptr<ZoneServer>> &getZoneServerList() const;
	inline uint32_t GetServerListCount() { return zone_server_list.size(); }
	void SendServerReload(ServerReload::Type type, uchar *packet = nullptr);
	std::mutex m_queued_reloads_mutex;
	std::vector<ServerReload::Type> m_queued_reloads = {};

	void QueueServerReload(ServerReload::Type &type);

private:
	void OnTick(EQ::Timer* t);
	uint32 NextID;
	uint16 pLockedZones[MaxLockedZones];
	uint32 CurGroupID;
	std::deque<uint16> m_ports_free;
	std::unique_ptr<EQ::Timer> m_tick;
	std::unique_ptr<EQ::Timer> m_keepalive;

	std::list<std::unique_ptr<ZoneServer>> zone_server_list;
};

#endif /*ZONELIST_H_*/
