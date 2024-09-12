#ifndef ZONEDB_H_
#define ZONEDB_H_

#include <unordered_set>

#include "../common/shareddb.h"
#include "../common/eq_packet_structs.h"
#include "position.h"
#include "../common/faction.h"
#include "../common/eqemu_logsys.h"
#include "../common/repositories/doors_repository.h"

class Client;
class Corpse;
class NPC;
class Petition;
class Spawn2;
class SpawnGroupList;
class Trap;
struct CharacterEventLog_Struct;
struct Door;
struct ExtendedProfile_Struct;
struct NPCType;
struct PlayerCorpse_Struct;
struct ZonePoint;
template <class TYPE> class LinkedList;

namespace EQ
{
	class ItemInstance;
}

//#include "doors.h"

struct wplist {
	int index;
	float x;
	float y;
	float z;
	int pause;
	float heading;
	bool centerpoint;
};

#pragma pack(1)
struct DBnpcspells_entries_Struct {
	int16	spellid;
	uint16	type;
	uint8	minlevel;
	uint8	maxlevel;
	int16	manacost;
	int32	recast_delay;
	int16	priority;
	int16	resist_adjust;
};
#pragma pack()

#pragma pack(1)
struct DBnpcspellseffects_entries_Struct {
	int16	spelleffectid;
	uint8	minlevel;
	uint8	maxlevel;
	int32	base;
	int32	limit;
	int32	max;
};
#pragma pack()

struct DBnpcspells_Struct {
	uint32	parent_list;
	uint16	attack_proc;
	uint8	proc_chance;
	uint16	range_proc;
	int16	rproc_chance;
	uint16	defensive_proc;
	int16	dproc_chance;
	uint32	numentries;
	uint32	fail_recast;
	uint32	engaged_no_sp_recast_min;
	uint32	engaged_no_sp_recast_max;
	uint8	engaged_beneficial_self_chance;
	uint8	engaged_beneficial_other_chance;
	uint8	engaged_detrimental_chance;
	uint32  idle_no_sp_recast_min;
	uint32  idle_no_sp_recast_max;
	uint8	idle_beneficial_chance;
	std::vector<DBnpcspells_entries_Struct> entries;
};

struct DBnpcspellseffects_Struct {
	uint32	parent_list;
	uint32	numentries;
	DBnpcspellseffects_entries_Struct entries[0];
};

struct DBTradeskillRecipe_Struct {
	EQ::skills::SkillType tradeskill;
	int16 skill_needed;
	uint16 trivial;
	bool nofail;
	bool replace_container;
	std::vector< std::pair<uint32,uint8> > onsuccess;
	std::vector< std::pair<uint32,uint8> > onfail;
	std::string name;
	uint32 recipe_id;
	bool quest;
};

struct PetRecord {
	uint32 npc_type;	// npc_type id for the pet data to use
	bool temporary;
	int16 petpower;
	uint8 petcontrol;	// What kind of control over the pet is possible (Animation, familiar, ...)
	uint8 petnaming;		// How to name the pet (Warder, pet, random name, familiar, ...)
	bool monsterflag;	// flag for if a random monster appearance should get picked
	uint32 equipmentset;	// default equipment for the pet
};

struct CharacterCorpseEntry 
{
	uint32	crc;
	bool	locked;
	uint32	itemcount;
	uint32	exp;
	uint32	gmexp;
	float	size;
	uint8	level;
	uint32	race;
	uint8	gender;
	uint8	class_;
	uint8	deity;
	uint8	texture;
	uint8	helmtexture;
	uint32	copper;
	uint32	silver;
	uint32	gold;
	uint32	plat;
	EQ::TintProfile item_tint;
	uint8 haircolor;
	uint8 beardcolor;
	uint8 eyecolor1;
	uint8 eyecolor2;
	uint8 hairstyle;
	uint8 face;
	uint8 beard;
	uint8 killedby;
	bool  rezzable;
	uint32	rez_time;
	uint32 time_of_death;
	LootItem items[0];
};

// Actual pet info for a client.
struct PetInfo {
	uint16	SpellID;
	int16	petpower;
	uint32	HP;
	uint32	Mana;
	float	size;
	int16	focusItemId;
	SpellBuff_Struct	Buffs[BUFF_COUNT];
	uint32	Items[EQ::invslot::EQUIPMENT_COUNT];
	char	Name[64];
};

