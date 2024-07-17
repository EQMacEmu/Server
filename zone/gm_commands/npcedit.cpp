#include "../client.h"

void command_npcedit(Client *c, const Seperator *sep){
	if (!c->GetTarget() || !c->GetTarget()->IsNPC())
	{
		c->Message(Chat::White, "Error: Must have NPC targeted");
		return;
	}

	if (strcasecmp(sep->arg[1], "help") == 0) {

		c->Message(Chat::White, "Help File for #npcedit. Syntax for commands are:");
		c->Message(Chat::White, "#npcedit Name - Sets an NPC's name");
		c->Message(Chat::White, "#npcedit Lastname - Sets an NPC's lastname");
		c->Message(Chat::White, "#npcedit Level - Sets an NPC's level");
		c->Message(Chat::White, "#npcedit Maxlevel - Sets an NPC's maximum level");
		c->Message(Chat::White, "#npcedit Race - Sets an NPC's race");
		c->Message(Chat::White, "#npcedit Class - Sets an NPC's class");
		c->Message(Chat::White, "#npcedit Bodytype - Sets an NPC's bodytype");
		c->Message(Chat::White, "#npcedit HP - Sets an NPC's hitpoints");
		c->Message(Chat::White, "#npcedit Gender - Sets an NPC's gender");
		c->Message(Chat::White, "#npcedit Texture - Sets an NPC's texture");
		c->Message(Chat::White, "#npcedit Helmtexture - Sets an NPC's helmtexture");
		c->Message(Chat::White, "#npcedit Armtexture - Sets an NPC's arm texture");
		c->Message(Chat::White, "#npcedit Bracertexture - Sets an NPC's bracer texture");
		c->Message(Chat::White, "#npcedit Handtexture - Sets an NPC's hand texture");
		c->Message(Chat::White, "#npcedit Legtexture - Sets an NPC's leg texture");
		c->Message(Chat::White, "#npcedit Feettexture - Sets an NPC's feettexture");
		c->Message(Chat::White, "#npcedit Size - Sets an NPC's size");
		c->Message(Chat::White, "#npcedit Hpregen - Sets an NPC's hitpoint regen rate per tick");
		c->Message(Chat::White, "#npcedit Manaregen - Sets an NPC's mana regen rate per tick");
		c->Message(Chat::White, "#npcedit Loottable - Sets the loottable ID for an NPC ");
		c->Message(Chat::White, "#npcedit Merchantid - Sets the merchant ID for an NPC");
		c->Message(Chat::White, "#npcedit npc_spells_effects_id - Sets the NPC Spell Effects ID");
		c->Message(Chat::White, "#npcedit special_abilities - Sets the NPC's Special Abilities");
		c->Message(Chat::White, "#npcedit Spell - Sets the npc spells list ID for an NPC");
		c->Message(Chat::White, "#npcedit Faction - Sets the NPC's faction id");
		c->Message(Chat::White, "#npcedit Mindmg - Sets an NPC's minimum damage");
		c->Message(Chat::White, "#npcedit Maxdmg - Sets an NPC's maximum damage");
		c->Message(Chat::White, "#npcedit Aggroradius - Sets an NPC's aggro radius");
		c->Message(Chat::White, "#npcedit Assistradius - Sets an NPC's assist radius");
		c->Message(Chat::White, "#npcedit Social - Set to 1 if an NPC should assist others on its faction");
		c->Message(Chat::White, "#npcedit Runspeed - Sets an NPC's run speed");
		c->Message(Chat::White, "#npcedit AGI - Sets an NPC's Agility");
		c->Message(Chat::White, "#npcedit CHA - Sets an NPC's Charisma");
		c->Message(Chat::White, "#npcedit DEX - Sets an NPC's Dexterity");
		c->Message(Chat::White, "#npcedit INT - Sets an NPC's Intelligence");
		c->Message(Chat::White, "#npcedit STA - Sets an NPC's Stamina");
		c->Message(Chat::White, "#npcedit STR - Sets an NPC's Strength");
		c->Message(Chat::White, "#npcedit WIS - Sets an NPC's Wisdom");
		c->Message(Chat::White, "#npcedit MR - Sets an NPC's Magic Resistance");
		c->Message(Chat::White, "#npcedit PR - Sets an NPC's Poison Resistance");
		c->Message(Chat::White, "#npcedit DR - Sets an NPC's Disease Resistance");
		c->Message(Chat::White, "#npcedit FR - Sets an NPC's Fire Resistance");
		c->Message(Chat::White, "#npcedit CR - Sets an NPC's cold resistance");
		c->Message(Chat::White, "#npcedit Seeinvis - Sets an NPC's ability to see invis");
		c->Message(Chat::White, "#npcedit Seeinvisundead - Sets an NPC's ability to see through invis vs. undead");
		c->Message(Chat::White, "#npcedit Seehide - Sets an NPC's ability to see through hide");
		c->Message(Chat::White, "#npcedit Seeimprovedhide - Sets an NPC's ability to see through improved hide");
		c->Message(Chat::White, "#npcedit AC - Sets an NPC's Armor Class");
		c->Message(Chat::White, "#npcedit ATK - Sets an NPC's Attack");
		c->Message(Chat::White, "#npcedit Accuracy - Sets an NPC's Accuracy");
		c->Message(Chat::White, "#npcedit npcaggro - Sets an NPC's npc_aggro flag");
		c->Message(Chat::White, "#npcedit qglobal - Sets an NPC's quest global flag");
		c->Message(Chat::White, "#npcedit limit - Sets an NPC's spawn limit counter");
		c->Message(Chat::White, "#npcedit Attackspeed - Sets an NPC's attack speed modifier");
		c->Message(Chat::White, "#npcedit Attackdelay - Sets an NPC's attack delay");
		c->Message(Chat::White, "#npcedit wep1 - Sets an NPC's primary weapon model");
		c->Message(Chat::White, "#npcedit wep2 - Sets an NPC's secondary weapon model");
		c->Message(Chat::White, "#npcedit featuresave - Saves all current facial features to the database");
		c->Message(Chat::White, "#npcedit color - Sets an NPC's red, green, and blue armor tint");
		c->Message(Chat::White, "#npcedit armortint_id - Set an NPC's Armor tint ID");
		c->Message(Chat::White, "#npcedit setanimation - Set an NPC's animation on spawn (Stored in spawn2 table)");
		c->Message(Chat::White, "#npcedit scalerate - Set an NPC's scaling rate");
		c->Message(Chat::White, "#npcedit healscale - Set an NPC's heal scaling rate");
		c->Message(Chat::White, "#npcedit spellscale - Set an NPC's spell scaling rate");
		c->Message(Chat::White, "#npcedit version - Set an NPC's version");

	}

	uint32 npcTypeID = c->GetTarget()->CastToNPC()->GetNPCTypeID();

	if (strcasecmp(sep->arg[1], "name") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has the name %s.", npcTypeID, sep->argplus[2]);

		std::string query = StringFormat("UPDATE npc_types SET name = '%s' WHERE id = %i",
			sep->argplus[2], npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "lastname") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has the lastname %s.", npcTypeID, sep->argplus[2]);

		std::string query = StringFormat("UPDATE npc_types SET lastname = '%s' WHERE id = %i",
			sep->argplus[2], npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "race") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has the race %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET race = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "class") == 0) {
		c->Message(Chat::Yellow, "NPCID %u is now class %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET class = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "bodytype") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has type %i bodytype.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET bodytype = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "hp") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Hitpoints.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET hp = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "gender") == 0) {
		c->Message(Chat::Yellow, "NPCID %u is now gender %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET gender = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "texture") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now uses texture %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET texture = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "helmtexture") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now uses helmtexture %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET helmtexture = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "armtexture") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now uses armtexture %i.", npcTypeID, atoi(sep->argplus[2]));
		std::string query = StringFormat("UPDATE npc_types SET armtexture = %i WHERE id = %i", atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "bracertexture") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now uses bracertexture %i.", npcTypeID, atoi(sep->argplus[2]));
		std::string query = StringFormat("UPDATE npc_types SET bracertexture = %i WHERE id = %i", atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "handtexture") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now uses handtexture %i.", npcTypeID, atoi(sep->argplus[2]));
		std::string query = StringFormat("UPDATE npc_types SET handtexture = %i WHERE id = %i", atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "legtexture") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now uses legtexture %i.", npcTypeID, atoi(sep->argplus[2]));
		std::string query = StringFormat("UPDATE npc_types SET legtexture = %i WHERE id = %i", atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "feettexture") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now uses feettexture %i.", npcTypeID, atoi(sep->argplus[2]));
		std::string query = StringFormat("UPDATE npc_types SET feettexture = %i WHERE id = %i", atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "size") == 0) {
		c->Message(Chat::Yellow, "NPCID %u is now size %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET size = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "hpregen") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now regens %i hitpoints per tick.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET hp_regen_rate = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "manaregen") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now regens %i mana per tick.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET mana_regen_rate = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "loottable") == 0) {
		c->Message(Chat::Yellow, "NPCID %u is now on loottable_id %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET loottable_id = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "merchantid") == 0) {
		c->Message(Chat::Yellow, "NPCID %u is now merchant_id %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET merchant_id = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "npc_spells_effects_id") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has field 'npc_spells_effects_id' set to %s.", npcTypeID, sep->argplus[2]);

		std::string query = StringFormat("UPDATE npc_types SET npc_spells_effects_id = '%s' WHERE id = %i",
			sep->argplus[2], npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "special_abilities") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has field 'special_abilities' set to %s.", npcTypeID, sep->argplus[2]);

		std::string query = StringFormat("UPDATE npc_types SET special_abilities = '%s' WHERE id = %i",
			sep->argplus[2], npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "spell") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now uses spell list %i", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET npc_spells_id = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "faction") == 0) {
		c->Message(Chat::Yellow, "NPCID %u is now faction %i", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET npc_faction_id = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "mindmg") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now hits for a min of %i", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET mindmg = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "maxdmg") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now hits for a max of %i", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET maxdmg = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "aggroradius") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has an aggro radius of %i", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET aggroradius = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "assistradius") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has an assist radius of %i", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET assistradius = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "social") == 0) {
		c->Message(Chat::Yellow, "NPCID %u social status is now %i", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET social = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "runspeed") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now runs at %f", npcTypeID, atof(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET runspeed = %f WHERE id = %i",
			atof(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "AGI") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Agility.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET AGI = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "CHA") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Charisma.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET CHA = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "DEX") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Dexterity.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET DEX = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "INT") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Intelligence.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET _INT = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "STA") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Stamina.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET STA = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "STR") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Strength.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET STR = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "WIS") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a Magic Resistance of %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET WIS = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "MR") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a Magic Resistance of %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET MR = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "DR") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a Disease Resistance of %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET DR = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "CR") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a Cold Resistance of %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET CR = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "FR") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a Fire Resistance of %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET FR = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "PR") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a Poison Resistance of %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET PR = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "seeinvis") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has seeinvis set to %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET see_invis = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "seeinvisundead") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has seeinvisundead set to %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET see_invis_undead = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "seesneak") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has seesneak set to %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET see_sneak = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "seeimprovedhide") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has seeimprovedhide set to %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET see_improved_hide = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "AC") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Armor Class.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET ac = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "ATK") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Attack.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET atk = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "Accuracy") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i Accuracy.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET accuracy = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "level") == 0) {
		c->Message(Chat::Yellow, "NPCID %u is now level %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET level = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "maxlevel") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a maximum level of %i.", npcTypeID, atoi(sep->argplus[2]));

		std::string query = StringFormat("UPDATE npc_types SET maxlevel = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "qglobal") == 0) {
		c->Message(Chat::Yellow, "Quest globals have been %s for NPCID %u",
			atoi(sep->arg[2]) == 0 ? "disabled" : "enabled", npcTypeID);

		std::string query = StringFormat("UPDATE npc_types SET qglobal = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "npcaggro") == 0) {
		c->Message(Chat::Yellow, "NPCID %u will now %s other NPCs with negative faction npc_value",
			npcTypeID, atoi(sep->arg[2]) == 0 ? "not aggro" : "aggro");

		std::string query = StringFormat("UPDATE npc_types SET npc_aggro = %i WHERE id = %i",
			atoi(sep->argplus[2]) == 0 ? 0 : 1, npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "limit") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a spawn limit of %i",
			npcTypeID, atoi(sep->arg[2]));

		std::string query = StringFormat("UPDATE npc_types SET limit = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "Attackdelay") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has attack_delay set to %i", npcTypeID, atoi(sep->arg[2]));

		std::string query = StringFormat("UPDATE npc_types SET attack_delay = %i WHERE id = %i", atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}


	if (strcasecmp(sep->arg[1], "wep1") == 0) {
		c->Message(Chat::Yellow, "NPCID %u will have item graphic %i set to his primary on repop.",
			npcTypeID, atoi(sep->arg[2]));

		std::string query = StringFormat("UPDATE npc_types SET d_melee_texture1 = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "wep2") == 0) {
		c->Message(Chat::Yellow, "NPCID %u will have item graphic %i set to his secondary on repop.",
			npcTypeID, atoi(sep->arg[2]));

		std::string query = StringFormat("UPDATE npc_types SET d_melee_texture2 = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "featuresave") == 0) {
		c->Message(Chat::Yellow, "NPCID %u saved with all current facial feature settings",
			npcTypeID);

		Mob* target = c->GetTarget();

		std::string query = StringFormat("UPDATE npc_types "
			"SET luclin_haircolor = %i, luclin_beardcolor = %i, "
			"luclin_hairstyle = %i, luclin_beard = %i, "
			"face = %i "
			"WHERE id = %i",
			target->GetHairColor(), target->GetBeardColor(),
			target->GetHairStyle(), target->GetBeard(),
			target->GetLuclinFace(), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "color") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has %i red, %i green, and %i blue tinting on their armor.",
			npcTypeID, atoi(sep->arg[2]), atoi(sep->arg[3]), atoi(sep->arg[4]));

		std::string query = StringFormat("UPDATE npc_types "
			"SET armortint_red = %i, armortint_green = %i, armortint_blue = %i "
			"WHERE id = %i",
			atoi(sep->arg[2]), atoi(sep->arg[3]), atoi(sep->arg[4]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "armortint_id") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has field 'armortint_id' set to %s",
			npcTypeID, sep->arg[2]);

		std::string query = StringFormat("UPDATE npc_types SET armortint_id = '%s' WHERE id = %i",
			sep->argplus[2], npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "setanimation") == 0) {
		int animation = 0;
		if (sep->arg[2] && atoi(sep->arg[2]) <= 4) {
			if ((strcasecmp(sep->arg[2], "stand") == 0) || atoi(sep->arg[2]) == 0)
				animation = 0; //Stand
			if ((strcasecmp(sep->arg[2], "sit") == 0) || atoi(sep->arg[2]) == 1)
				animation = 1; //Sit
			if ((strcasecmp(sep->arg[2], "crouch") == 0) || atoi(sep->arg[2]) == 2)
				animation = 2; //Crouch
			if ((strcasecmp(sep->arg[2], "dead") == 0) || atoi(sep->arg[2]) == 3)
				animation = 3; //Dead
			if ((strcasecmp(sep->arg[2], "loot") == 0) || atoi(sep->arg[2]) == 4)
				animation = 4; //Looting Animation
		}
		else {
			c->Message(Chat::White, "You must specifiy an animation stand, sit, crouch, dead, loot (0-4)");
			c->Message(Chat::White, "Example: #npcedit setanimation sit");
			c->Message(Chat::White, "Example: #npcedit setanimation 0");
			return;
		}

		c->Message(Chat::Yellow,"NPCID %u now has the animation set to %i on spawn with spawngroup %i", npcTypeID, animation, c->GetTarget()->CastToNPC()->GetSp2() );

		std::string query = StringFormat("UPDATE spawn2 SET animation = %i "
			"WHERE spawngroupID = %i",
			animation, c->GetTarget()->CastToNPC()->GetSp2());
		database.QueryDatabase(query);

		c->GetTarget()->SetAppearance(EmuAppearance(animation));
		return;
		}

	if (strcasecmp(sep->arg[1], "scalerate") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a scaling rate of %i.",
			npcTypeID, atoi(sep->arg[2]));

		std::string query = StringFormat("UPDATE npc_types SET scalerate = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "healscale") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a heal scaling rate of %i.",
			npcTypeID, atoi(sep->arg[2]));

		std::string query = StringFormat("UPDATE npc_types SET healscale = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if (strcasecmp(sep->arg[1], "spellscale") == 0) {
		c->Message(Chat::Yellow, "NPCID %u now has a spell scaling rate of %i.",
			npcTypeID, atoi(sep->arg[2]));

		std::string query = StringFormat("UPDATE npc_types SET spellscale = %i WHERE id = %i",
			atoi(sep->argplus[2]), npcTypeID);
		database.QueryDatabase(query);
		return;
	}

	if ((sep->arg[1][0] == 0 || strcasecmp(sep->arg[1], "*") == 0) || ((c->GetTarget() == 0) || (c->GetTarget()->IsClient())))
		c->Message(Chat::White, "Type #npcedit help for more info");

}