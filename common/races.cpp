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

const char* GetRaceIDName(uint16 race_id)
{
	switch (race_id) {
	case RT_ABHORRENT:
		return "Abhorrent";
	case RT_AIR_ELEMENTAL:
		return "Air Elemental";
	case RT_AIR_MEPHIT:
		return "Air Mephit";
	case RT_AKHEVA:
		return "Akheva";
	case RT_ALLIGATOR:
		return "Alligator";
	case RT_AMYGDALAN:
		return "Amygdalan";
	case RT_ANIMATED_ARMOR:
		return "Animated Armor";
	case RT_ANIMATED_HAND:
		return "Animated Hand";
	case RT_ARACHNID:
		return "Arachnid";
	case RT_ARMADILLO:
		return "Armadillo";
	case RT_AVIAK:
		return "Aviak";
	case RT_BANSHEE:
		return "Banshee";
	case RT_BARBARIAN:
	case RT_BARBARIAN_2:
		return "Barbarian";
	case RT_BAT:
	case RT_BAT_2:
		return "Bat";
	case RT_BEAR:
	case RT_BEAR_2:
		return "Bear";
	case RT_BEETLE:
		return "Beetle";
	case RT_BEGGAR:
		return "Beggar";
	case RT_BERTOXXULOUS:
	case RT_BERTOXXULOUS_2:
		return "Bertoxxulous";
	case RT_BIXIE:
		return "Bixie";
	case RT_BLOOD_RAVEN:
		return "Blood Raven";
	case RT_BOAT:
		return "Boat";
	case RT_BOT_PORTAL:
		return "BoT Portal";
	case RT_BRISTLEBANE:
		return "Bristlebane";
	case RT_BROKEN_CLOCKWORK:
		return "Broken Clockwork";
	case RT_BRONTOTHERIUM:
		return "Brontotherium";
	case RT_BROWNIE:
		return "Brownie";
	case RT_BUBONIAN:
		return "Bubonian";
	case RT_BUBONIAN_UNDERLING:
		return "Bubonian Underling";
	case RT_BURYNAI:
		return "Burynai";
	case RT_CAZIC_THULE:
		return "Cazic Thule";
	case RT_CENTAUR:
		return "Centaur";
	case RT_CLAM:
		return "Clam";
	case RT_CLOCKWORK_BEETLE:
		return "Clockwork Beetle";
	case RT_CLOCKWORK_BRAIN:
		return "Clockwork Brain";
	case RT_CLOCKWORK_GNOME:
		return "Clockwork Gnome";
	case RT_CLOCKWORK_GOLEM:
		return "Clockwork Golem";
	case RT_COCKATRICE:
		return "Cockatrice";
	case RT_COLDAIN:
		return "Coldain";
	case RT_CRAB:
		return "Crab";
	case RT_CROCODILE:
		return "Crocodile";
	case RT_CRYSTAL_SPIDER:
		return "Crystal Spider";
	case RT_DAISY_MAN:
		return "Daisy Man";
	case RT_DARK_ELF:
	case RT_DARK_ELF_2:
		return "Dark Elf";
	case RT_DEMI_LICH:
		return "Demi Lich";
	case RT_DERVISH:
		return "Dervish";
	case RT_DEVOURER:
		return "Devourer";
	case RT_DIRE_WOLF:
		return "Dire Wolf";
	case RT_DISEASED_FIEND:
		return "Diseased Fiend";
	case RT_DJINN:
		return "Djinn";
	case RT_DRACHNID:
		return "Drachnid";
	case RT_DRAGLOCK:
		return "Draglock";
	case RT_DRAGON:
	case RT_DRAGON_2:
	case RT_DRAGON_3:
	case RT_DRAGON_4:
	case RT_DRAGON_5:
	case RT_DRAGON_6:
	case RT_DRAGON_7:
	case RT_DRAGON_8:
	case RT_DRAGON_9:
		return "Dragon";
	case RT_DRAKE:
		return "Drake";
	case RT_DRIXIE:
		return "Drixie";
	case RT_DROLVARG:
		return "Drolvarg";
	case RT_DRYAD:
		return "Dryad";
	case RT_DWARF:
	case RT_DWARF_2:
		return "Dwarf";
	case RT_EARTH_ELEMENTAL:
		return "Earth Elemental";
	case RT_EARTH_MEPHIT:
		return "Earth Mephit";
	case RT_EEL:
		return "Eel";
	case RT_EFREETI:
	case RT_EFREETI_2:
		return "Efreeti";
	case RT_ELEMENTAL:
		return "Elemental";
	case RT_ENCHANTED_ARMOR:
		return "Enchanted Armor";
	case RT_EROLLISI:
		return "Erollisi";
	case RT_ERUDITE:
	case RT_ERUDITE_2:
		return "Erudite";
	case RT_EVAN_TEST:
		return "Evan Test";
	case RT_EVIL_EYE:
		return "Evil Eye";
	case RT_EYE:
		return "Eye";
	case RT_FAIRY:
		return "Fairy";
	case RT_FAUN:
		return "Faun";
	case RT_FAY_DRAKE:
		return "Fay Drake";
	case RT_FENNIN_RO:
		return "Fennin Ro";
	case RT_FIEND:
		return "Fiend";
	case RT_FIRE_ELEMENTAL:
		return "Fire Elemental";
	case RT_FIRE_MEPHIT:
		return "Fire Mephit";
	case RT_FISH:
	case RT_FISH_2:
		return "Fish";
	case RT_FLY:
		return "Fly";
	case RT_FROGLOK:
	case RT_FROGLOK_2:
		return "Froglok";
	case RT_FUNGAL_FIEND:
		return "Fungal Fiend";
	case RT_FUNGUSMAN:
		return "Fungusman";
	case RT_GALORIAN:
		return "Galorian";
	case RT_GARGOYLE:
		return "Gargoyle";
	case RT_GASBAG:
		return "Gasbag";
	case RT_GELATINOUS_CUBE:
		return "Gelatinous Cube";
	case RT_GEONID:
		return "Geonid";
	case RT_GHOST:
	case RT_GHOST_2:
	case RT_GHOST_3:
		return "Ghost";
	case RT_GHOST_SHIP:
		return "Ghost Ship";
	case RT_GHOUL:
		return "Ghoul";
	case RT_GIANT:
	case RT_GIANT_2:
	case RT_GIANT_3:
	case RT_GIANT_4:
	case RT_GIANT_5:
	case RT_GIANT_6:
	case RT_GIANT_7:
	case RT_GIANT_8:
	case RT_GIANT_9:
	case RT_GIANT_10:
	case RT_GIANT_11:
		return "Giant";
	case RT_GIANT_CLOCKWORK:
		return "Giant Clockwork";
	case RT_GNOLL:
		return "Gnoll";
	case RT_GNOME:
		return "Gnome";
	case RT_GOBLIN:
	case RT_GOBLIN_2:
	case RT_GOBLIN_3:
		return "Goblin";
	case RT_GOLEM:
		return "Golem";
	case RT_GOO:
		return "Goo";
	case RT_GORGON:
		return "Gorgon";
	case RT_GORILLA:
		return "Gorilla";
	case RT_GRIEG_VENEFICUS:
		return "Grieg Veneficus";
	case RT_GRIFFIN:
		return "Griffin";
	case RT_GRIMLING:
		return "Grimling";
	case RT_GROUND_SHAKER:
		return "Ground Shaker";
	case RT_GUARD:
	case RT_GUARD_2:
	case RT_GUARD_3:
	case RT_GUARD_4:
		return "Guard";
	case RT_GUARD_OF_JUSTICE:
		return "Guard of Justice";
	case RT_HAG:
		return "Hag";
	case RT_HALF_ELF:
		return "Half Elf";
	case RT_HALFLING:
	case RT_HALFLING_2:
		return "Halfling";
	case RT_HARPY:
		return "Harpy";
	case RT_HIGH_ELF:
		return "High Elf";
	case RT_HIPPOGRIFF:
		return "Hippogriff";
	case RT_HOLGRESH:
		return "Holgresh";
	case RT_HORSE:
		return "Horse";
	case RT_HRAQUIS:
		return "Hraquis";
	case RT_HUMAN:
	case RT_HUMAN_2:
	case RT_HUMAN_3:
		return "Human";
	case RT_ICE_SPECTRE:
		return "Ice Spectre";
	case RT_IKSAR:
	case RT_IKSAR_2:
		return "Iksar";
	case RT_IKSAR_GOLEM:
		return "Iksar Golem";
	case RT_IKSAR_SPIRIT:
		return "Iksar Spirit";
	case RT_IMP:
		return "Imp";
	case RT_INNORUUK:
		return "Innoruuk";
	case RT_INVISIBLE_MAN:
		return "Invisible Man";
	case RT_JUNK_BEAST:
		return "Junk Beast";
	case RT_KARANA:
		return "Karana";
	case RT_KEDGE:
		return "Kedge";
	case RT_KERRAN:
		return "Kerran";
	case RT_KNIGHT_OF_PESTILENCE:
		return "Knight of Pestilence";
	case RT_KOBOLD:
		return "Kobold";
	case RT_KRAKEN:
		return "Kraken";
	case RT_LAUNCH:
		return "Launch";
	case RT_LEECH:
		return "Leech";
	case RT_LIGHTCRAWLER:
		return "Lightcrawler";
	case RT_LION:
		return "Lion";
	case RT_LIZARD_MAN:
		return "Lizard Man";
	case RT_MALARIAN:
		return "Malarian";
	case RT_MAMMOTH:
		return "Mammoth";
	case RT_MAN_EATING_PLANT:
		return "Man - Eating Plant";
	case RT_MANTICORE:
		return "Manticore";
	case RT_MERMAID:
		return "Mermaid";
	case RT_MIMIC:
		return "Mimic";
	case RT_MINI_POM:
		return "Mini POM";
	case RT_MINOTAUR:
		return "Minotaur";
	case RT_MITHANIEL_MARR:
		return "Mithaniel Marr";
	case RT_MOSQUITO:
		return "Mosquito";
	case RT_MOUTH_OF_INSANITY:
		return "Mouth of Insanity";
	case RT_NETHERBIAN:
		return "Netherbian";
	case RT_NIGHTMARE:
		return "Nightmare";
	case RT_NIGHTMARE_GARGOYLE:
		return "Nightmare Gargoyle";
	case RT_NIGHTMARE_GOBLIN:
		return "Nightmare Goblin";
	case RT_NIGHTMARE_MEPHIT:
		return "Nightmare Mephit";
	case RT_NIGHTMARE_WRAITH:
		return "Nightmare Wraith";
	case RT_NILBORIEN:
		return "Nilborien";
	case RT_NYMPH:
		return "Nymph";
	case RT_OGRE:
	case RT_OGRE_2:
		return "Ogre";
	case RT_ORC:
		return "Orc";
	case RT_OTHMIR:
		return "Othmir";
	case RT_OWLBEAR:
		return "Owlbear";
	case RT_PEGASUS:
		return "Pegasus";
	case RT_PHOENIX:
		return "Phoenix";
	case RT_PIRANHA:
		return "Piranha";
	case RT_PIXIE:
		return "Pixie";
	case RT_POISON_FROG:
		return "Poison Frog";
	case RT_PUMA:
		return "Puma";
	case RT_PUSLING:
		return "Pusling";
	case RT_RALLOS_ZEK:
	case RT_RALLOS_ZEK_2:
		return "Rallos Zek";
	case RT_RALLOS_ZEK_MINION:
		return "Rallos Zek Minion";
	case RT_RAPTOR:
		return "Raptor";
	case RT_RAT:
		return "Rat";
	case RT_RATMAN:
		return "Ratman";
	case RT_REANIMATED_HAND:
		return "Reanimated Hand";
	case RT_RECUSO:
		return "Recuso";
	case RT_RHINO_BEETLE:
		return "Rhino Beetle";
	case RT_RHINOCEROS:
		return "Rhinoceros";
	case RT_ROCKHOPPER:
		return "Rockhopper";
	case RT_RONNIE_TEST:
		return "Ronnie Test";
	case RT_SABER_TOOTHED_CAT:
		return "Saber - toothed Cat";
	case RT_SARNAK:
		return "Sarnak";
	case RT_SARNAK_GOLEM:
		return "Sarnak Golem";
	case RT_SARNAK_SPIRIT:
		return "Sarnak Spirit";
	case RT_SARYRN:
		return "Saryrn";
	case RT_SCARECROW:
		return "Scarecrow";
	case RT_SCARLET_CHEETAH:
		return "Scarlet Cheetah";
	case RT_SCORPION:
	case RT_SCORPION_2:
		return "Scorpion";
	case RT_SEA_TURTLE:
		return "Sea Turtle";
	case RT_SEAHORSE:
		return "Seahorse";
	case RT_SERU:
		return "Seru";
	case RT_SHADE:
		return "Shade";
	case RT_SHADEL:
		return "Shadel";
	case RT_SHARK:
		return "Shark";
	case RT_SHIKNAR:
		return "Shik'Nar";
	case RT_SHIP:
		return "Ship";

	case RT_SHISSAR:
		return "Shissar";
	case RT_SHRIEKER:
		return "Shrieker";
	case RT_SIREN:
		return "Siren";
	case RT_SKELETAL_HORSE:
		return "Skeletal Horse";
	case RT_SKELETON:
		return "Skeleton";
	case RT_SKUNK:
		return "Skunk";
	case RT_SNAKE:
		return "Snake";
	case RT_SNAKE_ELEMENTAL:
		return "Snake Elemental";
	case RT_SNOW_DERVISH:
		return "Snow Dervish";
	case RT_SNOW_RABBIT:
		return "Snow Rabbit";
	case RT_SOLUSEK_RO:
	case RT_SOLUSEK_RO_2:
		return "Solusek Ro";
	case RT_SOLUSEK_RO_GUARD:
		return "Solusek Ro Guard";
	case RT_SONIC_WOLF:
		return "Sonic Wolf";
	case RT_SOUL_DEVOURER:
		return "Soul Devourer";
	case RT_SPECTRE:
		return "Spectre";
	case RT_SPHINX:
		return "Sphinx";
	case RT_SPIDER:
		return "Spider";
	case RT_STONEGRABBER:
		return "Stonegrabber";
	case RT_STORMRIDER:
		return "Stormrider";
	case RT_SUCCULENT:
		return "Succulent";
	case RT_SUN_REVENANT:
		return "Sun Revenant";
	case RT_SUNFLOWER:
		return "Sunflower";
	case RT_SWORDFISH:
		return "Swordfish";
	case RT_TADPOLE:
		return "Tadpole";
	case RT_TALLON_ZEK:
		return "Tallon Zek";
	case RT_TAREW_MARR:
		return "Tarew Marr";
	case RT_TEGI:
		return "Tegi";
	case RT_TELEPORT_MAN:
		return "Teleport Man";
	case RT_TENTACLE_TERROR:
		return "Tentacle Terror";
	case RT_TERRIS_THULE:
		return "Terris Thule";
	case RT_TEST_OBJECT:
		return "Test Object";
	case RT_THE_RATHE:
		return "The Rathe";
	case RT_THE_TRIBUNAL:
		return "The Tribunal";
	case RT_THOUGHT_HORROR:
		return "Thought Horror";
	case RT_TIGER:
		return "Tiger";
	case RT_TIN_SOLDIER:
		return "Tin Soldier";
	case RT_TORMENTOR:
		return "Tormentor";
	case RT_TOTEM:
		return "Totem";
	case RT_TRAKANON:
		return "Trakanon";
	case RT_TRANQUILION:
		return "Tranquilion";
	case RT_TREANT:
	case RT_TREANT_2:
		return "Treant";
	case RT_TRIBUNAL:
		return "Tribunal";
	case RT_TROLL:
	case RT_TROLL_2:
		return "Troll";
	case RT_TUNARE:
		return "Tunare";
	case RT_ULTHORK:
		return "Ulthork";
	case RT_UNDEAD_FOOTMAN:
		return "Undead Footman";
	case RT_UNDEAD_IKSAR:
		return "Undead Iksar";
	case RT_UNDEAD_KNIGHT:
		return "Undead Knight";
	case RT_UNDEAD_SARNAK:
		return "Undead Sarnak";
	case RT_UNDERBULK:
		return "Underbulk";
	case RT_UNICORN:
		return "Unicorn";
	case RT_UNKNOWN_RACE:
	case RT_UNKNOWN_RACE_2:
	case RT_UNKNOWN_RACE_3:
	case RT_UNKNOWN_RACE_4:
	case RT_UNKNOWN_RACE_5:
		return "UNKNOWN RACE";
	case RT_VAH_SHIR:
	case RT_VAH_SHIR_2:
		return "Vah Shir";
	case RT_VAH_SHIR_SKELETON:
		return "Vah Shir Skeleton";
	case RT_VALLON_ZEK:
		return "Vallon Zek";
	case RT_VALORIAN:
	case RT_VALORIAN_2:
		return "Valorian";
	case RT_VAMPIRE:
	case RT_VAMPIRE_2:
	case RT_VAMPIRE_3:
	case RT_VAMPIRE_4:
		return "Vampire";
	case RT_VEGEROG:
		return "Vegerog";
	case RT_VENRIL_SATHIR:
		return "Venril Sathir";
	case RT_WALRUS:
		return "Walrus";
	case RT_WAR_BOAR:
	case RT_WAR_BOAR_2:
		return "War Boar";
	case RT_WAR_WRAITH:
		return "War Wraith";
	case RT_WASP:
		return "Wasp";
	case RT_WATER_ELEMENTAL:
		return "Water Elemental";
	case RT_WATER_MEPHIT:
		return "Water Mephit";
	case RT_WEREWOLF:
	case RT_WEREWOLF_2:
		return "Werewolf";
	case RT_WETFANG_MINNOW:
		return "Wetfang Minnow";
	case RT_WILL_O_WISP:
		return "Will - O - Wisp";
	case RT_WOLF:
	case RT_WOLF_2:
		return "Wolf";
	case RT_WOOD_ELF:
		return "Wood Elf";
	case RT_WORM:
		return "Worm";
	case RT_WRETCH:
		return "Wretch";
	case RT_WRULON:
		return "Wrulon";
	case RT_WURM:
		return "Wurm";
	case RT_WYVERN:
		return "Wyvern";
	case RT_XALGOZ:
		return "Xalgoz";
	case RT_XEGONY:
		return "Xegony";
	case RT_YAKKAR:
		return "Yakkar";
	case RT_YETI:
		return "Yeti";
	case RT_ZEBUXORUK:
		return "Zebuxoruk";
	case RT_ZEBUXORUKS_CAGE:
		return "Zebuxoruk's Cage";
	case RT_ZELNIAK:
		return "Zelniak";
	case RT_ZOMBIE:
		return "Zombie";
	default:
		return "UNKNOWN RACE";
	}
}