struct ZoneSpellsBlocked {
	uint32 spellid;
	int8 type;
	glm::vec3 m_Location;
	glm::vec3 m_Difference;
	char message[256];
};

struct TraderCharges_Struct {
	uint32 ItemID[80];
	int32 SlotID[80];
	uint32 ItemCost[80];
	int32 Charges[80];
};

namespace NPCSpawnTypes {
	enum : uint8 {
		CreateNewSpawn,
		AddNewSpawngroup,
		UpdateAppearance,
		RemoveSpawn,
		DeleteSpawn,
		AddSpawnFromSpawngroup,
		CreateNewNPC
	};
}

class ZoneDatabase : public SharedDatabase {
	typedef std::list<LootItem*> ItemList;
public:
	ZoneDatabase();
	ZoneDatabase(const char* host, const char* user, const char* passwd, const char* database,uint32 port);
	virtual ~ZoneDatabase();

	/* Objects and World Containers  */
	void	LoadWorldContainer(uint32 parentid, EQ::ItemInstance* container);
	void	SaveWorldContainer(uint32 zone_id, uint32 parent_id, const EQ::ItemInstance* container);
	void	DeleteWorldContainer(uint32 parent_id,uint32 zone_id);
	uint32	AddObject(uint32 type, uint32 icon, const Object_Struct& object, const EQ::ItemInstance* inst);
	void	UpdateObject(uint32 id, uint32 type, uint32 icon, const Object_Struct& object, const EQ::ItemInstance* inst);
	void	DeleteObject(uint32 id);
	Ground_Spawns*	LoadGroundSpawns(uint32 zone_id, Ground_Spawns* gs);

	/* Traders  */
	void	SaveTraderItem(uint32 char_id,uint32 itemid,int16 islotid, int32 charges,uint32 itemcost,uint8 slot);
	void	UpdateTraderItemCharges(int char_id, int16 slotid, int32 charges);
	void	UpdateTraderItemPrice(int CharID, uint32 ItemID, uint32 Charges, uint32 NewPrice);
	void	DeleteTraderItem(uint32 char_id);
	void	DeleteTraderItem(uint32 char_id,uint16 slot_id);
	void	IncreaseTraderSlot(uint32 char_id,uint16 slot_id);

	EQ::ItemInstance* LoadSingleTraderItem(uint32 char_id, int16 islotid, uint16 slotid);
	Trader_Struct* LoadTraderItem(uint32 char_id);
	TraderCharges_Struct* LoadTraderItemWithCharges(uint32 char_id);
	int8 ItemQuantityType(int16 item_id);
	int16 GetTraderItemBySlot(uint32 char_id, int8 slotid);

	void CommandLogs(const char* char_name, const char* acct_name, float y, float x, float z, const char* command, const char* targetType, const char* target, float tar_y, float tar_x, float tar_z, uint32 zone_id, const char* zone_name);
	void SaveBuffs(Client *c);
	void LoadBuffs(Client *c);
	void LoadPetInfo(Client *c);
	void SavePetInfo(Client *c);
	void RemoveTempFactions(Client *c);

	/* Character Data Loaders  */
	bool	LoadCharacterFactionValues(uint32 character_id, faction_map & val_list);
	bool	LoadCharacterSpellBook(uint32 character_id, PlayerProfile_Struct* pp);
	bool	LoadCharacterMemmedSpells(uint32 character_id, PlayerProfile_Struct* pp);
	bool	LoadCharacterLanguages(uint32 character_id, PlayerProfile_Struct* pp);
	bool	LoadCharacterSkills(uint32 character_id, PlayerProfile_Struct* pp);
	bool	LoadCharacterData(uint32 character_id, PlayerProfile_Struct* pp, ExtendedProfile_Struct* m_epp);
	bool	LoadCharacterCurrency(uint32 character_id, PlayerProfile_Struct* pp);
	bool	LoadCharacterBindPoint(uint32 character_id, PlayerProfile_Struct* pp);

