


#ifndef RULE_CATEGORY
#define RULE_CATEGORY(name)
#endif
#ifndef RULE_INT
#define RULE_INT(cat, rule, default_value)
#endif
#ifndef RULE_REAL
#define RULE_REAL(cat, rule, default_value)
#endif
#ifndef RULE_BOOL
#define RULE_BOOL(cat, rule, default_value)
#endif
#ifndef RULE_CATEGORY_END
#define RULE_CATEGORY_END()
#endif




RULE_CATEGORY(Character)
RULE_BOOL(Character, CanCreate, true)
RULE_INT ( Character, MaxLevel, 65 )
RULE_BOOL ( Character, PerCharacterQglobalMaxLevel, false) // This will check for qglobal 'CharMaxLevel' character qglobal (Type 5), if player tries to level beyond that point, it will not go beyond that level
RULE_INT ( Character, MaxExpLevel, 0 ) //Sets the Max Level attainable via Experience
RULE_INT ( Character, DeathExpLossLevel, 10 )	// Any level greater than this will lose exp on death
RULE_INT ( Character, DeathExpLossMaxLevel, 255 )	// Any level greater than this will no longer lose exp on death
RULE_INT ( Character, DeathItemLossLevel, 10 )
RULE_INT ( Character, CorpseDecayTimeMS, 604800000 ) // 7 days
RULE_INT ( Character, EmptyCorpseDecayTimeMS, 10800000 ) // 3 hours
RULE_INT ( Character, CorpseResTimeMS, 10800000 ) // time before cant res corpse(3 hours)
RULE_INT ( Character, DuelCorpseResTimeMS, 600000 ) // time before cant res corpse after a duel (10 minutes)
RULE_INT ( Character, CorpseOwnerOnlineTimeMS, 30000 ) // how often corpse will check if its owner is online
RULE_BOOL( Character, LeaveCorpses, true )
RULE_BOOL( Character, LeaveNakedCorpses, true )
RULE_INT ( Character, MaxDraggedCorpses, 2 )
RULE_REAL( Character, DragCorpseDistance, 400) // If the corpse is <= this distance from the player, it won't move
RULE_REAL( Character, ExpMultiplier, 1.0 )
RULE_REAL( Character, AAExpMultiplier, 1.0 )
RULE_REAL( Character, GroupExpMultiplier, 1.0 )
RULE_REAL( Character, RaidExpMultiplier, 0.6 )	// showeq forum says raid exp was 60%
RULE_REAL ( Character, EXPLossMultiplier, 1.0)
RULE_INT ( Character, AutosaveIntervalS, 240 )	//0=disabled
RULE_INT ( Character, ConsumptionValue, 6000) //How "full" each consumption of food or drink will make the player. EQEmu default is 6000.
RULE_REAL ( Character, FoodLossPerUpdate, 75) // How much food/water you lose per stamina update
RULE_INT ( Character, FamishedLevel, 120) // When famished reaches this value, we lose endurance and stop regen on HP/mana
RULE_BOOL( Character, HealOnLevel, false)
RULE_BOOL( Character, ManaOnLevel, false)
RULE_BOOL( Character, FeignKillsPet, false)
RULE_INT(Character, ItemManaRegenCap, 15)
RULE_INT(Character, ItemATKCap, 250)
RULE_INT ( Character, ItemEnduranceRegenCap, 15)
RULE_INT ( Character, HasteCap, 100) // Haste cap for non-v3(overhaste) haste.
RULE_BOOL ( Character, BindAnywhere, false)
RULE_INT ( Character, MaxFearDurationForPlayerCharacter, 4) //4 tics, each tic calculates every 6 seconds.
RULE_INT ( Character, MaxCharmDurationForPlayerCharacter, 21)
RULE_INT ( Character, BaseHPRegenBonusRaces, 4352)	//a bitmask of race(s) that receive the regen bonus. Iksar (4096) & Troll (256) = 4352. see common/races.h for the bitmask values
RULE_BOOL ( Character, ItemCastsUseFocus, false) // If true, this allows item clickies to use focuses that have limited max levels on them
RULE_INT ( Character, MinStatusForNoDropExemptions, 80) // This allows status x and higher to trade no drop items.
RULE_INT ( Character, SkillCapMaxLevel, 65 )	// Sets the Max Level used for Skill Caps (from skill_caps table). -1 makes it use MaxLevel rule value. It is set to 75 because PEQ only has skillcaps up to that level, and grabbing the players' skill past 75 will return 0, breaking all skills past that level. This helps servers with obsurd level caps (75+ level cap) function without any modifications.
RULE_INT ( Character, StatCap, 0 )
RULE_BOOL ( Character, CheckCursorEmptyWhenLooting, true ) // If true, a player cannot loot a corpse (player or NPC) with an item on their cursor
RULE_BOOL ( Character, MaintainIntoxicationAcrossZones, true ) // If true, alcohol effects are maintained across zoning and logging out/in.
RULE_BOOL ( Character, EnableDiscoveredItems, false ) // If enabled, it enables EVENT_DISCOVER_ITEM and also saves character names and timestamps for the first time an item is discovered.
RULE_BOOL ( Character, KeepLevelOverMax, false) // Don't delevel a character that has somehow gone over the level cap
RULE_INT ( Character, BaseInstrumentSoftCap, 36) // Softcap for instrument mods, 36 commonly referred to as "3.6" as well.
RULE_REAL (Character, BaseRunSpeed, 0.7)
RULE_REAL(Character, EnvironmentDamageMulipliter, 1)
RULE_BOOL(Character, ForageNeedFoodorDrink, false)
RULE_BOOL (Character, DisableAAs, false) // Disables server side AA support, since the client allows some AA activity through even with a pre-Luclin expansion set.
RULE_BOOL ( Character, SacrificeCorpseDepop, false) // If true, Sacrificed corpses will depop 3 minutes after they become empty in Pok, Nexus, or Bazaar
RULE_INT ( Character, DefaultExpansions, 15) // When a new account is created, this is the default expansions it is given. 1 Kunark 2 Velious 4 Luclin 8 PoP.
RULE_CATEGORY_END()

