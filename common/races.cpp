/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2016 EQEMu Development Team (http://eqemu.org)

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

#include "../common/races.h"
#include "data_verification.h"

const char* GetRaceIDName(uint16 race_id)
{
	switch (race_id) {
	case Race::Abhorrent:
		return "Abhorrent";
	case Race::AirElemental:
		return "Air Elemental";
	case Race::AirMephit:
		return "Air Mephit";
	case Race::Akhevan:
		return "Akheva";
	case Race::Alligator:
		return "Alligator";
	case Race::Denizen:
		return "Amygdalan";
	case Race::AnimatedArmor:
		return "Animated Armor";
	case Race::AnimatedHand:
		return "Animated Hand";
	case Race::Arachnid:
		return "Arachnid";
	case Race::Armadillo:
		return "Armadillo";
	case Race::Aviak:
		return "Aviak";
	case Race::Banshee:
		return "Banshee";
	case Race::Barbarian:
	case Race::HalasCitizen:
		return "Barbarian";
	case Race::GiantBat:
	case Race::Bat:
		return "Bat";
	case Race::Bear:
	case Race::Bear2:
		return "Bear";
	case Race::Beetle:
		return "Beetle";
	case Race::HumanBeggar:
		return "Beggar";
	case Race::Bertoxxulous:
	case Race::BertoxxulousNew:
		return "Bertoxxulous";
	case Race::Bixie:
		return "Bixie";
	case Race::BloodRaven:
		return "Blood Raven";
	case Race::Boat:
		return "Boat";
	case Race::Portal:
		return "BoT Portal";
	case Race::Bristlebane:
		return "Bristlebane";
	case Race::BrokenClockwork:
		return "Broken Clockwork";
	case Race::Brontotherium:
		return "Brontotherium";
	case Race::Brownie:
		return "Brownie";
	case Race::Bubonian:
		return "Bubonian";
	case Race::BubonianUnderling:
		return "Bubonian Underling";
	case Race::Burynai:
		return "Burynai";
	case Race::CazicThule:
		return "Cazic Thule";
	case Race::Centaur:
		return "Centaur";
	case Race::Clam:
		return "Clam";
	case Race::ClockworkBeetle:
		return "Clockwork Beetle";
	case Race::ClockworkBrain:
		return "Clockwork Brain";
	case Race::ClockworkGnome:
		return "Clockwork Gnome";
	case Race::ClockworkGolem:
		return "Clockwork Golem";
	case Race::Cockatrice:
		return "Cockatrice";
	case Race::Coldain:
		return "Coldain";
	case Race::Crab:
		return "Crab";
	case Race::Crocodile:
		return "Crocodile";
	case Race::CrystalSpider:
		return "Crystal Spider";
	case Race::DaisyMan:
		return "Daisy Man";
	case Race::DarkElf:
	case Race::NeriakCitizen:
		return "Dark Elf";
	case Race::DemiLich:
		return "Demi Lich";
	case Race::Dervish:
		return "Dervish";
	case Race::Devourer:
		return "Devourer";
	case Race::DireWolf:
		return "Dire Wolf";
	case Race::DiseasedFiend:
		return "Diseased Fiend";
	case Race::Djinn:
		return "Djinn";
	case Race::Drachnid:
		return "Drachnid";
	case Race::Draglock:
		return "Draglock";
	case Race::LavaDragon:
	case Race::DragonSkeleton:
	case Race::WaterDragon:
	case Race::VeliousDragon:
	case Race::ClockworkDragon:
	case Race::BlackAndWhiteDragon:
	case Race::GhostDragon:
	case Race::PrismaticDragon:
	case Race::Quarm:
		return "Dragon";
	case Race::Drake:
		return "Drake";
	case Race::Drixie:
		return "Drixie";
	case Race::Drolvarg:
		return "Drolvarg";
	case Race::Dryad:
		return "Dryad";
	case Race::Dwarf:
	case Race::KaladimCitizen:
		return "Dwarf";
	case Race::EarthElemental:
		return "Earth Elemental";
	case Race::EarthMephit:
		return "Earth Mephit";
	case Race::GiantEel:
		return "Eel";
	case Race::Efreeti:
	case Race::Efreeti2:
		return "Efreeti";
	case Race::Elemental:
		return "Elemental";
	case Race::EnchantedArmor:
		return "Enchanted Armor";
	case Race::Erollisi:
		return "Erollisi";
	case Race::Erudite:
	case Race::EruditeCitizen:
		return "Erudite";
	case Race::EvanTest:
		return "Evan Test";
	case Race::EvilEye:
		return "Evil Eye";
	case Race::EyeOfZomm:
		return "Eye";
	case Race::Fairy:
		return "Fairy";
	case Race::Faun:
		return "Faun";
	case Race::FayDrake:
		return "Fay Drake";
	case Race::FenninRo:
		return "Fennin Ro";
	case Race::Fiend:
		return "Fiend";
	case Race::FireElemental:
		return "Fire Elemental";
	case Race::FireMephit:
		return "Fire Mephit";
	case Race::Fish:
	case Race::KunarkFish:
		return "Fish";
	case Race::Fly:
		return "Fly";
	case Race::Froglok:
	case Race::FroglokGhoul:
		return "Froglok";
	case Race::FungalFiend:
		return "Fungal Fiend";
	case Race::Fungusman:
		return "Fungusman";
	case Race::Galorian:
		return "Galorian";
	case Race::Gargoyle:
		return "Gargoyle";
	case Race::Gasbag:
		return "Gasbag";
	case Race::GelatinousCube:
		return "Gelatinous Cube";
	case Race::Geonid:
		return "Geonid";
	case Race::Ghost:
	case Race::DwarfGhost:
	case Race::EruditeGhost:
		return "Ghost";
	case Race::GhostShip:
		return "Ghost Ship";
	case Race::Ghoul:
		return "Ghoul";
	case Race::Giant:
	case Race::ForestGiant:
	case Race::FrostGiant:
	case Race::StormGiant:
	case Race::EarthGolem:
	case Race::IronGolem:
	case Race::StormGolem:
	case Race::AirGolem:
	case Race::WoodGolem:
	case Race::FireGolem:
	case Race::WaterGolem:
		return "Giant";
	case Race::GiantClockwork:
		return "Giant Clockwork";
	case Race::Gnoll:
		return "Gnoll";
	case Race::Gnome:
		return "Gnome";
	case Race::Goblin:
	case Race::Bloodgill:
	case Race::KunarkGoblin:
		return "Goblin";
	case Race::Golem:
		return "Golem";
	case Race::Goo:
		return "Goo";
	case Race::Gorgon:
		return "Gorgon";
	case Race::Gorilla:
		return "Gorilla";
	case Race::GriegVeneficus:
		return "Grieg Veneficus";
	case Race::Griffin:
		return "Griffin";
	case Race::Grimling:
		return "Grimling";
	case Race::GroundShaker:
		return "Ground Shaker";
	case Race::FreeportGuard:
	case Race::Felguard:
	case Race::Fayguard:
	case Race::VahShirGuard:
		return "Guard";
	case Race::GuardOfJustice:
		return "Guard of Justice";
	case Race::Hag:
		return "Hag";
	case Race::HalfElf:
		return "Half Elf";
	case Race::Halfling:
	case Race::RivervaleCitizen:
		return "Halfling";
	case Race::Harpy:
		return "Harpy";
	case Race::HighElf:
		return "High Elf";
	case Race::Hippogriff:
		return "Hippogriff";
	case Race::Holgresh:
		return "Holgresh";
	case Race::Horse:
		return "Horse";
	case Race::Hraquis:
		return "Hraquis";
	case Race::Human:
	case Race::HighpassCitizen:
	case Race::QeynosCitizen:
		return "Human";
	case Race::IceSpectre:
		return "Ice Spectre";
	case Race::Iksar:
	case Race::IksarCitizen:
		return "Iksar";
	case Race::IksarGolem:
		return "Iksar Golem";
	case Race::IksarSpirit:
		return "Iksar Spirit";
	case Race::Imp:
		return "Imp";
	case Race::Innoruuk:
		return "Innoruuk";
	case Race::InvisibleMan:
		return "Invisible Man";
	case Race::JunkBeast:
		return "Junk Beast";
	case Race::Karana:
		return "Karana";
	case Race::PhinigelAutropos:
		return "Kedge";
	case Race::Kerran:
		return "Kerran";
	case Race::KnightOfPestilence:
		return "Knight of Pestilence";
	case Race::Kobold:
		return "Kobold";
	case Race::Kraken:
		return "Kraken";
	case Race::Launch:
		return "Launch";
	case Race::Leech:
		return "Leech";
	case Race::Lepertoloth:
		return "Lepertoloth";
	case Race::Lightcrawler:
		return "Lightcrawler";
	case Race::Lion:
		return "Lion";
	case Race::LizardMan:
		return "Lizard Man";
	case Race::Malarian:
		return "Malarian";
	case Race::Mammoth:
		return "Mammoth";
	case Race::ManEatingPlant:
		return "Man - Eating Plant";
	case Race::Manticore:
		return "Manticore";
	case Race::Mermaid:
		return "Mermaid";
	case Race::Mimic:
		return "Mimic";
	case Race::MiniPom:
		return "Mini POM";
	case Race::Minotaur:
		return "Minotaur";
	case Race::MithanielMarr:
		return "Mithaniel Marr";
	case Race::Mosquito:
		return "Mosquito";
	case Race::MouthOfInsanity:
		return "Mouth of Insanity";
	case Race::Netherbian:
		return "Netherbian";
	case Race::Nightmare:
		return "Nightmare";
	case Race::NightmareGargoyle:
		return "Nightmare Gargoyle";
	case Race::NightmareGoblin:
		return "Nightmare Goblin";
	case Race::NightmareMephit:
		return "Nightmare Mephit";
	case Race::NightmareWraith:
		return "Nightmare Wraith";
	case Race::Nilborien:
		return "Nilborien";
	case Race::Nymph:
		return "Nymph";
	case Race::Ogre:
	case Race::OggokCitizen:
		return "Ogre";
	case Race::Orc:
		return "Orc";
	case Race::Othmir:
		return "Othmir";
	case Race::Owlbear:
		return "Owlbear";
	case Race::Pegasus:
		return "Pegasus";
	case Race::Phoenix:
		return "Phoenix";
	case Race::Piranha:
		return "Piranha";
	case Race::Pixie:
		return "Pixie";
	case Race::PoisonFrog:
		return "Poison Frog";
	case Race::Puma:
		return "Puma";
	case Race::Pusling:
		return "Pusling";
	case Race::StatueOfRallosZek:
	case Race::NewRallosZek:
		return "Rallos Zek";
	case Race::RallosOgre:
		return "Rallos Zek Minion";
	case Race::Raptor:
		return "Raptor";
	case Race::GiantRat:
		return "Rat";
	case Race::Ratman:
		return "Ratman";
	case Race::ReanimatedHand:
		return "Reanimated Hand";
	case Race::Recuso:
		return "Recuso";
	case Race::RhinoBeetle:
		return "Rhino Beetle";
	case Race::Rhinoceros:
		return "Rhinoceros";
	case Race::Rockhopper:
		return "Rockhopper";
	case Race::RonnieTest:
		return "Ronnie Test";
	case Race::Sabertooth:
		return "Saber - toothed Cat";
	case Race::Sarnak:
		return "Sarnak";
	case Race::SarnakGolem:
		return "Sarnak Golem";
	case Race::SarnakSpirit:
		return "Sarnak Spirit";
	case Race::Saryrn:
		return "Saryrn";
	case Race::Scarecrow:
		return "Scarecrow";
	case Race::ScarletCheetah:
		return "Scarlet Cheetah";
	case Race::Scorpion:
	case Race::IksarScorpion:
		return "Scorpion";
	case Race::SeaTurtle:
		return "Sea Turtle";
	case Race::SeaHorse:
		return "Seahorse";
	case Race::LordInquisitorSeru:
		return "Seru";
	case Race::Shade:
		return "Shade";
	case Race::KhatiSha:
		return "Shadel";
	case Race::Shark:
		return "Shark";
	case Race::Shiknar:
		return "Shik'Nar";
	case Race::Ship:
		return "Ship";
	case Race::Shissar:
		return "Shissar";
	case Race::Shrieker:
		return "Shrieker";
	case Race::Siren:
		return "Siren";
	case Race::SkeletalHorse:
		return "Skeletal Horse";
	case Race::Skeleton:
		return "Skeleton";
	case Race::Skunk:
		return "Skunk";
	case Race::GiantSnake:
		return "Snake";
	case Race::SnakeElemental:
		return "Snake Elemental";
	case Race::SnowDervish:
		return "Snow Dervish";
	case Race::SnowRabbit:
		return "Snow Rabbit";
	case Race::SolusekRo:
	case Race::SolusekRo2:
		return "Solusek Ro";
	case Race::SolusekRoGuard:
		return "Solusek Ro Guard";
	case Race::SonicWolf:
		return "Sonic Wolf";
	case Race::SoulDevourer:
		return "Soul Devourer";
	case Race::Spectre:
		return "Spectre";
	case Race::Sphinx:
		return "Sphinx";
	case Race::GiantSpider:
		return "Spider";
	case Race::Stonegrabber:
		return "Stonegrabber";
	case Race::Stormrider:
		return "Stormrider";
	case Race::Succulent:
		return "Succulent";
	case Race::Shadel:
		return "Sun Revenant";
	case Race::Sunflower:
		return "Sunflower";
	case Race::Swordfish:
		return "Swordfish";
	case Race::FroglokTadpole:
		return "Tadpole";
	case Race::TallonZek:
		return "Tallon Zek";
	case Race::TarewMarr:
		return "Tarew Marr";
	case Race::Tegi:
		return "Tegi";
	case Race::TeleportMan:
		return "Teleport Man";
	case Race::TentacleTerror:
		return "Tentacle Terror";
	case Race::TerrisThule:
		return "Terris Thule";
	case Race::TestObject:
		return "Test Object";
	case Race::Rathe:
		return "The Rathe";
	case Race::TribunalNew:
		return "The Tribunal";
	case Race::ThoughtHorror:
		return "Thought Horror";
	case Race::Tiger:
		return "Tiger";
	case Race::TinSoldier:
		return "Tin Soldier";
	case Race::Tormentor:
		return "Tormentor";
	case Race::Totem:
		return "Totem";
	case Race::Trakanon:
		return "Trakanon";
	case Race::Tranquilion:
		return "Tranquilion";
	case Race::Treant:
	case Race::Treant2:
		return "Treant";
	case Race::Tribunal:
		return "Tribunal";
	case Race::Troll:
	case Race::GrobbCitizen:
		return "Troll";
	case Race::Tunare:
		return "Tunare";
	case Race::Ulthork:
		return "Ulthork";
	case Race::UndeadFootman:
		return "Undead Footman";
	case Race::UndeadIksar:
		return "Undead Iksar";
	case Race::UndeadKnight:
		return "Undead Knight";
	case Race::UndeadSarnak:
		return "Undead Sarnak";
	case Race::Underbulk:
		return "Underbulk";
	case Race::Unicorn:
		return "Unicorn";
	case Race::Doug:
	case Race::MinorIllusion:
	case Race::Tree:
	case Race::Unknown:
	case Race::Unknown2:
		return "UNKNOWN RACE";
	case Race::VahShir:
	case Race::VahShirKing:
		return "Vah Shir";
	case Race::VahShirSkeleton:
		return "Vah Shir Skeleton";
	case Race::VallonZek:
		return "Vallon Zek";
	case Race::Valorian:
	case Race::Valorian2:
		return "Valorian";
	case Race::Vampire:
	case Race::ElfVampire:
	case Race::Vampire2:
	case Race::VampireVolatalis:
		return "Vampire";
	case Race::Vegerog:
		return "Vegerog";
	case Race::VenrilSathir:
		return "Venril Sathir";
	case Race::Walrus:
		return "Walrus";
	case Race::WarBoar:
	case Race::WarBoar2:
		return "War Boar";
	case Race::WarWraith:
		return "War Wraith";
	case Race::Wasp:
		return "Wasp";
	case Race::WaterElemental:
		return "Water Elemental";
	case Race::WaterMephit:
		return "Water Mephit";
	case Race::Werewolf:
	case Race::Werewolf2:
		return "Werewolf";
	case Race::WetfangMinnow:
		return "Wetfang Minnow";
	case Race::Wisp:
		return "Will - O - Wisp";
	case Race::Wolf:
	case Race::WolfElemental:
		return "Wolf";
	case Race::WoodElf:
		return "Wood Elf";
	case Race::Worm:
		return "Worm";
	case Race::Wretch:
		return "Wretch";
	case Race::Wrulon:
		return "Wrulon";
	case Race::Wurm:
		return "Wurm";
	case Race::Wyvern:
		return "Wyvern";
	case Race::Xalgoz:
		return "Xalgoz";
	case Race::Xegony:
		return "Xegony";
	case Race::Yakkar:
		return "Yakkar";
	case Race::Yeti:
		return "Yeti";
	case Race::Zebuxoruk:
		return "Zebuxoruk";
	case Race::ZebuxoruksCage:
		return "Zebuxoruk's Cage";
	case Race::Zelniak:
		return "Zelniak";
	case Race::Zombie:
		return "Zombie";
	default:
		return "UNKNOWN RACE";
	}
}

uint32 GetPlayerRaceValue(uint16 race_id)
{
	switch (race_id) {
	case Race::Human:
	case Race::Barbarian:
	case Race::Erudite:
	case Race::WoodElf:
	case Race::HighElf:
	case Race::DarkElf:
	case Race::HalfElf:
	case Race::Dwarf:
	case Race::Troll:
	case Race::Ogre:
	case Race::Halfling:
	case Race::Gnome:
		return race_id;
	case Race::Iksar:
		return RaceIndex::Iksar;
	case Race::VahShir:
		return RaceIndex::VahShir;
	default:
		return Race::Doug;
	}
}

uint16 GetPlayerRaceBit(uint16 race_id)
{
	switch (race_id) {
	case Race::Human:
		return RaceBitmask::Human;
	case Race::Barbarian:
		return RaceBitmask::Barbarian;
	case Race::Erudite:
		return RaceBitmask::Erudite;
	case Race::WoodElf:
		return RaceBitmask::WoodElf;
	case Race::HighElf:
		return RaceBitmask::HighElf;
	case Race::DarkElf:
		return RaceBitmask::DarkElf;
	case Race::HalfElf:
		return RaceBitmask::HalfElf;
	case Race::Dwarf:
		return RaceBitmask::Dwarf;
	case Race::Troll:
		return RaceBitmask::Troll;
	case Race::Ogre:
		return RaceBitmask::Ogre;
	case Race::Halfling:
		return RaceBitmask::Halfling;
	case Race::Gnome:
		return RaceBitmask::Gnome;
	case Race::Iksar:
		return RaceBitmask::Iksar;
	case Race::VahShir:
		return RaceBitmask::VahShir;
	default:
		return RaceBitmask::Unknown;
	}
}

uint16 GetRaceIDFromPlayerRaceValue(uint32 player_race_value)
{
	switch (player_race_value) {
	case RaceIndex::Human:
	case RaceIndex::Barbarian:
	case RaceIndex::Erudite:
	case RaceIndex::WoodElf:
	case RaceIndex::HighElf:
	case RaceIndex::DarkElf:
	case RaceIndex::HalfElf:
	case RaceIndex::Dwarf:
	case RaceIndex::Troll:
	case RaceIndex::Ogre:
	case RaceIndex::Halfling:
	case RaceIndex::Gnome:
		return player_race_value;
	case RaceIndex::Iksar:
		return Race::Iksar;
	case RaceIndex::VahShir:
		return Race::VahShir;
	default:
		return Race::Doug;
	}
}

float GetRaceGenderDefaultHeight(int race, int gender)
{
	static float male_height[] = {
	    6.0f,  6.0f,  7.0f,   6.0f,   5.0f,  6.0f,  5.0f,   5.5f,  4.0f,  8.0f,  9.0f,  3.5f,  3.0f,  6.0f,   6.0f,
	    2.0f,  8.5f,  8.0f,   21.0f,  20.0f, 6.0f,  6.0f,   3.5f,  3.0f,  6.0f,  2.0f,  5.0f,  5.0f,  6.0f,   6.0f,
	    6.0f,  7.5f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  5.0f,  6.0f,  6.0f,  7.0f,  4.0f,  4.7f,   6.0f,
	    8.0f,  3.0f,  12.0f,  5.0f,   21.0f, 6.0f,  6.0f,   3.0f,  9.0f,  6.0f,  6.0f,  2.0f,  6.0f,  3.0f,   6.0f,
	    4.0f,  20.0f, 5.0f,   5.0f,   6.0f,  9.0f,  25.0f,  6.0f,  6.0f,  10.0f, 6.0f,  6.0f,  6.0f,  6.0f,   2.5f,
	    7.0f,  6.0f,  5.0f,   6.0f,   1.5f,  1.0f,  3.5f,   7.0f,  6.0f,  6.0f,  6.0f,  6.0f,  7.0f,  3.0f,   3.0f,
	    7.0f,  12.0f, 8.0f,   9.0f,   4.0f,  11.5f, 8.0f,   6.0f,  6.0f,  12.0f, 6.0f,  6.0f,  6.0f,  20.0f,  10.0f,
	    6.5f,  6.0f,  17.0f,  1.0f,   4.0f,  6.0f,  8.0f,   5.0f,  1.0f,  6.0f,  6.0f,  5.0f,  5.0f,  5.0f,   9.0f,
	    3.0f,  8.0f,  2.0f,   24.0f,  6.0f,  10.0f, 6.0f,   6.0f,  6.0f,  3.0f,  7.0f,  9.0f,  6.0f,  11.0f,  2.5f,
	    14.0f, 8.0f,  7.0f,   12.0f,  6.0f,  27.0f, 6.0f,   6.0f,  6.0f,  6.0f,  2.0f,  9.0f,  9.0f,  6.0f,   9.0f,
	    3.0f,  3.0f,  6.0f,   6.0f,   10.0f, 6.0f,  6.0f,   15.0f, 15.0f, 9.0f,  7.0f,  6.0f,  6.0f,  7.0f,   8.0f,
	    3.0f,  3.0f,  6.0f,   7.0f,   13.0f, 6.0f,  6.0f,   9.0f,  5.0f,  7.0f,  9.0f,  6.0f,  6.0f,  8.0f,   6.0f,
	    6.0f,  5.5f,  6.0f,   4.0f,   25.0f, 6.0f,  6.0f,   6.0f,  22.0f, 20.0f, 6.0f,  10.0f, 13.5f, 12.0f,  3.0f,
	    30.0f, 6.0f,  6.0f,   35.0f,  1.5f,  8.0f,  3.0f,   6.0f,  2.0f,  6.0f,  6.0f,  5.0f,  2.0f,  7.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   4.0f,   6.0f,  6.0f,  6.0f,   8.0f,  8.0f,  7.0f,  8.0f,  6.0f,  7.0f,  6.0f,   7.0f,
	    6.0f,  10.0f, 3.0f,   6.0f,   8.0f,  9.0f,  15.0f,  5.0f,  10.0f, 7.0f,  6.0f,  7.0f,  6.0f,  7.0f,   7.0f,
	    12.0f, 6.0f,  4.0f,   6.0f,   5.0f,  3.0f,  30.0f,  30.0f, 15.0f, 20.0f, 6.0f,  10.0f, 6.0f,  14.0f,  14.0f,
	    16.0f, 15.0f, 30.0f,  15.0f,  7.5f,  5.0f,  4.0f,   6.0f,  15.0f, 6.5f,  3.0f,  12.0f, 10.0f, 10.5f,  10.0f,
	    7.5f,  6.0f,  6.0f,   12.5f,  9.0f,  20.0f, 2.0f,   10.0f, 25.0f, 8.0f,  6.0f,  6.0f,  10.0f, 18.0f,  45.0f,
	    13.0f, 15.0f, 8.0f,   30.0f,  25.0f, 25.0f, 10.0f,  13.0f, 5.0f,  3.5f,  15.0f, 35.0f, 11.0f, 15.0f,  50.0f,
	    13.0f, 6.0f,  7.0f,   6.0f,   60.0f, 6.0f,  22.0f,  22.0f, 21.0f, 22.0f, 15.0f, 25.0f, 23.0f, 8.0f,   15.0f,
	    10.0f, 6.0f,  7.0f,   6.0f,   12.0f, 9.5f,  6.0f,   12.0f, 12.0f, 12.0f, 15.0f, 4.0f,  5.0f,  105.0f, 20.0f,
	    5.0f,  10.0f, 10.0f,  10.0f,  20.0f, 13.5f, 8.0f,   10.0f, 3.0f,  5.0f,  9.0f,  6.0f,  6.0f,  6.0f,   10.0f,
	    8.0f,  8.0f,  8.0f,   6.0f,   6.0f,  5.0f,  5.0f,   5.0f,  9.0f,  9.0f,  9.0f,  6.0f,  8.5f,  6.0f,   7.0f,
	    8.0f,  7.0f,  11.0f,  6.0f,   7.0f,  9.0f,  8.0f,   6.0f,  8.0f,  6.0f,  6.0f,  6.0f,  6.0f,  9.0f,   10.0f,
	    6.0f,  3.0f,  4.0f,   3.0f,   3.0f,  4.0f,  10.0f,  10.0f, 2.0f,  8.0f,  6.0f,  6.0f,  14.0f, 7.0f,   5.0f,
	    9.0f,  7.0f,  7.0f,   10.0f,  10.0f, 12.0f, 9.0f,   7.0f,  12.0f, 13.0f, 16.0f, 6.0f,  9.0f,  6.0f,   6.0f,
	    10.0f, 25.0f, 15.0f,  6.0f,   25.0f, 6.0f,  6.0f,   8.0f,  11.0f, 6.0f,  9.0f,  2.0f,  6.0f,  5.0f,   4.0f,
	    8.5f,  6.0f,  6.0f,   6.0f,   4.0f,  6.0f,  15.0f,  1.0f,  2.0f,  6.0f,  40.0f, 8.0f,  12.0f, 3.0f,   8.0f,
	    99.0f, 9.0f,  100.0f, 100.0f, 10.0f, 6.0f,  27.5f,  20.0f, 6.0f,  6.0f,  5.0f,  6.0f,  8.0f,  5.0f,   3.0f,
	    11.5f, 25.0f, 80.0f,  20.0f,  9.0f,  8.0f,  5.0f,   4.0f,  7.0f,  10.0f, 6.0f,  11.0f, 8.0f,  5.0f,   6.0f,
	    6.0f,  30.0f, 7.0f,   15.0f,  9.0f,  6.0f,  9.0f,   6.0f,  3.0f,  32.5f, 15.0f, 7.5f,  10.0f, 10.0f,  6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  9.0f,  20.0f,  6.0f,  6.0f,  6.0f,  25.0f, 12.0f, 6.0f,  8.0f,   6.0f,
	    6.0f,  20.0f, 10.0f,  8.0f,   12.0f, 8.0f,  2.0f,   6.0f,  3.0f,  6.0f,  7.0f,  1.5f,  6.0f,  3.0f,   3.0f,
	    3.0f,  3.0f,  2.0f,   3.0f,   3.0f,  6.0f,  6.0f,   6.0f,  4.5f,  7.0f,  6.0f,  7.0f,  6.0f,  22.0f,  8.0f,
	    15.0f, 22.0f, 8.0f,   15.0f,  6.0f,  80.0f, 150.0f, 7.0f,  6.0f,  6.0f,  6.0f,  12.0f, 6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   35.0f, 20.0f, 9.0f,  6.0f,  6.0f,  6.0f,  20.0f,  20.0f,
	    20.0f, 20.0f, 20.0f,  9.0f,   4.0f,  4.0f,  10.0f,  5.0f,  8.0f,  6.0f,  10.0f, 6.0f,  6.0f,  2.0f,   36.0f,
	    14.0f, 7.0f,  250.0f, 6.0f,   9.0f,  6.0f,  7.0f,   4.0f,  6.0f,  8.0f,  6.0f,  23.0f, 6.0f,  6.0f,   6.0f,
	    70.0f, 6.0f,  7.0f,   6.0f,   6.0f,  6.0f,  20.0f,  6.0f,  6.0f,  6.0f,  5.0f,  1.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    4.0f,  4.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  10.0f, 6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  7.0f,  7.0f,   7.0f,  7.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   8.0f,
	    6.0f,  6.0f,  6.0f,   7.0f,   6.0f,  6.0f,  6.0f,   7.5f,  6.0f,  6.0f,  4.0f,  6.0f,  3.0f,  6.0f,   6.0f,
	    1.0f,  9.0f,  7.0f,   8.0f,   7.0f,  8.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  8.0f,
	};

	static float female_height[] = {
	    6.0f,  6.0f,  7.0f,   6.0f,   5.0f,  6.0f,  5.0f,   5.5f,  4.0f,  8.0f,  9.0f,  3.5f,  3.0f,  6.0f,   6.0f,
	    2.0f,  8.5f,  8.0f,   21.0f,  20.0f, 6.0f,  6.0f,   3.5f,  3.0f,  6.0f,  2.0f,  5.0f,  5.0f,  6.0f,   6.0f,
	    6.0f,  7.5f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  5.0f,  6.0f,  6.0f,  7.0f,  4.0f,  4.7f,   6.0f,
	    8.0f,  3.0f,  12.0f,  5.0f,   21.0f, 6.0f,  6.0f,   3.0f,  9.0f,  6.0f,  6.0f,  2.0f,  6.0f,  3.0f,   6.0f,
	    4.0f,  20.0f, 5.0f,   5.0f,   6.0f,  9.0f,  25.0f,  6.0f,  6.0f,  10.0f, 6.0f,  6.0f,  6.0f,  6.0f,   2.5f,
	    7.0f,  6.0f,  5.0f,   6.0f,   1.5f,  1.0f,  3.5f,   7.0f,  6.0f,  6.0f,  6.0f,  6.0f,  7.0f,  3.0f,   3.0f,
	    7.0f,  12.0f, 8.0f,   9.0f,   4.0f,  11.5f, 8.0f,   6.0f,  6.0f,  12.0f, 6.0f,  6.0f,  6.0f,  20.0f,  10.0f,
	    6.5f,  6.0f,  17.0f,  1.0f,   4.0f,  6.0f,  8.0f,   5.0f,  1.0f,  6.0f,  6.0f,  5.0f,  5.0f,  5.0f,   9.0f,
	    3.0f,  8.0f,  2.0f,   24.0f,  6.0f,  10.0f, 6.0f,   6.0f,  6.0f,  3.0f,  7.0f,  9.0f,  6.0f,  11.0f,  2.5f,
	    14.0f, 8.0f,  7.0f,   12.0f,  6.0f,  27.0f, 6.0f,   6.0f,  6.0f,  6.0f,  2.0f,  9.0f,  9.0f,  6.0f,   9.0f,
	    3.0f,  3.0f,  6.0f,   6.0f,   10.0f, 6.0f,  6.0f,   15.0f, 15.0f, 9.0f,  7.0f,  6.0f,  6.0f,  7.0f,   8.0f,
	    3.0f,  3.0f,  6.0f,   7.0f,   13.0f, 6.0f,  6.0f,   9.0f,  5.0f,  7.0f,  9.0f,  6.0f,  6.0f,  8.0f,   6.0f,
	    6.0f,  5.5f,  6.0f,   4.0f,   25.0f, 6.0f,  6.0f,   6.0f,  22.0f, 20.0f, 6.0f,  10.0f, 13.5f, 12.0f,  3.0f,
	    30.0f, 6.0f,  6.0f,   35.0f,  1.5f,  8.0f,  3.0f,   6.0f,  2.0f,  6.0f,  6.0f,  5.0f,  2.0f,  7.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   4.0f,   6.0f,  6.0f,  6.0f,   8.0f,  8.0f,  7.0f,  8.0f,  6.0f,  7.0f,  6.0f,   7.0f,
	    6.0f,  10.0f, 3.0f,   6.0f,   8.0f,  9.0f,  15.0f,  5.0f,  10.0f, 7.0f,  6.0f,  7.0f,  6.0f,  7.0f,   7.0f,
	    12.0f, 6.0f,  4.0f,   6.0f,   5.0f,  3.0f,  30.0f,  30.0f, 15.0f, 20.0f, 6.0f,  10.0f, 6.0f,  14.0f,  14.0f,
	    16.0f, 15.0f, 30.0f,  15.0f,  7.5f,  5.0f,  4.0f,   6.0f,  15.0f, 6.5f,  3.0f,  12.0f, 10.0f, 10.5f,  10.0f,
	    7.5f,  6.0f,  6.0f,   12.5f,  9.0f,  20.0f, 2.0f,   10.0f, 25.0f, 8.0f,  6.0f,  6.0f,  10.0f, 18.0f,  45.0f,
	    13.0f, 15.0f, 8.0f,   30.0f,  25.0f, 25.0f, 10.0f,  13.0f, 5.0f,  3.5f,  15.0f, 35.0f, 11.0f, 15.0f,  50.0f,
	    13.0f, 6.0f,  7.0f,   6.0f,   60.0f, 6.0f,  22.0f,  22.0f, 21.0f, 22.0f, 15.0f, 25.0f, 23.0f, 8.0f,   15.0f,
	    10.0f, 6.0f,  7.0f,   6.0f,   12.0f, 9.5f,  6.0f,   12.0f, 12.0f, 12.0f, 15.0f, 4.0f,  5.0f,  105.0f, 20.0f,
	    5.0f,  10.0f, 10.0f,  10.0f,  20.0f, 13.5f, 8.0f,   10.0f, 3.0f,  5.0f,  9.0f,  6.0f,  6.0f,  6.0f,   10.0f,
	    8.0f,  8.0f,  8.0f,   6.0f,   6.0f,  5.0f,  5.0f,   5.0f,  9.0f,  9.0f,  9.0f,  6.0f,  8.5f,  6.0f,   7.0f,
	    8.0f,  7.0f,  11.0f,  6.0f,   7.0f,  9.0f,  8.0f,   6.0f,  8.0f,  6.0f,  6.0f,  6.0f,  6.0f,  9.0f,   10.0f,
	    6.0f,  3.0f,  4.0f,   3.0f,   3.0f,  4.0f,  10.0f,  10.0f, 2.0f,  8.0f,  6.0f,  6.0f,  14.0f, 7.0f,   5.0f,
	    9.0f,  7.0f,  7.0f,   10.0f,  10.0f, 12.0f, 9.0f,   7.0f,  12.0f, 13.0f, 16.0f, 6.0f,  9.0f,  6.0f,   6.0f,
	    10.0f, 25.0f, 15.0f,  6.0f,   25.0f, 6.0f,  6.0f,   8.0f,  11.0f, 6.0f,  9.0f,  2.0f,  6.0f,  5.0f,   4.0f,
	    8.5f,  6.0f,  6.0f,   6.0f,   4.0f,  6.0f,  15.0f,  1.0f,  2.0f,  6.0f,  40.0f, 8.0f,  12.0f, 3.0f,   8.0f,
	    99.0f, 9.0f,  100.0f, 100.0f, 10.0f, 6.0f,  27.5f,  20.0f, 6.0f,  6.0f,  5.0f,  6.0f,  8.0f,  5.0f,   3.0f,
	    11.5f, 25.0f, 80.0f,  20.0f,  9.0f,  8.0f,  5.0f,   4.0f,  7.0f,  10.0f, 6.0f,  11.0f, 8.0f,  5.0f,   6.0f,
	    6.0f,  30.0f, 7.0f,   15.0f,  9.0f,  6.0f,  9.0f,   6.0f,  3.0f,  32.5f, 15.0f, 7.5f,  10.0f, 10.0f,  6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  9.0f,  20.0f,  6.0f,  6.0f,  6.0f,  25.0f, 12.0f, 6.0f,  8.0f,   6.0f,
	    6.0f,  20.0f, 9.0f,   8.0f,   12.0f, 8.0f,  2.0f,   6.0f,  3.0f,  6.0f,  7.0f,  1.5f,  6.0f,  3.0f,   3.0f,
	    3.0f,  3.0f,  2.0f,   3.0f,   3.0f,  6.0f,  6.0f,   6.0f,  4.5f,  7.0f,  6.0f,  7.0f,  5.7f,  22.0f,  8.0f,
	    15.0f, 22.0f, 8.0f,   15.0f,  6.0f,  80.0f, 150.0f, 7.0f,  6.0f,  6.0f,  6.0f,  12.0f, 6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   35.0f, 20.0f, 9.0f,  6.0f,  6.0f,  6.0f,  20.0f,  20.0f,
	    20.0f, 20.0f, 20.0f,  9.0f,   4.0f,  4.0f,  10.0f,  5.0f,  8.0f,  6.0f,  10.0f, 5.7f,  6.0f,  2.0f,   36.0f,
	    14.0f, 7.0f,  250.0f, 6.0f,   9.0f,  6.0f,  7.0f,   4.0f,  6.0f,  8.0f,  6.0f,  23.0f, 6.0f,  6.0f,   6.0f,
	    70.0f, 6.0f,  7.0f,   6.0f,   6.0f,  6.0f,  20.0f,  6.0f,  6.0f,  6.0f,  5.0f,  1.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    4.0f,  4.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  10.0f, 6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  6.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   6.0f,
	    6.0f,  6.0f,  6.0f,   6.0f,   6.0f,  7.0f,  7.0f,   7.0f,  7.0f,  6.0f,  6.0f,  6.0f,  6.0f,  6.0f,   8.0f,
	    6.0f,  6.0f,  6.0f,   7.0f,   6.0f,  6.0f,  6.0f,   7.5f,  6.0f,  6.0f,  4.0f,  6.0f,  3.0f,  6.0f,   6.0f,
	    1.0f,  9.0f,  7.0f,   8.0f,   7.0f,  8.0f,  6.0f,   6.0f,  6.0f,  6.0f,  6.0f,  8.0f,
	};

	const auto size = sizeof(male_height) / sizeof(male_height[0]);

	if (race >= size)
		return 6.0f;

	if (gender == Gender::Female)
		return female_height[race];

	return male_height[race];
}

#define GetRaceGenderMask(race_id, gender_id) (((int)race_id << 8) | gender_id)

bool RaceAppearance::IsValidBeard(uint16 race_id, uint8 gender_id, uint8 beard_value, bool use_luclin)
{
	if (beard_value == std::numeric_limits<uint8>::max()) {
		return true;
	}

	if (use_luclin) {
		switch (GetRaceGenderMask(race_id, gender_id)) {
		case RaceAppearance::DarkElfFemale:
			if (beard_value <= 1)
				return true;
			break;
		case RaceAppearance::HighElfMale:
		case RaceAppearance::DarkElfMale:
		case RaceAppearance::HalfElfMale:
			if (beard_value <= 3) {
				return true;
			}

			break;
		case RaceAppearance::HumanMale:
		case RaceAppearance::BarbarianMale:
		case RaceAppearance::EruditeMale:
		case RaceAppearance::DwarfMale:
		case RaceAppearance::HalflingMale:
		case RaceAppearance::GnomeMale:
			if (beard_value <= 5) {
				return true;
			}

			break;
		default:
			break;
		}
		return false;
	}
}

bool RaceAppearance::IsValidBeardColor(uint16 race_id, uint8 gender_id, uint8 beard_color_value, bool use_luclin)
{
	if (beard_color_value == std::numeric_limits<uint8>::max()) {
		return true;
	}

	switch (GetRaceGenderMask(race_id, gender_id)) {
	case RaceAppearance::GnomeMale:
		if (beard_color_value <= 24) {
			return true;
		}

		break;
	case RaceAppearance::HumanMale:
	case RaceAppearance::BarbarianMale:
	case RaceAppearance::EruditeMale:
	case RaceAppearance::HalfElfMale:
	case RaceAppearance::DwarfMale:
	case RaceAppearance::DwarfFemale:
	case RaceAppearance::HalflingMale:
		if (beard_color_value <= 19) {
			return true;
		}

		break;
	case RaceAppearance::DarkElfMale:
		if (EQ::ValueWithin(beard_color_value, 13, 18)) {
			return true;
		}

		break;
	case RaceAppearance::HighElfMale:
		if (beard_color_value <= 14) {
			return true;
		}

		break;
	default:
		break;
	}

	return false;
}

bool RaceAppearance::IsValidEyeColor(uint16 race_id, uint8 gender_id, uint8 eye_color_value, bool use_luclin)
{
	if (eye_color_value == std::numeric_limits<uint8>::max()) {
		return true;
	}

	switch (GetRaceGenderMask(race_id, gender_id)) {
	case RaceAppearance::HumanMale:
	case RaceAppearance::HumanFemale:
	case RaceAppearance::BarbarianMale:
	case RaceAppearance::BarbarianFemale:
	case RaceAppearance::EruditeMale:
	case RaceAppearance::EruditeFemale:
	case RaceAppearance::WoodElfMale:
	case RaceAppearance::WoodElfFemale:
	case RaceAppearance::HighElfMale:
	case RaceAppearance::HighElfFemale:
	case RaceAppearance::DarkElfMale:
	case RaceAppearance::DarkElfFemale:
	case RaceAppearance::HalfElfMale:
	case RaceAppearance::HalfElfFemale:
	case RaceAppearance::DwarfMale:
	case RaceAppearance::DwarfFemale:
	case RaceAppearance::OgreMale:
	case RaceAppearance::OgreFemale:
	case RaceAppearance::HalflingMale:
	case RaceAppearance::HalflingFemale:
	case RaceAppearance::GnomeMale:
	case RaceAppearance::GnomeFemale:
	case RaceAppearance::IksarMale:
	case RaceAppearance::IksarFemale:
	case RaceAppearance::VahShirMale:
	case RaceAppearance::VahShirFemale:
		if (eye_color_value <= 9) {
			return true;
		}

		break;
	case RaceAppearance::TrollMale:
	case RaceAppearance::TrollFemale:
		if (eye_color_value <= 10) {
			return true;
		}

		break;
	default:
		break;
	}

	return false;
}

bool RaceAppearance::IsValidFace(uint16 race_id, uint8 gender_id, uint8 face_value, bool use_luclin)
{
	if (face_value == std::numeric_limits<uint8>::max()) {
		return true;
	}

	switch (GetRaceGenderMask(race_id, gender_id)) {
	case RaceAppearance::HumanMale:
	case RaceAppearance::HumanFemale:
	case RaceAppearance::BarbarianMale:
	case RaceAppearance::BarbarianFemale:
	case RaceAppearance::EruditeMale:
	case RaceAppearance::EruditeFemale:
	case RaceAppearance::WoodElfMale:
	case RaceAppearance::WoodElfFemale:
	case RaceAppearance::HighElfMale:
	case RaceAppearance::HighElfFemale:
	case RaceAppearance::DarkElfMale:
	case RaceAppearance::DarkElfFemale:
	case RaceAppearance::HalfElfMale:
	case RaceAppearance::HalfElfFemale:
	case RaceAppearance::DwarfMale:
	case RaceAppearance::DwarfFemale:
	case RaceAppearance::TrollMale:
	case RaceAppearance::TrollFemale:
	case RaceAppearance::OgreMale:
	case RaceAppearance::OgreFemale:
	case RaceAppearance::HalflingMale:
	case RaceAppearance::HalflingFemale:
	case RaceAppearance::GnomeMale:
	case RaceAppearance::GnomeFemale:
	case RaceAppearance::IksarMale:
	case RaceAppearance::IksarFemale:
	case RaceAppearance::VahShirMale:
	case RaceAppearance::VahShirFemale:
		if (face_value <= 7) {
			return true;
		}

		break;
	default:
		break;
	}

	return false;
}

bool RaceAppearance::IsValidHair(uint16 race_id, uint8 gender_id, uint8 hair_value, bool use_luclin)
{
	if (hair_value == std::numeric_limits<uint8>::max()) {
		return true;
	}

	if (use_luclin) {
		switch (GetRaceGenderMask(race_id, gender_id)) {
		case RaceAppearance::HumanMale:
		case RaceAppearance::HumanFemale:
		case RaceAppearance::BarbarianMale:
		case RaceAppearance::BarbarianFemale:
		case RaceAppearance::WoodElfMale:
		case RaceAppearance::WoodElfFemale:
		case RaceAppearance::HighElfMale:
		case RaceAppearance::HighElfFemale:
		case RaceAppearance::DarkElfMale:
		case RaceAppearance::DarkElfFemale:
		case RaceAppearance::HalfElfMale:
		case RaceAppearance::HalfElfFemale:
		case RaceAppearance::DwarfMale:
		case RaceAppearance::DwarfFemale:
		case RaceAppearance::TrollFemale:
		case RaceAppearance::OgreFemale:
		case RaceAppearance::HalflingMale:
		case RaceAppearance::HalflingFemale:
		case RaceAppearance::GnomeMale:
		case RaceAppearance::GnomeFemale:
			if (hair_value <= 3) {
				return true;
			}

			break;
		case RaceAppearance::EruditeMale:
			if (hair_value <= 5) {
				return true;
			}

			break;
		case RaceAppearance::EruditeFemale:
			if (hair_value <= 8) {
				return true;
			}

			break;
		default:
			break;
		}

		return false;
	}
}

bool RaceAppearance::IsValidHairColor(uint16 race_id, uint8 gender_id, uint8 hair_color_value, bool use_luclin)
{
	if (hair_color_value == std::numeric_limits<uint8>::max()) {
		return true;
	}

	switch (GetRaceGenderMask(race_id, gender_id)) {
	case RaceAppearance::GnomeMale:
	case RaceAppearance::GnomeFemale:
		if (hair_color_value <= 24) {
			return true;
		}

		break;
	case RaceAppearance::TrollFemale:
	case RaceAppearance::OgreFemale:
		if (hair_color_value <= 23) {
			return true;
		}

		break;
	case RaceAppearance::HumanMale:
	case RaceAppearance::HumanFemale:
	case RaceAppearance::BarbarianMale:
	case RaceAppearance::BarbarianFemale:
	case RaceAppearance::WoodElfMale:
	case RaceAppearance::WoodElfFemale:
	case RaceAppearance::HalfElfMale:
	case RaceAppearance::HalfElfFemale:
	case RaceAppearance::DwarfMale:
	case RaceAppearance::DwarfFemale:
	case RaceAppearance::HalflingMale:
	case RaceAppearance::HalflingFemale:
		if (hair_color_value <= 19) {
			return true;
		}

		break;
	case RaceAppearance::DarkElfMale:
	case RaceAppearance::DarkElfFemale:
		if (EQ::ValueWithin(hair_color_value, 13, 18)) {
			return true;
		}

		break;
	case RaceAppearance::HighElfMale:
	case RaceAppearance::HighElfFemale:
		if (hair_color_value <= 14) {
			return true;
		}

		break;
	default:
		break;
	}

	return false;
}

bool RaceAppearance::IsValidWoad(uint16 race_id, uint8 gender_id, uint8 woad_value, bool use_luclin)
{
	if (woad_value == std::numeric_limits<uint8>::max()) {
		return true;
	}

	if (use_luclin) {
		switch (GetRaceGenderMask(race_id, gender_id)) {
		case RaceAppearance::BarbarianMale:
		case RaceAppearance::BarbarianFemale:
			if (woad_value <= 8) {
				return true;
			}

			break;
		default:
			break;
		}
	}

	return false;
}

const char *GetGenderName(uint32 gender_id) {
	const char *gender_name = "Unknown";
	if (gender_id == Gender::Male) {
		gender_name = "Male";
	}
	else if (gender_id == Gender::Female) {
		gender_name = "Female";
	}
	else if (gender_id == Gender::Neuter) {
		gender_name = "Neuter";
	}
	return gender_name;
}

const std::string GetPlayerRaceAbbreviation(uint16 race_id)
{
	if (!IsPlayerRace(race_id)) {
		return std::string("UNK");
	}

	switch (race_id) {
	case Race::Human:
		return "HUM";
	case Race::Barbarian:
		return "BAR";
	case Race::Erudite:
		return "ERU";
	case Race::WoodElf:
		return "ELF";
	case Race::HighElf:
		return "HIE";
	case Race::DarkElf:
		return "DEF";
	case Race::HalfElf:
		return "HEF";
	case Race::Dwarf:
		return "DWF";
	case Race::Troll:
		return "TRL";
	case Race::Ogre:
		return "OGR";
	case Race::Halfling:
		return "HFL";
	case Race::Gnome:
		return "GNM";
	case Race::Iksar:
		return "IKS";
	case Race::VahShir:
		return "VAH";
	}

	return std::string("UNK");
}

bool IsPlayerRace(uint16 race_id) {
	return (
		EQ::ValueWithin(race_id, Race::Human, Race::Gnome) ||
		race_id == Race::Iksar ||
		race_id == Race::VahShir
	);
}