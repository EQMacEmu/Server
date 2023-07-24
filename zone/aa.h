
#ifndef AA_H
#define AA_H

struct AA_Ability;
struct SendAA_Struct;


#define MANA_BURN 664

#include <map>
#include "zonedump.h"

#define MAX_SWARM_PETS 12	//this can change as long as you make more coords (swarm_pet_x/swarm_pet_y)

//this might be missing some, and some might not be used...
typedef enum {	//AA Targeting Constants
	aaTargetUser = 1,
	aaTargetCurrent = 2,		//use current target
	aaTargetGroup = 3,			//target group of user
	aaTargetCurrentGroup = 4,	//target group of current target
	aaTargetPet = 5				//target the user's pet
} aaTargetType;


typedef enum {
	aaActionNone				= 0,
	aaActionAETaunt				= 1,
	aaActionMassBuff			= 2,
	aaActionPurgePoison			= 3,
	aaActionWarcry				= 4,
	aaActionRampage				= 5,
	aaActionUnused6				= 6,
	aaActionUnused7				= 7,
	aaActionUnused8				= 8,
	aaActionImprovedFamiliar	= 9,
	aaActionActOfValor			= 10,
	aaActionUnused11			= 11,
	aaActionEscape				= 12,
	aaActionBeastialAlignment	= 13,
	aaActionLeechTouch			= 14,
	aaActionProjectIllusion		= 15,
	aaActionFadingMemories		= 16,
	aaActionFrenziedBurnout		= 17
} aaNonspellAction;

//use these for AAs which dont cast spells, yet need effects
//if this list grows beyond 32, more work is needed in *AAEffect
typedef enum {	//AA Effect IDs
	aaEffectMassGroupBuff = 1,
	aaEffectWarcry,
	aaEffectLeechTouch,
	aaEffectProjectIllusion,
	aaEffectFrenziedBurnout
} aaEffectType;

