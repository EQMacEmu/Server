#ifndef TRILOGY_STRUCTS_H_
#define TRILOGY_STRUCTS_H_

#include <string>

namespace Trilogy {
	namespace structs {

		/*
		** Compiler override to ensure
		** byte aligned structures
		*/
#pragma pack(1)

		// Valus of weapon types
		enum SKILLTYPE : uint8
		{
			SKILLTYPE_1HSLASHING = 0x00,
			SKILLTYPE_2HSLASHING = 0x01,
			SKILLTYPE_PIERCING = 0x02,
			SKILLTYPE_1HBLUNT = 0x03,
			SKILLTYPE_2HBLUNT = 0x04,
			SKILLTYPE_2HPIERCING = 0x23
		};

		enum TSpawnAppearanceType : uint16
		{
			SAT_SendToBind = 0,	// Comment: 0=send to bind, >0 = kill them
			SAT_LevelChange = 1,	// Comment: the level that shows up on /who
			SAT_Invis = 3,	// Comment: 0=visable, 1=invisable
			SAT_PvP = 4,	// Comment:	0=disable, 1=enable
			SAT_Light = 5,	// Comment:	
			SAT_Position_Update = 14,	// Comment: 100=standing, 102=lose control (fear, mez, charm), 110=sitting, 111=ducking, 115=feigned, 105=looting
			SAT_Sneaking = 15,	// Comment:
			SAT_Camp = 16,	// Comment:
			SAT_HP_Regen = 17,	// Comment: Client->Server, my HP has changed (like regen tic)
			SAT_LD = 18,	// Comment:
			SAT_Levitate = 19,	// Comment: 0=off, 1=flymode, 2=levitate
			SAT_GM_Tag = 20,
			SAT_NameColor = 21,	// Comment:	AT_PVP[0=green,1=dark red,2=light red] or !AT_PVP[0=green,1=blue,2=purple]
			SAT_Guild_ID = 22,	// Comment: Joins player to the specified guild id param.
			SAT_Guild_Rank = 23,	// Comment: 0=member, 1=officer, 2=leader
			SAT_AFK = 24,	// Comment: 0=off, 1=on
			SAT_Autosplit = 28,
			SAT_Size = 29,	// Comment: spawn's size (0=smallest)
			SAT_NPC_Name = 30	// Comment: change PC's name's color to NPC color (unreversable)
		};

		enum TBodyType : uint16
		{
			BT_Humanoid = 1,
			BT_Lycanthrope = 2,
			BT_Undead = 3,
			BT_Giant = 4,
			BT_Construct = 5,
			BT_Extraplanar = 6,
			BT_Magical = 7,	//this name might be a bit off, 
			BT_SummonedUndead = 8,
			BT_NoTarget = 11,	// can't target this bodytype
			BT_Vampire = 12,
			BT_Atenha_Ra = 13,
			BT_Greater_Akheva = 14,
			BT_Khati_Sha = 15,
			BT_Zek = 19,
			BT_Luggald = 20,
			BT_Animal = 21,
			BT_Insect = 22,
			BT_Monster = 23,
			BT_Summoned = 24,	//Elemental?
			BT_Plant = 25,
			BT_Dragon = 26,
			BT_Summoned2 = 27,
			BT_Summoned3 = 28,
			BT_VeliousDragon = 30,	//might not be a tight set
			BT_Dragon3 = 32,
			BT_Boxes = 33,
			BT_Muramite = 34,	//tribal dudes
			BT_NoTarget2 = 60,
			BT_SwarmPet = 63,	//is this valid, or made up?
			BT_Trigger = 65,   //Body type to completely hide the spawn
			BT_InvisMan = 66,	//seen on 'InvisMan'
			BT_Special = 67
		};

		enum TSpawnAppearancePositionParameter : uint32
		{
			SAPP_Sitting_To_Standing = 100,
			SAPP_Lose_Control = 102,
			SAPP_Looting = 105,
			SAPP_Standing_To_Sitting = 110,
			SAPP_Ducking = 111,
			SAPP_Unconscious = 115
		};

		struct Color_Struct
		{
			union
			{
				struct
				{
					uint8	blue;
					uint8	green;
					uint8	red;
					uint8	use_tint;	// if there's a tint this is FF
				} rgb;
				uint32 color;
			};
		};

		struct CharacterSelect_Struct
		{
			/*0000*/	char	name[10][30];	// Comment: Name of characters
			/*0300*/	int8	level[10];			// Comment: Level of characters
			/*0310*/	int8	class_[10];			// Comment: Class of characters
			/*0320*/	int16	race[10];			// Comment: Race of characters
			/*0340*/	char	zone[10][20];		// Comment: Current zone of characters
			/*0540*/	int8	gender[10];			// Comment: Gender of characters
			/*0550*/	int8	face[10];			// Comment: Face of characters (Values?)
			/*0560*/	int8	equip[10][9];		// Comment: Needs confirming -> What weap the char is equiping at char select??? (0 - 4 (4 only for humans))
			/*0650*/	int8	unknown1[2];
			/*0652*/	Color_Struct cs_colors[10][9];	// Comment: Values (RR GG BB AA)
			/*1010*/	uint16	deity[10];		// Comment: Unknown => needs to be found
			/*1020*/	uint32	primary[10];
			uint32	secondary[10];
			/*1020*/	int8	unknown2[148];		// Comment: Unknown => needs to be found
		};

		//This is used to show weapons on char select screen
		struct CharWeapon_Struct
		{
			/*000*/	uint16 righthand[10];
			/*020*/ uint16 lefthand[10];
			/*040*/
		};

		struct ClientZoneEntry_Struct {
			/*0000*/	uint32	unknown00;
			/*0004*/	char	char_name[32];			// Character Name
		};

		struct ServerZoneEntry_Struct
		{
			/*000*/	int8	checksum[4];			// Comment: 
			/*004*/	int8	sze_unknown1;			// Comment: 
			/*005*/	char	name[30];				// Comment: 
			/*035*/	char	zone[15];				// Comment: 
			/*050*/	int8	sze_unknown2[6];		// Comment: 
			/*056*/	float	y_pos;						// Comment: 
			/*060*/	float	x_pos;						// Comment: 
			/*064*/	float	z_pos;						// Comment: 
			/*068*/ float   heading;				// Comment: Cofruben: es cierto que heading est aqu? sino aadir 4 bytes mas al siguiente.
			/*072*/ int8    sze_unknown3[66];		// Comment: 
			/*138*/	uint16	petOwnerId;
			/*140*/	uint32	max_hp;
			/*144*/	uint32	curHP;
			/*148*/ int16   GuildID;				// Comment: 
			/*150*/	uint8	my_socket[6];		// ***Placeholder
			/*156*/ uint8	NPC;
			/*157*/ int8    class_;					// Comment: 
			/*158*/	int16	race;					// Comment: Cofruben: sino va bien, hacerlo int8 y aadir despues un unknown
			/*160*/	int8	gender;					// Comment: 
			/*161*/	int8	level;					// Comment: 
			/*162*/	int8    invis;
			/*163*/	int8	sneaking;
			/*164*/	int8    pvp;					// Comment: 0x0=non_pvp 0x1=pvp
			/*165*/	uint8    anim_type;
			/*166*/	int8    light;
			/*167*/	int8    face;					// Comment: 
			/*168*/	uint8   equipment[9]; // Array elements correspond to struct equipment above
			/*177*/	int8    sze_unknown5_2_2_6[3];	// Comment: 
			/*180*/	Color_Struct equipcolors[9]; // Array elements correspond to struct equipment_colors above
			/*216*/	uint32	bodytexture;	// Texture (0xFF=Player - See list of textures for more)
			/*220*/	float	size;
			/*224*/	float	width;
			/*228*/	float	length;
			/*232*/	uint32	helm;
			/*236*/	float   walkspeed;				// Comment: 
			/*240*/	float   runspeed;				// Comment: 
			/*244*/	int8	LD;
			/*245*/	int8	GM;
			/*246*/	int16	flymode;
			/*248*/	int8	bodytype;
			/*249*/	int8	view_player[7];
			/*256*/	int8    anon;					// Comment: 
			/*257*/	int8    sze_unknown6_13[23];	// Comment: At least one flag in here disables a zone point or all
			/*280*/ char	Surname[20];			// Comment: 
			/*300*/	int8    sze_unknown_add2[2];	// Comment: 
			/*302*/ int16	deity;					// Comment: 
			/*304*/ int8	sze_unknown_add2_1[8];	// Comment: 
			/*312*/
		};

		struct ZoneServerInfo_Struct
		{
			/*0000*/	char	ip[128];
			/*0128*/	uint16	port;
		};

		struct NewZone_Struct
		{
			/*0000*/	char	char_name[30];			// Character Name
			/*0064*/	char	zone_short_name[20];	// Zone Short Name
			/*0096*/	char	zone_long_name[180];	// Zone Long Name
			/*0374*/	uint8	ztype;
			/*0375*/	uint8	fog_red[4];				// Red Fog 0-255 repeated over 4 bytes (confirmed)
			/*0379*/	uint8	fog_green[4];			// Green Fog 0-255 repeated over 4 bytes (confirmed)
			/*0383*/	uint8	fog_blue[4];			// Blue Fog 0-255 repeated over 4 bytes (confirmed)
			/*0387*/	uint8	unknown387;
			/*0388*/	float	fog_minclip[4];			// Where the fog begins (lowest clip setting). Repeated over 4 floats. (confirmed)
			/*0404*/	float	fog_maxclip[4];			// Where the fog ends (highest clip setting). Repeated over 4 floats. (confirmed)	
			/*0420*/	float	gravity;
			/*0424*/	uint8	time_type;
			/*0425*/    uint8   rain_chance[4];
			/*0429*/    uint8   rain_duration[4];
			/*0433*/    uint8   snow_chance[4];
			/*0437*/    uint8   snow_duration[4];
			/*0441*/	uint8	specialdates[16];
			/*0457*/	uint8	specialcodes[16];
			/*0473*/	uint8	timezone;
			/*0474*/	uint8	sky;					// Sky Type
			/*0475*/	uint8   unknown0475;
			/*0476*/	uint16  water_music;
			/*0478*/	uint16  normal_music_day;
			/*0480*/	uint16  normal_music_night;
			/*0482*/	uint8	unknown0482[2];
			/*0484*/	float	zone_exp_multiplier;	// Experience Multiplier
			/*0488*/	float	safe_y;					// Zone Safe Y
			/*0492*/	float	safe_x;					// Zone Safe X
			/*0496*/	float	safe_z;					// Zone Safe Z
			/*0500*/	float	max_z;					// Guessed
			/*0504*/	float	underworld;				// Underworld, min z (Not Sure?)
			/*0508*/	float	minclip;				// Minimum View Distance
			/*0512*/	float	maxclip;				// Maximum View DIstance
			/*0516*/	uint32	forage_novice;
			/*0520*/	uint32	forage_medium;
			/*0524*/	uint32	forage_advanced;
			/*0528*/	uint32	fishing_novice;
			/*0532*/	uint32	fishing_medium;
			/*0536*/	uint32	fishing_advanced;
			/*0540*/	uint32	skylock;
			uint32  unknown_end;
			/*0572*/
		};

		struct MemorizeSpell_Struct
		{
			/*000*/		uint32 slot;			// Comment:  Spot in the spell book/memorized slot 
			/*004*/		uint32 spell_id;		// Comment:  Spell id (200 or c8 is minor healing, etc) 
			/*008*/		uint32 scribing;		// Comment:  1 if memorizing a spell, set to 0 if scribing to book 
			/*012*/
		};

		struct CombatDamage_Struct
		{
			/*000*/	uint32	target; // confirmed
			/*004*/	uint32	source; // confirmed
			/*008*/	uint8	type; // confirmed
			/*009*/	uint8   unknown;
			/*010*/	uint16	spellid; // confirmed
			/*012*/	int32	damage; // confirmed
			/*016*/	float	force;
			/*020*/	float	sequence;
			/*024*/ float	pushup_angle;
			/*028*/
		};

		struct Action_Struct
		{
			/*00*/	uint32	target;				// Target ID
			/*04*/	uint32	source;				// Caster ID
			/*08*/	uint8	level;				// Caster Level
			/*09*/	uint8	unknown09;			// 0x00
			/*10*/	uint8	spell_level;		// 0x41
			/*11*/	uint8	unknown11;			// 0x00
			/*12*/	uint8	instrument_mod;		// 0x0A (or instrument mod)
			/*13*/	uint8	unknown13[7];		// zeros
			/*20*/	float	sequence;			// heading
			/*24*/	uint8	unknown24[4];		// zeros
			/*28*/	uint32	type;				// 231 (0xE7) for spell
			/*32*/	uint16	spell;				// spell ID
			/*34*/	uint8	unknown34;			// 0x00
			/*35*/	uint8	buff_unknown;		// 0x04 for buff success!
			/*36*/
		};

		struct InterruptCast_Struct
		{
			/*00*/	uint32	spawnid;		// Comment: ID of Spell Caster
			/*04*/	char	message[0];		// Comment: Interrupt Message.
			/*05*/
		};

		struct ManaChange_Struct
		{
			/*00*/	uint16 new_mana;	// Comment:  New Mana AMount
			/*02*/	uint16 spell_id;	// Comment:  Last Spell Cast
			/*04*/
		};

		struct ManaChange_Struct2
		{
			/*00*/	uint16 new_mana;	// Comment:  New Mana AMount
			/*02*/
		};

