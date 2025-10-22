#include <cstring>
#include <stdlib.h>
#include "strings.h"
#include "name_generator.h"

// this is translated from the eqmac client code

bool NameGen = 1; // additional validation to disallow strings of consonants, set on client in SendLogServer(), normally on

const char *name_tables[] =
{
	// Pet
	"K", "ab", "ar", "tik", "a",
	"G", "on", "an", "er", "u",
	"J", "ib", "ek", "n", "y",
	"V", "ab", "ar", "tik", "a",
	"G", "on", "an", "er", "u",
	"X", "as", "ek", "n", "y",
	"K", "ar", "ar", "tik", "a",
	"Z", "ob", "an", "er", "u",
	"J", "eb", "ek", "n", "y",
	"L", "en", "ob", "ab", "i",

	// Human
	"D", "en", "e", "lron", "a",
	"B", "al", "ad", "orn", "y",
	"L", "an", "a", "dor", "e",
	"G", "en", "el", "en", "i",
	"S", "ol", "an", "dil", "la",
	"F", "e", "o", "dar", "e",
	"R", "on", "ad", "an", "ie",
	"A", "ar", "th", "in", "y",
	"G", "am", "al", "iel", "ae",
	"C", "ae", "la", "eb", "i",

	// Barbarian
	"J", "ar", "ud", "an", "a",
	"H", "al", "ad", "in", "u",
	"L", "ud", "am", "il", "y",
	"B", "en", "gur", "en", "a",
	"S", "or", "da", "orf", "i",
	"F", "um", "am", "dar", "i",
	"T", "in", "da", "orm", "ie",
	"K", "il", "ed", "an", "ea",
	"G", "an", "el", "or", "a",
	"W", "al", "la", "ce", "a",

	// Erudite
	"A", "b", "er", "ar", "a",
	"E", "rr", "ud", "in", "u",
	"U", "z", "u", "n", "e",
	"Yi", "t", "um", "no", "ya",
	"I", "zz", "nus", "ien", "la",
	"O", "um", "am", "ar", "ka",
	"Fa", "ni", "da", "an", "ki",
	"I", "li", "ad", "en", "eth",
	"Ni", "bi", "el", "yen", "a",
	"Aa", "mm", "la", "zen", "i",

	// Wood Elf
	"M", "en", "er", "ian", "a",
	"W", "al", "ad", "in", "u",
	"L", "an", "we", "il", "e",
	"T", "eg", "di", "en", "ya",
	"U", "ul", "uv", "ien", "iel",
	"A", "lu", "am", "dar", "ye",
	"K", "in", "da", "an", "ie",
	"I", "il", "ol", "as", "ea",
	"N", "ea", "lu", "yen", "ae",
	"G", "ae", "la", "wen", "i",

	// High Elf
	"Q", "ue", "ny", "ar", "a",
	"E", "al", "ad", "in", "u",
	"L", "an", "am", "il", "e",
	"Y", "en", "di", "en", "ya",
	"S", "al", "dy", "ien", "iel",
	"A", "um", "am", "dar", "ye",
	"F", "in", "da", "an", "ie",
	"I", "il", "ad", "in", "ea",
	"N", "an", "el", "yen", "ae",
	"C", "ae", "la", "wen", "i",

	// Dark Elf
	"Ut", "eu", "nay", "ar", "aa",
	"V", "al", "ad", "ael", "u",
	"M", "an", "am", "il", "au",
	"U", "en", "di", "en", "ya",
	"S", "ael", "day", "r", "ie",
	"A", "um", "ael", "dar", "ye",
	"T", "in", "da", "an", "iu",
	"Br", "il", "uu", "in", "ea",
	"N", "an", "el", "yan", "ae",
	"K", "ae", "la", "ven", "ia",

	// Half Elf
	"P", "en", "er", "ian", "a",
	"W", "al", "ad", "in", "u",
	"L", "an", "da", "il", "e",
	"T", "em", "id", "en", "ya",
	"S", "ul", "uv", "ien", "i",
	"R", "u", "am", "dar", "aya",
	"K", "in", "b", "an", "y",
	"D", "in", "ol", "as", "a",
	"M", "e", "lu", "yen", "ai",
	"B", "a", "var", "wen", "i",

	// Dwarf
	"H", "in", "dol", "af", "a",
	"D", "al", "ad", "in", "u",
	"K", "an", "am", "ek", "y",
	"H", "in", "dol", "ur", "a",
	"D", "al", "ad", "ar", "u",
	"K", "ak", "an", "ek", "y",
	"A", "in", "dol", "af", "a",
	"D", "ar", "th", "in", "u",
	"K", "ik", "am", "ek", "y",
	"B", "or", "da", "mer", "i",

	// Troll
	"G", "u", "r", "uk", "a",
	"K", "i", "g", "al", "u",
	"B", "a", "k", "ak", "o",
	"R", "an", "z", "ek", "u",
	"N", "am", "m", "ab", "au",
	"G", "um", "l", "az", "a",
	"Z", "ak", "b", "an", "u",
	"G", "ag", "uk", "uk", "a",
	"B", "az", "bk", "ak", "u",
	"K", "u", "la", "um", "o",

	// Ogre
	"O", "g", "ur", "ak", "u",
	"T", "ar", "m", "og", "a",
	"Z", "uk", "am", "ol", "u",
	"B", "ha", "ag", "ok", "a",
	"R", "hu", "ar", "gar", "u",
	"G", "um", "tu", "uk", "a",
	"O", "im", "gr", "an", "u",
	"K", "ug", "ob", "im", "o",
	"N", "ag", "gul", "buk", "a",
	"R", "og", "la", "bok", "i",

	// Halfling
	"M", "a", "rr", "in", "a",
	"B", "e", "tt", "en", "u",
	"D", "i", "nn", "il", "e",
	"F", "o", "bb", "en", "eth",
	"S", "u", "ff", "on", "ry",
	"H", "ae", "kk", "as", "dy",
	"G", "ai", "pp", "o", "ie",
	"T", "ei", "ll", "win", "ea",
	"N", "ea", "gg", "an", "y",
	"C", "ee", "dd", "un", "i",

	// Gnome
	"M", "is", "tal", "bik", "a",
	"Z", "al", "as", "tin", "u",
	"W", "an", "am", "til", "e",
	"Y", "en", "di", "ten", "ya",
	"S", "al", "ik", "ben", "iel",
	"B", "at", "ma", "tik", "ye",
	"F", "in", "da", "bus", "ie",
	"X", "il", "um", "pus", "ea",
	"N", "ab", "li", "ren", "y",
	"C", "ur", "la", "kin", "i",

	// Iksar
	"K", "ab", "ar", "tiz", "a",
	"X", "eb", "an", "il", "u",
	"J", "ik", "sha", "is", "i",
	"C", "atr", "kor", "iz", "a",
	"S", "il", "an", "sis", "u",
	"R", "as", "th", "le", "y",
	"Ch", "ar", "as", "rn", "a",
	"Z", "uk", "ra", "ik", "u",
	"S", "or", "ek", "ak", "y",
	"M", "ak", "ul", "az", "i",

	// Vah Shir
	"K", "a", "ra", "sh", "a",
	"G", "o", "na", "r", "u",
	"J", "i", "ta", "n", "i",
	"H", "e", "ri", "yn", "a",
	"S", "u", "n", "is", "u",
	"R", "a", "k", "n", "ya",
	"K", "a", "ra", "th", "a",
	"Z", "u", "na", "l", "u",
	"T", "e", "ka", "n", "ia",
	"L", "a", "ba", "ab", "i"
};