typedef enum {	//AA IDs
	aaNone									=0,
	aaInnateStrength						=2,//implemented as bonus
	aaInnateStamina							=7,//implemented as bonus
	aaInnateAgility							=12,//implemented as bonus
	/*aaCompleteHeal						=13,*///not implemented, but is in dbstr_us.txt
	aaInnateDexterity						=17,//implemented as bonus
	aaInnateIntelligence					=22,//implemented as bonus
	aaInnateWisdom							=27,//implemented as bonus
	aaInnateCharisma						=32,//implemented as bonus
	aaInnateFireProtection					=37,//implemented as bonus
	aaInnateColdProtection					=42,//implemented as bonus
	aaInnateMagicProtection					=47,//implemented as bonus
	aaInnatePoisonProtection				=52,//implemented as bonus
	aaInnateDiseaseProtection				=57,//implemented as bonus
	aaInnateRunSpeed						=62,//implemented as bonus
	aaInnateRegeneration					=65,//implemented as bonus
	aaInnateMetabolism						=68,
	aaInnateLungCapacity					=71,//handled by client
	aaFirstAid								=74,//implemented as bonus
	aaHealingAdept							=77,//implemented as bonus-focus
	aaHealingGift							=80,//implemented as bonus
	aaSpellCastingMastery					=83,//untested
	aaSpellCastingReinforcement				=86,//implemented
	aaMentalClarity							=89,//implemented as bonus
	aaSpellCastingFury						=92,//implemented as bonus
	aaChanellingFocus						=95,//implemented as bonus *Live AA effect removed in 2006
	aaSpellCastingSubtlety					=98,//untested
	aaSpellCastingExpertise					=101,//untested
	aaSpellCastingDeftness					=104,//implemented as bonus-focus
	aaNaturalDurability						=107,//implemented as bonus
	aaNaturalHealing						=110,//implemented as bonus
	aaCombatFury							=113,//implemented as bonus
	aaFearResistance						=116,//untested
	aaFinishingBlow							=119,//untested
	aaCombatStability						=122,//implemented as bonus
	aaCombatAgility							=125,//implemented as bonus
	aaMassGroupBuff							=128,//untested
	aaDivineResurrection					=129,//DB
	aaInnateInvisToUndead					=130,//DB
	aaCelestialRegeneration					=131,//untested
	aaBestowDivineAura						=132,//DB
	aaTurnUndead							=133,//DB
	aaPurifySoul							=136,//DB
	aaQuickEvacuation						=137,//implemented as bonus-focus
	aaExodus								=140,//untested
	aaQuickDamage							=141,//implemented as bonus-focus
	aaEnhancedRoot							=144,//implemented as bonus
	aaDireCharm								=145,//untested
	aaCannibalization						=146,//DB
	aaQuickBuff								=147,//implemented as bonus-focus
	aaAlchemyMastery						=150,
	aaRabidBear								=153,//DB
	aaManaBurn								=154,//DB
	aaImprovedFamiliar						=155,//untested, implemented?
	aaNexusGate								=156,//DB
	aaUnknown54								=157,
	aaPermanentIllusion						=158,
	aaJewelCraftMastery						=159,
	aaGatherMana							=162,//DB
	aaMendCompanion							=163,//DB
	aaQuickSummoning						=164,//implemented as bonus-focus
	aaFrenziedBurnout						=167,//DB
	aaElementalFormFire						=168,//DB
	aaElementalFormWater					=171,//DB
	aaElementalFormEarth					=174,//DB
	aaElementalFormAir						=177,//DB
	aaImprovedReclaimEnergy					=180,//untested
	aaTurnSummoned							=181,//DB
	aaElementalPact							=182,//DB
	aaLifeBurn								=183,//DB
	aaDeadMesmerization						=184,//DB
	aaFearstorm								=185,//DB
	aaFleshToBone							=186,//DB
	aaCallToCorpse							=187,//DB
	aaDivineStun							=188,//DB
	aaImprovedLayOnHands					=189,
	aaSlayUndead							=190,//implemented as bonus
	aaActOfValor							=193,//DB
	aaHolySteed								=194,//DB
	aaFearless								=195,
	aa2HandBash								=196,//works. handled by client?
	aaInnateCamouflage						=197,//DB
	aaAmbidexterity							=198,//implemented as bonus
	aaArcheryMastery						=199,//implemented as bonus
	aaFletchingMastery						=202,//removed from db?
	aaEndlessQuiver							=205,//implemented as bonus
	aaUnholySteed							=206,//DB
	aaImprovedHarmTouch						=207,//untested
	aaLeechTouch							=208,//DB
	aaDeathPeace							=209,
	aaSoulAbrasion							=210,//implemented as bonus-focus
	aaInstrumentMastery						=213,//untested
	aaUnknown91								=216,//not used
	aaUnknown92								=219,//not used
	aaUnknown93								=222,//not used
	aaJamFest								=225,//implemented as bonus
	aaUnknown95								=228,
	aaSonicCall								=229,
	aaCriticalMend							=230,//untested
	aaPurifyBody							=233,//DB
	aaChainCombo							=234,
	aaRapidFeign							=237,//works
	aaReturnKick							=240,//implemented as bonus
	aaEscape								=243,//DB
	aaPoisonMastery							=244,
	aaDoubleRiposte							=247,//implemented as bonus
	aaQuickHide								=250,
	aaQuickThrow							=253,//corrected from dbstr_us.txt
	aaPurgePoison							=254,//DB
	aaFlurry								=255,//implemented as bonus
	aaRampage								=258,//untested
	aaAreaTaunt								=259,//untested
	aaWarcry								=260,//DB
	aaBandageWound							=263,//implemented as bonus
	aaSpellCastingReinforcementMastery		=266,//implemented
	aaSpellCastingFuryMastery				=267,//untested
	aaExtendedNotes							=270,//implemented as bonus
	aaDragonPunch							=273,//implemented
	aaStrongRoot							=274,//DB
	aaSingingMastery						=275,//untested
	aaBodyAndMindRejuvenation				=278,//added
	aaPhysicalEnhancement					=279,//implemented as bonus
	aaAdvTrapNegotiation					=280,//untested
	aaAcrobatics							=283,//untested
	aaScribbleNotes							=286,
	aaChaoticStab							=287,//implemented as bonus
	aaPetDiscipline							=288,//added
	aaHobbleofSpirits						=289,//DB
	aaFrenzyofSpirit						=290,//DB
	aaParagonofSpirit						=291,//DB
	aaAdvancedInnateStrength				=292,//implemented as bonus
	aaAdvancedInnateStamina					=302,//implemented as bonus
	aaAdvancedInnateAgility					=312,//implemented as bonus
	aaAdvancedInnateDexterity				=322,//implemented as bonus
	aaAdvancedInnateIntelligence			=332,//implemented as bonus
	aaAdvancedInnateWisdom					=342,//implemented as bonus
	aaAdvancedInnateCharisma				=352,//implemented as bonus
	aaWardingofSolusek						=362,//implemented as bonus
	aaBlessingofEci							=372,//implemented as bonus
	aaMarrsProtection						=382,//implemented as bonus
	aaShroudofTheFaceless					=392,//implemented as bonus
	aaBertoxxulousGift						=402,//implemented as bonus
	aaNewTanaanCraftingMastery				=412,
	aaPlanarPower							=418,//untested
	aaPlanarDurability						=423,//added
	aaInnateEnlightenment					=426,//added
	aaAdvancedSpellCastingMastery			=431,//untested
	aaAdvancedHealingAdept					=434,//untested
	aaAdvancedHealingGift					=437,//untested
	aaCoupdeGrace							=440,//added
	aaFuryoftheAges							=443,//implemented as bonus
	aaMasteryofthePast						=446,//implemented as bonus
	aaLightningReflexes						=449,//implemented as bonus
	aaInnateDefense							=454,//implemented as bonus
	aaRadiantCure							=459,//DB
	aaHastenedDivinity						=462,//DB
	aaHastenedTurning						=465,//DB
	aaHastenedPurificationofSoul			=468,//DB
	aaHastenedGathering						=471,//DB
	aaHastenedRabidity						=474,//DB
	aaHastenedExodus						=477,//DB
	aaHastenedRoot							=480,//DB
	aaHastenedMending						=483,//DB
	aaHastenedBanishment					=486,//DB
	aaHastenedInstigation					=489,//DB, maybe
	aaFuriousRampage						=492,//DB
	aaHastenedPurificationoftheBody			=495,//DB
	aaHastyExit								=498,//DB
	aaHastenedPurification					=501,//DB
	aaFlashofSteel							=504,//implemented as bonus
	aaDivineArbitration						=507,//DB
	aaWrathoftheWild						=510,//DB
	aaVirulentParalysis						=513,//DB
	aaHarvestofDruzzil						=516,//DB
	aaEldritchRune							=517,//DB
	aaServantofRo							=520,//DB
	aaWaketheDead							=523,//DB
	aaSuspendedMinion						=526,//untested
	aaSpiritCall							=528,//DB
	aaCelestialRenewal						=531,//DB
	aaAllegiantFamiliar						=533,
	aaHandofPiety							=534,//DB
	aaMithanielsBinding						=537,//implemented as bonus
	aaMendingoftheTranquil					=539,
	aaRagingFlurry							=542,//implemented as bonus
	aaGuardianoftheForest					=545,//DB
	aaSpiritoftheWood						=548,//DB
	aaBestialFrenzy							=551,//implemented as bonus
	aaHarmoniousAttack						=556,//implemented as bonus
	aaKnightsAdvantage						=561,//implemented as bonus
	aaFerocity								=564,//implemented as bonus
	aaViscidRoots							=567,
	aaSionachiesCrescendo					=568,//implemented as bonus
	aaAyonaesTutelage						=571,
	aaFeignedMinion							=574,
	aaUnfailingDivinity						=577,
	aaAnimationEmpathy						=580,// Implemented
	aaRushtoJudgement						=583,
	aaLivingShield							=586,
	aaConsumptionoftheSoul					=589,//untested
	aaBoastfulBellow						=592,//DB
	aaFervrentBlessing						=593,//untested
	aaTouchoftheWicked						=596,//untested
	aaPunishingBlade						=599,//implemented as bonus
	aaSpeedoftheKnight						=602,//implemented as bonus
	aaShroudofStealth						=605,
	aaNimbleEvasion							=606,
	aaTechniqueofMasterWu					=611,
	aaHostoftheElements						=616,//DB
	aaCallofXuzl							=619,//DB
	aaHastenedStealth						=622,
	aaIngenuity								=625,
	aaFleetofFoot							=628,//implemented as bonus
	aaFadingMemories						=630,
	aaTacticalMastery						=631,//implemented as bonus
	aaTheftofLife							=634,//implemented as bonus-focus
	aaFuryofMagic							=637,
	aaFuryofMagicMastery2					=640,//whats the difference?
	aaProjectIllusion						=643,
	aaHeadshot								=644,//added
	aaEntrap								=645,//DB
	aaUnholyTouch							=646,//untested
	aaTotalDomination						=649,// Implemented
	aaStalwartEndurance						=652,//implemented as bonus
	aaQuickSummoning2						=655,//*not implemented - Liva AA that replaces prior version in later exp.
	aaMentalClarity2						=658,//whats the difference?
	aaInnateRegeneration2					=661,//whats the difference?
	aaManaBurn2								=664,//whats the difference?
	aaExtendedNotes2						=665,//*not implemented - Live AA that replaces prior version in later exp.
	aaSionachiesCrescendo2					=668,//*not implemented - Live AA that replaces prior version in later exp.
	aaImprovedReclaimEnergy2				=671,//whats the difference? untetsed
	aaSwiftJourney							=672,//implemented as bonus
	aaConvalescence							=674,//added 9/26/08
	aaLastingBreath							=676,//handled by client
	aaPackrat								=678,//added 9/29/08
	aaHeightenedEndurance					=683,
	aaWeaponAffinity						=686,//implemented as bonus
	aaSecondaryForte						=691,
	aaPersistantCasting						=692,
	aaTuneofPursuance						=695,
	aaImprovedInstrumentMastery				=700,
	aaImprovedSingingMastery				=701,
	aaExultantBellowing						=702,
	aaEchoofTaelosia						=707,
	aaInternalMetronome						=710,//implemented as bonus *Live AA removed in 2006
	aaPiousSupplication						=715,
	aaBeastialAlignment						=718,//untested
	aaWrathofXuzl							=721,
	aaFeralSwipe							=723,//DB?
	aaWardersFury							=724,//implemented as bonus
	aaWardersAlacrity						=729,//implemented as bonus
	aaPetAffinity							=734,//implemented as bonus
	aaMasteryofthePast2						=735,//implemented as bonus [Different classes]
	aaSpellCastingSubtlety2					=738,//whats the difference?
	aaTouchoftheDivine						=741,
	aaDivineAvatar							=746,//DB
	aaExquisiteBenediction					=749,//DB
	aaQuickenedCuring						=754,
	aaNaturesBoon							=757,//DB
	aaAdvancedTracking						=762,
	aaCriticalAffliction					=767,
	aaFuryofMagicMastery					=770,//whats the difference?
	aaDoppelganger							=773,
	aaEnchancedForgetfulness				=776,
	aaMesmerizationMastery					=781,
	aaQuickMassGroupBuff					=782,
	aaSharedHealth							=785,
	aaElementalFury							=790,//implemented as bonus
	aaElementalAlacrity						=795,//implemented as bonus
	aaElementalAgility						=800,//implemented as bonus
	aaElementalDurability					=803,//implemented as bonus
	aaSinisterStrikes						=806,//implemented as bonus
	aaStrikethrough							=807,//implemented as bonus
	aaStonewall								=810,
	aaRapidStrikes							=815,//implemented as bonus
	aaKickMastery							=820,//implemented as bonus
	aaHightenedAwareness					=823,
	aaDestructiveForce						=828,//DB
	aaSwarmofDecay							=831,//DB
	aaDeathsFury							=834,
	aaQuickeningofDeath						=839,//implemented as bonus
	aaAdvancedTheftofLife					=844,//implemented as bonus-focus
	aaTripleBackstab						=846,// removed; GoD AA
	aaHastenedPiety							=849,
	aaImmobilizingBash						=852,
	aaViciousSmash							=855,//implemented as bonus
	aaRadiantCure2							=860,//whats the difference?
	aaPurification							=863,
	aaPrecisionofthePathfinder				=864,//implemented as bonus
	aaCoatofThistles						=867,
	aaFlamingArrows							=872,//untested
	aaFrostArrows							=875,//untested
	aaSeizedOpportunity						=878,
	aaTrapCircumvention						=881,
	aaImprovedHastyExit						=886,
	aaVirulentVenom							=888,
	aaImprovedConsumptionofSoul				=893,
	aaIntenseHatred							=895,
	aaAdvancedSpiritCall					=900,
	aaCalloftheAncients						=902,//DB
	aaSturdiness							=907,
	aaWarlordsTenacity						=912,//implemented as effect
	aaStrengthenedStrike					=915,//implemented as bonus
	aaExtendedShielding						=918,
	aaRosFlamingFamiliar					=921,//DB
	aaEcisIcyFamiliar						=922,//DB
	aaDruzzilsMysticalFamiliar				=923,//DB
	aaAdvancedFuryofMagicMastery			=924,//added 9/29/08
	aaWardofDestruction						=926,//DB
	aaFrenziedDevastation					=931,//DB
	aaCombatFury2							=934,//implemented as bonus
	aaCombatFury3							=937,//implemented as bonus
	aaCombatFury4							=940,//implemented as bonus
	aaFuryoftheAges2						=943,//implemented as bonus
	aaFuryoftheAges3						=946,//implemented as bonus
	aaFuryoftheAges4						=949,//implemented as bonus
	aaPlanarDurability2						=952,//whats the difference?
	aaInnateEnlightenment2					=955,//whats the difference?
	aaDireCharm2							=960,//whats the difference?
	aaDireCharm3							=961,//whats the difference?
	aaTouchoftheDivine2						=962,//whats the difference?
	aaTouchofDecay							=967,
	aaCalloftheAncients2					=970,//whats the difference?
	aaImprovedVision						=975,
	aaEternalBreath							=978,//handled by client
	aaBlacksmithingMastery					=979,//added 9/29/08
	aaBakingMastery							=982,//added 9/29/08
	aaBrewingMastery						=985,//added 9/29/08
	aaFletchingMastery2						=988,//added 9/29/08
	aaPotteryMastery						=991,//added 9/29/08
	aaTailoringMastery						=994,//added 9/29/08
	aaOrigin								=1000,//spell
	aaChaoticPotential						=1001,//added
	aaDiscordantDefiance					=1006,//implemented as bonus
	aaTrialsofMataMuram						=1011,
	aaMysticalAttuning						=1021,
	aaDelayDeath							=1026,//implemented as bonus
	aaHealthyAura							=1031,
	aaFitness								=1036,//implemented as bonus
	aaVeteransWrath							=1041,//implemented as bonus [Different classes/values on each Veteran's Wrath]
	aaVeteransWrath2						=1044,//implemented as bonus

	aaHighestID		//this should always be last, and should always
					//follow the highest AA ID
} aaID;