		struct SwapSpell_Struct
		{
			/*00*/	uint32 from_slot;	// Comment: Moving Spells around?
			/*04*/	uint32 to_slot;		// Comment: Moving Spells Around?
			/*08*/
		};

		struct BeginCast_Struct
		{
			/*000*/	uint32	caster_id;		// Comment: Unknown -> needs confirming -> ID of Spell Caster? 
			/*004*/	uint32	spell_id;		// Comment: Unknown -> needs confirming -> ID of Spell being Cast?
			/*008*/	uint32	cast_time;		// Comment: Unknown -> needs confirming -> in miliseconds?
			/*012*/
		};

		struct Buff_Struct
		{
			/*000*/	uint32	visible;		// Comment: Unknown -> needs confirming -> Target of the Buff
			/*004*/	uint32	b_unknown1;		// Comment: Unknown -> needs confirming
			/*008*/	uint16	spellid;		// Comment: Unknown -> needs confirming -> Spell ID?
			/*010*/	uint32	b_unknown2;		// Comment: Unknown -> needs confirming
			/*014*/	uint16	b_unknown3;		// Comment: Unknown -> needs confirming
			/*016*/	uint32	slotid;		// Comment: Unknown -> needs confirming -> Which buff slot on the target maybe?
			/*020*/
		};

#define SLOT_ITEMSPELL		10		// we right clicked into a clickable item which allow us casting some spell.
		struct CastSpell_Struct
		{
			/*000*/	uint16	slot;
			/*002*/	uint16	spell_id;
			/*004*/	int16	inventoryslot;  // slot for clicky item, 0xFFFF = normal cast
			/*006*/ uint8	unknown6[2];
			/*008*/	uint32	target_id;
			/*012*/	uint32	cs_unknown2;
			/*016*/
		};

		struct SpawnAppearance_Struct
		{
			// len = 12
			/*000*/ uint16 spawn_id;          // ID of the spawn
			/*002*/ uint16 unknown2;
			/*004*/ uint16 type;              // Values associated with the type
			/*006*/ uint16 unknown6;
			/*008*/ uint32 parameter;         // Type of data sent
			/*012*/
		};

		// Length: 24
		struct SpellBuffFade_Struct
		{
			/*000*/	uint32	entityid;
			/*004*/	uint8   slot;
			/*005*/ uint8   level;
			/*006*/ uint8   effect;
			/*007*/ uint8   unknown1;
			/*008*/	uint16  spellid;
			/*010*/	uint32	slotid;
			/*014*/	uint32	duration;
			/*018*/	uint16	bufffade;
			/*020*/
		};

		/*
		** client changes target struct
		** Length: 2 Bytes
		** OpCode: 6221
		*/
		struct ClientTarget_Struct
		{
			/*000*/	uint16	new_target;			// Target ID
			/*002*/
		};

		struct Spawn_Struct
		{
			/*0000*/	uint32	unknown_start;
			/*0004*/	float	size;
			/*0008*/	float	walkspeed;
			/*0012*/	float	runspeed;
			/*0016*/	Color_Struct	equipcolors[9];
			/*0052*/	uint8	accel;
			/*0053*/	uint8	heading;			// Current Heading
			/*0054*/	int8	deltaHeading;		// Delta Heading
			/*0055*/	int16	y_pos;				// Y Position
			/*0057*/	int16	x_pos;				// X Position
			/*0059*/	int16	z_pos;				// Z Position
			/*0061*/	int32	deltaY : 11,			// Velocity Y
			deltaZ : 11,			// Velocity Z
				 deltaX : 10;			// Velocity X
			/*0065*/	uint8	unknown065;
			/*0066*/	uint16	spawn_id;			// Id of new spawn
			/*0068*/	uint16	bodytype;			// 65 is disarmable trap, 66 and 67 are invis triggers/traps
			/*0070*/	uint16	petOwnerId;		// Id of pet owner (0 if not a pet)
			/*0072*/	int16	cur_hp;				// Current hp's of Spawn
			/*0074*/	uint16	GuildID;			// GuildID - previously Current hp's of Spawn
			/*0076*/	uint8	race;				// Race
			/*0077*/	uint8	NPC;				// NPC type: 0=Player, 1=NPC, 2=Player Corpse, 3=Monster Corpse, 4=???, 5=Unknown Spawn,10=Self
			/*0078*/	uint8	class_;				// Class
			/*0079*/	uint8	gender;				// Gender Flag, 0 = Male, 1 = Female, 2 = Other
			/*0080*/	uint8	level;				// Level of spawn (might be one int8)
			/*0081*/	uint8	invis;				// 0=visable, 1=invisable
			/*0082*/	uint8	sneaking;
			/*0083*/	uint8	pvp;
			/*0084*/	uint8	anim_type;
			/*0085*/	uint8	light;				// Light emitting
			/*0086*/	uint8	anon;				// 0=normal, 1=anon, 2=RP
			/*0087*/	uint8	AFK;				// 0=off, 1=on
			/*0088*/	uint8	summoned;
			/*0089*/	uint8	LD;					// 0=NotLD, 1=LD
			/*0090*/	uint8	GM;					// 0=NotGM, 1=GM
			/*0091*/	uint8	flymode;
			/*0092*/	uint8	bodytexture;
			/*0093*/	uint8	helm;
			/*0094*/	uint8	face;
			/*0095*/	uint8	equipment[9];		// Equipment worn: 0=helm, 1=chest, 2=arm, 3=bracer, 4=hand, 5=leg, 6=boot, 7=melee1, 8=melee2
			/*0104*/	char	name[30];			// Name of spawn (len is 30 or less)
			/*0134*/	char	Surname[20];		// Last Name of player	
			/*0154*/	int8	guildrank;			// ***Placeholder
			/*0155*/	int8	unknown155;				// unknown
			/*0156*/	int8	deity;
			/*0157*/	uint8	unknown_end[11];
			/*0168*/
		};

		struct NewSpawn_Struct
		{
			/*000*/	uint32  ns_unknown1;            // Comment: ***Placeholder
			/*004*/	struct Spawn_Struct spawn;     // Comment: Spawn Information
			/*228*/
		};

		struct DeleteSpawn_Struct
		{
			/*00*/ uint16 spawn_id;				// Comment: Spawn ID to delete
			/*02*/
		};

		//New ChannelMessage_Struct struct
		struct ChannelMessage_Struct
		{
			/*000*/	char	targetname[32];		// Tell recipient
			/*032*/	char	sender[32];			// The senders name (len might be wrong)
			/*064*/	uint16	language;			// Language
			/*066*/	uint16	chan_num;			// Channel
			/*068*/	uint16	skill_in_language;	// The players skill in this language? might be wrong
			/*070*/	char	message[0];			// Variable length message
		};

		struct FormattedMessage_Struct
		{
			/*000*/	char	unknown[32]; // not used?
			/*032*/	char	targetname[32];
			/*064*/ char	sender[32];
			/*096*/	uint16	type;
			/*098*/	char	message[0];
		};

		struct WearChange_Struct
		{
			/*000*/ uint32 spawn_id;
			/*004*/ uint8  wear_slot_id;
			/*005*/ uint8  material;
			/*006*/ uint16 color;
			/*008*/ uint8  blue;
			uint8  green;
			uint8  red;
			uint8  use_tint;
			/*012*/	uint8  	wc_unknown;
			/*013*/	uint8  flag;
			/*014*/ uint8  wc_unknown4[2];
		};

		struct BindWound_Struct
		{
			/*000*/	uint16	to;
			/*002*/	uint16	unknown2;
			/*004*/	uint16	type;
			/*006*/	uint16	unknown6;
			/*008*/
		};

		struct Object_Data_Struct
		{
			/*000*/	uint8	max_charges;	//Max stack or charges. (Confirmed Sp0tter).
			/*001*/	uint8	stack_size;		//Current stack size or current charges.
			/*002*/	uint8	has_effect;		// 1 for has effect?  (not sure)
			/*003*/	uint16	click_effect;	//ID of click effect.	(Confirmed Sp0tter).
			/*005*/	uint8	unknown[5];
			/*010*/
		};

		struct Object_Struct
		{
			/*000*/		int8	unknown001[8];
			/*008*/		uint16  itemid;
			/*010*/		uint16  unknown010;
			/*012*/		int32	drop_id;	//this id will be used, if someone clicks on this object
			/*016*/		int16	zone_id;
			/*018*/		uint8   unknown014[6];
			/*024*/		uint8   charges;
			/*025*/		uint8   unknown25;
			/*026*/		uint8   maxcharges;
			/*027*/		uint8	unknown027[113];		// ***Placeholder
			/*140*/		float   heading;
			/*144*/		float	z;
			/*148*/		float	x;
			/*152*/		float	y;					// Z Position
			/*156*/		char	object_name[16];				// ACTOR ID
			/*172*/		int8	unknown172[14];
			/*186*/		int16	itemsinbag[10]; //if you drop a bag, thats where the items are
			/*206*/		int16	unknown206;
			/*208*/		int32	unknown208;
			/*212*/		uint32	object_type;
			/*216*/		int16	unknown216[4];
			/*224*/
		};

		struct ClickObject_Struct
		{
			/*000*/	uint32	objectID;
			/*004*/	uint16	PlayerID;
			/*006*/	uint16	unknown;
			/*008*/
		};

		struct ClickObjectAction_Struct
		{
			/*000*/	uint32	player_id;		// Comment: Entity Id of player who clicked object
			/*004*/	uint32	drop_id;		// Comment: Unknown 
			/*008*/	uint8	open;			// Comment: 0=close 1=open
			/*009*/	uint8	unknown9;		// Comment: Unknown 
			/*010*/	uint8	type;			// Comment: number that determines the type of the item (i.e forge) 
			/*011*/	uint8	unknown11;		// Comment: Unknown 
			/*012*/	uint8	slot;			// Comment: number of slots of the container
			/*013*/  uint8	unknown10[3];	// Comment: Unknown 
			/*016*/	uint16	icon;		// Comment: Icon of the item
			/*018*/	uint8	unknown16[2];	// Comment: Unknown
			/*020*/
		};

		struct DropCoins_Struct
		{
			/*000*/ uint8	unknown0002[8];		    // Comment: Always 00's
			/*008*/ uint32  platinumPieces;         // Comment: Number of PP dropped
			/*012*/ uint32  goldPieces;				// Comment: Number of GP dropped
			/*016*/ uint32  silverPieces;			// Comment: Number of SP dropped
			/*020*/ uint32  copperPieces;			// Comment: Number of CP dropped
			/*024*/ uint8	unknown0024[4];         // Comment: ***Placeholder
			/*028*/ uint32	always34;				// Comment: Always 0x34
			/*032*/ uint8	unknown0028[16];		// Comment: ***Placeholder
			/*048*/ uint32	amount;					// Comment: Total Dropped
			/*052*/ uint16  test1;                  // Comment: ***Placeholder
			/*054*/ uint16	test2;					// Comment: ***Placeholder
			/*056*/ float	yPos;					// Comment: Y Position
			/*060*/ float	xPos;					// Comment: X Position
			/*064*/ float	zPos;					// Comment: Z Position
			/*068*/ uint8	unknown0070[12];		// Comment: ***Placeholder
			/*080*/ char	coinName[16];			// Comment: Texture file name; ie, PLAT_ACTORDEF
			/*096*/ uint8	unknown0097[16];		// Comment: ***Placeholder
			/*112*/
		};

		struct Beg_Struct
		{
			/*000*/	uint16	target;		// The player's target at time of beg.
			/*002*/	uint16	begger;		// The player making the coin request.
			/*004*/	uint8	skill;		// The player's begging skill level.
			/*005*/	uint8	success;	// -1 = request | 0 = failure | 1 = plat | 2 = gold | 3 = silver | 4 = copper
			/*006*/	uint16	unknown1;		// This is the time since player logged in.
			/*008*/	uint32	coins;		// The number of coins.  Whatever success is set to.
			/*012*/	uint8	unknown3[6];	// Not sure.  Maybe we'll find out one day.
			/*018*/
		};

		struct Charm_Struct
		{
			/*000*/	uint16	owner_id;
			/*002*/	uint16	pet_id;
			/*004*/	uint16	command; // 1: make pet, 0: release pet
			/*006*/
		};

#define ZONE_ERROR_NOMSG 0
#define ZONE_ERROR_NOTREADY -1
#define ZONE_ERROR_VALIDPC -2
#define ZONE_ERROR_STORYZONE -3
#define ZONE_ERROR_NOEXPANSION -6
#define ZONE_ERROR_NOEXPERIENCE -7
		struct ZoneChange_Struct
		{
			/*000*/	char	char_name[32];     // Character Name
			/*032*/ char	short_name[16];
			/*048*/	uint32	unknown048;
			/*052*/ uint32  unknown052;
			/*056*/ uint32  unknown056;
			/*064*/ uint32  zone_reason;		// =0 client->server, =1 server->client, -X=specific error
			/*066*/	uint32  success; // =0 ok, =ffffff error
			/*068*/
		};

		struct Animation_Struct
		{
			/*00*/	uint32 spawnid;
			/*04*/	uint8  action;
			/*05*/  uint8  value;
			/*06*/	uint32 unknown06;
			/*10*/	uint16 unknown10; // 80 3F
		};