RULE_CATEGORY( Guild )
RULE_INT ( Guild, MaxGuilds, 32)
RULE_CATEGORY_END()

RULE_CATEGORY( Skills )
RULE_INT ( Skills, MaxTrainTradeskills, 21 )
RULE_INT ( Skills, MaxTrainSpecializations, 50 )	// Max level a GM trainer will train casting specializations
RULE_INT ( Skills, LangSkillUpModifier, 70) //skill ups for skills with value under 100
RULE_REAL ( Skills, SkillUpModifier, 1.0) // combat skill-up rate multiplier.  1.0 = accurate skill-up rate for AK/old EQ
RULE_CATEGORY_END()

RULE_CATEGORY( Pets )
RULE_REAL( Pets, AttackCommandRange, 200 )
RULE_BOOL( Pets, UnTargetableSwarmPet, false )
RULE_CATEGORY_END()

RULE_CATEGORY(GM)
RULE_INT(GM, GMWhoList, 80)
RULE_INT ( GM, MinStatusToUseGMItem, 80)
RULE_INT ( GM, MinStatusToZoneAnywhere, 250 )
RULE_INT(GM, MinStatusToLevelTarget, 100)
RULE_CATEGORY_END()

RULE_CATEGORY( World )
RULE_INT ( World, ZoneAutobootTimeoutMS, 60000 )
RULE_INT ( World, ClientKeepaliveTimeoutMS, 65000 )
RULE_BOOL ( World, UseBannedIPsTable, false ) // Toggle whether or not to check incoming client connections against the Banned_IPs table. Set this value to false to disable this feature.
RULE_INT ( World, MaxClientsPerIP, -1 ) // Maximum number of clients allowed to connect per IP address if account status is < AddMaxClientsStatus. Default value: -1 (feature disabled)
RULE_INT( World, MaxClientsPerForumName, -1 ) // Maximum number of clients allowed to connect per forum name if account status is < AddMaxClientsStatus. Default value: -1 (feature disabled)
RULE_INT ( World, ExemptMaxClientsStatus, -1 ) // Exempt accounts from the MaxClientsPerIP and AddMaxClientsStatus rules, if their status is >= this value. Default value: -1 (feature disabled)
RULE_INT ( World, AddMaxClientsPerIP, -1 ) // Maximum number of clients allowed to connect per IP address if account status is < ExemptMaxClientsStatus. Default value: -1 (feature disabled)
RULE_INT ( World, AddMaxClientsStatus, -1 ) // Accounts with status >= this rule will be allowed to use the amount of accounts defined in the AddMaxClientsPerIP. Default value: -1 (feature disabled)
RULE_BOOL ( World, MaxClientsSetByStatus, false) // If True, IP Limiting will be set to the status on the account as long as the status is > MaxClientsPerIP
RULE_BOOL ( World, ClearTempMerchantlist, false) // Clears temp merchant items when world boots.
RULE_INT ( World, AccountSessionLimit, 1 ) //Max number of characters allowed on at once from a single account (-1 is disabled)
RULE_INT ( World, ExemptAccountLimitStatus, 100 ) //Min status required to be exempt from multi-session per account limiting (-1 is disabled)
RULE_BOOL ( World, GMAccountIPList, false) // Check ip list against GM Accounts, AntiHack GM Accounts.
RULE_INT ( World, MinGMAntiHackStatus, 1 ) //Minimum GM status to check against AntiHack list
RULE_INT ( World, PVPSettings, 0) // Sets the PVP settings for the server, 1 = Rallos Zek RuleSet, 2 = Tallon/Vallon Zek Ruleset, 4 = Sullon Zek Ruleset, 6 = Discord Ruleset, anything above 6 is the Discord Ruleset without the no-drop restrictions removed. TODO: Edit IsAttackAllowed in Zone to accomodate for these rules.
RULE_INT (World, FVNoDropFlag, 0) // Sets the Firiona Vie settings on the client. If set to 2, the flag will be set for GMs only, allowing trading of no-drop items.
RULE_BOOL (World, IPLimitDisconnectAll, false)
RULE_INT (World, TellQueueSize, 200) 
RULE_BOOL (World, AdjustRespawnTimes, false) //Determines if spawntimes with a boot time variable take effect or not. Set to false in the db for emergency patches.
RULE_INT (World, BootHour, 0) // Sets the in-game hour world will set when it first boots. 0-24 are valid options, where 0 disables this rule.
RULE_INT(World, StreamDataRate, 50) // Sets the datarate for EQOldStream. Defaults to 50.
RULE_INT(World, WhoListLimit, 20) //The max players returned in /who all.
RULE_INT(World, MuleToonLimit, 8) // The number of characters a mule account can create/access.
RULE_BOOL( World, DontBootDynamics, false) // If true, dynamic zones will not boot when a player tries to enter them.
RULE_REAL( World, CurrentExpansion, 15.0)
RULE_CATEGORY_END()

