/**
 * DO NOT MODIFY THIS FILE
 *
 * This repository was automatically generated and is NOT to be modified directly.
 * Any repository modifications are meant to be made to the repository extending the base.
 * Any modifications to base repositories are to be made by the generator only
 *
 * @generator ./utils/scripts/generators/repository-generator.pl
 * @docs https://eqemu.gitbook.io/server/in-development/developer-area/repositories
 */

#ifndef EQEMU_BASE_NPC_TYPES_REPOSITORY_H
#define EQEMU_BASE_NPC_TYPES_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>


class BaseNpcTypesRepository {
public:
	struct NpcTypes {
		int32_t     id;
		std::string name;
		std::string lastname;
		uint8_t     level;
		uint16_t    race;
		uint8_t     class_;
		int32_t     bodytype;
		int32_t     hp;
		int32_t     mana;
		uint8_t     gender;
		uint8_t     texture;
		uint8_t     helmtexture;
		float       size;
		uint32_t    hp_regen_rate;
		uint32_t    mana_regen_rate;
		uint32_t    loottable_id;
		uint32_t    merchant_id;
		uint32_t    npc_spells_id;
		uint32_t    npc_spells_effects_id;
		int32_t     npc_faction_id;
		uint32_t    mindmg;
		uint32_t    maxdmg;
		int16_t     attack_count;
		std::string special_abilities;
		uint32_t    aggroradius;
		uint32_t    assistradius;
		uint32_t    face;
		uint32_t    luclin_hairstyle;
		uint32_t    luclin_haircolor;
		uint32_t    luclin_eyecolor;
		uint32_t    luclin_eyecolor2;
		uint32_t    luclin_beardcolor;
		uint32_t    luclin_beard;
		uint32_t    armortint_id;
		uint8_t     armortint_red;
		uint8_t     armortint_green;
		uint8_t     armortint_blue;
		int32_t     d_melee_texture1;
		int32_t     d_melee_texture2;
		uint8_t     prim_melee_type;
		uint8_t     sec_melee_type;
		uint8_t     ranged_type;
		float       runspeed;
		int16_t     MR;
		int16_t     CR;
		int16_t     DR;
		int16_t     FR;
		int16_t     PR;
		int16_t     see_invis;
		int16_t     see_invis_undead;
		uint32_t    qglobal;
		int16_t     AC;
		int8_t      npc_aggro;
		int8_t      spawn_limit;
		uint8_t     attack_delay;
		uint32_t    STR;
		uint32_t    STA;
		uint32_t    DEX;
		uint32_t    AGI;
		uint32_t    _INT;
		uint32_t    WIS;
		uint32_t    CHA;
		int8_t      see_sneak;
		int8_t      see_improved_hide;
		int32_t     ATK;
		int32_t     Accuracy;
		int16_t     slow_mitigation;
		int8_t      maxlevel;
		int32_t     scalerate;
		uint8_t     private_corpse;
		uint8_t     unique_spawn_by_name;
		uint8_t     underwater;
		int8_t      isquest;
		uint32_t    emoteid;
		float       spellscale;
		float       healscale;
		uint8_t     raid_target;
		int8_t      chesttexture;
		uint8_t     armtexture;
		uint8_t     bracertexture;
		uint8_t     handtexture;
		uint8_t     legtexture;
		uint8_t     feettexture;
		uint8_t     light;
		float       walkspeed;
		int32_t     combat_hp_regen;
		int32_t     combat_mana_regen;
		uint8_t     aggro_pc;
		float       ignore_distance;
		int8_t      encounter;
		int8_t      ignore_despawn;
		int16_t     avoidance;
		uint16_t    exp_pct;
		uint8_t     greed;
		int8_t      engage_notice;
		int8_t      stuck_behavior;
		int8_t      flymode;
		uint32_t	loot_lockout;
	};

	static std::string PrimaryKey()
	{
		return std::string("id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"id",
			"name",
			"lastname",
			"level",
			"race",
			"`class`",
			"bodytype",
			"hp",
			"mana",
			"gender",
			"texture",
			"helmtexture",
			"size",
			"hp_regen_rate",
			"mana_regen_rate",
			"loottable_id",
			"merchant_id",
			"npc_spells_id",
			"npc_spells_effects_id",
			"npc_faction_id",
			"mindmg",
			"maxdmg",
			"attack_count",
			"special_abilities",
			"aggroradius",
			"assistradius",
			"face",
			"luclin_hairstyle",
			"luclin_haircolor",
			"luclin_eyecolor",
			"luclin_eyecolor2",
			"luclin_beardcolor",
			"luclin_beard",
			"armortint_id",
			"armortint_red",
			"armortint_green",
			"armortint_blue",
			"d_melee_texture1",
			"d_melee_texture2",
			"prim_melee_type",
			"sec_melee_type",
			"ranged_type",
			"runspeed",
			"MR",
			"CR",
			"DR",
			"FR",
			"PR",
			"see_invis",
			"see_invis_undead",
			"qglobal",
			"AC",
			"npc_aggro",
			"spawn_limit",
			"attack_delay",
			"STR",
			"STA",
			"DEX",
			"AGI",
			"_INT",
			"WIS",
			"CHA",
			"see_sneak",
			"see_improved_hide",
			"ATK",
			"Accuracy",
			"slow_mitigation",
			"maxlevel",
			"scalerate",
			"private_corpse",
			"unique_spawn_by_name",
			"underwater",
			"isquest",
			"emoteid",
			"spellscale",
			"healscale",
			"raid_target",
			"chesttexture",
			"armtexture",
			"bracertexture",
			"handtexture",
			"legtexture",
			"feettexture",
			"light",
			"walkspeed",
			"combat_hp_regen",
			"combat_mana_regen",
			"aggro_pc",
			"ignore_distance",
			"encounter",
			"ignore_despawn",
			"avoidance",
			"exp_pct",
			"greed",
			"engage_notice",
			"stuck_behavior",
			"flymode",
			"loot_lockout",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"id",
			"name",
			"lastname",
			"level",
			"race",
			"`class`",
			"bodytype",
			"hp",
			"mana",
			"gender",
			"texture",
			"helmtexture",
			"size",
			"hp_regen_rate",
			"mana_regen_rate",
			"loottable_id",
			"merchant_id",
			"npc_spells_id",
			"npc_spells_effects_id",
			"npc_faction_id",
			"mindmg",
			"maxdmg",
			"attack_count",
			"special_abilities",
			"aggroradius",
			"assistradius",
			"face",
			"luclin_hairstyle",
			"luclin_haircolor",
			"luclin_eyecolor",
			"luclin_eyecolor2",
			"luclin_beardcolor",
			"luclin_beard",
			"armortint_id",
			"armortint_red",
			"armortint_green",
			"armortint_blue",
			"d_melee_texture1",
			"d_melee_texture2",
			"prim_melee_type",
			"sec_melee_type",
			"ranged_type",
			"runspeed",
			"MR",
			"CR",
			"DR",
			"FR",
			"PR",
			"see_invis",
			"see_invis_undead",
			"qglobal",
			"AC",
			"npc_aggro",
			"spawn_limit",
			"attack_delay",
			"STR",
			"STA",
			"DEX",
			"AGI",
			"_INT",
			"WIS",
			"CHA",
			"see_sneak",
			"see_improved_hide",
			"ATK",
			"Accuracy",
			"slow_mitigation",
			"maxlevel",
			"scalerate",
			"private_corpse",
			"unique_spawn_by_name",
			"underwater",
			"isquest",
			"emoteid",
			"spellscale",
			"healscale",
			"raid_target",
			"chesttexture",
			"armtexture",
			"bracertexture",
			"handtexture",
			"legtexture",
			"feettexture",
			"light",
			"walkspeed",
			"combat_hp_regen",
			"combat_mana_regen",
			"aggro_pc",
			"ignore_distance",
			"encounter",
			"ignore_despawn",
			"avoidance",
			"exp_pct",
			"greed",
			"engage_notice",
			"stuck_behavior",
			"flymode",
			"loot_lockout",
		};
	}