const char *combo_exclusions[] =
{
	// Pet
	"jt", "lt", "gt", "kt", "kt",
	"Jt", "Lt", "Gt", "Kt", "kt",
	"jt", "lt", "gt", "kt", "kt",
	"jt", "lt", "gt", "kt", "kt",
	"jt", "lt", "gt", "kt", "kt",

	// Human
	"Dm", "Gd", "Bd", "Fd", "Dd",
	"dlr", "Aaee", "mlr", "aeie", "Dth",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Fth", "Bm", "aeoo", "Bth", "Dd",

	// Barbarian
	"Dm", "Bm", "Wd", "Bd", "Dd",
	"Td", "Bg", "Bm", "Bm", "Fd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dm", "Bm", "Bm", "Tl", "Jd",

	// Erudite
	"Dm", "Bm", "Bm", "Bm", "Dd",
	"Dm", "Bm", "Bm", "Bm", "Dd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dm", "Bm", "Bm", "Bm", "Dd",

	// Wood Elf
	"Dm", "Bm", "Mw", "Wl", "Dd",
	"Ml", "Md", "Bm", "Bm", "Wd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dm", "Bm", "Bm", "Bm", "Td",

	// High Elf
	"Dm", "Bm", "Ld", "Yd", "Dd",
	"Cd", "Nm", "Sd", "Qd", "Dd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dm", "Bm", "Bm", "Bm", "Dd",

	// Dark Elf
	"Dm", "Bm", "Bm", "Bm", "Dd",
	"Sd", "Bm", "Bm", "Bm", "Dd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dm", "Bm", "Bm", "Bm", "Dd",

	// Half Elf
	"Dm", "Bm", "Wb", "Rd", "Rb",
	"Pd", "nbd", "Tb", "Td", "Dd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dm", "Bb", "Bm", "Bm", "Dd",

	// Dwarf
	"bd", "bf", "hd", "kd", "fy",
	"Bd", "Bf", "Hd", "Kd", "Ht",
	"Dd", "Df", "Hm", "Km", "Dt",
	"Dd", "Df", "Hm", "Km", "Dt",
	"Dm", "Bm", "Dt", "Ht", "Kt",

	// Troll
	"Dm", "Bm", "Bm", "Bm", "Dd",
	"Dm", "Bm", "Bm", "Bm", "Dd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dm", "Bm", "Bm", "Bm", "Dd",

	// Ogre
	"Dm", "Bm", "Bg", "Gg", "Dd",
	"Dm", "Bm", "Bm", "Bm", "Dd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dm", "Bm", "Bm", "Bm", "Dd",

	// Halfling
	"Dm", "Bm", "Bp", "Fd", "Dd",
	"Fp", "Cf", "Sf", "Hn", "Hf",
	"Db", "Hp", "Fff", "Dgg", "Hf",
	"Mt", "Df", "Stt", "Fb", "Np",
	"Bf", "Bt", "Bll", "Fk", "Cg",

	// Gnome
	"Dm", "Bm", "Nm", "Bm", "Dd",
	"pusy", "pusi", "puse", "Bm", "Sd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dd", "Df", "Hm", "Km", "Kd",
	"Dm", "Cd", "Wt", "Bm", "Nd",

	// Iksar
	"jt", "lt", "gt", "kt", "kt",
	"Jt", "Lt", "Gt", "Kt", "kt",
	"jt", "lt", "gt", "kt", "kt",
	"jt", "lt", "gt", "kt", "kt",
	"jt", "lt", "gt", "kt", "kt",

	// Vah Shir
	"jt", "lt", "gt", "kt", "kt",
	"Jt", "Lt", "Gt", "Kt", "Kk",
	"Trash", "lt", "gt", "kt", "Sn",
	"Karana", "lt", "gt", "kt", "kt",
	"jt", "lt", "gt", "kt", "kt"
};