RULE_CATEGORY( Zone )
RULE_INT ( Zone, ClientLinkdeadMS, 180000) //the time a client remains link dead on the server after a sudden disconnection
RULE_BOOL ( Zone, EnableShadowrest, true ) // enables or disables the shadowrest zone feature for player corpses. Default is turned on.
RULE_BOOL ( Zone, UsePlayerCorpseBackups, true) // Keeps backups of player corpses.
RULE_INT ( Zone, MQWarpExemptStatus, -1 ) // Required status level to exempt the MQWarpDetector. Set to -1 to disable this feature.
RULE_INT ( Zone, MQZoneExemptStatus, -1 ) // Required status level to exempt the MQZoneDetector. Set to -1 to disable this feature.
RULE_INT ( Zone, MQGateExemptStatus, -1 ) // Required status level to exempt the MQGateDetector. Set to -1 to disable this feature.
RULE_INT ( Zone, MQGhostExemptStatus, -1 ) // Required status level to exempt the MGhostDetector. Set to -1 to disable this feature.
RULE_BOOL ( Zone, EnableMQWarpDetector, false ) // Enable the MQWarp Detector. Set to False to disable this feature.
RULE_BOOL ( Zone, EnableMQZoneDetector, true ) // Enable the MQZone Detector. Set to False to disable this feature.
RULE_BOOL ( Zone, EnableMQGateDetector, true ) // Enable the MQGate Detector. Set to False to disable this feature.
RULE_BOOL ( Zone, EnableMQGhostDetector, true ) // Enable the MQGhost Detector. Set to False to disable this feature.
RULE_REAL ( Zone, MQWarpDetectionDistanceFactor, 9.0) //clients move at 4.4 about if in a straight line but with movement and to acct for lag we raise it a bit
RULE_BOOL ( Zone, MarkMQWarpLT, false )
RULE_INT ( Zone, AutoShutdownDelay, 5000 ) //How long a dynamic zone stays loaded while empty.  if the zone database field is longer then that is used instead
RULE_INT ( Zone, PEQZoneReuseTime, 900 )	//How long, in seconds, until you can reuse the #peqzone command.
RULE_INT ( Zone, PEQZoneDebuff1, 4454 )		//First debuff casted by #peqzone Default is Cursed Keeper's Blight.
RULE_INT ( Zone, PEQZoneDebuff2, 2209 )		//Second debuff casted by #peqzone Default is Tendrils of Apathy.
RULE_BOOL ( Zone, UsePEQZoneDebuffs, true )	//Will determine if #peqzone will debuff players or not when used.
RULE_BOOL ( Zone, LevelBasedEXPMods, true) // Allows you to use the level_exp_mods table in consideration to your players EXP hits.  This is used for Hell Level Balance Modifiers used for Luclin era hell level smoothing.  Make all the rows 1.0 for Velious era exp
RULE_INT ( Zone, WeatherTimer, 600) // Weather timer when no duration is available
RULE_INT (Zone, SpawnEventMin, 5) // When strict is set in spawn_events, specifies the max EQ minutes into the trigger hour a spawn_event will fire.
RULE_REAL ( Zone, GroupEXPRange, 500 )
RULE_BOOL ( Zone, IdleWhenEmpty, true) // After timer is expired, if zone is empty it will idle. Boat zones are excluded, as this will break boat functionality.
RULE_INT ( Zone, IdleTimer, 600000) // 10 minutes
RULE_INT ( Zone, BoatDistance, 50) //In zones where boat name is not set in the PP, this is how far away from the boat the client must be to move them to the boat's current location.
RULE_BOOL ( Zone, EnableNexusPortals, true)
RULE_INT ( Zone, NexusTimer, 900000) //Nexus timer in ms. Defaults to 15 minutes.
RULE_INT ( Zone, NexusScionTimer, 900000) //Nexus timer in ms. Defaults to 15 minutes.
RULE_CATEGORY_END()