	/* Character Data Saves  */
	bool	SaveCharacterCurrency(uint32 character_id, PlayerProfile_Struct* pp);
	bool	SaveCharacterData(uint32 character_id, uint32 account_id, PlayerProfile_Struct* pp, ExtendedProfile_Struct* m_epp);
	bool	SaveCharacterAA(uint32 character_id, uint32 aa_id, uint32 current_level);
	bool	SaveCharacterSpell(uint32 character_id, uint32 spell_id, uint32 slot_id);
	bool	SaveCharacterMemorizedSpell(uint32 character_id, uint32 spell_id, uint32 slot_id);
	bool	SaveCharacterSkill(uint32 character_id, uint32 skill_id, uint32 value);
	bool	SaveCharacterLanguage(uint32 character_id, uint32 lang_id, uint32 value);
	bool	SaveCharacterConsent(char grantname[64], char ownername[64]);
	bool	SaveCharacterConsent(char grantname[64], char ownername[64], std::list<CharacterConsent> &consent_list);
	bool	SaveAccountShowHelm(uint32 account_id, bool value);
	static void SaveCharacterBinds(Client* c);

	/* Character Data Deletes   */
	bool	DeleteCharacterSpell(uint32 character_id, uint32 spell_id, uint32 slot_id);
	bool	DeleteCharacterMemorizedSpell(uint32 character_id, uint32 spell_id, uint32 slot_id);
	bool	DeleteCharacterAAs(uint32 character_id);
	bool	DeleteCharacterConsent(char grantname[64], char ownername[64], uint32 corpse_id);

	/* Character Inventory  */
	bool	SaveSoulboundItems(Client* client, std::list<EQ::ItemInstance*>::const_iterator &start, std::list<EQ::ItemInstance*>::const_iterator &end);
	bool	SaveCursor(Client* client, std::list<EQ::ItemInstance*>::const_iterator &start, std::list<EQ::ItemInstance*>::const_iterator &end);

	/* Corpses  */
	bool		DeleteItemOffCharacterCorpse(uint32 db_id, uint32 equip_slot, uint32 item_id);
	uint32		GetCharacterCorpseItemCount(uint32 corpse_id);
	bool		LoadCharacterCorpseRezData(uint32 corpse_id, uint32 *exp, uint32 *gmexp, bool *rezzable, bool *is_rezzed);
	bool		LoadCharacterCorpseData(uint32 corpse_id, CharacterCorpseEntry* corpse);
	Corpse*		LoadCharacterCorpse(uint32 player_corpse_id);
	Corpse*		SummonBuriedCharacterCorpses(uint32 char_id, uint32 dest_zoneid, const glm::vec4& position);
	Corpse*		SummonCharacterCorpse(uint32 corpse_id, uint32 char_id, uint32 dest_zoneid, const glm::vec4& position);
	void		MarkCorpseAsRezzed(uint32 dbid);
	bool		BuryCharacterCorpse(uint32 dbid);
	bool		BuryAllCharacterCorpses(uint32 charid);
	bool		DeleteCharacterCorpse(uint32 dbid);
	bool		SummonAllCharacterCorpses(uint32 char_id, uint32 dest_zoneid, const glm::vec4& position);
	bool		UnburyCharacterCorpse(uint32 dbid, uint32 new_zoneid, const glm::vec4& position);
	bool		LoadCharacterCorpses(uint32 iZoneID);
	bool		DeleteGraveyard(uint32 zone_id, uint32 graveyard_id);
	uint32		GetCharacterCorpseDecayTimer(uint32 corpse_db_id);
	uint32		GetCharacterBuriedCorpseCount(uint32 char_id);
	uint32		SendCharacterCorpseToGraveyard(uint32 dbid, uint32 zoneid, const glm::vec4& position);
	uint32		CreateGraveyardRecord(uint32 graveyard_zoneid, const glm::vec4& position);
	uint32		AddGraveyardIDToZone(uint32 zone_id, uint32 graveyard_id);
	uint32		SaveCharacterCorpse(uint32 charid, const char* charname, uint32 zoneid, CharacterCorpseEntry* corpse, const glm::vec4& position);
	bool		SaveCharacterCorpseBackup(uint32 corpse_id, uint32 charid, const char* charname, uint32 zoneid, CharacterCorpseEntry* corpse, const glm::vec4& position);
	uint32		UpdateCharacterCorpse(uint32 dbid, uint32 charid, const char* charname, uint32 zoneid, CharacterCorpseEntry* corpse, const glm::vec4& position, bool rezzed = false);
	bool		UpdateCharacterCorpseBackup(uint32 dbid, uint32 charid, const char* charname, uint32 zoneid, CharacterCorpseEntry* corpse, const glm::vec4& position, bool rezzed = false);
	uint32		GetFirstCorpseID(uint32 char_id);
	uint32		GetCharacterCorpseCount(uint32 char_id);
	uint32		GetCharacterCorpseID(uint32 char_id, uint8 corpse);
	uint32		GetCharacterCorpseItemAt(uint32 corpse_id, uint16 slotid);
	bool		IsValidCorpseBackup(uint32 corpse_id);
	bool		IsValidCorpse(uint32 corpse_id);
	bool		CopyBackupCorpse(uint32 corpse_id);
	bool		IsCorpseBackupOwner(uint32 corpse_id, uint32 char_id);