const char *bad_name_exclusions[] =
{
	"uck", "shit", "ussy", "fag", "phuk",
	"unt", "jew", "nigger", "niger", "fuc",
	"hole", "tits", "piss", "ock", "fuk",
	"enis", "phag", "uhc", "phuc", "uhk",
	"nigga", "damien", "dpp", "labia", "satan"
};
int bad_name_excl_count = 25;

EQNameGen::EQNameGen(EQ::Random random)
{
	m_random = random;
}

int EQNameGen::RandNbr()
{
	return m_random.Roll0(RAND_MAX);
}

bool EQNameGen::IsVowel(char c)
{
	return c == 'a'
		|| c == 'e'
		|| c == 'i'
		|| c == 'o'
		|| c == 'u'
		|| c == 'y'
		|| c == 'A'
		|| c == 'E'
		|| c == 'I'
		|| c == 'O'
		|| c == 'U'
		|| c == 'Y';
}

char EQNameGen::ReturnRandomVowel()
{
	char vowels[8];

	strcpy(vowels, "aeiouy");

	return vowels[RandNbr() % 6];
}

char EQNameGen::NameContains(const char *haystack, const char *needle, char ignorecase)
{
	char needle2[256];
	char haystack2[256];

	if (!haystack || !needle)
		return 0;

	strcpy(haystack2, haystack);
	strcpy(needle2, needle);

	if (ignorecase)
	{
		MakeLowerString(haystack2, haystack2);
		MakeLowerString(needle2, needle2);
	}

	if (strstr(haystack2, needle2))
		return 1;

	return 0;
}