RULE_CATEGORY( AlKabor )
RULE_BOOL( AlKabor, AllowPetPull, false) // Allow Green Pet Pull (AK behavior is true)
RULE_BOOL( AlKabor, AllowTickSplit, false) //AK behavior is true
RULE_BOOL ( AlKabor, StripBuffsOnLowHP, true) //AK behavior is true
RULE_BOOL ( AlKabor, OutOfRangeGroupXPBonus, false) //AK behavior is true. When true, players out of range of the kill will still count towards the group bonus. (They will not receive XP.)  This enables the exploit that allowed a soloing player to recieve the entire 2.6x group bonus
RULE_BOOL ( AlKabor, VeliousGroupEXPBonuses, false) // AK behavior is false.  If true, use the Velious to mid PoP era group exp bonus. (2% for 2 members, up to 20% for 6 members)  This was the case from Jan 2001 to June 2003 on PC.  (prior to that was a 6 man bonus of 10%)  False will use the post June 2003 bonus of 20% per aditional member up to 80%
RULE_BOOL ( AlKabor, GroupEXPBonuses, false) //AK behavior is true. When true, the "broken" 4-6 member group bonuses will be used.  Note: ClassicGroupEXPBonuses must be false for this to work
RULE_BOOL ( AlKabor, Count6thGroupMember, true) //AK behavior is true. When true, the 6th member of the group will count towards the split, thus reducing the XP everybody gets.  Note: this should be false if using the post June 2003 PC era exp rules is intended
RULE_BOOL ( AlKabor, GreensGiveXPToGroup, true) //AK behavior is true. When true, lower level players will receive group XP when a green mob to the higher players is killed. 
RULE_BOOL (AlKabor, GreenExpBonus, true) // AK is (supposedly) true.  When true, any group member that the mob is green to will have their weighted division split reduced to that of the highest level player who does get exp, resulting in a larger share to those who get exp from the mob.  This results in higher level players being able to powerlevel even more effectively.  no hard evidence is known of this however
RULE_BOOL( AlKabor, AllowCharmPetRaidTanks, true) // AK behavior is true.  If false, NPCs will ignore charmed pets once MaxEntitiesCharmTanks players get on an NPC's hate list as per April 2003 patch.
RULE_INT( AlKabor, MaxEntitiesCharmTanks, 8) // If AllowCharmPetRaidTanks is false, this is the max number of entities on an NPC's hate list before the NPC will ignore charmed pets.  April 2003 patch set this to 4 on Live.
RULE_BOOL ( AlKabor, AllowPriceIncWhileBrowsing, true) // AK behavior is true. If true, this allows Bazaar traders to increase the price of an item while another player is browsing their wares.
RULE_INT ( AlKabor, LevelCorpsesAlwaysSpawn, 55) // AK behavior is 55. The level NPC corpses will not poof even if a NPC was top hate/final blow.
RULE_BOOL( AlKabor, NPCsSendHPUpdatesPerTic, false) // AK behavior is true. NPCs will only send HP updates every tic or when targeted instead of real time.
RULE_BOOL( AlKabor, NoMaxWhoGuild, true) // AK behavior is false. If true, /who all guild# will return without a limit.
RULE_BOOL( AlKabor, ServerExpBonus, true) // AK behavior is true.  This grants a multiplicative 20% experience bonus that was unique to AK
RULE_REAL( AlKabor, LightBlueExpMod, 100.0) // Make sure they are all 100.0 for accurate experience gains.  Exp scaling by level is handled in the exp routines
RULE_REAL( AlKabor, BlueExpMod, 100.0) 
RULE_REAL( AlKabor, WhiteExpMod, 100.0)
RULE_REAL( AlKabor, YellowExpMod, 100.0) 
RULE_REAL( AlKabor, RedExpMod, 100.0) 
RULE_BOOL (AlKabor, RememberAir, true) //AK behavior is true. //If zoning from one underwater area to another, remember air_remaining value. If false, it's set to 100.
RULE_BOOL (AlKabor, ClickyHateExploit, false) // AK behavior is true. When true, it allows Invis Vs Animals clicky items to generate massive hate. 
RULE_BOOL (AlKabor, InvulnHateReduction, false) // DA spells seemed to have reduced hate on AK by an unknown amount; if true this will halve the hate
RULE_BOOL (AlKabor, ReduceAEExp, true) // AK behavior is true.  Reduce the amount of experience gained when NPC is killed with a PBAoE spell.  Applies to NPCs around level 35 to 55
RULE_BOOL (AlKabor, RaceEffectsAASplit, true) // AK behavior is true.  If true then race exp penalties (and bonus in case of halfling) will modify AA Exp ONLY when AA Exp is under 100%
RULE_BOOL (AlKabor, NoDropRemoveTradeskill, true) // AK behavior is true.  If true then no drop items will be delete if container is closed.  If false, it will not delete for the original player only.
RULE_BOOL (AlKabor, ReducedMonkAC, true) // AK behavior is true.  Monks had a low AC softcap from October 16 2002 to April 8 2003 which made them squishy.  Sony partially unnerfed them in April 03.
RULE_BOOL (AlKabor, BlockProjectileCorners, true) // AK behavior is true.  If an NPC was in a corner, arrows and bolts would not hit them.
RULE_BOOL (AlKabor, BlockProjectileWalls, true) // AK behavior is true.  If an NPC was walled, then arrows and bolts had to be fired from an angle parallel to the wall in order to hit them. (if this is true, corners will also block)
RULE_CATEGORY_END()


RULE_CATEGORY( Map )
//enable these to help prevent mob hopping when they are pathing
RULE_BOOL ( Map, FixPathingZWhenLoading, true )		//increases zone boot times a bit to reduce hopping.
RULE_BOOL ( Map, FixZWhenPathing, true )		//very CPU intensive, but helps hopping with widely spaced waypoints.
RULE_BOOL ( Map, FixPathingZOnSendTo, false )		//try to repair Z coords in the SendTo routine as well.
RULE_REAL ( Map, FixPathingZMaxDeltaSendTo, 20 )	//at runtime in SendTo: max change in Z to allow the BestZ code to apply.
RULE_REAL ( Map, FixPathingZMaxDeltaLoading, 200 )	//while loading each waypoint: max change in Z to allow the BestZ code to apply.
RULE_INT ( Map, FindBestZHeightAdjust, 1)		// Adds this to the current Z before seeking the best Z position. If this is too high, mobs bounce when pathing.
RULE_REAL ( Map, BestZSizeMax, 20.0) // When calculating bestz using size, this is our size cap. Setting this too high causes dragons and giants to hop.
RULE_CATEGORY_END()

