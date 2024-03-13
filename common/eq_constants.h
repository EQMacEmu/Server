/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2003 EQEMu Development Team (http://eqemulator.net)

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
#ifndef EQ_CONSTANTS_H
#define EQ_CONSTANTS_H

#include "skills.h"
#include "types.h"
#include "say_link.h"
#include "light_source.h"

#include <string>

#define MAX_GROUP_MEMBERS 6

namespace AppearanceType {
	constexpr uint32 Die          = 0; // Causes the client to keel over and zone to bind point (default action)
	constexpr uint32 WhoLevel     = 1; // Level that shows up on /who
	constexpr uint32 MaxHealth    = 2;
	constexpr uint32 Invisibility = 3; // 0 = Visible, 1 = Invisible
	constexpr uint32 PVP          = 4; // 0 = Non-PVP, 1 = PVP
	constexpr uint32 Light        = 5; // Light type emitted by player (lightstone, shiny shield)
	constexpr uint32 Animation    = 14; // 100 = Standing, 102 = Freeze, 105 = Looting, 110 = Sitting, 111 = Crouching, 115 = Lying
	constexpr uint32 Sneak        = 15; // 0 = Normal, 1 = Sneaking
	constexpr uint32 SpawnID      = 16; // Server -> Client, sets player spawn ID
	constexpr uint32 Health       = 17; // Client->Server, my HP has changed (like regen tic)
	constexpr uint32 Linkdead     = 18; // 0 = Normal, 1 = Linkdead
	constexpr uint32 FlyMode      = 19; // 0 = Off, 1 = Flying, 2 = Levitating, 3 = Water, 4 = Floating, 5 = Levitating while Running
	constexpr uint32 GM           = 20; // 0 = Non-GM, 1 = GM
	constexpr uint32 Anonymous    = 21; // 0 = Non-Anonymous, 1 = Anonymous, 2 = Roleplaying
	constexpr uint32 GuildID      = 22;
	constexpr uint32 GuildRank    = 23;
	constexpr uint32 AFK          = 24; // 0 = Non-AFK, 1 = AFK
	constexpr uint32 Pet          = 25; // Parameter is Entity ID of owner, or 0 for when charm breaks
	constexpr uint32 Summoned     = 27;
	constexpr uint32 Split        = 28; // 0 = No Split, 1 = uint32 Split
	constexpr uint32 Size         = 29; // Spawn's Size
	constexpr uint32 SetType      = 30; // 0 = PC, 1 = NPC, 2 = Corpse
	constexpr uint32 NPCName      = 31; // Change PC name color to NPC name color
	constexpr uint32 DamageState  = 44; // The damage state of a destructible object (0 through 10) plays sound IDs, most only have 2 or 4 states though
}

// solar: Animations for AnimationType:Animation
namespace Animation {
	constexpr uint32 Standing  = 100;
	constexpr uint32 Freeze    = 102;
	constexpr uint32 Looting   = 105;
	constexpr uint32 Sitting   = 110;
	constexpr uint32 Crouching = 111;
	constexpr uint32 Lying     = 115;
	constexpr uint32 Corpse    = 120;
}

typedef enum EmuAppearance {
	eaStanding = 0,
	eaSitting,		//1
	eaCrouching,	//2
	eaDead,			//3
	eaLooting,		//4
	_eaMaxAppearance
} EmuAppearance;

// msg_type's for custom usercolors
#define MT_Say					256
#define MT_Tell					257
#define MT_Group				258
#define MT_Guild				259
#define MT_OOC					260
#define MT_Auction				261
#define MT_Shout				262
#define MT_Emote				263
#define MT_Spells				264
#define MT_YouHitOther			265
#define MT_OtherHitsYou			266
#define MT_YouMissOther			267
#define MT_OtherMissesYou		268
#define MT_Broadcasts			269
#define MT_Skills				270
#define MT_Disciplines			271
#define	MT_Unused1				272
#define MT_DefaultText			273
#define MT_Unused2				274
#define MT_MerchantOffer		275
#define MT_MerchantBuySell		276
#define	MT_YourDeath			277
#define MT_OtherDeath			278
#define MT_OtherHits			279
#define MT_OtherMisses			280
#define	MT_Who					281
#define MT_YellForHelp			282
#define MT_NonMelee				283
#define MT_WornOff				284
#define MT_MoneySplit			285
#define MT_LootMessages			286 // Filters under Damage Shield?
#define MT_DiceRoll				287
#define MT_OtherSpells			288
#define MT_SpellFailure			289
#define MT_Chat					290
#define MT_Channel1				291
#define MT_Channel2				292
#define MT_Channel3				293
#define MT_Channel4				294
#define MT_Channel5				295
#define MT_Channel6				296
#define MT_Channel7				297
#define MT_Channel8				298
#define MT_Channel9				299
#define MT_Channel10			300
#define MT_CritMelee			301
#define MT_SpellCrits			302
#define MT_TooFarAway			303
#define MT_NPCRampage			304
#define MT_NPCFlurry			305
#define MT_NPCEnrage			306
#define MT_SayEcho				307
#define MT_TellEcho				308
#define MT_GroupEcho			309
#define MT_GuildEcho			310
#define MT_OOCEcho				311
#define MT_AuctionEcho			312
#define MT_ShoutECho			313
#define MT_EmoteEcho			314
#define MT_Chat1Echo			315
#define MT_Chat2Echo			316
#define MT_Chat3Echo			317
#define MT_Chat4Echo			318
#define MT_Chat5Echo			319
#define MT_Chat6Echo			320
#define MT_Chat7Echo			321
#define MT_Chat8Echo			322
#define MT_Chat9Echo			323
#define MT_Chat10Echo			324
#define MT_DoTDamage			325
#define MT_ItemLink				326
#define MT_RaidSay				327
#define MT_MyPet				328
#define MT_DS					329 //White text (should be non-melee) unknown filter
#define MT_Leadership			330
#define MT_PetFlurry			331
#define MT_PetCrit				332
#define MT_FocusEffect			333
#define MT_Experience			334
#define MT_System				335
#define MT_PetSpell				336
#define MT_PetResponse			337
#define MT_ItemSpeech			338
#define MT_StrikeThrough		339
#define MT_Stun					340

//Unused numbers are either White, Grey, or LightGrey. After 20, all are LightGrey until 256.
enum ChatColor
{
	CC_Default					= 0, // Normal
	CC_Grey						= 1, 
	CC_Green					= 2, // Auction/OOC
	CC_Blue						= 4, // Skills/Spells/Emote
	CC_Purple					= 5, // Item Tags
	CC_LightGrey				= 6, 
	CC_Say						= 7,
	CC_NPCQuestSay				= 10,
	CC_DarkGray					= 12,
	CC_Red						= 13, // Shout/Fizzles
	CC_LightGreen				= 14, // Guild
	CC_Yellow					= 15, // Spell Worn Off/Broadcast
	CC_LightBlue				= 16, 
	CC_LightNavy				= 17,
	CC_Cyan						= 18, // Group/Raid
	CC_Black					= 20,
	CC_User_Say					= 256,
	CC_User_Tell				= 257,
	CC_User_Group				= 258,
	CC_User_Guild				= 259,
	CC_User_OOC					= 260,
	CC_User_Auction				= 261,
	CC_User_Shout				= 262,
	CC_User_Emote				= 263,
	CC_User_Spells				= 264,
	CC_User_YouHitOther			= 265,
	CC_User_OtherHitYou			= 266,
	CC_User_YouMissOther		= 267,
	CC_User_OtherMissYou		= 268,
	CC_User_Duels				= 269,
	CC_User_Skills				= 270,
	CC_User_Disciplines			= 271,
	CC_User_Default				= 273,
	CC_User_MerchantOffer		= 275,
	CC_User_MerchantExchange	= 276,
	CC_User_YourDeath			= 277,
	CC_User_OtherDeath			= 278,
	CC_User_OtherHitOther		= 279,
	CC_User_OtherMissOther		= 280,
	CC_User_Who					= 281,
	CC_User_Yell				= 282,
	CC_User_NonMelee			= 283,
	CC_User_SpellWornOff		= 284,
	CC_User_MoneySplit			= 285,
	CC_User_Loot				= 286,
	CC_User_Random				= 287,
	CC_User_OtherSpells			= 288,
	CC_User_SpellFailure		= 289,
	CC_User_ChatChannel			= 290,
	CC_User_Chat1				= 291,
	CC_User_Chat2				= 292,
	CC_User_Chat3				= 293,
	CC_User_Chat4				= 294,
	CC_User_Chat5				= 295,
	CC_User_Chat6				= 296,
	CC_User_Chat7				= 297,
	CC_User_Chat8				= 298,
	CC_User_Chat9				= 299,
	CC_User_Chat10				= 300,
	CC_User_MeleeCrit			= 301,
	CC_User_SpellCrit			= 302,
	CC_User_TooFarAway			= 303,
	CC_User_NPCRampage			= 304,
	CC_User_NPCFurry			= 305,
	CC_User_NPCEnrage			= 306,
	CC_User_EchoSay				= 307,
	CC_User_EchoTell			= 308,
	CC_User_EchoGroup			= 309,
	CC_User_EchoGuild			= 310,
	CC_User_EchoOOC				= 311,
	CC_User_EchoAuction			= 312,
	CC_User_EchoShout			= 313,
	CC_User_EchoEmote			= 314,
	CC_User_EchoChat1			= 315,
	CC_User_EchoChat2			= 316,
	CC_User_EchoChat3			= 317,
	CC_User_EchoChat4			= 318,
	CC_User_EchoChat5			= 319,
	CC_User_EchoChat6			= 320,
	CC_User_EchoChat7			= 321,
	CC_User_EchoChat8			= 322,
	CC_User_EchoChat9			= 323,
	CC_User_EchoChat10			= 324,
	CC_User_UnusedAtThisTime	= 325, //Yellow
	CC_User_ItemTags			= 326,
	CC_User_RaidSay				= 327,
	CC_User_MyPet				= 328,
	CC_User_DamageShield		= 329,
};

//ZoneChange_Struct->success values
enum ZoningMessage : int8
{
	ZoneNoMessage = 0,
	ZoneSuccess = 1,
	ZoneNotReady = -1,
	ZoneValidPC = -2,
	ZoneStoryZone = -3,
	ZoneNoExpansion = -6,
	ZoneNoExperience = -7
};

typedef enum {
	FilterDamageShields = 0,	//0 is on 1 is off
	FilterNPCSpells = 1,		//0 is on - doesn't send packet	
	FilterPCSpells = 2,			//0 is on 1 is off 2 is group
	FilterBardSongs = 3,		//0 is on 1 is self 2 is group 3 is off
	FilterNone = 4,				//0 is on
	FilterGuildChat = 5,		//0 is off 1 is on		
	FilterSocials = 6,			//0 is off 1 is on
	FilterGroupChat = 7,		//0 is off 1 is on	
	FilterShouts = 8,		    //0 is off 1 is on
	FilterAuctions = 9,		    //0 is off 1 is on
	FilterOOC = 10,				//0 is off 1 is on
	FilterMyMisses = 11,		//0 is off 1 is on
	FilterOthersMiss = 12,		//0 is off 1 is on
	FilterOthersHit = 13,		//0 is off 1 is on
	FilterMissedMe = 14,		//0 is off 1 is on
	FilterSpellCrits = 15,		//0 is on 1 is self 2 is off
	FilterMeleeCrits = 16,		//0 is on 1 is self 2 is off
	_FilterCount
} eqFilterType;

typedef enum {
	FilterHide,
	FilterShow,
	FilterShowGroupOnly,
	FilterShowSelfOnly
} eqFilterMode;

#define	STAT_STR		0
#define	STAT_STA		1
#define	STAT_AGI		2
#define	STAT_DEX		3
#define	STAT_INT		4
#define	STAT_WIS		5
#define	STAT_CHA		6
#define	STAT_MAGIC		7
#define	STAT_COLD		8
#define	STAT_FIRE		9
#define	STAT_POISON		10
#define	STAT_DISEASE		11
#define	STAT_MANA		12
#define	STAT_HP			13
#define	STAT_AC			14
#define STAT_ENDURANCE		15
#define STAT_ATTACK		16
#define STAT_HP_REGEN		17
#define STAT_MANA_REGEN		18
#define STAT_HASTE		19
#define STAT_DAMAGE_SHIELD	20

/*
**	Recast timer types. Used as an off set to charProfileStruct timers.
**
**	(Another orphaned enumeration...)
*/
enum RecastTimerTypes
{
	RecTimer_0 = 0,
	RecTimer_1,
	RecTimer_WeaponHealClick,		// 2
	RecTimer_MuramiteBaneNukeClick,	// 3
	RecTimer_4,
	RecTimer_DispellClick,			// 5 (also click heal orbs?)
	RecTimer_Epic,					// 6
	RecTimer_OoWBPClick,			// 7
	RecTimer_VishQuestClassItem,	// 8
	RecTimer_HealPotion,			// 9
	RecTimer_10,
	RecTimer_11,
	RecTimer_12,
	RecTimer_13,
	RecTimer_14,
	RecTimer_15,
	RecTimer_16,
	RecTimer_17,
	RecTimer_18,
	RecTimer_ModRod,				// 19
	_RecTimerCount
};

enum GroupUpdateAction
{
	GUA_Joined = 0,
	GUA_Left = 1,
	GUA_LastLeft = 6,
	GUA_FullGroupInfo = 7,
	GUA_MakeLeader = 8,
	GUA_Started = 9
};

static const uint8 DamageTypeSomething	= 0x1C;	//0x1c is something...
static const uint8 DamageTypeFalling	= 0xFC;
static const uint8 DamageTypeSpell		= 0xE7;
static const uint8 DamageTypeUnknown	= 0xFF;

/*
**	Skill damage types
**
**	(indexed by 'Skill' of SkillUseTypes)
*/
static const uint8 SkillDamageTypes[EQ::skills::HIGHEST_SKILL + 1] = // change to _SkillServerArraySize once activated
{
/*1HBlunt*/					0,
/*1HSlashing*/				1,
/*2HBlunt*/					0,
/*2HSlashing*/				1,
/*Abjuration*/				DamageTypeSpell,
/*Alteration*/				DamageTypeSpell,
/*ApplyPoison*/				DamageTypeUnknown,
/*Archery*/					7,
/*Backstab*/				8,
/*BindWound*/				DamageTypeUnknown,
/*Bash*/					10,
/*Block*/					DamageTypeUnknown,
/*BrassInstruments*/		DamageTypeSpell,
/*Channeling*/				DamageTypeUnknown,
/*Conjuration*/				DamageTypeSpell,
/*Defense*/					DamageTypeUnknown,
/*Disarm*/					DamageTypeUnknown,
/*DisarmTraps*/				DamageTypeUnknown,
/*Divination*/				DamageTypeSpell,
/*Dodge*/					DamageTypeUnknown,
/*DoubleAttack*/			DamageTypeUnknown,
/*DragonPunch*/				21,
/*DualWield*/				DamageTypeUnknown,
/*EagleStrike*/				23,
/*Evocation*/				DamageTypeSpell,
/*FeignDeath*/				4,
/*FlyingKick*/				30,
/*Forage*/					DamageTypeUnknown,
/*HandtoHand*/				4,
/*Hide*/					DamageTypeUnknown,
/*Kick*/					30,
/*Meditate*/				DamageTypeUnknown,
/*Mend*/					DamageTypeUnknown,
/*Offense*/					DamageTypeUnknown,
/*Parry*/					DamageTypeUnknown,
/*PickLock*/				DamageTypeUnknown,
/*1HPiercing*/				36,
/*Riposte*/					DamageTypeUnknown,
/*RoundKick*/				30,
/*SafeFall*/				DamageTypeUnknown,
/*SsenseHeading*/			DamageTypeUnknown,
/*Singing*/					DamageTypeSpell,
/*Sneak*/					DamageTypeUnknown,
/*SpecializeAbjure*/		DamageTypeUnknown,
/*SpecializeAlteration*/	DamageTypeUnknown,
/*SpecializeConjuration*/	DamageTypeUnknown,
/*SpecializeDivination*/	DamageTypeUnknown,
/*SpecializeEvocation*/		DamageTypeUnknown,
/*PickPockets*/				DamageTypeUnknown,
/*StringedInstruments*/		DamageTypeSpell,
/*Swimming*/				DamageTypeUnknown,
/*Throwing*/				51,
/*TigerClaw*/				23,
/*Tracking*/				DamageTypeUnknown,
/*WindInstruments*/			DamageTypeSpell,
/*Fishing*/					DamageTypeUnknown,
/*MakePoison*/				DamageTypeUnknown,
/*Tinkering*/				DamageTypeUnknown,
/*Research*/				DamageTypeUnknown,
/*Alchemy*/					DamageTypeUnknown,
/*Baking*/					DamageTypeUnknown,
/*Tailoring*/				DamageTypeUnknown,
/*SenseTraps*/				DamageTypeUnknown,
/*Blacksmithing*/			DamageTypeUnknown,
/*Fletching*/				DamageTypeUnknown,
/*Brewing*/					DamageTypeUnknown,
/*AlcoholTolerance*/		DamageTypeUnknown,
/*Begging*/					DamageTypeUnknown,
/*JewelryMaking*/			DamageTypeUnknown,
/*Pottery*/					DamageTypeUnknown,
/*PercussionInstruments*/	DamageTypeSpell,
/*Intimidation*/			30,
/*Berserking*/				DamageTypeUnknown,
/*Taunt*/					DamageTypeUnknown,
/*Frenzy*/					//74 //,
// /*RemoveTrap*/				DamageTypeUnknown,	// Needs research (set for SenseTrap value)
// /*TripleAttack*/			DamageTypeUnknown,	// Needs research (set for DoubleAttack value)
// /*2HPiercing*/				36					// Needs research (set for 1HPiercing value - similar to slash/blunt)
};

enum TextureTypes : uint8
{
	TextureCloth = 0,
	TextureLeather,
	TextureRingmail,
	TexturePlate,
	TextureSilk,
	TextureChitin,
	TextureUnknown1,
	TextureScale,
	TextureUnknown2,
	TextureUnknown3,
	TextureRedRobe,
	TextureElementRobe,
	TextureBlightedRobe,
	TextureCrystalRobe,
	TextureOracleRobe,
	TextureKedgeRobe,
	TextureMetallicRobe,
	TextureRobe,
	TextureVelLeather,
	TextureVelChain,
	TexturePogPlate,
	TextureUlthork,
	TextureRyGorr,
	TextureKael,
	_TextureTypesCount
};

#define INVALID_INDEX	-1

namespace EQ
{
	namespace legacy {
		enum InventorySlot {
			SLOT_CURSOR_END = (int16)0xFFFE, // I hope no one is using this...
			SLOT_TRADESKILL = 1000,
			SLOT_QUEST = 9999,
			//SLOT_INVALID = (int16)0xFFFF,
		};

	} // namespace legacy

}


enum Zones
{
	qeynos=1,
	qeynos2=2,
	qrg=3,
	qeytoqrg=4,
	highpass=5,
	highkeep=6,
	freportn=8,
	freportw=9,
	freporte=10,
	runnyeye=11,
	qey2hh1=12,
	northkarana=13,
	southkarana=14,
	eastkarana=15,
	beholder=16,
	blackburrow=17,
	paw=18,
	rivervale=19,
	kithicor=20,
	commons=21,
	ecommons=22,
	erudnint=23,
	erudnext=24,
	nektulos=25,
	cshome=26,
	lavastorm=27,
	nektropos=28,
	halas=29,
	everfrost=30,
	soldunga=31,
	soldungb=32,
	misty=33,
	nro=34,
	sro=35,
	befallen=36,
	oasis=37,
	tox=38,
	hole=39,
	neriaka=40,
	neriakb=41,
	neriakc=42,
	neriakd=43,
	najena=44,
	qcat=45,
	innothule=46,
	feerrott=47,
	cazicthule=48,
	oggok=49,
	rathemtn=50,
	lakerathe=51,
	grobb=52,
	aviak=53,
	gfaydark=54,
	akanon=55,
	steamfont=56,
	lfaydark=57,
	crushbone=58,
	mistmoore=59,
	kaladima=60,
	felwithea=61,
	felwitheb=62,
	unrest=63,
	kedge=64,
	guktop=65,
	gukbottom=66,
	kaladimb=67,
	butcher=68,
	oot=69,
	cauldron=70,
	airplane=71,
	fearplane=72,
	permafrost=73,
	kerraridge=74,
	paineel=75,
	hateplane=76,
	arena=77,
	fieldofbone=78,
	warslikswood=79,
	soltemple=80,
	droga=81,
	cabwest=82,
	swampofnohope=83,
	firiona=84,
	lakeofillomen=85,
	dreadlands=86,
	burningwood=87,
	kaesora=88,
	sebilis=89,
	citymist=90,
	skyfire=91,
	frontiermtns=92,
	overthere=93,
	emeraldjungle=94,
	trakanon=95,
	timorous=96,
	kurn=97,
	erudsxing=98,
	stonebrunt=100,
	warrens=101,
	karnor=102,
	chardok=103,
	dalnir=104,
	charasis=105,
	cabeast=106,
	nurga=107,
	veeshan=108,
	veksar=109,
	iceclad=110,
	frozenshadow=111,
	velketor=112,
	kael=113,
	skyshrine=114,
	thurgadina=115,
	eastwastes=116,
	cobaltscar=117,
	greatdivide=118,
	wakening=119,
	westwastes=120,
	crystal=121,
	necropolis=123,
	templeveeshan=124,
	sirens=125,
	mischiefplane=126,
	growthplane=127,
	sleeper=128,
	thurgadinb=129,
	erudsxing2=130,
	shadowhaven=150,
	bazaar=151,
	nexus=152,
	echo=153,
	acrylia=154,
	sharvahl=155,
	paludal=156,
	fungusgrove=157,
	vexthal=158,
	sseru=159,
	katta=160,
	netherbian=161,
	ssratemple=162,
	griegsend=163,
	thedeep=164,
	shadeweaver=165,
	hollowshade=166,
	grimling=167,
	mseru=168,
	letalis=169,
	twilight=170,
	thegrey=171,
	tenebrous=172,
	maiden=173,
	dawnshroud=174,
	scarlet=175,
	umbral=176,
	akheva=179,
	arena2=180,
	jaggedpine=181,
	tutorial = 183,
	load=184,
	load2=185,
	clz=190,
	codecay=200,
	pojustice=201,
	poknowledge=202,
	potranquility=203,
	ponightmare=204,
	podisease=205,
	poinnovation=206,
	potorment=207,
	povalor=208,
	bothunder=209,
	postorms=210,
	hohonora=211,
	solrotower=212,
	powar=213,
	potactics=214,
	poair=215,
	powater=216,
	pofire=217,
	poeartha=218,
	potimea=219,
	hohonorb=220,
	nightmareb=221,
	poearthb=222,
	potimeb=223,
};

//These are NPCIDs in the database. All of these boats send a BoardBoat opcode when boarded.
enum Boats
{
	Stormbreaker = 770, //freporte-oot-butcherblock
	SirensBane  = 771,
	Sea_King = 772, //erudext-erudsxing-qeynos
	Golden_Maiden  = 773,
	Maidens_Voyage  = 838, //timorous-firiona
	Bloated_Belly = 839, //timorous-overthere
	Barrel_Barge = 840, //Shuttle timorous-oasis
	Muckskimmer = 841,
	Sabrina = 24056, //Shuttle in Erud
	Island_Shuttle = 96075, //Shuttle to Elf docks in timorous
	Captains_Skiff = 842, //Shuttle timorous-butcherblock
	Icebreaker = 110083, //iceclad
	pirate_runners_skiff = 843 //Shuttle iceclad-nro
};

// Values are bitwise, so we can compare with the expansion field in account.
enum Expansions
{
	ClassicEQ = 0,
	KunarkEQ = 1,
	VeliousEQ = 2,
	LuclinEQ = 4,
	PlanesEQ = 8,
	AllEQ = 15
};

static const uint32 MAX_SPELL_DB_ID_VAL = 65535;

enum ChatChannelNames : uint16
{
	ChatChannel_Guild = 0,
	ChatChannel_Group = 2,
	ChatChannel_Shout = 3,
	ChatChannel_Auction = 4,
	ChatChannel_OOC = 5,
	ChatChannel_Broadcast = 6,
	ChatChannel_Tell = 7,
	ChatChannel_Say = 8,
	ChatChannel_Petition = 10,
	ChatChannel_GMSAY = 11,
	ChatChannel_TellEcho = 14,
	ChatChannel_Raid = 15,

	ChatChannel_UNKNOWN_Guild = 17,
	ChatChannel_UNKNOWN_GMSAY = 18
};

#endif