	static std::string ColumnsRaw()
	{
		return std::string(Strings::Implode(", ", Columns()));
	}

	static std::string SelectColumnsRaw()
	{
		return std::string(Strings::Implode(", ", SelectColumns()));
	}

	static std::string TableName()
	{
		return std::string("npc_types");
	}

	static std::string BaseSelect()
	{
		return fmt::format(
			"SELECT {} FROM {}",
			SelectColumnsRaw(),
			TableName()
		);
	}

	static std::string BaseInsert()
	{
		return fmt::format(
			"INSERT INTO {} ({}) ",
			TableName(),
			ColumnsRaw()
		);
	}

	static NpcTypes NewEntity()
	{
		NpcTypes e{};

		e.id                    = 0;
		e.name                  = "";
		e.lastname              = "";
		e.level                 = 0;
		e.race                  = 0;
		e.class_                = 0;
		e.bodytype              = 1;
		e.hp                    = 0;
		e.mana                  = 0;
		e.gender                = 0;
		e.texture               = 0;
		e.helmtexture           = 0;
		e.size                  = 0;
		e.hp_regen_rate         = 0;
		e.mana_regen_rate       = 0;
		e.loottable_id          = 0;
		e.merchant_id           = 0;
		e.npc_spells_id         = 0;
		e.npc_spells_effects_id = 0;
		e.npc_faction_id        = 0;
		e.mindmg                = 0;
		e.maxdmg                = 0;
		e.attack_count          = -1;
		e.special_abilities     = "";
		e.aggroradius           = 0;
		e.assistradius          = 0;
		e.face                  = 1;
		e.luclin_hairstyle      = 1;
		e.luclin_haircolor      = 1;
		e.luclin_eyecolor       = 1;
		e.luclin_eyecolor2      = 1;
		e.luclin_beardcolor     = 1;
		e.luclin_beard          = 0;
		e.armortint_id          = 0;
		e.armortint_red         = 0;
		e.armortint_green       = 0;
		e.armortint_blue        = 0;
		e.d_melee_texture1      = 0;
		e.d_melee_texture2      = 0;
		e.prim_melee_type       = 28;
		e.sec_melee_type        = 28;
		e.ranged_type           = 7;
		e.runspeed              = 0;
		e.MR                    = 0;
		e.CR                    = 0;
		e.DR                    = 0;
		e.FR                    = 0;
		e.PR                    = 0;
		e.see_invis             = 0;
		e.see_invis_undead      = 0;
		e.qglobal               = 0;
		e.AC                    = 0;
		e.npc_aggro             = 0;
		e.spawn_limit           = 0;
		e.attack_delay          = 30;
		e.STR                   = 75;
		e.STA                   = 75;
		e.DEX                   = 75;
		e.AGI                   = 75;
		e._INT                  = 80;
		e.WIS                   = 75;
		e.CHA                   = 75;
		e.see_sneak             = 0;
		e.see_improved_hide     = 0;
		e.ATK                   = 0;
		e.Accuracy              = 0;
		e.slow_mitigation       = 0;
		e.maxlevel              = 0;
		e.scalerate             = 100;
		e.private_corpse        = 0;
		e.unique_spawn_by_name  = 0;
		e.underwater            = 0;
		e.isquest               = 0;
		e.emoteid               = 0;
		e.spellscale            = 100;
		e.healscale             = 100;
		e.raid_target           = 0;
		e.chesttexture          = 0;
		e.armtexture            = 0;
		e.bracertexture         = 0;
		e.handtexture           = 0;
		e.legtexture            = 0;
		e.feettexture           = 0;
		e.light                 = 0;
		e.walkspeed             = 0;
		e.combat_hp_regen       = 0;
		e.combat_mana_regen     = 0;
		e.aggro_pc              = 0;
		e.ignore_distance       = 600;
		e.encounter             = 0;
		e.ignore_despawn        = 0;
		e.avoidance             = 0;
		e.exp_pct               = 100;
		e.greed                 = 0;
		e.engage_notice         = 0;
		e.stuck_behavior        = 0;
		e.flymode               = -1;
		e.loot_lockout			= 0;

		return e;
	}

	static NpcTypes GetNpcTypes(
		const std::vector<NpcTypes> &npc_typess,
		int npc_types_id
	)
	{
		for (auto &npc_types : npc_typess) {
			if (npc_types.id == npc_types_id) {
				return npc_types;
			}
		}

		return NewEntity();
	}

