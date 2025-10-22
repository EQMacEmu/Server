#pragma once
#include "random.h"

class EQNameGen
{
public:
	EQNameGen(EQ::Random random);
	char *CreateName(int race, char gender);

protected:
	int RandNbr();
	static bool IsVowel(char c);
	char ReturnRandomVowel();
	static char NameContains(const char *haystack, const char *needle, char ignorecase);
	static char NameFailsOnBasicChecks(char *Str);
	static char NameFailsOnComboExclusions(char *str, int race);

	EQ::Random m_random;
	char new_name[40];
};