		struct Consider_Struct
		{
			/*000*/ uint32	playerid;               // PlayerID
			/*002*/ uint32	targetid;               // TargetID
			/*004*/ uint32	faction;                // Faction
			/*008*/ uint32	level;                  // Level
			/*012*/ int32	cur_hp;			// Current Hitpoints
			/*016*/ int32	max_hp;			// Maximum Hitpoints
			/*020*/ uint8	pvpcon;			// Pvp con flag 0/1
			/*021*/ uint8	unknown3[3];
			/*024*/
		};

		struct ConsentRequest_Struct
		{
			/*000*/	char name[1];				//Comment: Name of player who consent is being given to.
		};

		struct ConsentResponse_Struct
		{
			/*000*/ char consentee[32];        // Name of player who was consented
			/*032*/ char consenter[32];        // Name of player who consented
			/*064*/ uint8   allow;             // 00 = deny, 01 = allow
			/*065*/ char corpseZoneName[32];   // Zone where the corpse is
			/*097*/
		};

		struct Death_Struct
		{
			/*000*/	uint32	spawn_id;		// Comment: 
			/*004*/	uint32	killer_id;		// Comment: 
			/*008*/	uint32	corpseid;		// Comment: corpseid used for looting PC corpses !
			/*012*/	uint16	spell_id;		// Comment: 
			/*014*/	uint8	attack_skill;		// Comment: 
			/*015*/ uint8	unknown015;
			/*016*/	uint16	damage;			// Comment: Damage taken, (Confirmed )
			/*018*/ uint8   is_PC;		// Comment: 
			/*019*/ uint8   unknown017;
			/*020*/
		};

		struct SpawnPositionUpdate_Struct
		{
			/*0000*/ uint16	spawn_id;               // Id of spawn to update
			/*0002*/ uint8	anim_type; // ??
			/*0003*/ uint8	heading;                // Heading
			/*0004*/ int8	delta_heading;          // Heading Change
			/*0005*/ int16	y_pos;                  // New X position of spawn
			/*0007*/ int16	x_pos;                  // New Y position of spawn
			/*0009*/ int16	z_pos;                  // New Z position of spawn
			/*0011*/ uint32	delta_y : 11,             // Y Velocity
			delta_z : 11,             // Z Velocity
				  delta_x : 10;             // Z Velocity
			/*015*/
		};

		struct SpawnPositionUpdates_Struct
		{
			/*0000*/ uint32  num_updates;               // Number of SpawnUpdates
			/*0004*/ struct SpawnPositionUpdate_Struct // Spawn Position Update
				spawn_update[0];
		};

		struct SpawnHPUpdate_Struct
		{
			/*000*/ uint32  spawn_id;		// Comment: Id of spawn to update
			/*004*/ int32 cur_hp;		// Comment:  Current hp of spawn
			/*008*/ int32 max_hp;		// Comment: Maximum hp of spawn
			/*012*/
		};

		struct Stamina_Struct
		{
			/*000*/ uint16 food;		// Comment: low more hungry 127-0
			/*002*/ uint16 water;		// Comment: low more thirsty 127-0
			/*004*/ uint16 fatigue;	// Comment: high more fatigued 0-100
			/*006*/
		};

		struct Resurrect_Struct
		{
			/*000*/	uint32	corpseEntityID; // Client do not use any bytes till 52
			/*004*/	char	zoneName[16];
			/*020*/	char	unknown20[16];
			/*036*/	float	y;
			/*040*/	float	x;
			/*044*/	float	z;
			/*048*/	uint32	fullGMRez;
			/*052*/	char	targetName[28]; // Client uses this !!! If its wrong you wont see any window
			/*080*/ char	unknown82[8];
			/*088*/	char	casterName[28]; // Caster name (Confirmed)
			/*116*/ uint32	unknown118;
			/*120*/	uint16	spellID; // ID of the spell (Confirmed)
			/*122*/	char	corpseName[28];
			/*150*/	char	unknown[6];
			/*156*/	uint32	action; // Action => player clicked yes = 1 or no = 0 (Confirmed)
			/*160*/
		};

		struct Sacrifice_Struct
		{
			/*000*/	uint32 casterID;	// Caster who sends the request (Confirmed)
			/*004*/	uint32 targetID; // Player who recieves the request (Confirmed)
			/*008*/	uint32 action; // Action => player clicked yes = 1 or no = 0 (Confirmed)
			/*012*/
		};

		struct SpecialMesg_Struct
		{
			/*0000*/ uint32 msg_type;		// Comment: Type of message
			/*0004*/ char  message[0];		// Comment: Message, followed by four bytes?
		};

		struct LevelUpdate_Struct
		{
			/*000*/	uint8	level;			// Comment: Target Level of character
			/*001*/	uint8	can_delevel;	// Comment: If zero, don't delevel (clientside)
			/*002*/
		};

		struct ExpUpdate_Struct
		{
			/*000*/ uint32 exp;			// Comment: Current experience value
			/*004*/
		};

#define ITEM_STRUCT_SIZE 292
#define SHORT_BOOK_ITEM_STRUCT_SIZE	208
#define SHORT_CONTAINER_ITEM_STRUCT_SIZE 220
		struct Item_Struct
		{
			/*0000*/ char      Name[35];     // Name of item
			/*0035*/ char      Lore[60];    // Lore text
			/*0095*/ char      IDFile[6];	  // This is the filename of the item graphic when held/worn.
			/*0101*/ uint16	   flag;
			/*0103*/ int8	   unknown0103[22];
			/*0125*/ uint8	   Weight;          // Weight of item
			/*0126*/ int8      NoRent;          // Nosave flag 1=normal, 0=nosave, -1=spell?
			/*0127*/ int8      NoDrop;          // Nodrop flag 1=normal, 0=nodrop, -1=??
			/*0128*/ uint8     Size;            // Size of item
			/*0129*/ int8      ItemClass;
			/*0130*/ uint16	   ID;         // Unique Item number confirmed to be signed.
			/*0132*/ uint16    Icon;         // Icon Number
			/*0134*/ int16     equipSlot;       // Current slot location of item
			/*0136*/ uint32    Slots;  // Slots where this item is allowed
			/*0140*/ int32     Price;            // Item cost in copper
			/*0144*/ float     cur_x; //Here to 227 are named from client struct dump.
			/*0148*/ float	   cur_y;
			/*0152*/ float     cur_z;
			/*0156*/ float     heading;
			/*0160*/ uint32	   inv_refnum;
			/*0164*/ int16	   log;
			/*0166*/ int16     loot_log;
			/*0168*/ uint16    avatar_level;  //Usually 01, sometimes seen as FFFF, once as 0.
			/*0170*/ uint16    bottom_feed;
			union
			{
				struct
				{
					/*0172*/ int8     Book;      // Are we a book
					/*0173*/ int16	  BookType;	 // Type of book (scroll, note, etc)
					/*0175*/ char     Filename[15];            // Filename of book text on server
					/*0190*/ uint8    buffer1[102];    // Not used, fills out space in the packet so ShowEQ doesn't complain.
				} book;
				struct {
					/*0172*/ int8      AStr;              // Strength
					/*0173*/ int8      ASta;              // Stamina
					/*0174*/ int8      ACha;              // Charisma
					/*0175*/ int8      ADex;              // Dexterity
					/*0176*/ int8      AInt;              // Intelligence
					/*0177*/ int8      AAgi;              // Agility
					/*0178*/ int8      AWis;              // Wisdom
					/*0179*/ int8      MR;               // Magic Resistance
					/*0180*/ int8      FR;               // Fire Resistance
					/*0181*/ int8      CR;               // Cold Resistance
					/*0182*/ int8      DR;               // Disease Resistance
					/*0183*/ int8      PR;               // Poison Resistance
					/*0184*/ int8      HP;               // Hitpoints
					/*0185*/ int8      Mana;             // Mana
					/*0186*/ int8      AC;				 // Armor Class
					/*0187*/ uint8     Stackable;       // Maximum number of charges, for rechargable? (Sept 25, 2002)
					/*0188*/ int8      GMFlag;           // GM flag 0  - normal item, -1 - gm item (Sept 25, 2002)
					/*0189*/ uint8     Light;            // Light effect of this item
					/*0190*/ uint8     Delay;            // Weapon Delay
					/*0191*/ uint8     Damage;           // Weapon Damage
					/*0192*/ int8      EffectType1;      // 0=combat, 1=click anywhere w/o class check, 2=latent/worn, 3=click anywhere EXPENDABLE, 4=click worn, 5=click anywhere w/ class check, -1=no effect
					/*0193*/ uint8     Range;            // Range of weapon
					/*0194*/ uint8     ItemType;            // Skill of this weapon, refer to weaponskill chart
					/*0195*/ int8      Magic;            // Magic flag
					/*0196*/ int8      EffectLevel1;           // Casting level
					/*0197*/ uint8     Material;         // Material
					/*0198*/ uint8	   unkown3f[2];
					/*0200*/ uint32    Color;            // Amounts of RGB in original color
					/*0204*/ uint16    Faction;			// Structs dumped from client has this as Faction
					/*0206*/ uint16    Effect1;         // SpellID of special effect
					/*0208*/ uint16    Classes;          // Classes that can use this item
					/*0210*/ uint8     unkown23[2];		// Comment: ***Placeholder
					union
					{
						struct							// Comment: normal non-containers
						{
							/*0212*/ uint16  Races;				// Comment: Races that can use this item
							/*0214*/ uint16  unknown0214;		// Comment: ***Placeholder
							/*0216*/ int8    click_effect_type;	// Comment: Harakiri click_effect_id seems to be used to differentiate consuming items like arrows, food etc, must be 3 for EFFECT and Spells items, if 3 then there should be click_effect_id	 
						} normal;
						struct
						{
							/*0212*/ uint8	  BagType;			//Bag type (obviously)
							/*0213*/ uint8    BagSlots;        // number of slots in container
							/*0214*/ int8     IsBagOpen;     // 1 if bag is open, 0 if not.
							/*0215*/ int8     BagSize;    // Maximum size item container can hold
							/*0216*/ uint8    BagWR; // % weight reduction of container
						} container;
					};
					/*0217*/ uint8    EffectLevel2;            // Casting level
					/*0218*/ int8     Charges;         // Number of charges (-1 = unlimited)
					/*0219*/ int8     EffectType2;      // 0=combat, 1=click anywhere w/o class check, 2=latent/worn, 3=click anywhere EXPENDABLE, 4=click worn, 5=click anywhere w/ class check, -1=no effect
					/*0220*/ uint16   Effect2;         // spellId of special effect
					/*0222*/ int8     unknown0282; //FF
					/*0223*/ int8     unknown0283; //FF
					/*0224*/ uint8    unknown0284[4]; // ***Placeholder 0288
					/*0228*/ float    SellRate;
					/*0232*/ uint32   CastTime;        // Cast time of clicky item in miliseconds
					/*0236*/ uint8    unknown0296[16]; // ***Placeholder
					/*0252*/ uint16   SkillModType;
					/*0254*/ int16    SkillModValue;
					/*0256*/ int16    BaneDmgRace;
					/*0258*/ int16    BaneDmgBody;
					/*0260*/ uint8    BaneDmgAmt;
					/*0261*/ uint8    unknown0321[3]; //title_flag
					/*0264*/ uint8    RecLevel;         // max should be 65
					/*0265*/ uint8    RecSkill;         // Max should be 252
					/*0266*/ uint16   ProcRate;
					/*0268*/ uint8    ElemDmgType;
					/*0269*/ uint8    ElemDmgAmt;
					/*0270*/ uint16   FactionMod1;
					/*0272*/ uint16	  FactionMod2;
					/*0274*/ uint16   FactionMod3;
					/*0276*/ uint16	  FactionMod4;
					/*0278*/ uint16   FactionAmt1;
					/*0280*/ uint16	  FactionAmt2;
					/*0282*/ uint16   FactionAmt3;
					/*0284*/ uint16	  FactionAmt4;
					/*0286*/ uint16	  Void346;
					/*0288*/ uint16	  Deity;
					/*0290*/ uint16	  unknown290;
					/*0292*/
				} common;
			};
		};

		struct PlayerItemsPacket_Struct
		{
			/*000*/	int16		opcode;		// OP_ItemTradeIn
			/*002*/	struct Item_Struct	item;
		};

		struct PlayerItems_Struct
		{
			/*000*/	int16		count;
			/*002*/	struct PlayerItemsPacket_Struct	packets[0];
		};

		struct MerchantItemsPacket_Struct
		{
			/*000*/	uint8 itemtype;
			/*002*/	struct Item_Struct	item;
		};

		struct TradeItemsPacket_Struct
		{
			/*000*/	uint16 fromid;
			/*002*/	uint16 slotid;
			/*004*/	uint8  unknown;
			/*005*/	struct Item_Struct	item;
			/*000*/	uint8 unknown1[5];
			/*000*/
		};

		struct MerchantItems_Struct
		{
			/*000*/ uint32		merchant_id;
			/*004*/	uint8 itemtype;
			/*005*/	unsigned char	item[0];
		};

		struct SummonedItem_Struct
		{
			/*0000*/ struct Item_Struct item;        // Comment: Refer to itemStruct for members
		};

		struct SummonedItemWaiting_Struct
		{
			/*000*/ uint16 itemID;
			/*002*/ uint8  charge;
			/*003*/
		};

		enum CONSUMETYPE : uint8
		{
			CONSUMEFOOD = 0x01,						// Comment: Food
			CONSUMEDRINK = 0x02						// Comment: Drink
		};

		enum HOWCONSUMEDTYPE : uint32
		{
			AUTO = 0xffffffff,				// Comment: Auto Consumed by the client as food/drink is needed
			RIGHTCLICKED = 222				// Comment: User has right clicked on food/drink to force eating/drinking
		};