	static NpcTypes FindOne(
		Database& db,
		int npc_types_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				npc_types_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			NpcTypes e{};

			e.id                    = static_cast<int32_t>(atoi(row[0]));
			e.name                  = row[1] ? row[1] : "";
			e.lastname              = row[2] ? row[2] : "";
			e.level                 = static_cast<uint8_t>(strtoul(row[3], nullptr, 10));
			e.race                  = static_cast<uint16_t>(strtoul(row[4], nullptr, 10));
			e.class_                = static_cast<uint8_t>(strtoul(row[5], nullptr, 10));
			e.bodytype              = static_cast<int32_t>(atoi(row[6]));
			e.hp                    = static_cast<int32_t>(atoi(row[7]));
			e.mana                  = static_cast<int32_t>(atoi(row[8]));
			e.gender                = static_cast<uint8_t>(strtoul(row[9], nullptr, 10));
			e.texture               = static_cast<uint8_t>(strtoul(row[10], nullptr, 10));
			e.helmtexture           = static_cast<uint8_t>(strtoul(row[11], nullptr, 10));
			e.size                  = strtof(row[12], nullptr);
			e.hp_regen_rate         = static_cast<uint32_t>(strtoul(row[13], nullptr, 10));
			e.mana_regen_rate       = static_cast<uint32_t>(strtoul(row[14], nullptr, 10));
			e.loottable_id          = static_cast<uint32_t>(strtoul(row[15], nullptr, 10));
			e.merchant_id           = static_cast<uint32_t>(strtoul(row[16], nullptr, 10));
			e.npc_spells_id         = static_cast<uint32_t>(strtoul(row[17], nullptr, 10));
			e.npc_spells_effects_id = static_cast<uint32_t>(strtoul(row[18], nullptr, 10));
			e.npc_faction_id        = static_cast<int32_t>(atoi(row[19]));
			e.mindmg                = static_cast<uint32_t>(strtoul(row[20], nullptr, 10));
			e.maxdmg                = static_cast<uint32_t>(strtoul(row[21], nullptr, 10));
			e.attack_count          = static_cast<int16_t>(atoi(row[22]));
			e.special_abilities     = row[23] ? row[23] : "";
			e.aggroradius           = static_cast<uint32_t>(strtoul(row[24], nullptr, 10));
			e.assistradius          = static_cast<uint32_t>(strtoul(row[25], nullptr, 10));
			e.face                  = static_cast<uint32_t>(strtoul(row[26], nullptr, 10));
			e.luclin_hairstyle      = static_cast<uint32_t>(strtoul(row[27], nullptr, 10));
			e.luclin_haircolor      = static_cast<uint32_t>(strtoul(row[28], nullptr, 10));
			e.luclin_eyecolor       = static_cast<uint32_t>(strtoul(row[29], nullptr, 10));
			e.luclin_eyecolor2      = static_cast<uint32_t>(strtoul(row[30], nullptr, 10));
			e.luclin_beardcolor     = static_cast<uint32_t>(strtoul(row[31], nullptr, 10));
			e.luclin_beard          = static_cast<uint32_t>(strtoul(row[32], nullptr, 10));
			e.armortint_id          = static_cast<uint32_t>(strtoul(row[33], nullptr, 10));
			e.armortint_red         = static_cast<uint8_t>(strtoul(row[34], nullptr, 10));
			e.armortint_green       = static_cast<uint8_t>(strtoul(row[35], nullptr, 10));
			e.armortint_blue        = static_cast<uint8_t>(strtoul(row[36], nullptr, 10));
			e.d_melee_texture1      = static_cast<int32_t>(atoi(row[37]));
			e.d_melee_texture2      = static_cast<int32_t>(atoi(row[38]));
			e.prim_melee_type       = static_cast<uint8_t>(strtoul(row[39], nullptr, 10));
			e.sec_melee_type        = static_cast<uint8_t>(strtoul(row[40], nullptr, 10));
			e.ranged_type           = static_cast<uint8_t>(strtoul(row[41], nullptr, 10));
			e.runspeed              = strtof(row[42], nullptr);
			e.MR                    = static_cast<int16_t>(atoi(row[43]));
			e.CR                    = static_cast<int16_t>(atoi(row[44]));
			e.DR                    = static_cast<int16_t>(atoi(row[45]));
			e.FR                    = static_cast<int16_t>(atoi(row[46]));
			e.PR                    = static_cast<int16_t>(atoi(row[47]));
			e.see_invis             = static_cast<int16_t>(atoi(row[48]));
			e.see_invis_undead      = static_cast<int16_t>(atoi(row[49]));
			e.qglobal               = static_cast<uint32_t>(strtoul(row[50], nullptr, 10));
			e.AC                    = static_cast<int16_t>(atoi(row[51]));
			e.npc_aggro             = static_cast<int8_t>(atoi(row[52]));
			e.spawn_limit           = static_cast<int8_t>(atoi(row[53]));
			e.attack_delay          = static_cast<uint8_t>(strtoul(row[54], nullptr, 10));
			e.STR                   = static_cast<uint32_t>(strtoul(row[55], nullptr, 10));
			e.STA                   = static_cast<uint32_t>(strtoul(row[56], nullptr, 10));
			e.DEX                   = static_cast<uint32_t>(strtoul(row[57], nullptr, 10));
			e.AGI                   = static_cast<uint32_t>(strtoul(row[58], nullptr, 10));
			e._INT                  = static_cast<uint32_t>(strtoul(row[59], nullptr, 10));
			e.WIS                   = static_cast<uint32_t>(strtoul(row[60], nullptr, 10));
			e.CHA                   = static_cast<uint32_t>(strtoul(row[61], nullptr, 10));
			e.see_sneak             = static_cast<int8_t>(atoi(row[62]));
			e.see_improved_hide     = static_cast<int8_t>(atoi(row[63]));
			e.ATK                   = static_cast<int32_t>(atoi(row[64]));
			e.Accuracy              = static_cast<int32_t>(atoi(row[65]));
			e.slow_mitigation       = static_cast<int16_t>(atoi(row[66]));
			e.maxlevel              = static_cast<int8_t>(atoi(row[67]));
			e.scalerate             = static_cast<int32_t>(atoi(row[68]));
			e.private_corpse        = static_cast<uint8_t>(strtoul(row[69], nullptr, 10));
			e.unique_spawn_by_name  = static_cast<uint8_t>(strtoul(row[70], nullptr, 10));
			e.underwater            = static_cast<uint8_t>(strtoul(row[71], nullptr, 10));
			e.isquest               = static_cast<int8_t>(atoi(row[72]));
			e.emoteid               = static_cast<uint32_t>(strtoul(row[73], nullptr, 10));
			e.spellscale            = strtof(row[74], nullptr);
			e.healscale             = strtof(row[75], nullptr);
			e.raid_target           = static_cast<uint8_t>(strtoul(row[76], nullptr, 10));
			e.chesttexture          = static_cast<int8_t>(atoi(row[77]));
			e.armtexture            = static_cast<uint8_t>(strtoul(row[78], nullptr, 10));
			e.bracertexture         = static_cast<uint8_t>(strtoul(row[79], nullptr, 10));
			e.handtexture           = static_cast<uint8_t>(strtoul(row[80], nullptr, 10));
			e.legtexture            = static_cast<uint8_t>(strtoul(row[81], nullptr, 10));
			e.feettexture           = static_cast<uint8_t>(strtoul(row[82], nullptr, 10));
			e.light                 = static_cast<uint8_t>(strtoul(row[83], nullptr, 10));
			e.walkspeed             = strtof(row[84], nullptr);
			e.combat_hp_regen       = static_cast<int32_t>(atoi(row[85]));
			e.combat_mana_regen     = static_cast<int32_t>(atoi(row[86]));
			e.aggro_pc              = static_cast<uint8_t>(strtoul(row[87], nullptr, 10));
			e.ignore_distance       = strtof(row[88], nullptr);
			e.encounter             = static_cast<int8_t>(atoi(row[89]));
			e.ignore_despawn        = static_cast<int8_t>(atoi(row[90]));
			e.avoidance             = static_cast<int16_t>(atoi(row[91]));
			e.exp_pct               = static_cast<uint16_t>(strtoul(row[92], nullptr, 10));
			e.greed                 = static_cast<uint8_t>(strtoul(row[93], nullptr, 10));
			e.engage_notice         = static_cast<int8_t>(atoi(row[94]));
			e.stuck_behavior        = static_cast<int8_t>(atoi(row[95]));
			e.flymode               = static_cast<int8_t>(atoi(row[96]));
			e.flymode				= static_cast<uint32_t>(atoi(row[97]));

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int npc_types_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				npc_types_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const NpcTypes &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[1] + " = '" + Strings::Escape(e.name) + "'");
		v.push_back(columns[2] + " = '" + Strings::Escape(e.lastname) + "'");
		v.push_back(columns[3] + " = " + std::to_string(e.level));
		v.push_back(columns[4] + " = " + std::to_string(e.race));
		v.push_back(columns[5] + " = " + std::to_string(e.class_));
		v.push_back(columns[6] + " = " + std::to_string(e.bodytype));
		v.push_back(columns[7] + " = " + std::to_string(e.hp));
		v.push_back(columns[8] + " = " + std::to_string(e.mana));
		v.push_back(columns[9] + " = " + std::to_string(e.gender));
		v.push_back(columns[10] + " = " + std::to_string(e.texture));
		v.push_back(columns[11] + " = " + std::to_string(e.helmtexture));
		v.push_back(columns[12] + " = " + std::to_string(e.size));
		v.push_back(columns[13] + " = " + std::to_string(e.hp_regen_rate));
		v.push_back(columns[14] + " = " + std::to_string(e.mana_regen_rate));
		v.push_back(columns[15] + " = " + std::to_string(e.loottable_id));
		v.push_back(columns[16] + " = " + std::to_string(e.merchant_id));
		v.push_back(columns[17] + " = " + std::to_string(e.npc_spells_id));
		v.push_back(columns[18] + " = " + std::to_string(e.npc_spells_effects_id));
		v.push_back(columns[19] + " = " + std::to_string(e.npc_faction_id));
		v.push_back(columns[20] + " = " + std::to_string(e.mindmg));
		v.push_back(columns[21] + " = " + std::to_string(e.maxdmg));
		v.push_back(columns[22] + " = " + std::to_string(e.attack_count));
		v.push_back(columns[23] + " = '" + Strings::Escape(e.special_abilities) + "'");
		v.push_back(columns[24] + " = " + std::to_string(e.aggroradius));
		v.push_back(columns[25] + " = " + std::to_string(e.assistradius));
		v.push_back(columns[26] + " = " + std::to_string(e.face));
		v.push_back(columns[27] + " = " + std::to_string(e.luclin_hairstyle));
		v.push_back(columns[28] + " = " + std::to_string(e.luclin_haircolor));
		v.push_back(columns[29] + " = " + std::to_string(e.luclin_eyecolor));
		v.push_back(columns[30] + " = " + std::to_string(e.luclin_eyecolor2));
		v.push_back(columns[31] + " = " + std::to_string(e.luclin_beardcolor));
		v.push_back(columns[32] + " = " + std::to_string(e.luclin_beard));
		v.push_back(columns[33] + " = " + std::to_string(e.armortint_id));
		v.push_back(columns[34] + " = " + std::to_string(e.armortint_red));
		v.push_back(columns[35] + " = " + std::to_string(e.armortint_green));
		v.push_back(columns[36] + " = " + std::to_string(e.armortint_blue));
		v.push_back(columns[37] + " = " + std::to_string(e.d_melee_texture1));
		v.push_back(columns[38] + " = " + std::to_string(e.d_melee_texture2));
		v.push_back(columns[39] + " = " + std::to_string(e.prim_melee_type));
		v.push_back(columns[40] + " = " + std::to_string(e.sec_melee_type));
		v.push_back(columns[41] + " = " + std::to_string(e.ranged_type));
		v.push_back(columns[42] + " = " + std::to_string(e.runspeed));
		v.push_back(columns[43] + " = " + std::to_string(e.MR));
		v.push_back(columns[44] + " = " + std::to_string(e.CR));
		v.push_back(columns[45] + " = " + std::to_string(e.DR));
		v.push_back(columns[46] + " = " + std::to_string(e.FR));
		v.push_back(columns[47] + " = " + std::to_string(e.PR));
		v.push_back(columns[48] + " = " + std::to_string(e.see_invis));
		v.push_back(columns[49] + " = " + std::to_string(e.see_invis_undead));
		v.push_back(columns[50] + " = " + std::to_string(e.qglobal));
		v.push_back(columns[51] + " = " + std::to_string(e.AC));
		v.push_back(columns[52] + " = " + std::to_string(e.npc_aggro));
		v.push_back(columns[53] + " = " + std::to_string(e.spawn_limit));
		v.push_back(columns[54] + " = " + std::to_string(e.attack_delay));
		v.push_back(columns[55] + " = " + std::to_string(e.STR));
		v.push_back(columns[56] + " = " + std::to_string(e.STA));
		v.push_back(columns[57] + " = " + std::to_string(e.DEX));
		v.push_back(columns[58] + " = " + std::to_string(e.AGI));
		v.push_back(columns[59] + " = " + std::to_string(e._INT));
		v.push_back(columns[60] + " = " + std::to_string(e.WIS));
		v.push_back(columns[61] + " = " + std::to_string(e.CHA));
		v.push_back(columns[62] + " = " + std::to_string(e.see_sneak));
		v.push_back(columns[63] + " = " + std::to_string(e.see_improved_hide));
		v.push_back(columns[64] + " = " + std::to_string(e.ATK));
		v.push_back(columns[65] + " = " + std::to_string(e.Accuracy));
		v.push_back(columns[66] + " = " + std::to_string(e.slow_mitigation));
		v.push_back(columns[67] + " = " + std::to_string(e.maxlevel));
		v.push_back(columns[68] + " = " + std::to_string(e.scalerate));
		v.push_back(columns[69] + " = " + std::to_string(e.private_corpse));
		v.push_back(columns[70] + " = " + std::to_string(e.unique_spawn_by_name));
		v.push_back(columns[71] + " = " + std::to_string(e.underwater));
		v.push_back(columns[72] + " = " + std::to_string(e.isquest));
		v.push_back(columns[73] + " = " + std::to_string(e.emoteid));
		v.push_back(columns[74] + " = " + std::to_string(e.spellscale));
		v.push_back(columns[75] + " = " + std::to_string(e.healscale));
		v.push_back(columns[76] + " = " + std::to_string(e.raid_target));
		v.push_back(columns[77] + " = " + std::to_string(e.chesttexture));
		v.push_back(columns[78] + " = " + std::to_string(e.armtexture));
		v.push_back(columns[79] + " = " + std::to_string(e.bracertexture));
		v.push_back(columns[80] + " = " + std::to_string(e.handtexture));
		v.push_back(columns[81] + " = " + std::to_string(e.legtexture));
		v.push_back(columns[82] + " = " + std::to_string(e.feettexture));
		v.push_back(columns[83] + " = " + std::to_string(e.light));
		v.push_back(columns[84] + " = " + std::to_string(e.walkspeed));
		v.push_back(columns[85] + " = " + std::to_string(e.combat_hp_regen));
		v.push_back(columns[86] + " = " + std::to_string(e.combat_mana_regen));
		v.push_back(columns[87] + " = " + std::to_string(e.aggro_pc));
		v.push_back(columns[88] + " = " + std::to_string(e.ignore_distance));
		v.push_back(columns[89] + " = " + std::to_string(e.encounter));
		v.push_back(columns[90] + " = " + std::to_string(e.ignore_despawn));
		v.push_back(columns[91] + " = " + std::to_string(e.avoidance));
		v.push_back(columns[92] + " = " + std::to_string(e.exp_pct));
		v.push_back(columns[93] + " = " + std::to_string(e.greed));
		v.push_back(columns[94] + " = " + std::to_string(e.engage_notice));
		v.push_back(columns[95] + " = " + std::to_string(e.stuck_behavior));
		v.push_back(columns[96] + " = " + std::to_string(e.flymode));
		v.push_back(columns[97] + " = " + std::to_string(e.loot_lockout));