	/* Faction   */
	bool		GetNPCFactionList(uint32 npcfaction_id, int32* faction_id, int32* value, uint8* temp, int32* primary_faction = 0);
	bool		GetFactionData(FactionMods* fd, uint32 class_mod, uint32 race_mod, uint32 deity_mod, int32 faction_id, uint8 texture_mod, uint8 gender_mod, uint32 base_race, bool skip_illusions = false); //needed for factions Dec, 16 2001
	bool		GetFactionName(int32 faction_id, char* name, uint32 buflen); // needed for factions Dec, 16 2001
	std::string GetFactionName(int32 faction_id);
	bool		GetFactionIdsForNPC(uint32 nfl_id, std::list<struct NPCFaction*> *faction_list, int32* primary_faction = 0); // improve faction handling
	bool		SetCharacterFactionLevel(uint32 char_id, int32 faction_id, int32 value, uint8 temp, faction_map &val_list); // needed for factions Dec, 16 2001
	bool		LoadFactionData();
	bool		SameFactions(uint32 npcfaction_id1, uint32 npcfaction_id2); //Returns true if both factions have the same primary, and faction hit list is the same (hit values are ignored.)
	bool		SeeIllusion(int32 faction_id);
	int16		MinFactionCap(int32 faction_id);
	int16		MaxFactionCap(int32 faction_id);
	inline uint32 GetMaxFaction() { return max_faction; }

	/* AAs */
	bool	LoadAlternateAdvancementActions();
	bool	LoadAlternateAdvancementEffects();
	SendAA_Struct*	GetAASkillVars(uint32 skill_id);
	uint8	GetTotalAALevels(uint32 skill_id);
	uint32	GetMacToEmuAA(uint8 eqmacid);
	uint32	CountAAs();
	void	LoadAlternateAdvancement(SendAA_Struct **load);
	uint32 CountAAEffects();

	/* Zone related */
	bool	GetZoneCFG(uint32 zoneid, NewZone_Struct *data, bool &can_bind, bool &can_combat, bool &can_levitate, bool &can_castoutdoor, bool& is_hotzone, bool &is_city, uint8 &zone_type, int &ruleset, char **map_filename, bool &can_bind_others, bool &skip_los, bool &drag_aggro, bool &can_castdungeon, uint16 &pull_limit);
	bool	SaveZoneCFG(uint32 zoneid, NewZone_Struct* zd);
	bool	LoadStaticZonePoints(LinkedList<ZonePoint*>* zone_point_list,const char* zonename);
	bool		UpdateZoneSafeCoords(const char* zonename, const glm::vec3& location);
	uint8	GetUseCFGSafeCoords();
	int		getZoneShutDownDelay(uint32 zoneID);

	/* Spawns and Spawn Points  */
	bool		LoadSpawnGroups(const char* zone_name, SpawnGroupList* spawn_group_list);
	bool		LoadSpawnGroupsByID(int spawngroupid, SpawnGroupList* spawn_group_list);
	bool		PopulateZoneSpawnList(uint32 zoneid, LinkedList<Spawn2*> &spawn2_list);
	bool		PopulateZoneSpawnListClose(uint32 zoneid, LinkedList<Spawn2*> &spawn2_list, const glm::vec4& client_position, uint32 repop_distance);
	bool		PopulateRandomZoneSpawnList(uint32 zoneid, LinkedList<Spawn2*> &spawn2_list);
	bool		CreateSpawn2(Client *c, uint32 spawngroup, const char* zone, const glm::vec4& position, uint32 respawn, uint32 variance, uint16 condition, int16 cond_value);
	void		UpdateRespawnTime(uint32 id, uint32 timeleft);
	uint32		GetSpawnTimeLeft(uint32 id);
	void		UpdateSpawn2Status(uint32 id, uint8 new_status);