		struct Consume_Struct
		{
			/*000*/ uint32 slot;
			/*004*/ uint32 auto_consumed; // 0xffffffff when auto eating e7030000 when right click
			/*008*/ uint8 c_unknown1[4];
			/*012*/ uint8 type; // 0x01=Food 0x02=Water
			/*013*/ uint8 unknown13[3];
			/*016*/
		};

		struct MoveItem_Struct
		{
			/*000*/ uint32 from_slot;
			/*004*/ uint32 to_slot;
			/*008*/ uint32 number_in_stack;
			/*012*/
		};

		struct MoveCoin_Struct
		{
			/*000*/ uint32 from_slot;
			/*004*/ uint32 to_slot;
			/*008*/ uint32 cointype1;
			/*012*/ uint32 cointype2;
			/*016*/ uint32 amount;
			/*020*/
		};

		struct Split_Struct
		{
			/*000*/	uint32	platinum;
			/*004*/	uint32	gold;
			/*008*/	uint32	silver;
			/*012*/	uint32	copper;
			/*016*/
		};

		struct Surname_Struct
		{
			/*000*/	char name[32];				// Comment: Player's first name
			/*032*/	uint32 unknown032;			// Comment: put 1 in this in order to accept the surname  ^^
			/*036*/	char lastname[20];			// Comment: Player's Surname (max size 20 limited by client)
			/*056*/
		};

		struct MoneyOnCorpse_Struct
		{
			/*000*/ uint8	response;		// Comment: 0 = someone else is, 1 = OK, 2 = not at this time
			/*001*/ uint8	unknown1;		// Comment: = 0x5a
			/*002*/ uint8	unknown2;		// Comment: = 0x40
			/*003*/ uint8	unknown3;		// Comment: = 0
			/*004*/ uint32	platinum;		// Comment: Platinum Pieces
			/*008*/ uint32	gold;			// Comment: Gold Pieces
			/*012*/ uint32	silver;			// Comment: Silver Pieces
			/*016*/ uint32	copper;			// Comment: Copper Pieces
			/*020*/
		};

		struct ItemOnCorpse_Struct
		{
			/*000*/	Item_Struct item;
		};

		struct LootingItem_Struct
		{
			/*000*/	uint32	lootee;
			/*004*/	uint32	looter;
			/*008*/	uint16	slot_id;
			/*010*/	uint8	unknown3[2];
			/*012*/	uint32	auto_loot;
			/*016*/
		};

		struct RequestClientZoneChange_Struct
		{
			/*000*/	char	zone_name[32];
			/*032*/	float	y;
			/*036*/	float	x;
			/*040*/	float	z;
			/*044*/	float	heading;
			/*048*/
		};

		struct GMZoneRequest_Struct
		{
			/*000*/	char	charname[64];
			/*064*/	uint32	zoneID;
			/*068*/	uint8	unknown1[16];
			/*084*/	uint8	success; // 0 if command failed, 1 if succeeded?
			/*085*/	uint8	unknown2[3]; // 0x00
			/*088*/
		};

		struct GMSummon_Struct
		{
			/*000*/	char    charname[64];
			/*064*/	char    gmname[64];
			/*128*/ uint32	success;
			/*132*/	uint32	zoneID;
			/*136*/	int32	x;
			/*140*/	int32	y;
			/*144*/ int32	z;
			/*148*/	uint8 unknown2[4]; // E0 E0 56 00
			/*152*/
		};


		struct GMGoto_Struct
		{ // x,y is swapped as compared to summon and makes sense as own packet
			/*000*/ char    char_name[64];
			/*064*/ char    gm_name[64];
			/*128*/ uint32	success;
			/*132*/ uint32  zone_id;
			/*136*/ int32	x;
			/*140*/ int32	y;
			/*144*/ int32	z;
			/*148*/ uint32	zone_reason; // E0 E0 56 00
			/*152*/
		};

		struct GMSurname_Struct
		{
			/*000*/	char name[64];			// Comment: 
			/*064*/	char gmname[64];		// Comment: 
			/*128*/	char Surname[32];		// Comment: 
			/*160*/	uint8 unknown[2];		// Comment: 0x00, 0x00, 0x01, 0x00 = Update the clients
			/*162*/
		};

		struct Duel_Struct
		{
			/*000*/	int32 duel_initiator;
			/*004*/	int32 duel_target;
			/*008*/
		};

		struct DuelResponse_Struct
		{
			/*000*/	int32 target_id;
			/*004*/	int32 entity_id;
			/*008*/	int8 response[2];
			/*010*/
		};

		struct CombatAbility_Struct
		{
			/*000*/	uint16 m_target;		//the ID of the target mob
			/*002*/	uint16 unknown;
			/*004*/	uint32 m_atk;
			/*008*/	uint32 m_skill;
			/*012*/
		};

		//Instill Doubt
		struct Instill_Doubt_Struct
		{
			/*000*/	uint8 i_id;			// Comment: 
			/*001*/	uint8 ia_unknown;	// Comment: 
			/*002*/	uint8 ib_unknown;	// Comment: 
			/*003*/	uint8 ic_unknown;	// Comment: 
			/*004*/	uint8 i_atk;			// Comment: 
			/*005*/	uint8 id_unknown;	// Comment: 
			/*006*/	uint8 ie_unknown;	// Comment: 
			/*007*/	uint8 if_unknown;	// Comment: 
			/*008*/	uint8 i_type;		// Comment: 
			/*009*/	uint8 ig_unknown;	// Comment: 
			/*010*/	uint8 ih_unknown;	// Comment: 
			/*011*/	uint8 ii_unknown;	// Comment: 
			/*012*/
		};

		struct PickPockets_Struct
		{
			/*000*/	uint16 to;
			/*002*/	uint16 from;
			/*004*/	uint8 myskill;
			/*005*/	uint8 unknown0;
			/*006*/	uint8 type; // -1 you are being picked, 0 failed , 1 = plat, 2 = gold, 3 = silver, 4 = copper, 5 = item
			/*007*/	uint8 unknown1; // 0 for response, unknown for input
			/*008*/	uint32 coin;
			/*012*/	uint8 lastsix[6];
			/*018*/
		};

		struct UseAbility_Struct
		{
			/*000*/	uint16 player_id;	// Comment: Entity ID
			/*002*/	uint8  unkown[10];	// Comment: Only two of these bytesever have values.  Guessing lore and skill.
			/*012*/
		};

		struct GiveItem_Struct
		{
			/*000*/	uint16 to_entity;		// Comment: 
			/*002*/	int16 to_equipSlot;	// Comment: 
			/*004*/	uint16 from_entity;		// Comment: 
			/*006*/	int16 from_equipSlot;	// Comment: 
			/*008*/
		};

		struct CancelTrade_Struct
		{
			/*000*/	uint16 fromid;
			/*002*/	uint16 action;
			/*004*/
		};


		struct Random_Struct
		{
			/*000*/	uint32 low;		// Comment: 
			/*004*/	uint32 high;		// Comment: 
			/*008*/
		};

		//Old LFG_Struct (DO NOT DELETE)
		//struct LFG_Struct 
		//{
		//	char	name[64];	// Comment: 
		//	uint32	value;		// Comment: 
		//};

		//New LFG_Struct
		struct LFG_Struct
		{
			/*000*/	char	name[64 + 2];	// Comment: 
			/*066*/	uint32	value;		// Comment: 
			/*070*/
		};

		// EverQuest Time Information:
		// 72 minutes per EQ Day
		// 3 minutes per EQ Hour
		// 6 seconds per EQ Tick (2 minutes EQ Time)
		// 3 seconds per EQ Minute
		struct TimeOfDay_Struct
		{
			/*000*/	uint8	hour;			// Comment: 
			/*001*/	uint8	minute;			// Comment: 
			/*002*/	uint8	day;			// Comment: 
			/*003*/	uint8	month;			// Comment: 
			/*004*/	uint16	year;			// Comment: Kibanu - Changed from long to uint16 on 7/30/2009
			/*006*/
		};

		struct Merchant_Click_Struct
		{
			/*000*/ uint32	npcid;			// Merchant NPC's entity id
			/*002*/ uint32	playerid;
			/*004*/	uint8  command;
			/*005*/ uint8	unknown[3];
			/*008*/ float   rate;
			/*012*/
		};

		/*
		0 is e7 from 01 to // MAYBE SLOT IN PURCHASE
		1 is 03
		2 is 00
		3 is 00
		4 is ??
		5 is ??
		6 is 00 from a0 to
		7 is 00 from 3f to */
		/*
		0 is F6 to 01
		1 is CE CE
		4A 4A
		00 00
		00 E0
		00 CB
		00 90
		00 3F
		*/

		struct Merchant_Sell_Struct
		{
			/*000*/	uint32	npcid;			// Merchant NPC's entity id
			/*002*/	uint32	playerid;		// Player's entity id
			/*004*/	uint16	itemslot;
			/*006*/	/*uint8	IsSold;	*/	// Already sold
			/*007*/	uint8	unknown001;
			/*007*/	uint8	unknown002;
			/*008*/	uint8	quantity;	// Qty - when used in Merchant_Purchase_Struct
			/*009*/	uint8	unknown004[3];
			/*012*/	uint32	price;
			/*016*/
		};

		struct Merchant_Purchase_Struct
		{
			/*000*/	uint32	npcid;			// Merchant NPC's entity id
			/*004*/ uint32  playerid;
			/*008*/	uint16	itemslot;		// Player's entity id
			/*010*/ uint16	unknown010;
			/*012*/ uint8	quantity;
			/*013*/ uint8	unknown011[3];
			/*016*/ uint32  price;
			/*020*/
		};

		struct Item_Shop_Struct
		{
			/*000*/	uint8 itemtype;
			/*001*/	Item_Struct item;
		};

		struct Merchant_DelItem_Struct
		{
			/*000*/	uint32	npcid;			// Merchant NPC's entity id
			/*004*/	uint32	playerid;		// Player's entity id
			/*008*/	uint16	itemslot;       // Slot of the item you want to remove
			/*010*/	uint16	type;     // 0x40
			/*012*/
		};

		struct Illusion_Struct
		{
			/*000*/	int16	spawnid;
			/*002*/	int16	race;
			/*004*/	int8	gender;
			/*005*/ int8	texture;
			/*006*/ int8	helmtexture;
			/*007*/	int8	unknown007; //Always seems to be 0xFF
			/*008*/ int16	face;
			/*010*/ int8	hairstyle;
			/*011*/	int8	haircolor;
			/*012*/	int8	beard;
			/*013*/	int8	beardcolor;
			/*014*/	int16	size;		  //Client has height (int) listed for this, but it doesn't line up with what ShowEQ caught at all.
			/*016*/ int32	unknown_void; // Always 0xFFFFFFFF it seems.
			/*020*/
		};

		struct CPlayerItems_packet_Struct
		{
			/*000*/	uint16		opcode;		// Comment: OP_ItemTradeIn
			/*002*/	Item_Struct	item;		// Comment: 
		};

		struct CPlayerItems_Struct
		{
			/*000*/	uint16		count;							// Comment: 
			/*002*/	CPlayerItems_packet_Struct	packets[0];		// Comment: 
		};

		struct SkillUpdate_Struct
		{
			/*000*/	uint8 skillId;			// Comment: 
			/*001*/	uint8 unknown1[3];		// Comment: 
			/*004*/	uint8 value;			// Comment: 
			/*005*/	uint8 unknown2[3];		// Comment: 
			/*008*/
		};

		struct ZoneUnavail_Struct
		{
			//This actually varies, but...
			/*000*/	char zonename[16];		// Comment: 
			/*016*/	short int unknown[4];	// Comment: 
		};

		struct GroupGeneric_Struct {
			char name1[30];
			char name2[30];
		};

		struct GroupCancel_Struct {
			char	name1[30];
			char	name2[30];
			uint8	toggle;
		};

		struct GroupInvite_Struct
		{
			/*000*/	char invitee_name[30];		// Comment: 
			/*064*/	char inviter_name[30];		// Comment: 
			/*128*/	char unknown[31];		// Comment: 
			/*193*/
		};

		/* Do not touch this please.  Used to add and
		* remove group members from a group. Maybe more.*/
		struct GroupUpdate_Struct
		{
			/*0000*/	char	yourname[32];
			/*0032*/	char	othername[32];
			/*0064*/	char	membername[5][32];
			/*0224*/	uint32	action;				//0: Player joins the group; 1: ?? 2: ?? 3: Player leaves the group 4: You leave the group;	
			/*0228*/
		};

		struct GroupFollow_Struct
		{
			/*000*/	char leader[64];		// Comment: 
			/*064*/	char invited[64];		// Comment: 
			/*128*/
		};

		/* Sent when player clicks disband after receiving an invite
		* or if a player is already grouped and gets an invite it
		* is sent automaticaly. */
		struct GroupInviteDecline_Struct{
			/*000*/	char leader[64];		//Player1 who invited another Player2;
			/*064*/	char leaver[64];		//Player2 who is declining/unable to accept invite from Player1;
			/*128*/	uint8 action;			//Type of decline: 1: 'leaver' is considering another group invite; 2: 'leaver' already grouped; 3: 'leaver' rejects your offer to join the group;
			/*129*/
		};

		struct GroupDisband_Struct
		{
			/*000*/	char member[15];		// Comment: 
			/*015*/	char unknown1[41];		// Comment: 
			/*056*/	uint8 framecounter;		// A frame counter that increments by 32 each time same client sends this code;
			/*057*/	char unknown2[3];
			/*060*/
		};