		auto results = db.QueryDatabase(
			fmt::format(
				"UPDATE {} SET {} WHERE {} = {}",
				TableName(),
				Strings::Implode(", ", v),
				PrimaryKey(),
				e.id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static NpcTypes InsertOne(
		Database& db,
		NpcTypes e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back("'" + Strings::Escape(e.name) + "'");
		v.push_back("'" + Strings::Escape(e.lastname) + "'");
		v.push_back(std::to_string(e.level));
		v.push_back(std::to_string(e.race));
		v.push_back(std::to_string(e.class_));
		v.push_back(std::to_string(e.bodytype));
		v.push_back(std::to_string(e.hp));
		v.push_back(std::to_string(e.mana));
		v.push_back(std::to_string(e.gender));
		v.push_back(std::to_string(e.texture));
		v.push_back(std::to_string(e.helmtexture));
		v.push_back(std::to_string(e.size));
		v.push_back(std::to_string(e.hp_regen_rate));
		v.push_back(std::to_string(e.mana_regen_rate));
		v.push_back(std::to_string(e.loottable_id));
		v.push_back(std::to_string(e.merchant_id));
		v.push_back(std::to_string(e.npc_spells_id));
		v.push_back(std::to_string(e.npc_spells_effects_id));
		v.push_back(std::to_string(e.npc_faction_id));
		v.push_back(std::to_string(e.mindmg));
		v.push_back(std::to_string(e.maxdmg));
		v.push_back(std::to_string(e.attack_count));
		v.push_back("'" + Strings::Escape(e.special_abilities) + "'");
		v.push_back(std::to_string(e.aggroradius));
		v.push_back(std::to_string(e.assistradius));
		v.push_back(std::to_string(e.face));
		v.push_back(std::to_string(e.luclin_hairstyle));
		v.push_back(std::to_string(e.luclin_haircolor));
		v.push_back(std::to_string(e.luclin_eyecolor));
		v.push_back(std::to_string(e.luclin_eyecolor2));
		v.push_back(std::to_string(e.luclin_beardcolor));
		v.push_back(std::to_string(e.luclin_beard));
		v.push_back(std::to_string(e.armortint_id));
		v.push_back(std::to_string(e.armortint_red));
		v.push_back(std::to_string(e.armortint_green));
		v.push_back(std::to_string(e.armortint_blue));
		v.push_back(std::to_string(e.d_melee_texture1));
		v.push_back(std::to_string(e.d_melee_texture2));
		v.push_back(std::to_string(e.prim_melee_type));
		v.push_back(std::to_string(e.sec_melee_type));
		v.push_back(std::to_string(e.ranged_type));
		v.push_back(std::to_string(e.runspeed));
		v.push_back(std::to_string(e.MR));
		v.push_back(std::to_string(e.CR));
		v.push_back(std::to_string(e.DR));
		v.push_back(std::to_string(e.FR));
		v.push_back(std::to_string(e.PR));
		v.push_back(std::to_string(e.see_invis));
		v.push_back(std::to_string(e.see_invis_undead));
		v.push_back(std::to_string(e.qglobal));
		v.push_back(std::to_string(e.AC));
		v.push_back(std::to_string(e.npc_aggro));
		v.push_back(std::to_string(e.spawn_limit));
		v.push_back(std::to_string(e.attack_delay));
		v.push_back(std::to_string(e.STR));
		v.push_back(std::to_string(e.STA));
		v.push_back(std::to_string(e.DEX));
		v.push_back(std::to_string(e.AGI));
		v.push_back(std::to_string(e._INT));
		v.push_back(std::to_string(e.WIS));
		v.push_back(std::to_string(e.CHA));
		v.push_back(std::to_string(e.see_sneak));
		v.push_back(std::to_string(e.see_improved_hide));
		v.push_back(std::to_string(e.ATK));
		v.push_back(std::to_string(e.Accuracy));
		v.push_back(std::to_string(e.slow_mitigation));
		v.push_back(std::to_string(e.maxlevel));
		v.push_back(std::to_string(e.scalerate));
		v.push_back(std::to_string(e.private_corpse));
		v.push_back(std::to_string(e.unique_spawn_by_name));
		v.push_back(std::to_string(e.underwater));
		v.push_back(std::to_string(e.isquest));
		v.push_back(std::to_string(e.emoteid));
		v.push_back(std::to_string(e.spellscale));
		v.push_back(std::to_string(e.healscale));
		v.push_back(std::to_string(e.raid_target));
		v.push_back(std::to_string(e.chesttexture));
		v.push_back(std::to_string(e.armtexture));
		v.push_back(std::to_string(e.bracertexture));
		v.push_back(std::to_string(e.handtexture));
		v.push_back(std::to_string(e.legtexture));
		v.push_back(std::to_string(e.feettexture));
		v.push_back(std::to_string(e.light));
		v.push_back(std::to_string(e.walkspeed));
		v.push_back(std::to_string(e.combat_hp_regen));
		v.push_back(std::to_string(e.combat_mana_regen));
		v.push_back(std::to_string(e.aggro_pc));
		v.push_back(std::to_string(e.ignore_distance));
		v.push_back(std::to_string(e.encounter));
		v.push_back(std::to_string(e.ignore_despawn));
		v.push_back(std::to_string(e.avoidance));
		v.push_back(std::to_string(e.exp_pct));
		v.push_back(std::to_string(e.greed));
		v.push_back(std::to_string(e.engage_notice));
		v.push_back(std::to_string(e.stuck_behavior));
		v.push_back(std::to_string(e.flymode));
		v.push_back(std::to_string(e.loot_lockout));

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseInsert(),
				Strings::Implode(",", v)
			)
		);

		if (results.Success()) {
			e.id = results.LastInsertedID();
			return e;
		}

		e = NewEntity();

		return e;
	}