	/* Grids/Paths  */
	uint32		GetFreeGrid(uint16 zoneid);
	void		DeleteWaypoint(Client *c, uint32 grid_num, uint32 wp_num, uint16 zoneid);
	void		AddWP(Client *c, uint32 gridid, uint32 wpnum, const glm::vec4& position, uint32 pause, uint16 zoneid);
	uint32		AddWPForSpawn(Client *c, uint32 spawn2id, const glm::vec4& position, uint32 pause, int type1, int type2, uint16 zoneid);
	void		ModifyGrid(Client* c, bool remove, uint32 grid_id, uint8 type = 0, uint8 type2 = 0, uint32 zone_id = 0);
	bool		GridExistsInZone(uint32 zone_id, uint32 grid_id);
	uint8		GetGridType(uint32 grid, uint32 zoneid);
	uint8		GetGridType2(uint32 grid, uint16 zoneid);
	bool		GetWaypoints(uint32 grid, uint16 zoneid, uint32 num, wplist* wp);
	void        AssignGrid(Client *client, int grid, int spawn2id);
	int			GetHighestGrid(uint32 zoneid);
	int			GetHighestWaypoint(uint32 zoneid, uint32 gridid);
	int			GetRandomWaypointLocFromGrid(glm::vec4 &loc, uint16 zoneid, int grid);

	/* NPCs  */

	uint32		NPCSpawnDB(uint8 command, const char* zone, Client *c, NPC* spawn = 0, uint32 extra = 0); // 0 = Create 1 = Add; 2 = Update; 3 = Remove; 4 = Delete
	uint32		CreateNewNPCCommand(const char* zone, Client *client, NPC* spawn, uint32 extra);
	uint32		AddNewNPCSpawnGroupCommand(const char* zone,  Client *client, NPC* spawn, uint32 respawnTime);
	uint32		DeleteSpawnLeaveInNPCTypeTable(const char* zone, Client *client, NPC* spawn);
	uint32		DeleteSpawnRemoveFromNPCTypeTable(const char* zone, Client *client, NPC* spawn);
	uint32		AddSpawnFromSpawnGroup(const char* zone, Client *client, NPC* spawn, uint32 spawnGroupID);
	uint32		AddNPCTypes(const char* zone, Client *client, NPC* spawn, uint32 spawnGroupID);
	uint32		UpdateNPCTypeAppearance(Client *client, NPC* spawn);
	bool		GetPetEntry(const char *pet_type, PetRecord *into);
	bool		GetPoweredPetEntry(const char *pet_type, int16 petpower, PetRecord *into);
	void		AddLootTableToNPC(NPC* npc, uint32 loottable_id, LootItems* itemlist, uint32* copper, uint32* silver, uint32* gold, uint32* plat);
	void		AddLootDropToNPC(NPC* npc, uint32 lootdrop_id, LootItems* itemlist, uint8 droplimit, uint8 mindrop);
	uint32		GetMaxNPCSpellsID();
	uint32		GetMaxNPCSpellsEffectsID();
	void LoadGlobalLoot();

	DBnpcspells_Struct*				GetNPCSpells(uint32 npc_spells_id);
	DBnpcspellseffects_Struct*		GetNPCSpellsEffects(uint32 iDBSpellsEffectsID);
	void ClearNPCSpells() { npc_spells_cache.clear(); npc_spells_loadtried.clear(); }
	const NPCType*					LoadNPCTypesData(uint32 id, bool bulk_load = false);