		struct FaceChange_Struct
		{
			/*000*/	uint8	haircolor;	// Comment: 
			/*001*/	uint8	beardcolor;	// Comment: 
			/*002*/	uint8	eyecolor1;	// Comment: the eyecolors always seem to be the same, maybe left and right eye?
			/*003*/	uint8	eyecolor2;	// Comment: 
			/*004*/	uint8	hairstyle;	// Comment: 
			/*005*/	uint8	beard;		// Comment: Face Overlay? (barbarian only)
			/*006*/	uint8	face;		// Comment: and beard
			/*007*/
		};

		struct TradeRequest_Struct
		{
			/*000*/	uint32 to_mob_id;
			/*002*/	uint32 from_mob_id;
			/*004*/
		};

		struct TradeAccept_Struct
		{
			/*000*/	uint32 from_mob_id;
			/*002*/	uint32 to_mob_id;
			/*004*/
		};

		struct TradeMoneyUpdate_Struct
		{
			/*000*/	uint32	trader;
			/*002*/	uint32	type;
			/*004*/	uint32	amount;
			/*008*/
		};

		struct TradeCoin_Struct
		{
			/*000*/	uint32	trader;
			/*004*/	uint8	slot;		// Comment: 
			/*005*/	uint16	unknown005;		// Comment: 
			/*007*/	uint8	unknown007;		// Comment: 
			/*004*/	uint32	amount;
			/*008*/	uint8	unknown008[10];
			/*018*/
		};

		struct Trader_Struct
		{
			/*000*/	uint16	Code;
			/*002*/ uint16  TraderID;
			/*004*/	uint32	Items[80];
			/*324*/	uint32	ItemCost[80];
			/*644*/
		};

		struct Trader_ShowItems_Struct
		{
			/*000*/	uint16 Code;
			/*002*/	uint16 TraderID;
			/*004*/ uint32 SubAction;
			/*008*/	uint32 Items[3];
			/*012*/
		};

		struct TraderStatus_Struct
		{
			/*000*/	uint16 Code;
			/*002*/	uint16 Unkown04;
			/*004*/	uint32 TraderID;
			/*008*/	uint32 Unkown08[3];
			/*012*/
		};

		struct BecomeTrader_Struct
		{
			/*000*/	uint16 ID;
			/*002*/ uint16 unknown;
			/*004*/	uint32 Code;
			/*008*/
		};

		struct BazaarWindowStart_Struct
		{
			/*000*/	uint8   Action;
			/*001*/	uint8   Unknown001;
			/*002*/
		};

		struct BazaarWelcome_Struct
		{
			/*000*/	uint32  Action;
			/*004*/	uint32	Traders;
			/*008*/	uint32	Items;
			/*012*/	uint8	Unknown012[8];
			/*020*/
		};

		struct BazaarSearch_Struct
		{
			/*000*/	BazaarWindowStart_Struct Beginning;
			/*002*/	uint16	TraderID;
			/*004*/	uint16	Class_;
			/*006*/	uint16	Race;
			/*008*/	uint16	ItemStat;
			/*010*/	uint16	Slot;
			/*012*/	uint16	Type;
			/*014*/	char	Name[64];
			/*078*/ uint16  unknown;
			/*080*/	uint32	MinPrice;
			/*084*/	uint32	MaxPrice;
			/*088*/
		};

		struct BazaarSearchResults_Struct
		{
			/*000*/	BazaarWindowStart_Struct Beginning;
			/*002*/	uint16  NumItems;
			/*004*/	uint16	SerialNumber;
			/*006*/	uint16	SellerID;
			/*008*/	uint16	Cost;
			/*010*/	uint16	ItemStat;
			/*012*/	char	ItemName[64];
			/*076*/

		};

		struct TraderBuy_Struct
		{
			/*000*/	uint16 Action;
			/*002*/	uint16 TraderID;
			/*004*/	uint32 ItemID;
			/*008*/	uint32 Price;
			/*012*/	uint16 Quantity;
			/*014*/ uint16 Slot; //EQEmu has as AlreadySold for for EQMac this is slot.
			/*016*/	char   ItemName[64];
			/*080*/
		};

		struct TraderPriceUpdate_Struct
		{
			/*000*/	uint16	Action;
			/*002*/	uint16	SubAction;
			/*004*/	int32	SerialNumber;
			/*008*/	uint32	NewPrice;
			/*012*/
		};

		struct Give_Struct
		{
			/*000*/	uint16 unknown[2];		// Comment: 
			/*004*/
		};

		struct PetitionClientUpdate_Struct
		{
			/*000*/	uint32 petnumber;		// Comment: Petition Number
			/*004*/	uint32 color;			// Comment: 0x00 = green, 0x01 = yellow, 0x02 = red
			/*008*/	uint32 senttime;			// Comment: 
			/*012*/	uint8 unknown[4];		// Comment: 4 has to be 0x1F
			/*016*/	char accountid[32];		// Comment: 
			/*048*/	char gmsenttoo[32];		// Comment: 
			/*080*/	uint32 something;		// Comment: 
			/*084*/	char charname[64];		// Comment: 
			/*148*/
		};

		struct Petition_Struct
		{
			/*0000*/	uint32 petnumber;			// Comment: 
			/*0004*/	uint32 urgency;				// Comment: 
			/*0008*/	char accountid[32];			// Comment: 
			/*0040*/	char lastgm[32];			// Comment: 
			/*0072*/	char zone[32];				// Comment: 
			/*0104*/	char charname[64];			// Comment: 
			/*0168*/	uint32 charlevel;			// Comment: 
			/*0172*/	uint32 charclass;			// Comment: 
			/*0176*/	uint32 charrace;				// Comment: 
			/*0180*/	time_t senttime;			// Comment: Time?
			/*0188*/	uint32 checkouts;			// Comment: 
			/*0192*/	uint32 unavail;				// Comment: 
			/*0196*/	uint8 unknown5[4];			// Comment: 
			/*0200*/	char petitiontext[1024];	// Comment: 
			/*1224*/
		};

		//For reference, possibly used by Mac client.
		struct Bug_Report_Struct
		{
			/*0000*/	char playername[72];		// Comment: Players name
			/*0072*/	uint8 blankspot1;			// Comment: not used
			/*0073*/	uint8 cannotuplicate;		// Comment: 1 for cannot duplicate, 0 for can duplicate
			/*0074*/	uint8 noncrashbug;			// Comment: 1 for does NOT crash, 0 for does cause crash
			/*0075*/	char bugdescription[1024];	// Comment: The bug description
			/*1099*/
		};

		struct BugStruct
		{
			/*0000*/	char	chartype[64];
			/*0064*/	char	name[96];
			/*0160*/	float	x;
			/*0164*/	float	y;
			/*0168*/	float	z;
			/*0172*/	float	heading;
			/*0176*/	char	unknown176[16];
			/*0192*/	char	target_name[64];
			/*0256*/	uint32	type;
			/*0260*/	char	unknown256[2052];
			/*2312*/	char	bug[1024];
			/*3336*/	uint32	unknown3336;
			/*3340*/
		};

		struct Who_All_Struct
		{
			/*000*/	char	whom[64];
			/*064*/	int16	wrace;		// FF FF = no race
			/*066*/	int16	wclass;		// FF FF = no class
			/*068*/	int16	lvllow;		// FF FF = no numbers
			/*070*/	int16	lvlhigh;	// FF FF = no numbers
			/*072*/	int16	gmlookup;	// FF FF = not doing /who all gm
			/*074*/	int16	guildid;
			/*076*/	int8	unknown076[64];
			/*140*/
		};

		struct WhoAllPlayer
		{
			/*000*/	uint16	formatstring;
			/*000*/	uint16	pidstring;
			/*000*/	char*	name;
			/*000*/	uint16	rankstring;
			/*000*/	char*	guild;
			/*000*/	uint16	unknown80[3];
			/*000*/	uint16	zonestring;
			/*000*/	uint32	zone;
			/*000*/	uint16	class_;
			/*000*/	uint16	level;
			/*000*/	uint16	race;
			/*000*/	char*	account;
			/*000*/	uint16	ending;
		};

		struct WhoAllReturnStruct
		{
			/*000*/	uint32	id;
			/*004*/	uint16	playerineqstring;
			/*006*/	char	line[27];
			/*033*/	uint8	unknown35; //0A
			/*034*/	uint16	unknown36;//0s
			/*036*/	uint16	playersinzonestring;
			/*038*/	uint16	unknown44[5]; //0s
			/*048*/	uint32	unknown52;//1
			/*052*/	uint32	unknown56;//1
			/*056*/	uint16	playercount;//1
			struct WhoAllPlayer player[0];
		};

		// The following four structs are the WhoAllPlayer struct above broken down
		// for use in World ClientList::SendFriendsWho to accomodate the user of variable
		// length strings within the struct above.

		struct	WhoAllPlayerPart1
		{
			/*000*/	uint16	FormatMSGID;
			/*000*/	uint16	PIDMSGID;
			/*000*/	char	Name[1];;
		};

		struct	WhoAllPlayerPart2
		{
			/*000*/	uint16	RankMSGID;
			/*000*/	char	Guild[1];
		};

		struct	WhoAllPlayerPart3
		{
			/*000*/	uint16	Unknown80[3];
			/*000*/	uint16	ZoneMSGID;
			/*000*/	uint32	Zone;
			/*000*/	uint16	Class_;
			/*000*/	uint16	Level;
			/*000*/	uint16	Race;
			/*000*/	char	Account[1];
		};

		struct	WhoAllPlayerPart4
		{
			/*000*/	uint32	Unknown100;
		};

		// Comment: 4 bytes in total
		struct Stun_Struct
		{
			/*000*/	uint32 duration;		// Comment: Duration of stun
			/*004*/
		};

		// Size: 32 Bytes
		struct Combine_Struct
		{
			/*000*/	uint8 worldobjecttype;	// Comment: if its a world object like forge, id will be here
			/*000*/	uint8 unknown001;		// Comment: 
			/*000*/	uint8 success;		// Comment: 
			/*000*/	uint8 unknown003;
			/*000*/	uint16 container_slot;	// Comment: the position of the container, or 1000 if its a world container	
			/*000*/	uint16 iteminslot[10];	// Comment: IDs of items in container
			/*000*/	uint16 unknown005;		// Comment: 
			/*000*/	uint16 unknown006;		// Comment: 
			/*000*/	uint16 containerID;		// Comment: ID of container item
		};

		struct Emote_Struct
		{
			/*000*/	uint16 unknown01;			// Comment: 
			/*000*/	char message[1024];		// Comment: 
		};

		struct Social_Action_Struct
		{
			/*000*/	uint8 unknown1[4];	// Comment: 
			/*004*/	uint8 action;		// Comment: 
			/*005*/	uint8 unknown2[7];	// Comment: 
			/*012*/
		};

		//Size: 8  (Request)
		struct InspectRequest_Struct
		{
			/*000*/	uint32 TargetID;		// Comment: 
			/*004*/	uint32 PlayerID;		// Comment: 
			/*008*/
		};

		struct InspectAnswer_Struct
		{
			/*0000*/	uint16 TargetID;			// Comment: ? 
			/*0002*/	uint16 PlayerID;			// Comment: ?
			/*0004*/	uint8  unknown[128];
			/*0132*/	char itemnames[23][64];
			/*1604*/    char text[140];
			/*1744*/
		};


		struct SetDataRate_Struct
		{
			/*000*/	float newdatarate;	// Comment: 
			/*004*/
		};


		struct SetServerFilter_Struct
		{
			/*000*/	uint32 filters[17];	// Comment: 
			/*068*/
		};

		struct SetServerFilterAck_Struct
		{
			/*000*/	uint8 blank[8];		// Comment: 
			/*008*/
		};

		struct GMName_Struct
		{
			/*000*/	char oldname[64];	// Comment: 
			/*064*/	char gmname[64];	// Comment: 
			/*128*/	char newname[64];	// Comment: 
			/*192*/	uint8 badname;		// Comment: 
			/*193*/	uint8 unknown[3];	// Comment: 
			/*196*/
		};

		struct GMDelCorpse_Struct
		{
			/*000*/	char corpsename[64];	// Comment: 
			/*064*/	char gmname[64];		// Comment: 
			/*128*/	uint8 unknown;			// Comment: 
			/*129*/
		};

		struct GMKick_Struct
		{
			/*000*/	char name[64];		// Comment: 
			/*064*/	char gmname[64];	// Comment: 
			/*128*/	uint8 unknown;		// Comment: 
			/*129*/
		};

		struct GMKill_Struct
		{
			/*000*/	char name[64];		// Comment: 
			/*064*/	char gmname[64];	// Comment: 
			/*128*/	uint8 unknown;		// Comment: 
			/*129*/
		};

		struct GMEmoteZone_Struct
		{
			/*000*/	char text[512];		// Comment: 
		};

		// This is where the Text is sent to the client.
		// Use ` as a newline character in the text.
		// In Trilogy, text starts directly at offset 0 (no type byte).
		// Variable length, null-terminated.
		struct BookText_Struct
		{
			/*000*/	uint8 type;		//type: 0=scroll, 1=book, 2=item info.. prolly others.
			/*000*/	char booktext[1]; // Variable Length
		};

		// This is the request to read a book.
		// In Trilogy, the client sends the txtfile name starting at offset 0,
		// followed by a null-terminator and BookType.
		struct BookRequest_Struct
		{
			/*000*/	char txtfile[14]; // Variable
		};