RULE_CATEGORY( Pathing )
RULE_BOOL ( Pathing, Fear, true )		// Enable pathing for fear
RULE_INT ( Pathing, RouteUpdateFrequencyNodeCount, 5)
RULE_INT ( Pathing, MinNodesTraversedForLOSCheck, 3)	// Only check for LOS after we have traversed this many path nodes.
RULE_REAL ( Pathing, CandidateNodeRangeXY, 200)		// When searching for path start/end nodes, only nodes within this range will be considered.
RULE_REAL ( Pathing, CandidateNodeRangeZ, 25)		// When searching for path start/end nodes, only nodes within this range will be considered.
RULE_INT(Pathing, MaxNavmeshNodes, 8184) // Maximum navmesh nodes in a traversable path
RULE_REAL(Pathing, NavmeshStepSize, 100.0f) // Step size for the movement manager
RULE_REAL(Pathing, ShortMovementUpdateRange, 130.0f) // Range for short movement updates
RULE_CATEGORY_END()

RULE_CATEGORY( Watermap )
// enable these to use the water detection code. Requires Water Maps generated by awater utility
// WARNING: Bestz in water is the ocean floor, so if you have NPCs that float near the top (sharks, boats) disabling these rules may break them!
RULE_BOOL ( Watermap, CheckWaypointsInWaterWhenLoading, true ) // Does not apply BestZ as waypoints are loaded if they are in water
RULE_BOOL ( Watermap, CheckForWaterOnSendTo, true)		// Checks if a mob has moved into/out of water on SendTo
RULE_BOOL ( Watermap, CheckForWaterWhenFishing, true)		// Only lets a player fish near water (if a water map exists for the zone)
RULE_REAL ( Watermap, FishingRodLength, 30)			// How far in front of player water must be for fishing to work
RULE_REAL ( Watermap, FishingLineLength, 28)			// If water is more than this far below the player, it is considered too far to fish
RULE_REAL ( Watermap, FishingLineExtension, 12)		// In some zones, setting a longer length causes the line to go underworld. This gives us a variable to work with in areas that need a longer line.
RULE_CATEGORY_END()