const char* GetRaceIDNamePlural(uint16 race_id)
{
	switch (race_id)
	{
	case HUMAN:
		return "Humans";
	case BARBARIAN:
		return "Barbarians";
	case ERUDITE:
		return "Erudites";
	case WOOD_ELF:
		return "Wood Elves";
	case HIGH_ELF:
		return "High Elves";
	case DARK_ELF:
		return "Dark Elves";
	case HALF_ELF:
		return "Half Elves";
	case DWARF:
		return "Dwarves";
	case TROLL:
		return "Trolls";
	case OGRE:
		return "Ogres";
	case HALFLING:
		return "Halflings";
	case GNOME:
		return "Gnomes";
	case IKSAR:
		return "Iksar";
	case VAHSHIR:
		return "Vah Shir";
	default:
		return "Races"; break;
	}
}

const char* GetPlayerRaceName(uint32 player_race_value)
{
	return GetRaceIDName(GetRaceIDFromPlayerRaceValue(player_race_value));
}


uint32 GetPlayerRaceValue(uint16 race_id) {
	switch (race_id) {
		case HUMAN:
		case BARBARIAN:
		case ERUDITE:
		case WOOD_ELF:
		case HIGH_ELF:
		case DARK_ELF:
		case HALF_ELF:
		case DWARF:
		case TROLL:
		case OGRE:
		case HALFLING:
		case GNOME:
			return race_id;
		case IKSAR:
			return PLAYER_RACE_IKSAR;
		case VAHSHIR:
			return PLAYER_RACE_VAHSHIR;
		default:
			return PLAYER_RACE_UNKNOWN; // watch
	}
}