		struct GMTrainee_Struct
		{
			/*000*/ uint32 npcid;
			/*004*/ uint32 playerid;
			/*008*/ uint8  highesttrain[73]; // Highest value for each skill a trainer can train you to.
			/*081*/ uint8  unknown[32];      // One of these bits is required to open the train dialog (set to 1)
			/*113*/ uint8  highesttrainLang[24]; // Max language skill a trainer can teach you
			/*137*/ uint8  unknown2[11];     // Unknown
			/*148*/
		};

		struct GMTrainEnd_Struct
		{
			/*000*/ int16 npcid;
			/*002*/ int16 playerid;
			/*004*/
		};

		struct GMSkillChange_Struct
		{
			/*000*/	uint16		npcid;
			/*002*/ uint8		unknown1[2];	// something like PC_ID, but not really. stays the same thru the session though
			/*004*/ uint16		skillbank;		// 0 if normal skills, 1 if languages
			/*006*/ uint8		unknown2[2];
			/*008*/ uint16		skill_id;
			/*010*/ uint8		unknown3[2];
			/*012*/
		};


		// Struct for Clients Request to Show specific message board
		struct MBRetrieveMessages_Struct
		{
			uint8 entityID;
			uint8 unknown[3];
			uint8 category; // category is the type of board selected by the client
			/* 00 - OFFICIAL */
			/* 01 - FOR SALE */
			/* 02 - GENERAL */
			/* 03 - HELP WANTED */
			/* 04 - PERSONALS */
			/* 05 - GUILDS */
			uint8 unknown2[3];
		};

		// Valids of Guild Rank
		enum GUILDRANK
		{
			GuildUnknown = -1,		// Comment: Unknown guild rank
			GuildMember = 0,		// Comment: Guild Leader
			GuildOffice = 1,		// Comment: Guild Officer
			GuildLeader = 2,		// Comment: Guild Member
			NotInaGuild = 3,		// Comment: Char is not in a guild
			GuildInviteTimeOut = 4, // Comment: Client Invite Window has timed out
			GuildDeclined = 5		// Comment: User Declined to join Guild

		};

		struct GuildRankLevel_Struct
		{
			/*000*/	char rankname[101];		// Comment: 
			/*000*/	bool heargu;			// Comment: 
			/*000*/	bool speakgu;			// Comment: 
			/*000*/	bool invite;			// Comment: 
			/*000*/	bool remove;			// Comment: 
			/*000*/	bool promote;			// Comment: 
			/*000*/	bool demote;			// Comment: 
			/*000*/	bool motd;				// Comment: 
			/*000*/	bool warpeace;			// Comment: 
			/*000*/
		};

		struct Guild_Struct
		{
			/*000*/	char name[32];			// Comment: 
			/*000*/	uint32 databaseID;		// Comment: 
			/*000*/	uint32 leader;			// Comment: AccountID of guild leader
			/*000*/	GuildRankLevel_Struct rank[512 + 1];	// Comment: 
			/*000*/
		};

		struct GuildMOTD_Struct{
			/*000*/	char	name[32];
			/*032*/	uint32	unknown64;
			/*036*/	char	motd[512];
			/*548*/
		};

		struct GuildInviteAccept_Struct
		{
			/*000*/	char inviter[30];
			/*030*/	char newmember[30];
			/*060*/	uint32 response;
			/*064*/	uint16 guildeqid;
			/*066*/	uint16 unknown;
			/*068*/
		};

		struct GuildsListEntry_Struct
		{
			/*0000*/	uint32 guildID;				// Comment: empty = 0xFFFFFFFF
			/*0004*/	char name[32];				// Comment: 
			/*0036*/	uint32 unknown1;			// Comment: = 0xFF
			/*0040*/	uint16 exists;				// Comment: = 1 if exists, 0 on empty
			/*0042*/	uint8 unknown2[6];			// Comment: = 0x00
			/*0048*/	uint32 unknown3;			// Comment: = 0xFF
			/*0052*/	uint8 unknown4[8];			// Comment: = 0x00
			/*0060*/
		};

		struct GuildsList_Struct
		{
			/*000*/	uint8 head[4];							// Comment: 
			/*004*/	GuildsListEntry_Struct Guilds[512];		// Comment: 
		};


		struct GuildUpdate_Struct
		{
			/*000*/	uint32	guildID;				// Comment: 
			/*004*/	GuildsListEntry_Struct entry;	// Comment: 
		};

		// Guild invite, remove
		struct GuildCommand_Struct
		{
			/*000*/	char othername[30];			// Comment: Person who is being invited
			/*030*/	char myname[30];			// Comment: Person who did /guildinvite
			/*060*/	uint16 guildeqid;			// Comment: 
			/*062*/	uint8 unknown[2];			// Comment: for guildinvite all 0's, for remove 0=0x56, 2=0x02
			/*064*/	uint32 officer;					// rank
			/*068*/
		};

		struct GuildRemove_Struct
		{
			/*000*/	char Remover[64];
			/*064*/	char Removee[64];
			/*128*/	uint16 guildeqid;
			/*130*/	uint8 unknown[2];
			/*132*/	uint32 rank;
			/*136*/
		};

		// struct for deleting a specific message
		struct MBEraseMessage_Struct
		{
			/*000*/ uint32 entityID;
			/*004*/ uint32 id; /* id of the message
							   /*008*/ uint8 category;
			/*009*/ uint8 unknown3[3];
			/*012*/
		};

		// struct for retrieving a specific message
		struct MBRetrieveMessage_Struct
		{
			/*000*/ uint32 entityID;
			/*004*/ uint32 id; /* id of the message
							   /*008*/ uint8 category;
			/*009*/ uint8 unknown3[3];
			/*012*/
		};


		// Struct for of a message
		struct MBMessage_Struct
		{
			/*0000*/ uint32 id;
			/*0004*/ char date[10]; /* char year[2]; char month[2]; char day[2];
									/*0014*/ uint8 unknown2[6];
			/*0020*/ char author[64];
			/*0084*/ uint8 unknown3[2];
			/*0086*/ uint8 language;
			/*0087*/ uint8 unknown4;
			/*0088*/ char subject[29];
			/*0117*/ uint8 unknown5[3];
			/*0120*/ uint8 category;
			/*0121*/ uint8 unknown6;
			/*0122*/ char message[2048]; // see eqgame function at .text:0047D4E5
			/*2170*/
		};


		// Filter Types for Message Board
		enum MESSAGEBOARD_CATEGORY
		{
			OFFICIAL,
			FOR_SALE,
			GENERAL,
			HELP_WANTED,
			PERSONALS,
			GUILDS
		};

		struct Door_Struct
		{
			/*000*/ char    name[16];            // Filename of Door
			/*016*/ float   pos_y;               // y loc
			/*020*/ float   pos_x;               // x loc
			/*024*/ float   pos_z;               // z loc
			/*028*/ float	heading;
			/*032*/ float	incline;
			/*036*/ float	padding;
			/*040*/ uint8	doorid;             // door's id #
			/*041*/ uint8	opentype;
			/*042*/ uint8	doorIsOpen;
			/*043*/ uint8	inverted;
			/*044*/ int16	parameter;
			/*046*/ uint8	pad[2];
			/*048*/
		};

		/******Useful Open Types*******/
		//53: Spell Particle Effect
		//55: Bulletin Board
		/******************************/

		/****OpenType '53' accompanied by the 'inverted' flag results in a static spell effect, which are listed below:
		1:   Healing orb (blue sphere)
		2:   Buff aura (lots of color moving towards the center)
		3:   Rain of fire (Orange flames)
		4:   Summon item (green cone)
		5:   Vision buff (White small effects)
		6:   Healing cone (blue cone)
		7:   Misc buff (blue particles spreading from center)
		8:   Misc buff (large orange explosions)
		9:   Invis/Gate (blue/white balls)
		10:  Poison debuff (Large green balls and small green balls)
		11:  ?? (Fast moving gold balls and a gold ball aura)
		12:  Dispell (Slow moving blue balls and blue sphere)
		13:  ?? (Fast moving orange balls on 2 planes, moving from center)
		14:  Bind (Very slow moving blue balls and blue sphere)
		15:  Stun (Very fast moving red effects on two planes)
		16:  Root (Green circles moving outward from center)
		17:  ?? (Three different blue effects in static positions from center)
		18:  Rain of fire (Small orange flames)
		19:  Cannabalize (Blue clouds moving to center)
		20:  Lifetap Cast (Green balls moving to center)
		21:  ?? (Lots going on here: Sphere of blue crystals, small blue balls moving outward, big blue balls moving outward slowly)
		22:  ?? (Medium size white balls moving slowly outward, almost makes a sphere)
		23:  ?? (Blue crystals)
		24:  Lifetap or Poison (Green balls spewing everywhere)
		25:  Stun (Red sphere and red balls moving outward
		26:  ?? (Small blue balls rising strait up and small blue balls moving away from center)
		27:  ?? (Green balls raining down from center on a plane, small blue balls rising from center on a plane and yellow balls forming a sphere from center)
		28:  ?? (Green circle raining green balls towards center)
		29:  ?? (Small orange balls forming a ring in center with orange puffs moving to center)
		30:  Healing cone (Blue healing cone flipped on axis)
		31:  Fire (Fire with orange balls)
		32:  ?? (Blue crystals with blue balls moving upwards from center with blue smoke moving upwards)
		33:  ?? (Small teal balls raining from center with static blue buffs forming a sphere)
		34:  Poison debuff (Small green balls raining down fast from center)
		35:  Harvest (Many blue balls moving away in two directions from center forming large walls)
		40:  ?? (Very few blue blues flashing and a blue ring)
		41:  ?? (Small orange flames flickering in a sphere with a ring of orange buffs in center)
		42:  ?? (Gold star forming a sphere and random gold dots moving updwards from center)
		43:  ?? (Small white balls raining from center with very very small blue dots appearing randomly)
		44:  ?? (A sun type effect with orange dots raining from it)
		45:  ?? (A sphere a blue dots raining downwards)
		46:  ?? (A sphere of green dots raining outwards)
		47:  ?? (A sphere of whote dots raining outwards with small blue dots mixed in)
		48:  ?? (Orange dots on a flat plane running outwards from center (disc like))
		49:  Jboots (Red effects moving towards center quickly)
		50:  ?? (Blue dots forming a sphere and flickering)
		51:  Manastone (Green skulls forming a sphere
		66:  ?? (A small pale-green sprinkler)
		67:  ?? (same as above)
		68:  ?? (same as above)
		69:  ?? (same as above)
		105: Smoke stack
		110: Flame (single orange flame)
		111: Flames (8ish orange flames)
		112: Flames (15ish orange flames)
		113: ?? (Sphere of orange flames)
		114: ?? (Large sphere of orange flames)
		115: Smoke stack (GIGANTIC smoke stack, can be seen for miles)
		116: ?? (Pale blue sprinkler)
		117: ?? (same as 116)
		118: ?? (Pale blue sprinkler, little different from 116)
		120: ?? (Sphere of red sparkles)
		121: ?? (Sphere of orange sparkles)
		122: ?? (Sphere of few blue dots)
		123: ?? (Sphere of blue sparkles)
		124: Sprinkler (blue sparkles rushing out of a single hole)
		125: Sprinkler (orange sparkles rushing out of a single hole)
		126: Sprinkler (smaller blue sparkles rushing out of a single hole)
		127: Sprinkler (pale red sparkles rushing out of a single hole)
		128: Water fall (Blob of steam)
		129: Water fall (Steam rushing from a single hole)
		130: Fire ("Realistic" fire effect)
		131: Pulsing HUGE white star
		132: Sprinkler (large steam puffs)
		133: Water fall (same as 128)
		134: Water fall (same as 129)
		143: Water fall (same as 128)
		144: Water fall (same as 128)
		145: ?? (Blue sparkles raining down randomly)
		146: ?? (Same as 145 but smaller)*/

		struct DoorSpawns_Struct	//SEQ
		{
			/*000*/	uint16 count;
			/*002*/	struct Door_Struct doors[0];
		};

		struct ClickDoor_Struct
		{
			/*000*/	uint8	doorid;
			/*001*/	uint8	unknown001[3];
			/*004*/	uint16	item_id;
			/*006*/ uint8	unknown004[2];
			/*008*/	uint16	player_id;
			/*010*/ uint8	unknown010[2];
			/*012*/
		};

		struct DoorOpen_Struct
		{
			/*000*/	uint8	doorid;
			/*001*/	uint8	action;
			/*002*/
		};

		struct LSAuth_Struct
		{
			/*000*/	uint32	lsaccount_id;
			/*004*/	char	name[19];
			/*023*/	char	key[16];
			/*039*/	bool	stale;
			/*040*/	bool	inuse;
			/*041*/	bool	firstconnect;
			/*042*/
		};

		// Added this struct for eqemu and started eimplimentation ProcessOP_SendLoginInfo

		struct LoginInfo_Struct
		{
			/*000*/	char	AccountName[127];
			/*127*/	uint8	Password[24];
			/*151*/	uint8	unknown189[41];
			/*192*/ uint8   zoning;
			/*193*/ uint8   unknown193[3];
			/*196*/
		};

		struct EnterWorld_Struct
		{
			/*000*/	char	charname[30];
		};