RULE_CATEGORY( Spells )
RULE_REAL ( Spells, ResistChance, 2.0) //chance to resist given no resists and same level
RULE_INT ( Spells, WizCritLevel, 12) // level wizards first get spell crits
RULE_INT ( Spells, TranslocateTimeLimit, 0) // If not zero, time in seconds to accept a Translocate.
RULE_INT ( Spells, SacrificeMinLevel, 46)	//first level Sacrifice will work on
RULE_INT ( Spells, SacrificeMaxLevel, 69)	//last level Sacrifice will work on
RULE_INT ( Spells, SacrificeItemID, 9963)	//Item ID of the item Sacrifice will return (defaults to an EE)
RULE_BOOL ( Spells, EnableSpellGlobals, false)	// If Enabled, spells check the spell_globals table and compare character data from the quest globals before allowing that spell to scribe with scribespells
RULE_INT ( Spells, MaxBuffSlotsNPC, 30)
RULE_INT ( Spells, MaxTotalSlotsNPC, 30)
RULE_INT ( Spells, MaxTotalSlotsPET, 15)
RULE_INT ( Spells, ReflectType, 1) //0 = disabled, 1 = single target player spells only, 2 = all player spells, 3 = all single target spells, 4 = all spells
RULE_BOOL( Spells, LiveLikeFocusEffects, true) // Determines whether specific healing, dmg and mana reduction focuses are randomized
RULE_INT ( Spells, BaseImmunityLevel, 55) // The level that targets start to be immune to stun, fear and mez spells with a max level of 0.
RULE_INT ( Spells, ResistFalloff, 67) //Max that level that will adjust our resist chance based on level modifiers
RULE_INT ( Spells, CharmMinResist, 5) // When rolling charm tick save throws, this is the minimum value that resists can result to after debuffs and level advantage.  This essentially determines how good charm is
RULE_INT ( Spells, RootBreakFromSpells, 55) //Chance for root to break when cast on.
RULE_INT ( Spells, RootMinResist, 5) // When rolling root tick save throws, this is the minimum value that resists can result to after debuffs and level advantage.  This determines how good root is on low resist targets
RULE_BOOL ( Spells, AdditiveBonusValues, false) //Allow certain bonuses to be calculated by adding together the value from each item, instead of taking the highest value. (ie Add together all Cleave Effects)
RULE_BOOL ( Spells, BuffLevelRestrictions, true) //Buffs will not land on low level toons like live
RULE_INT ( Spells, RootBreakCheckChance, 75) //Determines chance for a root break check to occur each buff tick.
RULE_INT ( Spells, BlindBreakCheckChance, 75) // Determines chance for blind tick save throw
RULE_INT ( Spells, FearBreakCheckChance, 75) //Determines chance for a fear break check to occur each buff tick.
RULE_BOOL ( Spells, FocusCombatProcs, false) //Allow all combat procs to receive focus effects.
RULE_INT ( Spells, AI_SpellCastFinishedFailRecast, 800) // AI spell recast time(MS) when an spell is cast but fails (ie stunned).
RULE_INT ( Spells, AI_EngagedNoSpellMinRecast, 750) // AI spell recast time(MS) check when no spell is cast while engaged. (min time in random)
RULE_INT ( Spells, AI_EngagedNoSpellMaxRecast, 2000) // AI spell recast time(MS) check when no spell is cast engaged.(max time in random)
RULE_INT ( Spells, AI_EngagedBeneficialSelfChance, 100) // Chance during first AI Cast check to do a beneficial spell on self.
RULE_INT ( Spells, AI_EngagedBeneficialOtherChance, 25) // Chance during second AI Cast check to do a beneficial spell on others.
RULE_INT ( Spells, AI_EngagedDetrimentalChance, 25) // Chance during third AI Cast check for DPS classes do a determental spell on others.
RULE_INT ( Spells, AI_EngagedDetrimentalChanceHealer, 25) // Chance during third AI Cast check for healer classes to do a determental spell on others.
RULE_INT ( Spells, AI_EngagedDetrimentalChanceHybrid, 3) // Chance during third AI Cast check for hybrid classes to do a determental spell on others.
RULE_INT ( Spells, AI_IdleNoSpellMinRecast, 1500) // AI spell recast time(MS) check when no spell is cast while idle. (min time in random)
RULE_INT ( Spells, AI_IdleNoSpellMaxRecast, 6000) // AI spell recast time(MS) check when no spell is cast while idle. (max time in random)
RULE_INT ( Spells, AI_IdleBeneficialChance, 100) // Chance while idle to do a beneficial spell on self or others.
RULE_BOOL ( Spells, SHDProcIDOffByOne, true) // pre June 2009 SHD spell procs were off by 1, they stopped doing this in June 2009 (so UF+ spell files need this false)
RULE_BOOL ( Spells, SwarmPetTargetLock, false) // Use old method of swarm pets target locking till target dies then despawning.
RULE_INT ( Spells, SpellRecoveryTimer, 2500) // Begins when a cast is complete, and is checked after the next spell finishes casting. If not expired, the new spell is interrupted. Clickies are exempt.
RULE_BOOL ( Spells, JamFestAAOnlyAffectsBard, true) // Bard Jam Fest AA only worked on bards themselves but was changed after AK's era.  Changing this to false will put the client stats out of sync with the server.
RULE_CATEGORY_END()

RULE_CATEGORY( Combat )
RULE_INT ( Combat, RogueCritThrowingChance, 25) //Rogue throwing crit bonus
RULE_INT ( Combat, RogueDeadlyStrikeChance, 80) //Rogue chance throwing from behind crit becomes a deadly strike
RULE_INT ( Combat, RogueDeadlyStrikeMod, 2) //Deadly strike modifier to crit damage
RULE_INT ( Combat, ClientBaseCritChance, 0 ) //The base crit chance for all clients, this will stack with warrior's/zerker's crit chance.
RULE_BOOL ( Combat, EnableFearPathing, true)
RULE_INT ( Combat, FleeHPRatio, 20)	  //HP % under which an NPC starts to flee.
RULE_BOOL ( Combat, FleeIfNotAlone, false) // If false, mobs won't flee if other mobs are in combat with it.
RULE_REAL ( Combat, ArcheryHitPenalty, 0.25) //Archery has a hit penalty to try to help balance it with the plethora of long term +hit modifiers for it
RULE_INT ( Combat, MinRangedAttackDist, 25) //Minimum Distance to use Ranged Attacks
RULE_BOOL ( Combat, ArcheryBonusRequiresStationary, true) //does the 2x archery bonus chance require a stationary npc
RULE_REAL ( Combat, ArcheryBaseDamageBonus, 1) // % Modifier to Base Archery Damage (.5 = 50% base damage, 1 = 100%, 2 = 200%)
RULE_REAL ( Combat, ArcheryNPCMultiplier, 1.0) // this is multiplied by the regular dmg to get the archery dmg
RULE_BOOL ( Combat, AssistNoTargetSelf, true) //when assisting a target that does not have a target: true = target self, false = leave target as was before assist (false = live like)
RULE_INT ( Combat, MinHastedDelay, 400) // how fast we can get with haste.
RULE_INT ( Combat, NPCFlurryChance, 20) // Chance for NPC to flurry.
RULE_BOOL (Combat,TauntOverLevel, 1) //Allows you to taunt NPC's over warriors level.
RULE_INT ( Combat, QuiverWRHasteDiv, 4) //Weight Reduction is divided by this to get haste contribution for quivers
RULE_BOOL ( Combat, UseArcheryBonusRoll, false) //Make the 51+ archery bonus require an actual roll
RULE_INT ( Combat, ArcheryBonusChance, 50)
RULE_INT ( Combat, BerserkerFrenzyStart, 35)
RULE_INT ( Combat, BerserkerFrenzyEnd, 45)
RULE_BOOL ( Combat, UseDiscTimerGroups, false) // After about 2004 or so, disc reuse timers were combined into groups so multiple discs could be used in succession. 
RULE_INT ( Combat, PvPMeleeDmgPct, 100) // Percentage of melee damage done in PvP combat. 100 = 100% i.e. unmodified
RULE_INT ( Combat, PvPSpellDmgPct, 62) // Percentage of spell damage done in PvP combat. 100 = 100% i.e. unmodified
RULE_INT ( Combat, PvPArcheryDmgPct, 100) // Percentage of archery damage done in PvP combat. 100 = 100% i.e. unmodified
RULE_CATEGORY_END()