//Structure representing the database's AA actions
struct AA_DBAction {
	uint32 reuse_time;			//in seconds
	uint16 spell_id;				//spell to cast, SPELL_UNKNOWN=no spell
	aaTargetType target;		//from aaTargetType
	aaNonspellAction action;	//non-spell action to take
	uint16 mana_cost;			//mana the NON-SPELL action costs
	uint16 duration;			//duration of NON-SPELL effect, 0=N/A
	aaID redux_aa;				//AA which reduces reuse time
	int32 redux_rate;			//%/point in redux_aa reduction in reuse time
	aaID redux_aa2;				//AA which reduces reuse time
	int32 redux_rate2;			//%/point in redux_aa reduction in reuse time
};

//Structure representing the database's swarm pet configs
struct SwarmPet_Struct {
	uint8 count;		//number to summon
	uint32 npc_id;		//id from npc_types to represent it.
	uint16 duration;		//how long they last, in seconds
};

class TemporaryPetsEffect
{
public:
	TemporaryPetsEffect() : next_spawn_timer() {}

	int pet_count;
	int pet_count_remaining;
	int pet_duration_seconds;
	uint16 pet_target_id;
	bool followme;
	bool sticktarg;
	NPCType npc_type;
	Timer next_spawn_timer;

};

//assumes that no activatable aa.has more than 5 ranks
#define MAX_AA_ACTION_RANKS 20
extern AA_DBAction AA_Actions[aaHighestID][MAX_AA_ACTION_RANKS];	//[aaid][rank]

#define AA_Choose3(val, v1, v2, v3) (val==1?v1:(val==2?v2:v3))

extern std::map<uint32,SendAA_Struct*>aas_send;
extern std::map<uint32, std::map<uint32, AA_Ability> > aa_effects;

enum {	//values of AA_Action.action
	aaActionActivate = 0,
	aaActionSetEXP = 1,
	aaActionDisableEXP = 2,
	aaActionBuy = 3
};

class Timer;
class Mob;
class SwarmPet {
public:
	SwarmPet();
	~SwarmPet();
	Mob * GetOwner();
	Timer *duration;
	uint32 target; //the target ID
	uint32 owner_id;
};

#endif