		struct NameApproval_Struct
		{
			/*000*/	char charname[30];
			/*030*/	uint8 unknown1;
			/*031*/	uint8 unknown2;
			/*032*/	uint8 race;
			/*033*/	uint8 unknown3;
			/*034*/	uint8 unknown4;
			/*035*/	uint8 unknown5;
			/*036*/	uint8 class_;
			/*037*/	uint8 unknown6;
			/*038*/	uint8 unknown7;
			/*039*/	uint8 unknown8;
			/*040*/
		};

		struct NameApprovalReply_Struct
		{
			/*000*/ uint8 approval;
			/*001*/
		};

		struct ConsumeItem_Struct
		{
			/*000*/	uint16 slot;			//The slot id of the item consumed
			/*002*/	uint8 other[2];		//These are all 0x00.
			/*004*/	uint32 filler[2];	//These are both always 0xFFFFFFFF.
			/*0012*/
		};

		struct ServerMOTD_Struct
		{
			/*000*/	char motd[500];
		};

		struct ChangeWeather_Struct
		{
			/*000*/	uint32 type;
			/*004*/	uint32 intensity;
			/*008*/
		};

		struct BackSlashTarget_Struct
		{
			/*000*/	uint16  bst_target;              // Target ID
			/*002*/	uint16  bst_unknown1;             // ***Placeholder
			/*004*/
		};

		struct BackSlashAssist_Struct
		{
			/*000*/	uint16 bsa_target;
			/*002*/	uint16 bsa_unknown1;
			/*004*/
		};

		struct Make_Pet_Struct { //Simple struct for getting pet info 01/06/08
			/*000*/	uint8 level;
			/*001*/	uint8 class_;
			/*002*/	uint16 race;
			/*004*/	uint8 texture;
			/*005*/	uint8 pettype;
			/*006*/	float size;
			/*010*/	uint8 type;
			/*011*/	uint32 min_dmg;
			/*015*/	uint32 max_dmg;
			/*019*/	int32  max_hp;
			/*023*/
		};

		struct Boat_Struct
		{
			/*000*/	char boatname[64]; //Changed the lenght of the name
		};

		struct ControlBoat_Struct
		{
			/*000*/	uint16	boatId;			// entitylist id of the boat
			/*002*/	bool	TakeControl;	// 01 if taking control, 00 if releasing it
			/*003*/ uint8   unknown5;	// no idea what these last byte represent
			/*004*/
		};

		// this is the generic teleport struct
		// used to teleport a player
		struct TeleportPC_Struct
		{
			/*0000*/ char zone[16];
			/*0016*/ uint8 unknown2[16];
			/*0032*/ float   yPos;          // y loc
			/*0036*/ float   xPos;          // x loc			
			/*0040*/ float   zPos;          // z loc
			/*0044*/ float	 heading;		// will be divided by 2 by the client
		};

		// 
		// This is sent to the client for translocate requests
		// This is sent back from the client to the server if the request was either confirmed or denied
		struct Translocate_Struct
		{
			/*0000*/ char zone[16];
			/*0016*/ uint8 unknown[16];
			/*0032*/ uint16 spellID; // ID of the translocate spell being cast, Client will popup "..translocated to your bind point?" if it is translocate 1422 or group translocate 1334
			/*0034*/ uint8 unknown1[6];
			/*0040*/ char caster[16];
			/*0056*/ uint8 unknown2[16];
			/*0072*/ float y;		// translocate to this Y pos - the client will set this in the local playerProfile to have up2date values for save client request i think
			/*0076*/ float x;		// translocate to this X pos - the client will set this in the local playerProfile to have up2date values for save client request i think
			/*0080*/ float z;		// translocate to this Z pos - the client will set this in the local playerProfile to have up2date values for save client request i think			
			/*0084*/ uint32 confirmed; // Step 1 the server sents the initial request to the client, should be 0 = Confirmation box to player
			/*0088*/				   // Step 2 the client will response with this struct, if confirmed it will be 1 
			// Step 3 the server will response with the actual translocate this time confirmed is again 1
		};

		// 0x8c21, 8
		struct BecomeNPC_Struct
		{
			/*000*/ uint32 entityID;
			/*004*/ uint8 attackLevel;
			/*005*/ uint8 unknown[3];
			/*007*/
		};

		//Struct for NPC roam boxes
		struct RoamBox_Struct
		{
			uint16 id;
			float max_x;
			float min_x;
			float max_y;
			float min_y;
		};
		//Boat Commands
		enum BOATCOMMANDS
		{
			CHECK_FAMILY, // Check others boats before departing
			SPAWN, // Spawn the boat
			TELEPORT_PLAYERS, // Teleport player to the other zone
			GOTO, // Move boat to node number numnode
			WAIT, // Wait at zone/dock for timeneeded
			ROTATE, // Rotate the boat
		};
		//Struct for Boats routes
		struct WorldBoatCommand_Struct
		{
			BOATCOMMANDS cmd;
			std::string zonename;
			union {
				struct
				{
					float x;
					float y;
					float z;
					float heading;
				}position;
				struct
				{
					uint32 from_node; // -1 means that we keep the old one
					uint32 to_node;
					uint32 timeneeded;
				}go_to;
			};

		};

		//Struct for Boats routes
		struct ZoneBoatCommand_Struct
		{
			BOATCOMMANDS cmd;
			float x;
			float y;
			union
			{
				struct
				{
					float speed;
				}go_to;
				struct
				{
					float heading;
				}rotate;
			};

		};

		//Struct for the zone to communicate its status with World
		struct ZoneStatus_Struct
		{
			uint16 zoneID;
		};

		//           No idea what this is used for, but it creates a
		//           perminent object that no client may interact with.
		//			 It also accepts spell sprites (i.e., GENC00), but 
		//			 they do not currently display. I guess we could use 
		//			 this for GM events?
		//
		//Opcode: 0xF620
		struct ObjectDisplayOnly_Struct
		{
			/*0000*/ char test1[32];
			/*0032*/ char modelName[16];
			/*0048*/ char test2[12];
			/*0060*/ float size;
			/*0064*/ float y;
			/*0068*/ float x;
			/*0072*/ float z;
			/*0076*/ float heading;
			/*0080*/ float tilt;
			/*0084*/ char test4[40];
		};

		struct Arrow_Struct
		{
			/*000*/ uint32 type;			//Always 1
			/*004*/ uint32 unknown004;		//Always 0		
			/*008*/ uint32 unknown008;
			/*012*/ float src_y;
			/*016*/ float src_x;
			/*020*/ float src_z;
			/*024*/ float launch_angle;		//heading		
			/*028*/ float tilt;
			/*032*/ float velocity;
			/*036*/ float burstVelocity;
			/*040*/ float burstHorizontal;
			/*044*/ float burstVertical;
			/*048*/ float yaw;
			/*052*/ float pitch;
			/*056*/ float arc;
			/*060*/ uint8 unknown060[4];
			/*064*/	uint16	source_id;
			/*066*/ uint16	target_id;
			/*068*/ uint16  unknown068;
			/*070*/ uint16  unknown070;
			/*072*/ uint32	object_id; //Spell or ItemID
			/*076*/ uint8  light;
			/*077*/ uint8  unknown077;
			/*078*/ uint8  behavior;
			/*079*/ uint8  effect_type; //9 for spell, uses itemtype for items. 28 is also valid, possibly underwater attack?
			/*080*/ uint8  skill;
			/*081*/ char   model_name[16];
			/*097*/ char   buffer[15];
		};

		//			Size: 116 (0x74u)
		//           OpCode: 0x4520
		struct SpawnProjectile_Struct
		{
			/*0000*/ uint32 always1;				// Comment: ASM claims that this member should always be a value of 1
			/*0004*/ uint32 always0;				// Comment: ASM claims that this member should always be a value of 0
			/*0008*/ uint32 test1;				// Comment: Unknown (ASM claims that this member is defined by the client and size uint32)
			/*0012*/ float y;					// Comment: Object's y coordinate (Verified to be float)
			/*0016*/ float x;					// Comment: Object's x coordinate (Verified to be float)
			/*0020*/ float z;					// Comment: Object's z coordinate (Verified to be float)
			/*0024*/ float heading;				// Comment: Object's heading (Verified to be float)
			/*0028*/ float tilt;				// Comment: Object's tilt (Verified to be float)
			/*0032*/ float velocity;			// Comment: Object's velocity (Verified to be float)
			/*0036*/ float burstVelocity;		// Comment: Incremental velocity on the object during its first second of travel (Verified to be float)
			/*0040*/ float burstHorizontal;		// Comment: Shoves the object along the horizontal plane during its first second of travel (Verified to be float)
			/*0044*/ float burstVertical;		// Comment: Shoves the object along the vertical plane during its first second of travel (Verified to be float)
			/*0048*/ float yaw;					// Comment: Rotation around the z-axis (Verified to be float)
			/*0052*/ float pitch;				// Comment: Rotation around the x-axis (Verified to be float)
			/*0056*/ float arc;					// Comment: Applies a gravitational arc on the object (Verified to be float)
			/*0060*/ uint8 test5[4];				// Comment: Unknown (ASM claims that this member is defined by the client)
			/*0064*/ uint32 sourceID;			// Comment: Entity ID of the projectile's inflictor (Verified to be uint32)
			/*0068*/ uint32 targetID;			// Comment: Entity ID of the projectile's inflictee (Verified to be uint32)
			/*0072*/ uint16 test6;				// Comment: Unknown (ASM claims that this member is defined by the client and size uint16)
			/*0074*/ uint16 test7;				// Comment: Unknown (ASM claims that this member is defined by the client and size uint16)
			/*0076*/ uint32 spellID;				// Comment: Spell ID of the projectile (ASM claims uint32)
			/*0080*/ uint8 lightSource;			// Comment: Light source on the object (ASM claims uint8)
			/*0081*/ uint8 test9;				// Comment: Unknown (ASM claims uint8)
			/*0082*/ uint8 spawnBehavior;		// Comment: Boolean flag to enable the attack animation and spawn of the arrow projectile (See 0x11/0x1B below)
			//                     OR used to grant extended life to a spell projectile (See 0x09 below) (ASM claims uint8)
			/*0083*/ uint8 projectileType;		// Comment: 0x09 - Spell bolt, 0x11 - Arrow, 0x1B - Arrow, 0x1C - Under water attack? (Verified to be uint8)
			/*0084*/ uint8 sourceAnimation;		// Comment: Attack animation of the inflictor (Verified to be uint8)
			/*0085*/ char texture[16];			// Comment: Object's texture filename (Must be set to "GENC00" or the like when using spell bolt projectiles)
			/*0101*/ char spacer[15];			// Comment: ASM makes no reference to any bytes after the 85th, so these must be padding
		};


		// struct sent by client when using /discp
		struct UseDiscipline_Struct
		{
			/*0001*/ uint8 discipline;			   // Comment: The discipline executed
			/*0001*/ uint8 unknown[3];			   // Comment: Seems to be always 0 
		};

		struct EntityId_Struct
		{
			/*000*/	int32 entity_id;
			/*004*/
		};

		struct ApproveWorld_Struct
		{
			/*000*/uint8 response;
			/*001*/
		};

		struct ExpansionInfo_Struct
		{
			/*000*/uint32 Expansions;
			/*004*/
		};

		struct OldSpellBuff_Struct
		{
			/*000*/uint8  visable;		// Comment: 0 = Buff not visible, 1 = Visible and permanent buff(Confirmed ) , 2 = Visible and timer on(Confirmed ) 
			/*001*/uint8  level;			// Comment: Level of person who casted buff
			/*002*/uint8  bard_modifier;	// Comment: this seems to be the bard modifier, it is normally 0x0A because we set in in the CastOn_Struct when its not a bard, else its the instrument mod
			/*003*/uint8  activated;	// Comment: ***Placeholder
			/*004*/uint16 spellid;		// Comment: Unknown -> needs confirming -> ID of spell?
			/*006*/uint32 duration;		// Comment: Unknown -> needs confirming -> Duration in ticks
			/*010*/
		};

		// Length: 10
		struct OldItemProperties_Struct
		{

			/*000*/	uint8	unknown01[2];
			/*002*/	int8	charges;				// Comment: signed int because unlimited charges are -1
			/*003*/	uint8	unknown02[7];
			/*010*/
		};

		struct OldBindStruct
		{
			/*000*/ float x;
			/*004*/ float y;
			/*008*/ float z;
			/*012*/
		};

		/*
		*Used in PlayerProfile
		*/
		struct AA_Array
		{
			uint8 AA;
			uint8 value;
		};

		static const uint32 MAX_PP_MEMSPELL = 8;
		static const uint32  MAX_PP_AA_ARRAY = 120;
		static const uint32 MAX_PP_SKILL = 74; // _SkillPacketArraySize;	// 100 - actual skills buffer size