RULE_CATEGORY( NPC )
RULE_INT ( NPC, MinorNPCCorpseDecayTimeMS, 480000 ) //level<55
RULE_INT ( NPC, MajorNPCCorpseDecayTimeMS, 1800000 ) //level>=55
RULE_INT ( NPC, CorpseUnlockTimer, 120000 )
RULE_INT ( NPC, EmptyNPCCorpseDecayTimeMS, 30000 )
RULE_INT ( NPC, SayPauseTimeInSec, 70)
RULE_INT ( NPC, OOCRegen, 0)
RULE_BOOL ( NPC, BuffFriends, true )
RULE_INT ( NPC, LastFightingDelayMovingMin, 3000)
RULE_INT ( NPC, LastFightingDelayMovingMax, 26000)
RULE_BOOL ( NPC, ReturnNonQuestItems, true)	// Returns items on NPCs that don't have an EVENT_TRADE sub in their script or have KOS faction with the player
RULE_INT ( NPC, StartEnrageValue, 10) // % HP that an NPC will begin to enrage
RULE_BOOL ( NPC, LiveLikeEnrage, false) // If set to true then only player controlled pets will enrage
RULE_INT ( NPC, NPCTemplateID, 541) //MonsterSum1
RULE_BOOL ( NPC, BoatsRunByDefault, true) // Mainly to make it easier to adjust boats' timing on the fly.
RULE_BOOL(NPC, CheckSoWBuff, false)
RULE_BOOL( NPC, IgnoreQuestLoot, false)
RULE_CATEGORY_END()

RULE_CATEGORY ( Aggro )
RULE_INT ( Aggro, SpellAggroMod, 100 )
RULE_REAL ( Aggro, TunnelVisionAggroMod, 0.75 ) //people not currently the top hate generate this much hate on a Tunnel Vision mob
RULE_INT(Aggro, ClientAggroCheckInterval, 2) // Interval in which clients actually check for aggro - in seconds
RULE_CATEGORY_END()

RULE_CATEGORY ( Chat )
RULE_BOOL ( Chat, ServerWideOOC, false)
RULE_BOOL ( Chat, ServerWideAuction, false)
RULE_BOOL ( Chat, EnableMailKeyIPVerification, true)
RULE_BOOL ( Chat, EnableAntiSpam, true)
RULE_BOOL ( Chat, SuppressCommandErrors, false) // Do not suppress by default
RULE_INT ( Chat, MinStatusToBypassAntiSpam, 100)
RULE_INT ( Chat, MinimumMessagesPerInterval, 20)
RULE_INT ( Chat, MaximumMessagesPerInterval, 50)
RULE_INT ( Chat, MaxMessagesBeforeKick, 60)
RULE_INT ( Chat, IntervalDurationMS, 60000)
RULE_INT ( Chat, KarmaUpdateIntervalMS, 120000)
RULE_INT ( Chat, KarmaGlobalChatLimit, 12) //amount of karma you need to be able to talk in ooc/auction/chat below the level limit
RULE_INT ( Chat, GlobalChatLevelLimit, 8) //level limit you need to of reached to talk in ooc/auction/chat if your karma is too low.
RULE_CATEGORY_END()

RULE_CATEGORY ( Merchant )
RULE_BOOL( Merchant, UseGreed, false) // if true, merchants that do not already have a database set greed value will raise their prices the more they are sold to or bought from.  This is custom behavior and not classic
RULE_INT ( Merchant, GreedThreshold, 100) // How many purchases are required to or from a vendor before it becomes greedy and starts to raise their prices.
RULE_BOOL( Merchant, ClearTempList, true) // clear temp list after death if set true.
RULE_CATEGORY_END()

RULE_CATEGORY ( Bazaar )
RULE_INT ( Bazaar, MaxSearchResults, 50)
RULE_CATEGORY_END()

RULE_CATEGORY ( Channels )
RULE_INT ( Channels, RequiredStatusAdmin, 251) // Required status to administer chat channels
RULE_INT ( Channels, RequiredStatusListAll, 251) // Required status to list all chat channels
RULE_INT ( Channels, DeleteTimer, 1440) // Empty password protected channels will be deleted after this many minutes
RULE_CATEGORY_END()

RULE_CATEGORY ( EventLog )
RULE_BOOL ( EventLog, RecordSellToMerchant, false ) // Record sales from a player to an NPC merchant in eventlog table
RULE_BOOL ( EventLog, RecordBuyFromMerchant, false ) // Record purchases by a player from an NPC merchant in eventlog table
RULE_BOOL ( EventLog, SkipCommonPacketLogging, true ) // Doesn't log OP_MobHealth or OP_ClientUpdate
RULE_CATEGORY_END()

