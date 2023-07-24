/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2004 EQEMu Development Team (http://eqemu.org)

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
#ifndef FEATURES_H
#define FEATURES_H

/*

	This file defines many optional features for the emu
	as well as various parameters used by the emu.

	If ambitious, most of these could prolly be turned into
	database variables, but the really frequently run pieces
	of code, should not be done that way for speed reasons IMO

*/

/*

Map Configuration
In general, these computations are expensive, so if you have performance
problems, consider turning them off.

*/

//uncomment this to make the LOS code say all mobs can see all others
//when no map file is loaded, opposed to the default nobody-sees-anybody
#define LOS_DEFAULT_CAN_SEE

/*

Zone extensions and features

*/

//Uncomment to make group buffs affect group pets
#define GROUP_BUFF_PETS

//Uncomment this line to enable named quest files:
#define QUEST_SCRIPTS_BYNAME

#ifdef QUEST_SCRIPTS_BYNAME
//extends by name system to look in a templates directory
//independent of zone name
#define QUEST_GLOBAL_BYNAME
#define QUEST_GLOBAL_DIRECTORY "global"
#endif

//the min ratio at which a mob's speed is reduced
#define FLEE_HP_MINSPEED 22
//number of tics to try to run straight away before looking again
#define FLEE_RUN_DURATION 1000
//number of milliseconds between when a mob will check its flee state
#define FLEE_CHECK_TIMER 2000

//only log commands which require this minimum status or more
#define COMMANDS_LOGGING_MIN_STATUS 1

//path to where sql logs should be placed
#define SQL_LOG_PATH "sql_logs/"

//The highest you can #setskill / #setallskill
#define HARD_SKILL_CAP 252

#define SKILL_MAX_LEVEL 75

#define MERCHANT_CHARGE_CAP 500

#define RANDOM_SPAWNID 700000
/*

Zone Numerical configuration

*/

//Reuse times for various skills, here for convenience, in sec
//set to 0 to disable server side checking of timers.
enum {	//reuse times
	FeignDeathReuseTime = 9,
	SneakReuseTime = 7,
	HideReuseTime = 8,
	TauntReuseTime = 6,
	InstillDoubtReuseTime = 9,
	FishingReuseTime = 11,
	ForagingReuseTime = 50,
	MendReuseTime = 290,
	BashReuseTime = 8,
	BackstabReuseTime = 10,
	KickReuseTime = 8,
	TailRakeReuseTime = 6,				// use this for Dragon Punch
	EagleStrikeReuseTime = 6,
	RoundKickReuseTime = 8,
	TigerClawReuseTime = 7,
	FlyingKickReuseTime = 8,
	SenseTrapsReuseTime = 9,
	DisarmTrapsReuseTime = 9,
	HarmTouchReuseTime = 4320,
	LayOnHandsReuseTime = 4320,
	HarmTouchReuseTimeNPC = 2400,		// NPCs have 40 minute timers according to logs
	LayOnHandsReuseTimeNPC = 2400,
	FrenzyReuseTime = 10
};

enum {	//timer settings, all in milliseconds
	AIthink_duration = 50,
	AImovement_duration = 100,
	AIscanarea_delay = 1000,
	// AIClientScanarea_delay = 750,	//used in REVERSE_AGGRO
	AIassistcheck_delay = 3000,		//now often a fighting NPC will yell for help
	ClientProximity_interval = 150,
	CombatEventTimer_expire = 12000,
	ZoneTimerResolution = 3,			//sleep time between zone main loop runs (milliseconds)
	EnragedTimer = 360000,
	EnragedDurationTimer = 10000
};

#define MAGIC_ATTACK_LEVEL 10

//this is the number of levels below the thief's level that
//an npc can be and still let the thief PP them
#define THIEF_PICKPOCKET_UNDER 5

//minimum level to do alchemy
#define MIN_LEVEL_ALCHEMY 25

//chance ratio that a
#define THREATENINGLY_ARRGO_CHANCE 32 // 32/128 (25%) chance that a mob will aggro on con Threateningly

//max factions per npc faction list
#define MAX_NPC_FACTIONS 20

//individual faction pool
#define MAX_PERSONAL_FACTION 2000
#define MIN_PERSONAL_FACTION -2000

//The Level Cap:
//#define LEVEL_CAP RuleI(Character, MaxLevel)	//hard cap is 127
#define HARD_LEVEL_CAP 127

//the square of the maximum range at which you could possibly use NPC services (shop, etc)
#define USE_NPC_RANGE2 200*200		//arbitrary right now

#define USE_BANKER_RANGE 25*25 // client closes banking window at distance 20

//the formula for experience for killing a mob.
//level is the only valid variable to use
#define EXP_FORMULA level*level

#define HIGHEST_AA_VALUE 35

#define ZONE_CONTROLLER_NPC_ID 10

// NPCs this and below are considered deep green and have special behavior.
#define DEEP_GREEN_LEVEL 18

// Number of quest items a Quest NPC can hold
#define MAX_NPC_QUEST_INVENTORY 24

//Some hard coded statuses from commands and other places:
enum {
	minStatusToBeGM = 40,
	minStatusToUseGMCommands = 80,
	minStatusToKick = 150,
	minStatusToAvoidFalling = 80,
	minStatusToHaveInvalidSpells = 80,
	minStatusToHaveInvalidSkills = 80,
	minStatusToIgnoreZoneFlags = 80,
	minStatusToSeeOthersZoneFlags = 80,
	minStatusToEditOtherGuilds = 80,
	commandMovecharSelfOnly = 80,	//below this == only self move allowed
	commandMovecharToSpecials = 200,	//ability to send people to cshome/load zones
	commandZoneToSpecials = 80,		//zone to cshome, out of load zones
	commandToggleAI = 250,			//can turn NPC AI on and off
	commandCastSpecials = 100,		//can cast special spells
	commandInstacast = 100,			//instant-cast all #casted spells
	commandLevelAboveCap = 250,		//can #level players above level cap
	commandLevelNPCAboveCap = 250,	//can #level NPCs above level cap
	commandSetSkillsOther = 100,	//ability to setskills on others
	commandRaceOthers = 100,	//ability to #race on others
	commandGenderOthers = 100,	//ability to #gender on others
	commandTextureOthers = 100,	//ability to #texture on others
	commandDoAnimOthers = 100,	//can #doanim on others
	commandLockZones = 101,		//can lock or unlock zones
	commandEditPlayerCorpses = 95,	//can Edit Player Corpses
	commandChangeFlags = 200,		//ability to set/refresh flags
	commandBanPlayers = 100,		//can set bans on players
	commandChangeDatarate = 201,	//edit client's data rate
	commandZoneToCoords = 0,		//can #zone with coordinates
	commandInterrogateInv = 100		//below this == only log on error state and self-only target dump
};

/*

Developer configuration

*/

//#define EQPROFILE
#ifdef EQPROFILE
//Enable the zone profiler
#define ZONE_PROFILE

#define COMMON_PROFILE

#define PROFILE_DUMP_TIME 3*60
#endif	//EQPROFILE



#endif