		struct PlayerProfile_Struct
		{
			/*0000*/ int32  checksum;				// Comment: 
			/*0004*/ char   name[30];					// Comment: First Name of Character
			/*0034*/ char   Surname[20];				// Comment: Last Name of Character
			/*0054*/ int8   gender;						// Comment: (Confirmed by Sp0tter).
			/*0055*/ int8   deity;						// Comment: Bertoxxoulus=0x10, (Confirmed by Sp0tter). Needs full deity list fixed in header. 
			/*0056*/ int16  race;						// Comment: Race of Character
			/*0058*/ int8   class_;						// Comment: Class of Character
			/*0059*/ int8   pp_unknown3;				// Comment: Cofruben: should be uknownn?
			/*0060*/ int8   level;						// Comment: Level of Character
			/*0061*/ int8   pp_unknown4[3];				// Comment: 
			/*0064*/ int32  exp;						// Comment: Needs confirming -> Current Experiance Character has?
			/*0068*/ int16	points;				// Comment: Class training points (Confirmed by Wizzel)
			/*0070*/ int16  mana;						// Comment: Needs confirming -> Current Mana Character has?
			/*0072*/ int8	face;						// Comment: What face the Character has? (Values?)
			/*0073*/ int8   unknown0073[47];			// Comment: 
			/*0120*/ int16  cur_hp;						// Comment: Needs Confirming -> Current HP of Character
			/*0122*/ int8   pp_unknown7;				// Comment: 
			/*0123*/ int8   STR;						// Comment: STR of Character
			/*0124*/ int8   STA;						// Comment: STA of Character
			/*0125*/ int8   CHA;						// Comment: CHA of Character
			/*0126*/ int8   DEX;						// Comment: DEX of Character
			/*0127*/ int8   INT;						// Comment: INT of Character
			/*0128*/ int8   AGI;						// Comment: AGI of Character
			/*0129*/ int8   WIS;						// Comment: WIS of Character
			/*0130*/ int8   languages[24];				// Comment: Language of a char (Confirmed by Harakiri)
			/*0154*/ int8   pp_unknown8_2[14];			// Comment: 
			/*0168*/ uint16 inventory[30];				// Comment: Id of items in inventory (Confirmed by Tazadar)
			/*0228*/ uint32 inventoryitemPointers[30];	// Comment: Harakiri these seems to be only used by the client, they are the pointers to the actual item_struct of each item see offset 004C71A3, 
			// if these are not cleared before sending, the client will print "Got a bogus item, deleting it" - another indication these are pointers
			/*0348*/ struct	OldItemProperties_Struct	invItemProprieties[30]; // Comment: infos about the items in the inventory (confirmed by Tazadar)
			/*0648*/ struct OldSpellBuff_Struct buffs[15];	// Comment:
			/*0798*/ uint16 containerinv[80];           // Comment: Ids of items in bags (confirmed by Tazadar)
			/*0958*/ uint16 cursorbaginventory[10];     // Comment: Ids of items in the bag of your cursor (confirmed by Tazadar)
			/*0978*/ struct	OldItemProperties_Struct	bagItemProprieties[80]; // Comment: infos about the items in the bag (confirmed by Tazadar)
			/*1778*/ struct	OldItemProperties_Struct	cursorItemProprieties[10]; // Comment: infos about the items in the cursor bag (confirmed by Tazadar)
			/*1878*/ short  spell_book[256];			// Comment: 
			/*2390*/ short  mem_spells[8];			// Comment: 
			/*2406*/ int8   pp_unknown10[2];			// Comment: 
			/*2408*/ float  y;							// Comment: 
			/*2412*/ float  x;							// Comment: 
			/*2416*/ float  z;							// Comment: 
			/*2420*/ float  heading;					// Comment: 
			/*2424*/ char   current_zone[15];			// Comment: 
			/*2439*/ int8   pp_unknown13[21];			// Comment: 
			/*2460*/ int32  platinum;					// Comment: 
			/*2464*/ int32  gold;						// Comment: 
			/*2468*/ int32  silver;						// Comment: 
			/*2472*/ int32  copper;						// Comment: 
			/*2476*/ int32  platinum_bank;				// Comment: 
			/*2480*/ int32  gold_bank;					// Comment: 
			/*2484*/ int32  silver_bank;				// Comment: 
			/*2488*/ int32  copper_bank;				// Comment: 
			/*2492*/ int32  platinum_cursor;			// Comment:
			/*2496*/ int32  gold_cursor;				// Comment:
			/*2500*/ int32  silver_cursor;				// Comment:
			/*2504*/ int32  copper_cursor;				// Comment:
			/*2508*/ int8   skills[74];					// Comment: FF = Cannot Learn, FE = Have Not Trained Yet (Confirmed by Wizzel)
			/*2582*/ int8	unknown2582[34];			// Comment: 
			/*2616*/ int8	test_unknown;				// Comment:
			/*2617*/ int8	unknown2617[127];			// Comment: Harakiri some of these bits are very important, if they are not 0 the client will request something from the server with opcode 0x0e40 during login and will not load the zone further (there seems be a 60sec timeout on this), the opcode will be sent from sub_4D5968 if at byte 2648 there is a bit > 0, this bit is set by sending OP_ZoneUnavailable 0xa220 - seems like this is the fallback zone when one zone is down?
			/*2744*/ int8	autosplit;					// Comment: 00 = autosplit off ; 01 = autosplit on (Confirmed by Tazadar)
			/*2745*/ int8	unknown2745[3];				// Comment:
			/*2748*/ int8   pvp;					// Comment: Harakiri Client sets this bit when sending OP_PKAcknowledge back with 1, player name does not go red tho
			/*2749*/ int8	unknown2749[15];			// Comment:
			/*2764*/ int8	gm;							// Comment: 
			/*2765*/ int8	unknown2765[23]; 			// Comment: 
			/*2788*/ int8   expansions;					// Comment: Harakiri needs to be 1 or the client wont sent opcode
			/*2789*/ int8	unknown2789[23];
			/*2812*/ int32	hunger_level;				// Comment: Harakiri: the clients max level is actually 6000 for rightclick check, however to hardcap is 32k ("You could not possibly eat any more, you would explode!"), interesting to know is - MONK class has a longer period to autoconsume food in the client (92000 vs 46000) Min(not hungry): 32000+ Max(very hungry): 0 (Confirmed by Wizzel)
			/*2816*/ int32	thirst_level;				// Comment: Harakiri: the clients max level is actually 6000 for rightclick check, however to hardcap is 32k ("You could not possibly drink any more, you would explode!") Min(not thirsty): 32000+ Max(very thirsty): 0 (Confirmed by Wizzel)
			/*2820*/ int8	unknown2820[24];			// Comment: 
			/*2844*/ char   bind_point_zone[20];		// Comment: 
			/*2864*/ char   start_point_zone[4][20];	// Comment: 
			/*2944*/ struct	OldItemProperties_Struct bankinvitemproperties[8];			// Comment: infos of items in bank (Confirmed by Tazadar)
			/*3024*/ struct	OldItemProperties_Struct bankbagitemproperties[80];		// Comment: infos of items in bank containers (Confirmed by Tazadar)
			/*3824*/ int8	unknown2944[4];				// Comment: 
			/*3828*/ float	bind_location[3][5];		// Comment: Bind affinity location, Harakiri Confirmed by client method translocatePC in eqgame offset 00495996 : bind_location[1][1]; == x bind location bind_location[0][1] == y bind location bind_location[2][1]; == z bind location
			/*3888*/ int8	unknown3888[24];			// Comment: 
			/*3912*/ int32  bankinvitemPointers[8];		// Comment: Harakiri these seems to be only used by the client, they are the pointers to the actual item_struct of each item see offset 004C71A3
			/*3944*/ int8	unknown3944[12];
			/*3956*/ int32  time1;						// Comment: 
			/*3960*/ int8   unknown3960[20];			// Comment: 
			/*3980*/ int16	bank_inv[8];				// Comment: id of items in bank (Confirmed by Tazadar)
			/*3996*/ int16	bank_cont_inv[80];			// Comment: id of items in bank containers  (Confirmed by Tazadar)
			/*4156*/ int8	unknown4156[2];				// Comment: 
			/*4158*/ uint16	guild_id;					// Comment: 
			/*4160*/ int32  time2;						// Comment:
			/*4164*/ int8	unknown4164[6];			    // Comment:
			/*4170*/ int8	fatigue;					// Comment: full endu => fatigue = 0 ; zero endu => fatigue = 100 (Confirmed by Tazadar)
			/*4171*/ int8	unknown4171[2];				// Comment:
			/*4173*/ int8	anon;						// Comment: 
			/*4174*/ int8	unknown4174;				// Comment: 
			/*4175*/ int8	guildrank;				// Comment: 0=member, 1=officer, 2=leader
			/*4176*/ int8	intoxication;					// Comment: 0 = Not Drunk, 200ish = Max Drunk (Confirmed by Wizzel)
			/*4177*/ int8	eqbackground;				// Comment: Cofruben: if not set to 0, it'll show the eq background (unuseful?)		
			/*4178*/ int8	unknown4178;
			/*4179*/ int8	unknown4179;
			/*4180*/ uint32 spellSlotRefresh[8];		// Comment: Harakiri time in ms before a spell slot refreshes, i.e. after zoning to prevent that players refresh a spell just through camp or zone, i havent found out why it doesnt work for spell slot 1
			/*4212*/ int8	unknown4180[4];
			/*4216*/ uint32 abilitySlotRefresh;			// Comment: Harakiri cooldown for Lay on Hands or Death Touch from Paladin/SK in ms
			/*4220*/ char	groupMembers[6][48];		// Comment: Cofruben: 15 max length of each player name.
			/*4508*/ int8	unknown4532[3592];			// Comment:
			/*8100*/ int32	logtime;                    // Comment: Tazadar: The client does not seem to use the last bytes so we store the time in order to delete no rent items
			/*8104*/
		};

		/*
		** CharCreate
		** Length: 8100
		*/
		struct CharCreate_Struct
		{
			/*0000*/ char   name[30];					// Comment: First Name of Character
			/*0030*/ char   Surname[20];				// Comment: Last Name of Character
			/*0050*/ uint8   gender;						// Comment: (Confirmed by Sp0tter).
			/*0051*/ uint8   deity_;						// Comment: Bertoxxoulus=0x10, (Confirmed by Sp0tter). Needs full deity list fixed in header. 
			/*0052*/ uint16  race;						// Comment: Race of Character
			/*0054*/ uint8   class_;						// Comment: Class of Character
			/*0055*/ int8   pp_unknown3;				// Comment: Cofruben: should be uknownn?
			/*0056*/ uint8   level;						// Comment: Level of Character
			/*0057*/ int8   pp_unknown4[3];				// Comment: 
			/*0060*/ uint32  exp;						// Comment: Needs confirming -> Current Experiance Character has?
			/*0064*/ int16	trainingpoints;				// Comment: Class training points (Confirmed by Wizzel)
			/*0066*/ uint16  mana;						// Comment: Needs confirming -> Current Mana Character has?
			/*0068*/ uint8	face;						// Comment: What face the Character has? (Values?)
			/*0069*/ int8   unknown0073[50];			// Comment: 
			/*0119*/ uint8   STR;						// Comment: STR of Character
			/*0120*/ uint8   STA;						// Comment: STA of Character
			/*0121*/ uint8   CHA;						// Comment: CHA of Character
			/*0122*/ uint8   DEX;						// Comment: DEX of Character
			/*0123*/ uint8   INT;						// Comment: INT of Character
			/*0124*/ uint8   AGI;						// Comment: AGI of Character
			/*0125*/ uint8   WIS;						// Comment: WIS of Character
			/*0126*/ int8	unused[2294];
			/*2420*/ char   current_zone[15];			// Comment: 
			/*2435*/ int8	unused1[405];
			/*2840*/ char   bind_point_zone[20];
			/*2860*/ char   start_point_zone[4][20];
			/*2940*/ int8   unused2[1212];
			/*4152*/ uint8   deity;
			/*4153*/ int8   final_[3947];
			/*8100*/
		};

		struct Weather_Struct
		{
			/*000*/	uint32 type;
			/*004*/	uint32 intensity;
		};

		struct AltAdvStats_Struct
		{
			/*000*/	int32 experience;
			/*004*/	int16 unspent;
			/*006*/	int8	percentage;
			/*007*/	int8	unknown_void;
			/*008*/
		};

		struct AA_Skills
		{
			/*000*/	uint8 aa_value;
			/*001*/
		};

		struct AATable_Struct
		{
			/*000*/	uint8 unknown;
			/*001*/	structs::AA_Skills aa_list[MAX_PP_AA_ARRAY];
			/*121*/
		};

		//Server sends this packet for reuse timers
		//Client sends this packet as 256 bytes, and is our equivlent of AA_Action
		struct UseAA_Struct
		{
			/*000*/ int32 begin;
			/*004*/ int16 ability; // skill_id of a purchased AA.
			/*006*/ int16  unknown_void;
			/*008*/ int32 end;
			/*012*/

		};

		struct EnvDamage2_Struct
		{
			/*000*/	int16 id;
			/*002*/int16 unknown;
			/*004*/	int8 dmgtype; //FA = Lava; FC = Falling
			/*005*/	int8 unknown2;
			/*006*/	int16 constant; //Always FFFF
			/*008*/	int16 damage;
			/*010*/	int8 unknown3[14]; //A bunch of 00's...
			/*024*/
		};

		struct	ItemViewRequest_Struct
		{
			/*000*/int16	item_id;
			/*002*/char	item_name[64];
			/*066*/
		};

		struct LogServer_Struct
		{
			/*000*/	uint32	unknown[2];
		};

		/* _MAC_NET_MSG_reward_MacMsg, OP_Sound, Size: 48 */
		struct QuestReward_Struct
		{
			/*000*/	uint32	mob_id;
			/*004*/ uint32	unknown004;
			/*008*/	uint32	unknown008;
			/*012*/	uint32	unknown012;
			/*016*/	uint32	unknown016;
			/*020*/	uint32	copper;
			/*022*/	uint32	silver;
			/*024*/	uint32	gold;
			/*028*/	uint32	platinum;
			/*032*/
		};

	};	//end namespace structs
};	//end namespace TRILOGY

#endif /*TRILOGY_STRUCTS_H_*/