	/* Petitions   */
	void	UpdateBug(BugReport_Struct* bug_report, uint32 clienttype);
	void	UpdateFeedback(Feedback_Struct* feedback);
	void	DeletePetitionFromDB(Petition* wpet);
	void	UpdatePetitionToDB(Petition* wpet);
	void	InsertPetitionToDB(Petition* wpet);
	void	RefreshPetitionsFromDB();
	void	AddSoulMark(uint32 charid, const char* charname, const char* accname, const char* gmname, const char* gmacctname, uint32 utime, uint32 type, const char* desc);
	int		RemoveSoulMark(uint32 charid);

	/* Merchants  */
	void	SaveMerchantTemp(uint32 npcid, uint32 slot, uint32 item, uint32 charges, uint32 quantity);
	void	DeleteMerchantTemp(uint32 npcid, uint32 slot);
	void	DeleteMerchantTempList(uint32 npcid);

	/* Tradeskills  */
	bool	GetTradeRecipe(const EQ::ItemInstance* container, uint8 c_type, uint32 some_id, uint32 char_id, DBTradeskillRecipe_Struct *spec);
	bool	GetTradeRecipe(uint32 recipe_id, uint8 c_type, uint32 some_id, uint32 char_id, DBTradeskillRecipe_Struct *spec);
	uint32	GetZoneForage(uint32 ZoneID, uint8 skill); /* for foraging */
	uint32	GetZoneFishing(uint32 ZoneID, uint8 skill);
	bool	EnableRecipe(uint32 recipe_id);
	bool	DisableRecipe(uint32 recipe_id);
	bool	UpdateSkillDifficulty(uint16 skillid, float difficulty);

	/*
	* Doors
	*/
	bool	DoorIsOpen(uint8 door_id,const char* zone_name);
	void	SetDoorPlace(uint8 value,uint8 door_id,const char* zone_name);
	std::vector<DoorsRepository::Doors> LoadDoors(const std::string &zone_name);
	int32	GetDoorsCount(uint32* oMaxID, const char *zone_name);
	int32	GetDoorsCountPlusOne(const char *zone_name);
	int32	GetDoorsDBCountPlusOne(const char *zone_name);

	/* Blocked Spells   */
	int32	GetBlockedSpellsCount(uint32 zoneid);
	bool	LoadBlockedSpells(int32 blockedSpellsCount, ZoneSpellsBlocked* into, uint32 zoneid);

	/* Traps   */
	bool	LoadTraps(const char* zonename);
	bool	SetTrapData(Trap* trap, bool repopnow = false);

	/* Time   */
	uint32	GetZoneTZ(uint32 zoneid);
	bool	SetZoneTZ(uint32 zoneid, uint32 tz);

	/* Group   */
	void RefreshGroupFromDB(Client *c);
	void RefreshGroupLeaderFromDB(Client *c);
	uint8 GroupCount(uint32 groupid);

	/* Raid   */
	uint8 RaidGroupCount(uint32 raidid, uint32 groupid);

	/* QGlobals   */
	void QGlobalPurge();

	/*MBMessages*/
	bool RetrieveMBMessages(uint16 category, std::vector<MBMessageRetrievalGen_Struct>& outData);
	bool PostMBMessage(uint32 charid, const char* charName, MBMessageRetrievalGen_Struct* inData);
	bool EraseMBMessage(uint32 id, uint32 charid);
	bool ViewMBMessage(uint32 id, char* outData);

	/*
		* Misc stuff.
		* PLEASE DO NOT ADD TO THIS COLLECTION OF CRAP UNLESS YOUR METHOD
		* REALLY HAS NO BETTER SECTION
	*/
	bool	logevents(const char* accountname,uint32 accountid,uint8 status,const char* charname,const char* target, const char* descriptiontype, const char* description,int event_nid);
	uint32	GetKarma(uint32 acct_id);
	void	UpdateKarma(uint32 acct_id, uint32 amount);
	int16  GetTimerFromSkill(EQ::skills::SkillType skillid);


protected:
	void ZDBInitVars();

	uint32				max_faction;
	Faction**			faction_array;
	uint32 npc_spellseffects_maxid;
	std::unordered_map<uint32, DBnpcspells_Struct> npc_spells_cache;
	std::unordered_set<uint32> npc_spells_loadtried;
	DBnpcspellseffects_Struct **npc_spellseffects_cache;
	bool *npc_spellseffects_loadtried;
	uint8 door_isopen_array[255];
};

extern ZoneDatabase database;

#endif /*ZONEDB_H_*/