RULE_CATEGORY ( AA )
RULE_INT ( AA, ExpPerPoint, 18750000) // Amount of exp per AA
RULE_CATEGORY_END()

RULE_CATEGORY( Console )
RULE_INT ( Console, SessionTimeOut, 600000 )	// Amount of time in ms for the console session to time out
RULE_CATEGORY_END()

RULE_CATEGORY( QueryServ )
RULE_BOOL( QueryServ, PlayerLogChat, false) // Logs Player Chat 
RULE_BOOL( QueryServ, PlayerLogTrades, false) // Logs Player Trades
RULE_BOOL( QueryServ, PlayerLogNPCKills, false) // Logs Player NPC Kills
RULE_BOOL( QueryServ, PlayerLogDeletes, false) // Logs Player Deletes
RULE_BOOL( QueryServ, PlayerLogMoves, false) // Logs Player Moves
RULE_BOOL( QueryServ, PlayerLogMerchantTransactions, false) // Logs Merchant Transactions
RULE_BOOL( QueryServ, PlayerLogGroundSpawn, false) // Logs Player Dropped or Picked-up items.
RULE_BOOL( QueryServ, PlayerLogZone, false) // Logs Player Zone Events
RULE_BOOL( QueryServ, PlayerLogDeaths, false) // Logs Player Deaths
RULE_BOOL( QueryServ, PlayerLogConnectDisconnect, false) // Logs Player Connect Disconnect State
RULE_BOOL( QueryServ, PlayerLogLevels, false) // Logs Player Leveling/Deleveling
RULE_BOOL( QueryServ, PlayerLogAARate, false) // Logs Player AA Experience Rates 
RULE_BOOL( QueryServ, PlayerLogQGlobalUpdate, false) // Logs Player QGlobal Updates
RULE_BOOL( QueryServ, PlayerLogAAPurchases, false) // Log Player AA Purchases
RULE_BOOL( QueryServ, PlayerLogTradeSkillEvents, false) // Log Player Tradeskill Transactions
RULE_BOOL( QueryServ, PlayerLogMoneyTransactions, false) // Log Player Money Transaction/Splits
RULE_BOOL( QueryServ, PlayerLogBankTransactions, false) // Log Player Bank Transactions
RULE_BOOL( QueryServ, PlayerLogLoot, false) // Log Player Looting cash/Items
RULE_BOOL( QueryServ, PlayerLogItemDesyncs, true) // Log Player item desyncs
RULE_BOOL( QueryServ, BazaarAuditTrail, true) // Log Bazaar transactions
RULE_INT( QueryServ, LevelAlwaysLogKills, 1) // The NPC level where player kills are always logged. Below this only possible killsteals are logged. Set to 1 to disable.
RULE_CATEGORY_END()

RULE_CATEGORY( Groundspawns )
RULE_INT ( Groundspawns, DecayTime, 300000 )	// Decay time of player dropped items. 5 minutes
RULE_INT ( Groundspawns, DisarmDecayTime, 300000 )	// Decay time of weapons dropped due to disarm 5 minutes
RULE_INT ( Groundspawns, FullInvDecayTime, 21600000 )	// Decay time of items dropped due to full inventory (trades/merchants) Default 6 hours.
RULE_BOOL ( Groundspawns, RandomSpawn, true )	// Determines if groundspawns with random spawn locs will periodically despawn and respawn elsewhere.
RULE_CATEGORY_END()

RULE_CATEGORY( Range )
RULE_INT ( Range, EventSay, 5000 )
RULE_INT ( Range, EventAggroSay, 5000 )
RULE_INT ( Range, Say, 100 )
RULE_INT ( Range, Emote, 135 )
RULE_INT ( Range, BeginCast, 200)
RULE_INT ( Range, Anims, 135)
RULE_INT ( Range, ProjectileAnims, 135)
RULE_INT ( Range, SpellAnims, 135)
RULE_INT ( Range, DamageMessages, 75)
RULE_INT ( Range, CombatSpecials, 80)
RULE_INT ( Range, SpellMessages, 75)
RULE_INT ( Range, SongMessages, 60)
RULE_CATEGORY_END()

RULE_CATEGORY(Doors)
RULE_REAL(Doors, LargeRaceRange, 1180)
RULE_REAL(Doors, NormalRaceRange, 250)
RULE_REAL(Doors, LargeRaceZ, 150)
RULE_REAL(Doors, NormalRaceZ, 20)
RULE_CATEGORY_END()

RULE_CATEGORY(Items)
RULE_BOOL(Items, DisableNoDrop, false)
RULE_BOOL(Items, DisableNoRent, false)
RULE_CATEGORY_END()

RULE_CATEGORY(Bugs)
RULE_BOOL(Bugs, ReportingSystemActive, true) // Activates bug reporting
RULE_CATEGORY_END()

RULE_CATEGORY(Petitions)
RULE_BOOL(Petitions, PetitionSystemActive, false) // Activates bug reporting
RULE_CATEGORY_END()


#undef RULE_CATEGORY
#undef RULE_INT
#undef RULE_REAL
#undef RULE_BOOL
#undef RULE_CATEGORY_END
