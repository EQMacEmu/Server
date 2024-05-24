#include "../client.h"

void command_randomfeatures(Client *c, const Seperator *sep){
	Mob *target = c->GetTarget();
	if (!target)
		c->Message(CC_Default, "Error: This command requires a target");
	else
	{
		uint16 Race = target->GetRace();
		if (Race <= 12 || Race == 128 || Race == 130 || Race == 330) {

			uint8 Gender = target->GetGender();
			uint8 Texture = 0xFF;
			uint8 HelmTexture = 0xFF;
			uint8 HairColor = 0xFF;
			uint8 BeardColor = 0xFF;
			uint8 EyeColor1 = 0xFF;
			uint8 EyeColor2 = 0xFF;
			uint8 HairStyle = 0xFF;
			uint8 LuclinFace = 0xFF;
			uint8 Beard = 0xFF;

			// Set some common feature settings
			EyeColor1 = zone->random.Int(0, 9);
			EyeColor2 = zone->random.Int(0, 9);
			LuclinFace = zone->random.Int(0, 7);

			// Adjust all settings based on the min and max for each feature of each race and gender
			switch (Race)
			{
			case 1:	// Human
				HairColor = zone->random.Int(0, 19);
				if (Gender == 0) {
					BeardColor = HairColor;
					HairStyle = zone->random.Int(0, 3);
					Beard = zone->random.Int(0, 5);
				}
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 2);
				}
				break;
			case 2:	// Barbarian
				HairColor = zone->random.Int(0, 19);
				LuclinFace = zone->random.Int(0, 87);
				if (Gender == 0) {
					BeardColor = HairColor;
					HairStyle = zone->random.Int(0, 3);
					Beard = zone->random.Int(0, 5);
				}
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 2);
				}
				break;
			case 3: // Erudite
				if (Gender == 0) {
					BeardColor = zone->random.Int(0, 19);
					Beard = zone->random.Int(0, 5);
					LuclinFace = zone->random.Int(0, 57);
				}
				if (Gender == 1) {
					LuclinFace = zone->random.Int(0, 87);
				}
				break;
			case 4: // WoodElf
				HairColor = zone->random.Int(0, 19);
				if (Gender == 0) {
					HairStyle = zone->random.Int(0, 3);
				}
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 2);
				}
				break;
			case 5: // HighElf
				HairColor = zone->random.Int(0, 14);
				if (Gender == 0) {
					HairStyle = zone->random.Int(0, 3);
					LuclinFace = zone->random.Int(0, 37);
					BeardColor = HairColor;
				}
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 2);
				}
				break;
			case 6: // DarkElf
				HairColor = zone->random.Int(13, 18);
				if (Gender == 0) {
					HairStyle = zone->random.Int(0, 3);
					LuclinFace = zone->random.Int(0, 37);
					BeardColor = HairColor;
				}
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 2);
				}
				break;
			case 7: // HalfElf
				HairColor = zone->random.Int(0, 19);
				if (Gender == 0) {
					HairStyle = zone->random.Int(0, 3);
					LuclinFace = zone->random.Int(0, 37);
					BeardColor = HairColor;
				}
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 2);
				}
				break;
			case 8: // Dwarf
				HairColor = zone->random.Int(0, 19);
				BeardColor = HairColor;
				if (Gender == 0) {
					HairStyle = zone->random.Int(0, 3);
					Beard = zone->random.Int(0, 5);
				}
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 2);
					LuclinFace = zone->random.Int(0, 17);
				}
				break;
			case 9: // Troll
				EyeColor1 = zone->random.Int(0, 10);
				EyeColor2 = zone->random.Int(0, 10);
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 3);
					HairColor = zone->random.Int(0, 23);
				}
				break;
			case 10: // Ogre
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 3);
					HairColor = zone->random.Int(0, 23);
				}
				break;
			case 11: // Halfling
				HairColor = zone->random.Int(0, 19);
				if (Gender == 0) {
					BeardColor = HairColor;
					HairStyle = zone->random.Int(0, 3);
					Beard = zone->random.Int(0, 5);
				}
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 2);
				}
				break;
			case 12: // Gnome
				HairColor = zone->random.Int(0, 24);
				if (Gender == 0) {
					BeardColor = HairColor;
					HairStyle = zone->random.Int(0, 3);
					Beard = zone->random.Int(0, 5);
				}
				if (Gender == 1) {
					HairStyle = zone->random.Int(0, 2);
				}
				break;
			case 128: // Iksar
			case 130: // VahShir
				break;
			case 330: // Froglok
				LuclinFace = zone->random.Int(0, 9);
				break;
			default:
				break;
			}

			target->SendIllusionPacket(Race, Gender, Texture, HelmTexture, HairColor, BeardColor,
				EyeColor1, EyeColor2, HairStyle, LuclinFace, Beard, 0xFF);

			c->Message(CC_Default, "NPC Features Randomized");
		}
		else
			c->Message(CC_Default, "This command requires a Playable Race as the Target");
	}
}