	static int InsertMany(
		Database& db,
		const std::vector<NpcTypes> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back("'" + Strings::Escape(e.name) + "'");
			v.push_back("'" + Strings::Escape(e.lastname) + "'");
			v.push_back(std::to_string(e.level));
			v.push_back(std::to_string(e.race));
			v.push_back(std::to_string(e.class_));
			v.push_back(std::to_string(e.bodytype));
			v.push_back(std::to_string(e.hp));
			v.push_back(std::to_string(e.mana));
			v.push_back(std::to_string(e.gender));
			v.push_back(std::to_string(e.texture));
			v.push_back(std::to_string(e.helmtexture));
			v.push_back(std::to_string(e.size));
			v.push_back(std::to_string(e.hp_regen_rate));
			v.push_back(std::to_string(e.mana_regen_rate));
			v.push_back(std::to_string(e.loottable_id));
			v.push_back(std::to_string(e.merchant_id));
			v.push_back(std::to_string(e.npc_spells_id));
			v.push_back(std::to_string(e.npc_spells_effects_id));
			v.push_back(std::to_string(e.npc_faction_id));
			v.push_back(std::to_string(e.mindmg));
			v.push_back(std::to_string(e.maxdmg));
			v.push_back(std::to_string(e.attack_count));
			v.push_back("'" + Strings::Escape(e.special_abilities) + "'");
			v.push_back(std::to_string(e.aggroradius));
			v.push_back(std::to_string(e.assistradius));
			v.push_back(std::to_string(e.face));
			v.push_back(std::to_string(e.luclin_hairstyle));
			v.push_back(std::to_string(e.luclin_haircolor));
			v.push_back(std::to_string(e.luclin_eyecolor));
			v.push_back(std::to_string(e.luclin_eyecolor2));
			v.push_back(std::to_string(e.luclin_beardcolor));
			v.push_back(std::to_string(e.luclin_beard));
			v.push_back(std::to_string(e.armortint_id));
			v.push_back(std::to_string(e.armortint_red));
			v.push_back(std::to_string(e.armortint_green));
			v.push_back(std::to_string(e.armortint_blue));
			v.push_back(std::to_string(e.d_melee_texture1));
			v.push_back(std::to_string(e.d_melee_texture2));
			v.push_back(std::to_string(e.prim_melee_type));
			v.push_back(std::to_string(e.sec_melee_type));
			v.push_back(std::to_string(e.ranged_type));
			v.push_back(std::to_string(e.runspeed));
			v.push_back(std::to_string(e.MR));
			v.push_back(std::to_string(e.CR));
			v.push_back(std::to_string(e.DR));
			v.push_back(std::to_string(e.FR));
			v.push_back(std::to_string(e.PR));
			v.push_back(std::to_string(e.see_invis));
			v.push_back(std::to_string(e.see_invis_undead));
			v.push_back(std::to_string(e.qglobal));
			v.push_back(std::to_string(e.AC));
			v.push_back(std::to_string(e.npc_aggro));
			v.push_back(std::to_string(e.spawn_limit));
			v.push_back(std::to_string(e.attack_delay));
			v.push_back(std::to_string(e.STR));
			v.push_back(std::to_string(e.STA));
			v.push_back(std::to_string(e.DEX));
			v.push_back(std::to_string(e.AGI));
			v.push_back(std::to_string(e._INT));
			v.push_back(std::to_string(e.WIS));
			v.push_back(std::to_string(e.CHA));
			v.push_back(std::to_string(e.see_sneak));
			v.push_back(std::to_string(e.see_improved_hide));
			v.push_back(std::to_string(e.ATK));
			v.push_back(std::to_string(e.Accuracy));
			v.push_back(std::to_string(e.slow_mitigation));
			v.push_back(std::to_string(e.maxlevel));
			v.push_back(std::to_string(e.scalerate));
			v.push_back(std::to_string(e.private_corpse));
			v.push_back(std::to_string(e.unique_spawn_by_name));
			v.push_back(std::to_string(e.underwater));
			v.push_back(std::to_string(e.isquest));
			v.push_back(std::to_string(e.emoteid));
			v.push_back(std::to_string(e.spellscale));
			v.push_back(std::to_string(e.healscale));
			v.push_back(std::to_string(e.raid_target));
			v.push_back(std::to_string(e.chesttexture));
			v.push_back(std::to_string(e.armtexture));
			v.push_back(std::to_string(e.bracertexture));
			v.push_back(std::to_string(e.handtexture));
			v.push_back(std::to_string(e.legtexture));
			v.push_back(std::to_string(e.feettexture));
			v.push_back(std::to_string(e.light));
			v.push_back(std::to_string(e.walkspeed));
			v.push_back(std::to_string(e.combat_hp_regen));
			v.push_back(std::to_string(e.combat_mana_regen));
			v.push_back(std::to_string(e.aggro_pc));
			v.push_back(std::to_string(e.ignore_distance));
			v.push_back(std::to_string(e.encounter));
			v.push_back(std::to_string(e.ignore_despawn));
			v.push_back(std::to_string(e.avoidance));
			v.push_back(std::to_string(e.exp_pct));
			v.push_back(std::to_string(e.greed));
			v.push_back(std::to_string(e.engage_notice));
			v.push_back(std::to_string(e.stuck_behavior));
			v.push_back(std::to_string(e.flymode));
			v.push_back(std::to_string(e.loot_lockout));

			insert_chunks.push_back("(" + Strings::Implode(",", v) + ")");
		}