char EQNameGen::NameFailsOnBasicChecks(char *Str)
{
	if (!Str) return 1;

	int len = strlen(Str);

	for (int str_ix = 0; str_ix < len; str_ix++)
	{
		// check for double characters
		if (str_ix + 2 < len)
		{
			char c = Str[str_ix];
			if (c == Str[str_ix + 1] && c == Str[str_ix + 2])
				return 1;
		}

		// check for 4 consecutive consonants
		if (str_ix + 3 < len)
		{
			if (NameGen)
			{
				char *c2 = &Str[str_ix];
				if (!IsVowel(c2[0]) && !IsVowel(c2[1]) && !IsVowel(c2[2]) && !IsVowel(c2[3]))
					return 1;
			}
		}
	}

	// check bad name exclusion list
	for (int i = 0; i < bad_name_excl_count; i++)
	{
		if (NameContains(Str, bad_name_exclusions[i], 1))
			return 1;
	}

	return 0;
}

char EQNameGen::NameFailsOnComboExclusions(char *str, int race)
{
	if (!str) return 1;

	for (int i = 0; i < 25; i++)
	{
		if (NameContains(str, combo_exclusions[25 * race + i], 0))
			return 1;
	}

	return 0;
}

char *EQNameGen::CreateName(int race, char gender)
{
	strcpy(new_name, "YourNameHere");

	// race 0 is for generating pet name
	if (race == 128) // Iksar
		race = 13;
	if (race == 130) // Vah Shir
		race = 14;
	if (race > 14) // only supports up to vah shir, no frogloks
		return new_name;

	// 2 to 5 name fragments are selected from the name table and concatenated to form the character name
	int frag_chance[5];
	frag_chance[0] = 100; // beginning fragment is guaranteed
	frag_chance[1] = 80;
	frag_chance[2] = 80;
	frag_chance[3] = 100; // end fragment is guaranteed
	frag_chance[4] = gender != 0 ? 100 : 0; // add girlish sounding suffix for female

	// try up to 50 times to generate a valid name
	char name[2048];
	for (int namegen_retries = 0; namegen_retries < 50; namegen_retries++)
	{
		name[0] = 0;
		int name_len = 0;

		// pass 1 - pick up to five fragments from the name table
		for (int frag_position = 0; frag_position < 5; frag_position++)
		{
			// roll to determine if we add a fragment in this position or skip
			int rand100 = RandNbr() % 101;
			int chance = frag_chance[frag_position];
			if (chance == 100 || rand100 < chance)
			{
				// get one of 10 fragments for this position from the name table
				int rand = RandNbr() % 10;
				const char *random_name_frag = name_tables[50 * race + 5 * rand + frag_position];

				// append fragment to name
				strcat(name, random_name_frag);
				name_len += strlen(random_name_frag);
			}
		}

		// pass 2 - check for bad names
		if (name_len >= 4 && name_len <= 15)
		{
			// fix up double consonant at beginning of name
			if (!IsVowel(name[0]) && !IsVowel(name[1]) && name[1] == name[2])
			{
				char buf[16];
				buf[0] = name[0];
				buf[1] = ReturnRandomVowel();
				int buf_len = 2;
				if (name_len + 1 > 2)
				{
					memcpy(&buf[2], &name[1], name_len - 1);
					buf_len = name_len - 1 + 2;
				}
				buf[buf_len] = 0;
				strcpy(name, buf);
			}

			if (!NameFailsOnBasicChecks(name) && !NameFailsOnComboExclusions(name, race))
				break; // we passed all checks and generated a good name
		}
	}

	strncpy(new_name, name, 15);
	new_name[15] = 0; // add null termination

	return new_name;
}