uint32 GetPlayerRaceBit(uint16 race_id) {
	switch (race_id) {
		case HUMAN:
			return PLAYER_RACE_HUMAN_BIT;
		case BARBARIAN:
			return PLAYER_RACE_BARBARIAN_BIT;
		case ERUDITE:
			return PLAYER_RACE_ERUDITE_BIT;
		case WOOD_ELF:
			return PLAYER_RACE_WOOD_ELF_BIT;
		case HIGH_ELF:
			return PLAYER_RACE_HIGH_ELF_BIT;
		case DARK_ELF:
			return PLAYER_RACE_DARK_ELF_BIT;
		case HALF_ELF:
			return PLAYER_RACE_HALF_ELF_BIT;
		case DWARF:
			return PLAYER_RACE_DWARF_BIT;
		case TROLL:
			return PLAYER_RACE_TROLL_BIT;
		case OGRE:
			return PLAYER_RACE_OGRE_BIT;
		case HALFLING:
			return PLAYER_RACE_HALFLING_BIT;
		case GNOME:
			return PLAYER_RACE_GNOME_BIT;
		case IKSAR:
			return PLAYER_RACE_IKSAR_BIT;
		case VAHSHIR:
			return PLAYER_RACE_VAHSHIR_BIT;
		default:
			return PLAYER_RACE_UNKNOWN_BIT;
	}
}