		std::vector<std::string> v;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES {}",
				BaseInsert(),
				Strings::Implode(",", insert_chunks)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static std::vector<NpcTypes> All(Database& db)
	{
		std::vector<NpcTypes> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			NpcTypes e{};

			e.id                    = static_cast<int32_t>(atoi(row[0]));
			e.name                  = row[1] ? row[1] : "";
			e.lastname              = row[2] ? row[2] : "";
			e.level                 = static_cast<uint8_t>(strtoul(row[3], nullptr, 10));
			e.race                  = static_cast<uint16_t>(strtoul(row[4], nullptr, 10));
			e.class_                = static_cast<uint8_t>(strtoul(row[5], nullptr, 10));
			e.bodytype              = static_cast<int32_t>(atoi(row[6]));
			e.hp                    = static_cast<int32_t>(atoi(row[7]));
			e.mana                  = static_cast<int32_t>(atoi(row[8]));
			e.gender                = static_cast<uint8_t>(strtoul(row[9], nullptr, 10));
			e.texture               = static_cast<uint8_t>(strtoul(row[10], nullptr, 10));
			e.helmtexture           = static_cast<uint8_t>(strtoul(row[11], nullptr, 10));
			e.size                  = strtof(row[12], nullptr);
			e.hp_regen_rate         = static_cast<uint32_t>(strtoul(row[13], nullptr, 10));
			e.mana_regen_rate       = static_cast<uint32_t>(strtoul(row[14], nullptr, 10));
			e.loottable_id          = static_cast<uint32_t>(strtoul(row[15], nullptr, 10));
			e.merchant_id           = static_cast<uint32_t>(strtoul(row[16], nullptr, 10));
			e.npc_spells_id         = static_cast<uint32_t>(strtoul(row[17], nullptr, 10));
			e.npc_spells_effects_id = static_cast<uint32_t>(strtoul(row[18], nullptr, 10));
			e.npc_faction_id        = static_cast<int32_t>(atoi(row[19]));
			e.mindmg                = static_cast<uint32_t>(strtoul(row[20], nullptr, 10));
			e.maxdmg                = static_cast<uint32_t>(strtoul(row[21], nullptr, 10));
			e.attack_count          = static_cast<int16_t>(atoi(row[22]));
			e.special_abilities     = row[23] ? row[23] : "";
			e.aggroradius           = static_cast<uint32_t>(strtoul(row[24], nullptr, 10));
			e.assistradius          = static_cast<uint32_t>(strtoul(row[25], nullptr, 10));
			e.face                  = static_cast<uint32_t>(strtoul(row[26], nullptr, 10));
			e.luclin_hairstyle      = static_cast<uint32_t>(strtoul(row[27], nullptr, 10));
			e.luclin_haircolor      = static_cast<uint32_t>(strtoul(row[28], nullptr, 10));
			e.luclin_eyecolor       = static_cast<uint32_t>(strtoul(row[29], nullptr, 10));
			e.luclin_eyecolor2      = static_cast<uint32_t>(strtoul(row[30], nullptr, 10));
			e.luclin_beardcolor     = static_cast<uint32_t>(strtoul(row[31], nullptr, 10));
			e.luclin_beard          = static_cast<uint32_t>(strtoul(row[32], nullptr, 10));
			e.armortint_id          = static_cast<uint32_t>(strtoul(row[33], nullptr, 10));
			e.armortint_red         = static_cast<uint8_t>(strtoul(row[34], nullptr, 10));
			e.armortint_green       = static_cast<uint8_t>(strtoul(row[35], nullptr, 10));
			e.armortint_blue        = static_cast<uint8_t>(strtoul(row[36], nullptr, 10));
			e.d_melee_texture1      = static_cast<int32_t>(atoi(row[37]));
			e.d_melee_texture2      = static_cast<int32_t>(atoi(row[38]));
			e.prim_melee_type       = static_cast<uint8_t>(strtoul(row[39], nullptr, 10));
			e.sec_melee_type        = static_cast<uint8_t>(strtoul(row[40], nullptr, 10));
			e.ranged_type           = static_cast<uint8_t>(strtoul(row[41], nullptr, 10));
			e.runspeed              = strtof(row[42], nullptr);
			e.MR                    = static_cast<int16_t>(atoi(row[43]));
			e.CR                    = static_cast<int16_t>(atoi(row[44]));
			e.DR                    = static_cast<int16_t>(atoi(row[45]));
			e.FR                    = static_cast<int16_t>(atoi(row[46]));
			e.PR                    = static_cast<int16_t>(atoi(row[47]));
			e.see_invis             = static_cast<int16_t>(atoi(row[48]));
			e.see_invis_undead      = static_cast<int16_t>(atoi(row[49]));
			e.qglobal               = static_cast<uint32_t>(strtoul(row[50], nullptr, 10));
			e.AC                    = static_cast<int16_t>(atoi(row[51]));
			e.npc_aggro             = static_cast<int8_t>(atoi(row[52]));
			e.spawn_limit           = static_cast<int8_t>(atoi(row[53]));
			e.attack_delay          = static_cast<uint8_t>(strtoul(row[54], nullptr, 10));
			e.STR                   = static_cast<uint32_t>(strtoul(row[55], nullptr, 10));
			e.STA                   = static_cast<uint32_t>(strtoul(row[56], nullptr, 10));
			e.DEX                   = static_cast<uint32_t>(strtoul(row[57], nullptr, 10));
			e.AGI                   = static_cast<uint32_t>(strtoul(row[58], nullptr, 10));
			e._INT                  = static_cast<uint32_t>(strtoul(row[59], nullptr, 10));
			e.WIS                   = static_cast<uint32_t>(strtoul(row[60], nullptr, 10));
			e.CHA                   = static_cast<uint32_t>(strtoul(row[61], nullptr, 10));
			e.see_sneak             = static_cast<int8_t>(atoi(row[62]));
			e.see_improved_hide     = static_cast<int8_t>(atoi(row[63]));
			e.ATK                   = static_cast<int32_t>(atoi(row[64]));
			e.Accuracy              = static_cast<int32_t>(atoi(row[65]));
			e.slow_mitigation       = static_cast<int16_t>(atoi(row[66]));
			e.maxlevel              = static_cast<int8_t>(atoi(row[67]));
			e.scalerate             = static_cast<int32_t>(atoi(row[68]));
			e.private_corpse        = static_cast<uint8_t>(strtoul(row[69], nullptr, 10));
			e.unique_spawn_by_name  = static_cast<uint8_t>(strtoul(row[70], nullptr, 10));
			e.underwater            = static_cast<uint8_t>(strtoul(row[71], nullptr, 10));
			e.isquest               = static_cast<int8_t>(atoi(row[72]));
			e.emoteid               = static_cast<uint32_t>(strtoul(row[73], nullptr, 10));
			e.spellscale            = strtof(row[74], nullptr);
			e.healscale             = strtof(row[75], nullptr);
			e.raid_target           = static_cast<uint8_t>(strtoul(row[76], nullptr, 10));
			e.chesttexture          = static_cast<int8_t>(atoi(row[77]));
			e.armtexture            = static_cast<uint8_t>(strtoul(row[78], nullptr, 10));
			e.bracertexture         = static_cast<uint8_t>(strtoul(row[79], nullptr, 10));
			e.handtexture           = static_cast<uint8_t>(strtoul(row[80], nullptr, 10));
			e.legtexture            = static_cast<uint8_t>(strtoul(row[81], nullptr, 10));
			e.feettexture           = static_cast<uint8_t>(strtoul(row[82], nullptr, 10));
			e.light                 = static_cast<uint8_t>(strtoul(row[83], nullptr, 10));
			e.walkspeed             = strtof(row[84], nullptr);
			e.combat_hp_regen       = static_cast<int32_t>(atoi(row[85]));
			e.combat_mana_regen     = static_cast<int32_t>(atoi(row[86]));
			e.aggro_pc              = static_cast<uint8_t>(strtoul(row[87], nullptr, 10));
			e.ignore_distance       = strtof(row[88], nullptr);
			e.encounter             = static_cast<int8_t>(atoi(row[89]));
			e.ignore_despawn        = static_cast<int8_t>(atoi(row[90]));
			e.avoidance             = static_cast<int16_t>(atoi(row[91]));
			e.exp_pct               = static_cast<uint16_t>(strtoul(row[92], nullptr, 10));
			e.greed                 = static_cast<uint8_t>(strtoul(row[93], nullptr, 10));
			e.engage_notice         = static_cast<int8_t>(atoi(row[94]));
			e.stuck_behavior        = static_cast<int8_t>(atoi(row[95]));
			e.flymode               = static_cast<int8_t>(atoi(row[96]));
			e.loot_lockout			= static_cast<uint32_t>(atoi(row[97]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<NpcTypes> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<NpcTypes> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			NpcTypes e{};

			e.id                    = static_cast<int32_t>(atoi(row[0]));
			e.name                  = row[1] ? row[1] : "";
			e.lastname              = row[2] ? row[2] : "";
			e.level                 = static_cast<uint8_t>(strtoul(row[3], nullptr, 10));
			e.race                  = static_cast<uint16_t>(strtoul(row[4], nullptr, 10));
			e.class_                = static_cast<uint8_t>(strtoul(row[5], nullptr, 10));
			e.bodytype              = static_cast<int32_t>(atoi(row[6]));
			e.hp                    = static_cast<int32_t>(atoi(row[7]));
			e.mana                  = static_cast<int32_t>(atoi(row[8]));
			e.gender                = static_cast<uint8_t>(strtoul(row[9], nullptr, 10));
			e.texture               = static_cast<uint8_t>(strtoul(row[10], nullptr, 10));
			e.helmtexture           = static_cast<uint8_t>(strtoul(row[11], nullptr, 10));
			e.size                  = strtof(row[12], nullptr);
			e.hp_regen_rate         = static_cast<uint32_t>(strtoul(row[13], nullptr, 10));
			e.mana_regen_rate       = static_cast<uint32_t>(strtoul(row[14], nullptr, 10));
			e.loottable_id          = static_cast<uint32_t>(strtoul(row[15], nullptr, 10));
			e.merchant_id           = static_cast<uint32_t>(strtoul(row[16], nullptr, 10));
			e.npc_spells_id         = static_cast<uint32_t>(strtoul(row[17], nullptr, 10));
			e.npc_spells_effects_id = static_cast<uint32_t>(strtoul(row[18], nullptr, 10));
			e.npc_faction_id        = static_cast<int32_t>(atoi(row[19]));
			e.mindmg                = static_cast<uint32_t>(strtoul(row[20], nullptr, 10));
			e.maxdmg                = static_cast<uint32_t>(strtoul(row[21], nullptr, 10));
			e.attack_count          = static_cast<int16_t>(atoi(row[22]));
			e.special_abilities     = row[23] ? row[23] : "";
			e.aggroradius           = static_cast<uint32_t>(strtoul(row[24], nullptr, 10));
			e.assistradius          = static_cast<uint32_t>(strtoul(row[25], nullptr, 10));
			e.face                  = static_cast<uint32_t>(strtoul(row[26], nullptr, 10));
			e.luclin_hairstyle      = static_cast<uint32_t>(strtoul(row[27], nullptr, 10));
			e.luclin_haircolor      = static_cast<uint32_t>(strtoul(row[28], nullptr, 10));
			e.luclin_eyecolor       = static_cast<uint32_t>(strtoul(row[29], nullptr, 10));
			e.luclin_eyecolor2      = static_cast<uint32_t>(strtoul(row[30], nullptr, 10));
			e.luclin_beardcolor     = static_cast<uint32_t>(strtoul(row[31], nullptr, 10));
			e.luclin_beard          = static_cast<uint32_t>(strtoul(row[32], nullptr, 10));
			e.armortint_id          = static_cast<uint32_t>(strtoul(row[33], nullptr, 10));
			e.armortint_red         = static_cast<uint8_t>(strtoul(row[34], nullptr, 10));
			e.armortint_green       = static_cast<uint8_t>(strtoul(row[35], nullptr, 10));
			e.armortint_blue        = static_cast<uint8_t>(strtoul(row[36], nullptr, 10));
			e.d_melee_texture1      = static_cast<int32_t>(atoi(row[37]));
			e.d_melee_texture2      = static_cast<int32_t>(atoi(row[38]));
			e.prim_melee_type       = static_cast<uint8_t>(strtoul(row[39], nullptr, 10));
			e.sec_melee_type        = static_cast<uint8_t>(strtoul(row[40], nullptr, 10));
			e.ranged_type           = static_cast<uint8_t>(strtoul(row[41], nullptr, 10));
			e.runspeed              = strtof(row[42], nullptr);
			e.MR                    = static_cast<int16_t>(atoi(row[43]));
			e.CR                    = static_cast<int16_t>(atoi(row[44]));
			e.DR                    = static_cast<int16_t>(atoi(row[45]));
			e.FR                    = static_cast<int16_t>(atoi(row[46]));
			e.PR                    = static_cast<int16_t>(atoi(row[47]));
			e.see_invis             = static_cast<int16_t>(atoi(row[48]));
			e.see_invis_undead      = static_cast<int16_t>(atoi(row[49]));
			e.qglobal               = static_cast<uint32_t>(strtoul(row[50], nullptr, 10));
			e.AC                    = static_cast<int16_t>(atoi(row[51]));
			e.npc_aggro             = static_cast<int8_t>(atoi(row[52]));
			e.spawn_limit           = static_cast<int8_t>(atoi(row[53]));
			e.attack_delay          = static_cast<uint8_t>(strtoul(row[54], nullptr, 10));
			e.STR                   = static_cast<uint32_t>(strtoul(row[55], nullptr, 10));
			e.STA                   = static_cast<uint32_t>(strtoul(row[56], nullptr, 10));
			e.DEX                   = static_cast<uint32_t>(strtoul(row[57], nullptr, 10));
			e.AGI                   = static_cast<uint32_t>(strtoul(row[58], nullptr, 10));
			e._INT                  = static_cast<uint32_t>(strtoul(row[59], nullptr, 10));
			e.WIS                   = static_cast<uint32_t>(strtoul(row[60], nullptr, 10));
			e.CHA                   = static_cast<uint32_t>(strtoul(row[61], nullptr, 10));
			e.see_sneak             = static_cast<int8_t>(atoi(row[62]));
			e.see_improved_hide     = static_cast<int8_t>(atoi(row[63]));
			e.ATK                   = static_cast<int32_t>(atoi(row[64]));
			e.Accuracy              = static_cast<int32_t>(atoi(row[65]));
			e.slow_mitigation       = static_cast<int16_t>(atoi(row[66]));
			e.maxlevel              = static_cast<int8_t>(atoi(row[67]));
			e.scalerate             = static_cast<int32_t>(atoi(row[68]));
			e.private_corpse        = static_cast<uint8_t>(strtoul(row[69], nullptr, 10));
			e.unique_spawn_by_name  = static_cast<uint8_t>(strtoul(row[70], nullptr, 10));
			e.underwater            = static_cast<uint8_t>(strtoul(row[71], nullptr, 10));
			e.isquest               = static_cast<int8_t>(atoi(row[72]));
			e.emoteid               = static_cast<uint32_t>(strtoul(row[73], nullptr, 10));
			e.spellscale            = strtof(row[74], nullptr);
			e.healscale             = strtof(row[75], nullptr);
			e.raid_target           = static_cast<uint8_t>(strtoul(row[76], nullptr, 10));
			e.chesttexture          = static_cast<int8_t>(atoi(row[77]));
			e.armtexture            = static_cast<uint8_t>(strtoul(row[78], nullptr, 10));
			e.bracertexture         = static_cast<uint8_t>(strtoul(row[79], nullptr, 10));
			e.handtexture           = static_cast<uint8_t>(strtoul(row[80], nullptr, 10));
			e.legtexture            = static_cast<uint8_t>(strtoul(row[81], nullptr, 10));
			e.feettexture           = static_cast<uint8_t>(strtoul(row[82], nullptr, 10));
			e.light                 = static_cast<uint8_t>(strtoul(row[83], nullptr, 10));
			e.walkspeed             = strtof(row[84], nullptr);
			e.combat_hp_regen       = static_cast<int32_t>(atoi(row[85]));
			e.combat_mana_regen     = static_cast<int32_t>(atoi(row[86]));
			e.aggro_pc              = static_cast<uint8_t>(strtoul(row[87], nullptr, 10));
			e.ignore_distance       = strtof(row[88], nullptr);
			e.encounter             = static_cast<int8_t>(atoi(row[89]));
			e.ignore_despawn        = static_cast<int8_t>(atoi(row[90]));
			e.avoidance             = static_cast<int16_t>(atoi(row[91]));
			e.exp_pct               = static_cast<uint16_t>(strtoul(row[92], nullptr, 10));
			e.greed                 = static_cast<uint8_t>(strtoul(row[93], nullptr, 10));
			e.engage_notice         = static_cast<int8_t>(atoi(row[94]));
			e.stuck_behavior        = static_cast<int8_t>(atoi(row[95]));
			e.flymode               = static_cast<int8_t>(atoi(row[96]));
			e.loot_lockout = static_cast<uint32_t>(atoi(row[97]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static int DeleteWhere(Database& db, const std::string &where_filter)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {}",
				TableName(),
				where_filter
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int Truncate(Database& db)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"TRUNCATE TABLE {}",
				TableName()
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int64 GetMaxId(Database& db)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT COALESCE(MAX({}), 0) FROM {}",
				PrimaryKey(),
				TableName()
			)
		);

		return (results.Success() && results.begin()[0] ? strtoll(results.begin()[0], nullptr, 10) : 0);
	}

	static int64 Count(Database& db, const std::string &where_filter = "")
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT COUNT(*) FROM {} {}",
				TableName(),
				(where_filter.empty() ? "" : "WHERE " + where_filter)
			)
		);

		return (results.Success() && results.begin()[0] ? strtoll(results.begin()[0], nullptr, 10) : 0);
	}

};

#endif //EQEMU_BASE_NPC_TYPES_REPOSITORY_H
