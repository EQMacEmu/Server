#include "player_start_location.h"
#include "../common/races.h"
#include "../common/classes.h"
#include "../common/deity.h"
#include "../common/eq_constants.h"

// this function was extracted and translated from the compiled code in the eqmac intel macintosh client
void FillPlayerStartLocationInfo(PlayerStartLocationInfo *i)
{
	i->heading = 0.0;
	i->y = 0.0;
	i->x = 0.0;
	i->z = 9.0;
	i->zone_id = Zones::QEYNOS;

	// origin location
	switch (i->race)
	{
	case Race::VahShir:
		i->zone_id = Zones::SHARVAHL;
		switch (i->classnum)
		{
		case Class::Warrior:
			i->y = -143.0;
			i->x = -535.0;
			i->z = -260.0;
			i->heading = 128.0;
			break;

		case Class::Bard:
			i->y = 0.0;
			i->x = 550.0;
			i->z = -104.0;
			i->heading = 385.0;
			break;

		case Class::Rogue:
			i->y = 190.0;
			i->x = -300.0;
			i->z = -248.0;
			i->heading = 128.0;
			break;

		case Class::Shaman:
			i->y = 55.0;
			i->x = 100.0;
			i->z = -260.0;
			i->heading = 256.0;
			break;

		case Class::Beastlord:
			i->y = 275.0;
			i->x = 90.0;
			i->z = -260.0;
			i->heading = 0.0;
			break;
		}
		break;

	case Race::Halfling:
		i->zone_id = Zones::RIVERVALE;

		switch (i->classnum)
		{
		case Class::Warrior:
			i->y = 125.0;
			i->x = -157.0;
			i->z = 28.0;
			break;

		case Class::Cleric:
			i->y = -227.0;
			i->x = -381.0;
			i->z = -18.0;
			break;

		case Class::Paladin:
			i->y = -228.0;
			i->x = -309.0;
			i->z = -9.0;
			i->heading = 384.0;
			break;

		case Class::Ranger:
			i->y = -125.0;
			i->x = -320.0;
			i->z = -5.0;
			i->heading = 430.0;
			break;

		case Class::Druid:
			i->y = -104.0;
			i->x = -347.0;
			i->z = -10.0;
			break;

		case Class::Rogue:
			i->y = -34.0;
			i->x = -192.0;
			i->z = 2.0;
			break;
		}
		break;

	case Race::Gnome:
		i->zone_id = Zones::AKANON;

		switch (i->classnum)
		{

		case Class::Warrior:
			i->y = 900.0;
			i->x = -461.0;
			i->z = -30.0;
			break;

		case Class::Cleric:
			i->y = 1190.0;
			i->x = -550.0;
			i->z = -39.0;
			break;

		case Class::Paladin:
			i->y = 1281.0;
			i->x = -559.0;
			i->z = -40.0;
			i->heading = 292.0;
			break;

		case Class::Rogue:
			i->y = 1203.0;
			i->x = -599.0;
			i->z = -53.0;
			break;

		case Class::Wizard:
		case Class::Magician:
		case Class::Enchanter:
			i->y = 1124.0;
			i->x = -1008.0;
			i->z = 32.0;
			break;
		}

		if (i->deity == Deity::Bertoxxulous)
		{
			i->y = 2060.0;
			i->x = -588.0;
			i->z = -181.0;
		}
		break;

	case Race::WoodElf:
		i->zone_id = Zones::GFAYDARK;

		switch (i->classnum)
		{

		case Class::Warrior:
			i->y = 370.0;
			i->x = 304.0;
			i->z = 76.0;
			break;

		case Class::Ranger:
			i->y = -437.0;
			i->x = 501.0;
			i->z = 117.0;
			break;

		case Class::Druid:
			i->y = -698.0;
			i->x = 224.0;
			i->z = 76.0;
			break;

		case Class::Bard:
			i->y = -231.0;
			i->x = 244.0;
			i->z = 76.0;
			break;

		case Class::Rogue:
			i->y = -501.0;
			i->x = -210.0;
			i->z = 160.0;
			break;
		}
		break;

	case Race::Dwarf:
		i->zone_id = Zones::KALADIMB;

		switch (i->classnum)
		{
		case Class::Cleric:
			i->y = 771.0;
			i->x = 132.0;
			i->z = 2.0;
			break;

		case Class::Warrior:
			i->zone_id = Zones::KALADIMA;
			i->y = 41.0;
			i->x = 304.0;
			i->z = 14.0;
			break;

		case Class::Paladin:
			i->y = 1338.0;
			i->x = 113.0;
			i->z = 48.0;
			break;

		case Class::Rogue:
			i->y = 546.0;
			i->x = 218.0;
			i->z = -33.0;
			break;
		}
		break;

	case Race::Ogre:
		i->zone_id = Zones::OGGOK;

		switch (i->classnum)
		{
		case Class::ShadowKnight:
			i->y = 333.0;
			i->x = 9.0;
			i->z = 5.0;
			break;

		case Class::Shaman:
			i->y = 656.0;
			i->x = 1006.0;
			i->z = 80.0;
			break;

		case Class::Beastlord:
			i->y = 288.0;
			i->x = 1074.0;
			i->z = 80.0;
			break;

		case Class::Warrior:
			i->y = -2.0;
			i->x = 71.0;
			i->z = -27.0;
			break;
		}
		break;

	case Race::Troll:
		i->zone_id = Zones::GROBB;

		switch (i->classnum)
		{
		case Class::ShadowKnight:
			i->y = 674.0;
			i->x = -436.0;
			i->z = -6.0;
			break;
		case Class::Shaman:
			i->y = 57.0;
			i->x = -459.0;
			i->z = 54.0;
			break;
		case Class::Beastlord:
			i->y = -49.0;
			i->x = -454.0;
			i->z = 74.0;
			break;
		case Class::Warrior:
			i->y = 299.0;
			i->x = -94.0;
			i->z = 4.0;
			break;
		}
		break;

	case Race::DarkElf:
		i->zone_id = Zones::NERIAKB;

		switch (i->classnum)
		{
		case Class::Warrior:
			i->y = -28.0;
			i->x = -1124.0;
			i->z = -52.0;
			break;

		case Class::Cleric:
			i->zone_id = 42;
			i->y = 539.0;
			i->x = -811.0;
			i->z = -49.0;
			break;

		case Class::ShadowKnight:
		case Class::Necromancer:
			i->zone_id = 42;
			i->y = 1255.0;
			i->x = -1253.0;
			i->z = -80.0;
			break;

		case Class::Rogue:
			i->zone_id = 42;
			i->y = 667.0;
			i->x = -1358.0;
			i->z = -108.0;
			break;

		case Class::Wizard:
			i->y = 165.0;
			i->x = -885.0;
			i->z = -38.0;
			break;

		case Class::Magician:
			i->y = 131.0;
			i->x = -915.0;
			i->z = -38.0;
			break;

		case Class::Enchanter:
			i->y = 148.0;
			i->x = -951.0;
			i->z = -38.0;
			break;
		}
		break;

	case Race::Barbarian:
		i->zone_id = Zones::HALAS;

		switch (i->classnum)
		{
		case Class::Rogue:
			i->y = 186.0;
			i->x = 171.0;
			i->z = 29.0;
			break;

		case Class::Shaman:
			i->y = 334.0;
			i->x = 448.0;
			i->z = -20.0;
			break;

		case Class::Beastlord:
			i->y = 585.0;
			i->x = 208.0;
			i->z = 4.0;
			break;

		case Class::Warrior:
			i->y = 487.0;
			i->x = -448.0;
			i->z = 4.0;
			break;
		}
		break;

	case Race::HighElf:
		switch (i->classnum)
		{
		case Class::Cleric:
			i->zone_id = Zones::FELWITHEA;
			i->y = 14.0;
			i->x = -459.0;
			i->z = -1.0;
			break;

		case Class::Paladin:
			i->zone_id = Zones::FELWITHEA;
			i->y = 0.0;
			i->x = -284.0;
			i->z = 3.0;
			break;

		case Class::Wizard:
		case Class::Magician:
		case Class::Enchanter:
			i->zone_id = Zones::FELWITHEB;
			i->y = 425.0;
			i->x = -601.0;
			i->z = 29.0;
			break;
		}
		break;

	case Race::Erudite:
		switch (i->classnum)
		{
		case Class::Cleric:
			switch (i->deity)
			{
			case Deity::CazicThule:
				i->zone_id = Zones::PAINEEL;
				i->y = 1147.0;
				i->x = 504.0;
				i->z = -39.0;
				break;

			case Deity::Prexus:
				i->zone_id = Zones::ERUDNEXT;
				i->y = -1099.0;
				i->x = -433.0;
				i->z = 66.0;
				i->heading = 128.0;
				break;

			case Deity::Quellious:
				i->zone_id = Zones::ERUDNEXT;
				i->y = -712.0;
				i->x = -68.0;
				i->z = 66.0;
				break;
			}
			break;

		case Class::Paladin:
			i->zone_id = Zones::ERUDNEXT;
			i->y = -654.0;
			i->x = -69.0;
			i->z = 66.0;
			break;

		case Class::ShadowKnight:
			i->zone_id = Zones::PAINEEL;
			i->y = 880.0;
			i->x = 456.0;
			i->z = -121.0;
			break;

		case Class::Necromancer:
			i->zone_id = Zones::PAINEEL;
			i->y = 739.0;
			i->x = 840.0;
			i->z = -25.0;
			break;

		case Class::Wizard:
			i->zone_id = Zones::ERUDNINT;
			i->y = 868.0;
			i->x = 744.0;
			i->z = 51.0;
			break;

		case Class::Magician:
			i->zone_id = Zones::ERUDNINT;
			i->y = 716.0;
			i->x = 920.0;
			i->z = 82.0;
			break;

		case Class::Enchanter:
			i->zone_id = Zones::ERUDNINT;
			i->y = 552.0;
			i->x = 745.0;
			i->z = 51.0;
			break;
		}
		break;

	default:
		if (i->race == Race::Human || i->race == Race::HalfElf)
		{
			switch (i->classnum)
			{
			case Class::Warrior:
				i->zone_id = Zones::QEYNOS;
				i->y = 58.0;
				i->x = -544.0;
				i->z = 32.0;
				if (i->city_ix == 4)
				{
					i->zone_id = Zones::FREPORTW;
					i->y = -279.0;
					i->x = -42.0;
					i->z = -10.0;
				}
				if (i->race == Race::HalfElf && i->city_ix == 9)
				{
					i->zone_id = Zones::GFAYDARK;
					i->y = 370.0;
					i->x = 304.0;
					i->z = 76.0;
				}
				break;

			case Class::Cleric:
				if (i->deity == Deity::RodcetNife)
				{
					i->zone_id = Zones::QEYNOS2;
					i->y = -162.0;
					i->x = -259.0;
					i->z = 4.0;
				}
				else
				{
					i->zone_id = Zones::QEYNOS;
					i->y = -186.0;
					i->x = -441.0;
					i->z = 4.0;
				}
				if (i->city_ix == 4)
				{
					i->zone_id = Zones::FREPORTW;
					i->y = -1.0;
					i->x = 349.0;
					i->z = -9.0;
				}
				break;

			case Class::Paladin:
				if (i->deity == Deity::RodcetNife)
				{
					i->zone_id = Zones::QEYNOS2;
					i->y = -162.0;
					i->x = -259.0;
					i->z = 4.0;
				}
				else if (i->deity == Deity::Karana)
				{
					i->zone_id = Zones::QEYNOS;
					i->y = -186.0;
					i->x = -441.0;
					i->z = 4.0;
				}
				if (i->city_ix == 4)
				{
					i->zone_id = Zones::FREPORTN;
					i->y = -1.0;
					i->x = 349.0;
					i->z = -9.0;
				}
				else if (i->city_ix == 10)
				{
					i->zone_id = Zones::FELWITHEA;
					i->y = 0.0;
					i->x = -248.0;
					i->z = 3.0;
				}
				break;

			case Class::Ranger:
				i->zone_id = Zones::QRG;
				i->y = 146.0;
				i->x = -36.0;
				i->z = 4.0;
				if (i->race == Race::HalfElf && i->city_ix == 9)
				{
					i->zone_id = Zones::GFAYDARK;
					i->y = -437.0;
					i->x = 501.0;
					i->z = 117.0;
				}
				break;

			case Class::ShadowKnight:
			case Class::Necromancer:
				i->zone_id = Zones::QEYNOS;
				i->y = -22.0;
				i->x = -433.0;
				i->z = -24.0;
				if (i->city_ix == 4)
				{
					i->zone_id = Zones::FREPORTE;
					i->y = -141.0;
					i->x = -290.0;
					i->z = -80.0;
				}
				break;

			case Class::Druid:
				i->zone_id = Zones::QRG;
				i->y = -207.0;
				i->x = -301.0;
				i->z = 4.0;
				if (i->race == Race::HalfElf && i->city_ix == 9)
				{
					i->zone_id = Zones::GFAYDARK;
					i->y = -698.0;
					i->x = 224.0;
					i->z = 76.0;
				}
				break;

			case Class::Monk:
				i->zone_id = Zones::QEYNOS2;
				i->y = 342.0;
				i->x = 384.0;
				i->z = 4.0;
				if (i->city_ix == 4)
				{
					i->zone_id = Zones::FREPORTW;
					i->y = -196.0;
					i->x = -790.0;
					i->z = -24.0;
				}
				break;

			case Class::Bard:
				i->zone_id = Zones::QEYNOS;
				i->y = 537.0;
				i->x = -110.0;
				i->z = 4.0;
				if (i->city_ix == 4)
				{
					i->zone_id = Zones::FREPORTN;
					i->y = -92.0;
					i->x = 507.0;
				}
				if (i->race == Race::HalfElf && i->city_ix == 9)
				{
					i->zone_id = Zones::GFAYDARK;
					i->y = -231.0;
					i->x = 244.0;
					i->z = 76.0;
				}
				break;

			case Class::Rogue:
				i->zone_id = Zones::QEYNOS2;
				i->y = 54.0;
				i->x = 358.0;
				i->z = 4.0;
				if (i->city_ix == 4)
				{
					i->zone_id = Zones::FREPORTE;
					i->y = -318.0;
					i->x = -789.0;
					i->z = -109.0;
				}
				if (i->race == Race::HalfElf && i->city_ix == 9)
				{
					i->zone_id = Zones::GFAYDARK;
					i->y = -501.0;
					i->x = -210.0;
					i->z = 160.0;
				}
				break;

			case Class::Wizard:
			case Class::Magician:
			case Class::Enchanter:
				if (i->deity == Deity::Bertoxxulous)
				{
					i->zone_id = Zones::QEYNOS;
					i->y = -22.0;
					i->x = -433.0;
					i->z = -24.0;
				}
				else
				{
					i->zone_id = Zones::QEYNOS;
					i->y = 351.0;
					i->x = -587.0;
					i->z = 4.0;
				}
				if (i->city_ix == 4)
				{
					i->zone_id = Zones::FREPORTW;
					i->y = 97.0;
					i->x = -631.0;
					i->z = -38.0;
				}
				break;
			}
			if (i->deity == Deity::Innoruuk)
			{
				i->zone_id = Zones::FREPORTE;
				i->y = -141.0;
				i->x = -290.0;
				i->z = -80.0;
			}
		}
		else if (i->race == Race::Iksar)
		{
			switch (i->classnum)
			{
			case Class::Warrior:
				i->zone_id = Zones::CABEAST;
				i->y = -531.0;
				i->x = -125.0;
				i->z = 3.0;
				break;

			case Class::ShadowKnight:
				i->zone_id = Zones::CABEAST;
				i->y = 1029.0;
				i->x = -188.0;
				i->z = 72.0;
				break;

			case Class::Monk:
				i->zone_id = Zones::CABEAST;
				i->y = 426.0;
				i->x = -96.0;
				i->z = 5.0;
				break;

			case Class::Shaman:
				i->zone_id = Zones::CABEAST;
				i->y = 1017.0;
				i->x = -189.0;
				i->z = 17.0;
				break;

			case Class::Necromancer:
				i->zone_id = Zones::CABWEST;
				i->y = 311.0;
				i->x = 767.0;
				i->z = 46.0;
				break;

			case Class::Beastlord:
				i->zone_id = Zones::CABEAST;
				i->y = 1032.0;
				i->x = -189.0;
				i->z = 44.0;
				break;

			default:
				i->zone_id = Zones::FIELDOFBONE;
				i->y = 0.0;
				i->x = 0.0;
				i->z = -7.0;
				break;
			}
		}
		break;
	}

	// bind location
	switch (i->zone_id)
	{
	case Zones::QEYNOS:
	case Zones::QEYNOS2:
		i->bind_zone_id = Zones::QEYNOS2;
		i->bind_y = 646.0;
		i->bind_x = -131.0;
		i->bind_z = 5.0;
		break;

	case Zones::FREPORTN:
	case Zones::FREPORTW:
	case Zones::FREPORTE:
		i->bind_zone_id = Zones::FREPORTW;
		i->bind_y = 247.0;
		i->bind_x = 359.0;
		i->bind_z = -22.0;
		break;

	case Zones::RIVERVALE:
		i->bind_zone_id = Zones::MISTY;
		i->bind_y = 271.0;
		i->bind_x = -2126.0;
		i->bind_z = -3.0;
		break;

	case Zones::ERUDNINT:
	case Zones::ERUDNEXT:
		i->bind_zone_id = Zones::TOX;
		i->bind_y = 2230.0;
		i->bind_x = 67.0;
		i->bind_z = -44.0;
		break;

	case Zones::HALAS:
		i->bind_zone_id = Zones::EVERFROST;
		i->bind_y = 3139.0;
		i->bind_x = 629.0;
		i->bind_z = -57.0;
		break;

	case Zones::NERIAKA:
	case Zones::NERIAKB:
	case Zones::NERIAKC:
		i->bind_zone_id = Zones::NEKTULOS;
		i->bind_y = 2048.0;
		i->bind_x = -775.0;
		i->bind_z = 44.0;
		break;

	case Zones::OGGOK:
		i->bind_zone_id = Zones::FEERROTT;
		i->bind_y = 1103.0;
		i->bind_x = 948.0;
		i->bind_z = 31.0;
		break;

	case Zones::GROBB:
		i->bind_zone_id = Zones::INNOTHULE;
		i->bind_y = -2408.0;
		i->bind_x = -339.0;
		i->bind_z = -14.0;
		break;

	case Zones::GFAYDARK:
		i->bind_zone_id = Zones::GFAYDARK;
		i->bind_y = 192.0;
		i->bind_x = 64.0;
		i->bind_z = 35.0;
		break;

	case Zones::AKANON:
		i->bind_zone_id = Zones::STEAMFONT;
		i->bind_y = -1668.0;
		i->bind_x = 636.0;
		i->bind_z = -106.0;
		break;

	case Zones::KALADIMA:
	case Zones::KALADIMB:
		i->bind_zone_id = Zones::BUTCHER;
		i->bind_y = 2610.0;
		i->bind_x = -251.0;
		i->bind_z = 84.0;
		break;

	case Zones::FELWITHEA:
	case Zones::FELWITHEB:
		i->bind_zone_id = Zones::GFAYDARK;
		i->bind_y = -2040.0;
		i->bind_x = -2194.0;
		i->bind_z = 55.0;
		break;

	case Zones::PAINEEL:
		i->bind_zone_id = Zones::PAINEEL;
		i->bind_y = 800.0;
		i->bind_x = 200.0;
		i->bind_z = 0.0;
		break;

	default:
		i->bind_y = 4850.0;
		i->bind_zone_id = Zones::QEYTOQRG;
		i->bind_x = -46.0;
		i->bind_z = 4.0;
		break;
	}
	if (i->race == Race::Iksar)
	{
		i->bind_zone_id = Zones::FIELDOFBONE;
		i->bind_y = -2394.0;
		i->bind_x = 3244.0;
		i->bind_z = 12.0;
	}
	else if (i->race == Race::VahShir)
	{
		i->bind_zone_id = Zones::SHARVAHL;
		i->bind_y = -1132.0;
		i->bind_x = 86.0;
		i->bind_z = -187.0;
	}
}