uint16 GetRaceIDFromPlayerRaceValue(uint32 player_race_value) {
	switch (player_race_value) {
		case PLAYER_RACE_HUMAN:
		case PLAYER_RACE_BARBARIAN:
		case PLAYER_RACE_ERUDITE:
		case PLAYER_RACE_WOOD_ELF:
		case PLAYER_RACE_HIGH_ELF:
		case PLAYER_RACE_DARK_ELF:
		case PLAYER_RACE_HALF_ELF:
		case PLAYER_RACE_DWARF:
		case PLAYER_RACE_TROLL:
		case PLAYER_RACE_OGRE:
		case PLAYER_RACE_HALFLING:
		case PLAYER_RACE_GNOME:
			return player_race_value;
		case PLAYER_RACE_IKSAR:
			return IKSAR;
		case PLAYER_RACE_VAHSHIR:
			return VAHSHIR;
		default:
			return PLAYER_RACE_UNKNOWN; // watch
	}
}

uint16 GetRaceIDFromPlayerRaceBit(uint32 player_race_bit)
{
	switch (player_race_bit) {
	case PLAYER_RACE_HUMAN_BIT:
		return HUMAN;
	case PLAYER_RACE_BARBARIAN_BIT:
		return BARBARIAN;
	case PLAYER_RACE_ERUDITE_BIT:
		return ERUDITE;
	case PLAYER_RACE_WOOD_ELF_BIT:
		return WOOD_ELF;
	case PLAYER_RACE_HIGH_ELF_BIT:
		return HIGH_ELF;
	case PLAYER_RACE_DARK_ELF_BIT:
		return DARK_ELF;
	case PLAYER_RACE_HALF_ELF_BIT:
		return HALF_ELF;
	case PLAYER_RACE_DWARF_BIT:
		return DWARF;
	case PLAYER_RACE_TROLL_BIT:
		return TROLL;
	case PLAYER_RACE_OGRE_BIT:
		return OGRE;
	case PLAYER_RACE_HALFLING_BIT:
		return HALFLING;
	case PLAYER_RACE_GNOME_BIT:
		return GNOME;
	case PLAYER_RACE_IKSAR_BIT:
		return IKSAR;
	case PLAYER_RACE_VAHSHIR_BIT:
		return VAHSHIR;
	default:
		return PLAYER_RACE_UNKNOWN; // watch
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

	if (gender == 1)
		return female_height[race];

	return male_height[race];
}

// PlayerAppearance prep
#define HUMAN_MALE ((HUMAN << 8) | MALE)
#define HUMAN_FEMALE ((HUMAN << 8) | FEMALE)
#define BARBARIAN_MALE ((BARBARIAN << 8) | MALE)
#define BARBARIAN_FEMALE ((BARBARIAN << 8) | FEMALE)
#define ERUDITE_MALE ((ERUDITE << 8) | MALE)
#define ERUDITE_FEMALE ((ERUDITE << 8) | FEMALE)
#define WOOD_ELF_MALE ((WOOD_ELF << 8) | MALE)
#define WOOD_ELF_FEMALE ((WOOD_ELF << 8) | FEMALE)
#define HIGH_ELF_MALE ((HIGH_ELF << 8) | MALE)
#define HIGH_ELF_FEMALE ((HIGH_ELF << 8) | FEMALE)
#define DARK_ELF_MALE ((DARK_ELF << 8) | MALE)
#define DARK_ELF_FEMALE ((DARK_ELF << 8) | FEMALE)
#define HALF_ELF_MALE ((HALF_ELF << 8) | MALE)
#define HALF_ELF_FEMALE ((HALF_ELF << 8) | FEMALE)
#define DWARF_MALE ((DWARF << 8) | MALE)
#define DWARF_FEMALE ((DWARF << 8) | FEMALE)
#define TROLL_MALE ((TROLL << 8) | MALE)
#define TROLL_FEMALE ((TROLL << 8) | FEMALE)
#define OGRE_MALE ((OGRE << 8) | MALE)
#define OGRE_FEMALE ((OGRE << 8) | FEMALE)
#define HALFLING_MALE ((HALFLING << 8) | MALE)
#define HALFLING_FEMALE ((HALFLING << 8) | FEMALE)
#define GNOME_MALE ((GNOME << 8) | MALE)
#define GNOME_FEMALE ((GNOME << 8) | FEMALE)
#define IKSAR_MALE ((IKSAR << 8) | MALE)
#define IKSAR_FEMALE ((IKSAR << 8) | FEMALE)
#define VAHSHIR_MALE ((VAHSHIR << 8) | MALE)
#define VAHSHIR_FEMALE ((VAHSHIR << 8) | FEMALE)

#define BINDRG(r, g) (((int)r << 8) | g)


bool PlayerAppearance::IsValidBeard(uint16 race_id, uint8 gender_id, uint8 beard_value, bool use_luclin)
{
	if (beard_value == 0xFF)
		return true;

	if (use_luclin) {
		switch (BINDRG(race_id, gender_id)) {
		case DWARF_FEMALE:
			if (beard_value <= 1)
				return true;
			break;
		case HIGH_ELF_MALE:
		case DARK_ELF_MALE:
		case HALF_ELF_MALE:
			if (beard_value <= 3)
				return true;
			break;
		case HUMAN_MALE:
		case BARBARIAN_MALE:
		case ERUDITE_MALE:
		case DWARF_MALE:
		case HALFLING_MALE:
		case GNOME_MALE:
			if (beard_value <= 5)
				return true;
			break;
		default:
			break;
		}
		return false;
	}
	else {
		return false;
	}
}

bool PlayerAppearance::IsValidBeardColor(uint16 race_id, uint8 gender_id, uint8 beard_color_value, bool use_luclin)
{
	if (beard_color_value == 0xFF)
	return true;
	
	switch (BINDRG(race_id, gender_id)) {
	case GNOME_MALE:
		if (beard_color_value <= 24)
			return true;
		break;
	case HUMAN_MALE:
	case BARBARIAN_MALE:
	case ERUDITE_MALE:
	case HALF_ELF_MALE:
	case DWARF_MALE:
	case DWARF_FEMALE:
	case HALFLING_MALE:
		if (beard_color_value <= 19)
			return true;
		break;
	case DARK_ELF_MALE:
		if (beard_color_value >= 13 &&  beard_color_value <= 18)
			return true;
		break;
	case HIGH_ELF_MALE:
		if (beard_color_value <= 14)
			return true;
		break;
		if (beard_color_value <= 3)
			return true;
		break;
	default:
		break;
	}
	return false;
}

bool PlayerAppearance::IsValidDetail(uint16 race_id, uint8 gender_id, uint32 detail_value, bool use_luclin)
{
	return false;
}

bool PlayerAppearance::IsValidEyeColor(uint16 race_id, uint8 gender_id, uint8 eye_color_value, bool use_luclin)
{
	return true; // need valid criteria

	switch (BINDRG(race_id, gender_id)) {
	case HUMAN_MALE:
	case HUMAN_FEMALE:
	case BARBARIAN_MALE:
	case BARBARIAN_FEMALE:
	case ERUDITE_MALE:
	case ERUDITE_FEMALE:
	case WOOD_ELF_MALE:
	case WOOD_ELF_FEMALE:
	case HIGH_ELF_MALE:
	case HIGH_ELF_FEMALE:
	case DARK_ELF_MALE:
	case DARK_ELF_FEMALE:
	case HALF_ELF_MALE:
	case HALF_ELF_FEMALE:
	case DWARF_MALE:
	case DWARF_FEMALE:
	case OGRE_MALE:
	case OGRE_FEMALE:
	case HALFLING_MALE:
	case HALFLING_FEMALE:
	case GNOME_MALE:
	case GNOME_FEMALE:
	case IKSAR_MALE:
	case IKSAR_FEMALE:
	case VAHSHIR_MALE:
	case VAHSHIR_FEMALE:
		if (eye_color_value <= 9)
			return true;
		break;	
	case TROLL_MALE:
	case TROLL_FEMALE:
		if (eye_color_value <= 10)
			return true;
		break;
	default:
		break;
	}
	return false;
}

bool PlayerAppearance::IsValidFace(uint16 race_id, uint8 gender_id, uint8 face_value, bool use_luclin)
{
	if (face_value == 0xFF)
		return true;

	switch (BINDRG(race_id, gender_id)) {
	case HUMAN_MALE:
	case HUMAN_FEMALE:
	case BARBARIAN_MALE:
	case BARBARIAN_FEMALE:
	case ERUDITE_MALE:
	case ERUDITE_FEMALE:
	case WOOD_ELF_MALE:
	case WOOD_ELF_FEMALE:
	case HIGH_ELF_MALE:
	case HIGH_ELF_FEMALE:
	case DARK_ELF_MALE:
	case DARK_ELF_FEMALE:
	case HALF_ELF_MALE:
	case HALF_ELF_FEMALE:
	case DWARF_MALE:
	case DWARF_FEMALE:
	case TROLL_MALE:
	case TROLL_FEMALE:
	case OGRE_MALE:
	case OGRE_FEMALE:
	case HALFLING_MALE:
	case HALFLING_FEMALE:
	case GNOME_MALE:
	case GNOME_FEMALE:
	case IKSAR_MALE:
	case IKSAR_FEMALE:
	case VAHSHIR_MALE:
	case VAHSHIR_FEMALE:
		if (face_value <= 7)
			return true;
		break;
	default:
		break;
	}
	return false;
}

bool PlayerAppearance::IsValidHair(uint16 race_id, uint8 gender_id, uint8 hair_value, bool use_luclin)
{
	if (hair_value == 0xFF)
		return true;

	if (use_luclin) {
		switch (BINDRG(race_id, gender_id)) {
		case HUMAN_MALE:
		case HUMAN_FEMALE:
		case BARBARIAN_MALE:
		case BARBARIAN_FEMALE:
		case WOOD_ELF_MALE:
		case WOOD_ELF_FEMALE:
		case HIGH_ELF_MALE:
		case HIGH_ELF_FEMALE:
		case DARK_ELF_MALE:
		case DARK_ELF_FEMALE:
		case HALF_ELF_MALE:
		case HALF_ELF_FEMALE:
		case DWARF_MALE:
		case DWARF_FEMALE:
		case TROLL_FEMALE:
		case OGRE_FEMALE:
		case HALFLING_MALE:
		case HALFLING_FEMALE:
		case GNOME_MALE:
		case GNOME_FEMALE:
			if (hair_value <= 3)
				return true;
			break;
		case ERUDITE_MALE:
			if (hair_value <= 5)
				return true;
			break;
		case ERUDITE_FEMALE:
			if (hair_value <= 8)
				return true;
			break;
		default:
			break;
		}
		return false;
	}
	else {
		return false;
	}
}

bool PlayerAppearance::IsValidHairColor(uint16 race_id, uint8 gender_id, uint8 hair_color_value, bool use_luclin)
{
	if (hair_color_value == 0xFF)
		return true;

	switch (BINDRG(race_id, gender_id)) {
	case GNOME_MALE:
	case GNOME_FEMALE:
		if (hair_color_value <= 24)
			return true;
		break;
	case TROLL_FEMALE:
	case OGRE_FEMALE:
		if (hair_color_value <= 23)
			return true;
		break;
	case HUMAN_MALE:
	case HUMAN_FEMALE:
	case BARBARIAN_MALE:
	case BARBARIAN_FEMALE:
	case WOOD_ELF_MALE:
	case WOOD_ELF_FEMALE:
	case HALF_ELF_MALE:
	case HALF_ELF_FEMALE:
	case DWARF_MALE:
	case DWARF_FEMALE:
	case HALFLING_MALE:
	case HALFLING_FEMALE:
		if (hair_color_value <= 19)
			return true;
		break;
	case DARK_ELF_MALE:
	case DARK_ELF_FEMALE:
		if (hair_color_value >= 13 && hair_color_value <= 18)
			return true;
		break;
	case HIGH_ELF_MALE:
	case HIGH_ELF_FEMALE:
		if (hair_color_value <= 14)
			return true;
		break;
	default:
		break;
	}
	return false;
}

bool PlayerAppearance::IsValidHead(uint16 race_id, uint8 gender_id, uint8 head_value, bool use_luclin)
{
	if (head_value == 0xFF)
		return true;

	if (use_luclin) {
		switch (BINDRG(race_id, gender_id)) {
		case HUMAN_MALE:
		case HUMAN_FEMALE:
		case BARBARIAN_MALE:
		case BARBARIAN_FEMALE:
		case WOOD_ELF_MALE:
		case WOOD_ELF_FEMALE:
		case HIGH_ELF_MALE:
		case HIGH_ELF_FEMALE:
		case DARK_ELF_MALE:
		case DARK_ELF_FEMALE:
		case HALF_ELF_MALE:
		case HALF_ELF_FEMALE:
		case DWARF_MALE:
		case DWARF_FEMALE:
		case TROLL_MALE:
		case TROLL_FEMALE:
		case OGRE_MALE:
		case OGRE_FEMALE:
		case HALFLING_MALE:
		case HALFLING_FEMALE:
		case GNOME_MALE:
		case GNOME_FEMALE:
		case IKSAR_MALE:
		case IKSAR_FEMALE:
		case VAHSHIR_MALE:
		case VAHSHIR_FEMALE:
			if (head_value <= 3)
				return true;
			break;
		case ERUDITE_MALE:
		case ERUDITE_FEMALE:
			if (head_value <= 4)
				return true;
			break;
		default:
			break;
		}
		return false;
	}
	else {
		switch (BINDRG(race_id, gender_id)) {
		case HUMAN_MALE:
		case HUMAN_FEMALE:
		case BARBARIAN_MALE:
		case BARBARIAN_FEMALE:
		case ERUDITE_MALE:
		case ERUDITE_FEMALE:
		case WOOD_ELF_MALE:
		case WOOD_ELF_FEMALE:
		case HIGH_ELF_MALE:
		case HIGH_ELF_FEMALE:
		case DARK_ELF_MALE:
		case DARK_ELF_FEMALE:
		case HALF_ELF_MALE:
		case HALF_ELF_FEMALE:
		case DWARF_MALE:
		case DWARF_FEMALE:
		case TROLL_MALE:
		case TROLL_FEMALE:
		case OGRE_MALE:
		case OGRE_FEMALE:
		case HALFLING_MALE:
		case HALFLING_FEMALE:
		case IKSAR_MALE:
		case IKSAR_FEMALE:
		case VAHSHIR_MALE:
		case VAHSHIR_FEMALE:
			if (head_value <= 3)
				return true;
			break;
		case GNOME_MALE:
		case GNOME_FEMALE:
			if (head_value <= 4)
				return true;
			break;
		default:
			break;
		}
		return false;
	}
}

bool PlayerAppearance::IsValidWoad(uint16 race_id, uint8 gender_id, uint8 woad_value, bool use_luclin)
{
	if (woad_value == 0xFF)
		return true;

	if (use_luclin) {
		switch (BINDRG(race_id, gender_id)) {
		case BARBARIAN_MALE:
		case BARBARIAN_FEMALE:
			if (woad_value <= 8)
				return true;
			break;
		default:
			break;
		}
	}
	return false;
}

const char* GetGenderName(uint32 gender_id) {
	const char* gender_name = "Unknown";
	if (gender_id == MALE) {
		gender_name = "Male";
	}
	else if (gender_id == FEMALE) {
		gender_name = "Female";
	}
	else if (gender_id == NEUTER) {
		gender_name = "Neuter";
	}
	return gender_name;
}

const bool IsFlyingCreatureRace(const uint16 race_id) {
	switch (race_id) {
		case RT_GHOST:
		case RT_GHOST_2:
		case RT_GHOST_3:
		case RT_GARGOYLE:
		case RT_SPECTRE:
		case RT_ICE_SPECTRE:
		case RT_IMP:
		case RT_HARPY:
		case RT_PEGASUS:
		case RT_DJINN:
		case RT_GRIFFIN:
		case RT_PIXIE:
		case RT_FAIRY:
		case RT_WILL_O_WISP:
		case RT_AIR_ELEMENTAL:
		case RT_WASP:
		case RT_BIXIE:
		case RT_DRAKE:
		case RT_FAY_DRAKE:
		case RT_REANIMATED_HAND:
		case RT_DRAGON:
		case RT_DRAGON_2:
		case RT_DRAGON_3:
		case RT_DRAGON_4:
		case RT_DRAGON_5:
		case RT_DRAGON_6:
		case RT_DRAGON_7:
		case RT_DRAGON_8:
		case RT_DRAGON_9:
		case RT_EVIL_EYE:
		case RT_FISH:
		case RT_FISH_2:
		case RT_SHARK:
		case RT_SWORDFISH:
		case RT_DERVISH:
		case RT_SNOW_DERVISH:
		case RT_BAT:
		case RT_BAT_2:
		case RT_AVIAK:
			return true;
		default:
			return false;
	}
}
