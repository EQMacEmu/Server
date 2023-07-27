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

#ifndef EQEMU_BASE_ITEMS_REPOSITORY_H
#define EQEMU_BASE_ITEMS_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>


class BaseItemsRepository {
public:
	struct Items {
		int32_t     id;
		int16_t     minstatus;
		std::string Name;
		int32_t     aagi;
		int32_t     ac;
		int32_t     acha;
		int32_t     adex;
		int32_t     aint;
		int32_t     asta;
		int32_t     astr;
		int32_t     awis;
		int32_t     bagsize;
		int32_t     bagslots;
		int32_t     bagtype;
		int32_t     bagwr;
		int32_t     banedmgamt;
		int32_t     banedmgbody;
		int32_t     banedmgrace;
		int32_t     bardtype;
		int32_t     bardvalue;
		int32_t     book;
		int32_t     casttime;
		int32_t     casttime_;
		int32_t     classes;
		uint32_t    color;
		int32_t     price;
		int32_t     cr;
		int32_t     damage;
		int32_t     deity;
		int32_t     delay;
		int32_t     dr;
		int32_t     clicktype;
		int32_t     clicklevel2;
		int32_t     elemdmgtype;
		int32_t     elemdmgamt;
		int32_t     factionamt1;
		int32_t     factionamt2;
		int32_t     factionamt3;
		int32_t     factionamt4;
		int32_t     factionmod1;
		int32_t     factionmod2;
		int32_t     factionmod3;
		int32_t     factionmod4;
		std::string filename;
		int32_t     focuseffect;
		int32_t     fr;
		int32_t     fvnodrop;
		int32_t     clicklevel;
		int32_t     hp;
		int32_t     icon;
		std::string idfile;
		int32_t     itemclass;
		int32_t     itemtype;
		int32_t     light;
		std::string lore;
		int32_t     magic;
		int32_t     mana;
		int32_t     material;
		int32_t     maxcharges;
		int32_t     mr;
		int32_t     nodrop;
		int32_t     norent;
		int32_t     pr;
		int32_t     procrate;
		int32_t     races;
		int32_t     range;
		int32_t     reclevel;
		int32_t     recskill;
		int32_t     reqlevel;
		float       sellrate;
		int32_t     size;
		int32_t     skillmodtype;
		int32_t     skillmodvalue;
		int32_t     slots;
		int32_t     clickeffect;
		int32_t     tradeskills;
		int32_t     weight;
		int32_t     booktype;
		int32_t     recastdelay;
		int32_t     recasttype;
		time_t      updated;
		std::string comment;
		int32_t     stacksize;
		int32_t     stackable;
		int32_t     proceffect;
		int32_t     proctype;
		int32_t     proclevel2;
		int32_t     proclevel;
		int32_t     worneffect;
		int32_t     worntype;
		int32_t     wornlevel2;
		int32_t     wornlevel;
		int32_t     focustype;
		int32_t     focuslevel2;
		int32_t     focuslevel;
		int32_t     scrolleffect;
		int32_t     scrolltype;
		int32_t     scrolllevel2;
		int32_t     scrolllevel;
		time_t      serialized;
		time_t      verified;
		std::string serialization;
		std::string source;
		std::string lorefile;
		int32_t     questitemflag;
		int32_t     clickunk5;
		std::string clickunk6;
		int32_t     clickunk7;
		int32_t     procunk1;
		int32_t     procunk2;
		int32_t     procunk3;
		int32_t     procunk4;
		std::string procunk6;
		int32_t     procunk7;
		int32_t     wornunk1;
		int32_t     wornunk2;
		int32_t     wornunk3;
		int32_t     wornunk4;
		int32_t     wornunk5;
		std::string wornunk6;
		int32_t     wornunk7;
		int32_t     focusunk1;
		int32_t     focusunk2;
		int32_t     focusunk3;
		int32_t     focusunk4;
		int32_t     focusunk5;
		std::string focusunk6;
		int32_t     focusunk7;
		int32_t     scrollunk1;
		int32_t     scrollunk2;
		int32_t     scrollunk3;
		int32_t     scrollunk4;
		int32_t     scrollunk5;
		std::string scrollunk6;
		int32_t     scrollunk7;
		std::string clickname;
		std::string procname;
		std::string wornname;
		std::string focusname;
		std::string scrollname;
		std::string created;
		int16_t     bardeffect;
		int16_t     bardeffecttype;
		int16_t     bardlevel2;
		int16_t     bardlevel;
		int16_t     bardunk1;
		int16_t     bardunk2;
		int16_t     bardunk3;
		int16_t     bardunk4;
		int16_t     bardunk5;
		std::string bardname;
		int16_t     bardunk7;
		int8_t      gmflag;
		int8_t      soulbound;
	};

	static std::string PrimaryKey()
	{
		return std::string("id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"id",
			"minstatus",
			"Name",
			"aagi",
			"ac",
			"acha",
			"adex",
			"aint",
			"asta",
			"astr",
			"awis",
			"bagsize",
			"bagslots",
			"bagtype",
			"bagwr",
			"banedmgamt",
			"banedmgbody",
			"banedmgrace",
			"bardtype",
			"bardvalue",
			"book",
			"casttime",
			"casttime_",
			"classes",
			"color",
			"price",
			"cr",
			"damage",
			"deity",
			"delay",
			"dr",
			"clicktype",
			"clicklevel2",
			"elemdmgtype",
			"elemdmgamt",
			"factionamt1",
			"factionamt2",
			"factionamt3",
			"factionamt4",
			"factionmod1",
			"factionmod2",
			"factionmod3",
			"factionmod4",
			"filename",
			"focuseffect",
			"fr",
			"fvnodrop",
			"clicklevel",
			"hp",
			"icon",
			"idfile",
			"itemclass",
			"itemtype",
			"light",
			"lore",
			"magic",
			"mana",
			"material",
			"maxcharges",
			"mr",
			"nodrop",
			"norent",
			"pr",
			"procrate",
			"races",
			"range",
			"reclevel",
			"recskill",
			"reqlevel",
			"sellrate",
			"size",
			"skillmodtype",
			"skillmodvalue",
			"slots",
			"clickeffect",
			"tradeskills",
			"weight",
			"booktype",
			"recastdelay",
			"recasttype",
			"updated",
			"comment",
			"stacksize",
			"stackable",
			"proceffect",
			"proctype",
			"proclevel2",
			"proclevel",
			"worneffect",
			"worntype",
			"wornlevel2",
			"wornlevel",
			"focustype",
			"focuslevel2",
			"focuslevel",
			"scrolleffect",
			"scrolltype",
			"scrolllevel2",
			"scrolllevel",
			"serialized",
			"verified",
			"serialization",
			"source",
			"lorefile",
			"questitemflag",
			"clickunk5",
			"clickunk6",
			"clickunk7",
			"procunk1",
			"procunk2",
			"procunk3",
			"procunk4",
			"procunk6",
			"procunk7",
			"wornunk1",
			"wornunk2",
			"wornunk3",
			"wornunk4",
			"wornunk5",
			"wornunk6",
			"wornunk7",
			"focusunk1",
			"focusunk2",
			"focusunk3",
			"focusunk4",
			"focusunk5",
			"focusunk6",
			"focusunk7",
			"scrollunk1",
			"scrollunk2",
			"scrollunk3",
			"scrollunk4",
			"scrollunk5",
			"scrollunk6",
			"scrollunk7",
			"clickname",
			"procname",
			"wornname",
			"focusname",
			"scrollname",
			"created",
			"bardeffect",
			"bardeffecttype",
			"bardlevel2",
			"bardlevel",
			"bardunk1",
			"bardunk2",
			"bardunk3",
			"bardunk4",
			"bardunk5",
			"bardname",
			"bardunk7",
			"gmflag",
			"soulbound",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"id",
			"minstatus",
			"Name",
			"aagi",
			"ac",
			"acha",
			"adex",
			"aint",
			"asta",
			"astr",
			"awis",
			"bagsize",
			"bagslots",
			"bagtype",
			"bagwr",
			"banedmgamt",
			"banedmgbody",
			"banedmgrace",
			"bardtype",
			"bardvalue",
			"book",
			"casttime",
			"casttime_",
			"classes",
			"color",
			"price",
			"cr",
			"damage",
			"deity",
			"delay",
			"dr",
			"clicktype",
			"clicklevel2",
			"elemdmgtype",
			"elemdmgamt",
			"factionamt1",
			"factionamt2",
			"factionamt3",
			"factionamt4",
			"factionmod1",
			"factionmod2",
			"factionmod3",
			"factionmod4",
			"filename",
			"focuseffect",
			"fr",
			"fvnodrop",
			"clicklevel",
			"hp",
			"icon",
			"idfile",
			"itemclass",
			"itemtype",
			"light",
			"lore",
			"magic",
			"mana",
			"material",
			"maxcharges",
			"mr",
			"nodrop",
			"norent",
			"pr",
			"procrate",
			"races",
			"range",
			"reclevel",
			"recskill",
			"reqlevel",
			"sellrate",
			"size",
			"skillmodtype",
			"skillmodvalue",
			"slots",
			"clickeffect",
			"tradeskills",
			"weight",
			"booktype",
			"recastdelay",
			"recasttype",
			"UNIX_TIMESTAMP(updated)",
			"comment",
			"stacksize",
			"stackable",
			"proceffect",
			"proctype",
			"proclevel2",
			"proclevel",
			"worneffect",
			"worntype",
			"wornlevel2",
			"wornlevel",
			"focustype",
			"focuslevel2",
			"focuslevel",
			"scrolleffect",
			"scrolltype",
			"scrolllevel2",
			"scrolllevel",
			"UNIX_TIMESTAMP(serialized)",
			"UNIX_TIMESTAMP(verified)",
			"serialization",
			"source",
			"lorefile",
			"questitemflag",
			"clickunk5",
			"clickunk6",
			"clickunk7",
			"procunk1",
			"procunk2",
			"procunk3",
			"procunk4",
			"procunk6",
			"procunk7",
			"wornunk1",
			"wornunk2",
			"wornunk3",
			"wornunk4",
			"wornunk5",
			"wornunk6",
			"wornunk7",
			"focusunk1",
			"focusunk2",
			"focusunk3",
			"focusunk4",
			"focusunk5",
			"focusunk6",
			"focusunk7",
			"scrollunk1",
			"scrollunk2",
			"scrollunk3",
			"scrollunk4",
			"scrollunk5",
			"scrollunk6",
			"scrollunk7",
			"clickname",
			"procname",
			"wornname",
			"focusname",
			"scrollname",
			"created",
			"bardeffect",
			"bardeffecttype",
			"bardlevel2",
			"bardlevel",
			"bardunk1",
			"bardunk2",
			"bardunk3",
			"bardunk4",
			"bardunk5",
			"bardname",
			"bardunk7",
			"gmflag",
			"soulbound",
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
		return std::string("items");
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

	static Items NewEntity()
	{
		Items e{};

		e.id             = 0;
		e.minstatus      = 0;
		e.Name           = "";
		e.aagi           = 0;
		e.ac             = 0;
		e.acha           = 0;
		e.adex           = 0;
		e.aint           = 0;
		e.asta           = 0;
		e.astr           = 0;
		e.awis           = 0;
		e.bagsize        = 0;
		e.bagslots       = 0;
		e.bagtype        = 0;
		e.bagwr          = 0;
		e.banedmgamt     = 0;
		e.banedmgbody    = 0;
		e.banedmgrace    = 0;
		e.bardtype       = 0;
		e.bardvalue      = 0;
		e.book           = 0;
		e.casttime       = 0;
		e.casttime_      = 0;
		e.classes        = 0;
		e.color          = 0;
		e.price          = 0;
		e.cr             = 0;
		e.damage         = 0;
		e.deity          = 0;
		e.delay          = 0;
		e.dr             = 0;
		e.clicktype      = 0;
		e.clicklevel2    = 0;
		e.elemdmgtype    = 0;
		e.elemdmgamt     = 0;
		e.factionamt1    = 0;
		e.factionamt2    = 0;
		e.factionamt3    = 0;
		e.factionamt4    = 0;
		e.factionmod1    = 0;
		e.factionmod2    = 0;
		e.factionmod3    = 0;
		e.factionmod4    = 0;
		e.filename       = "";
		e.focuseffect    = 0;
		e.fr             = 0;
		e.fvnodrop       = 0;
		e.clicklevel     = 0;
		e.hp             = 0;
		e.icon           = 0;
		e.idfile         = "";
		e.itemclass      = 0;
		e.itemtype       = 0;
		e.light          = 0;
		e.lore           = "";
		e.magic          = 0;
		e.mana           = 0;
		e.material       = 0;
		e.maxcharges     = 0;
		e.mr             = 0;
		e.nodrop         = 0;
		e.norent         = 0;
		e.pr             = 0;
		e.procrate       = 0;
		e.races          = 0;
		e.range          = 0;
		e.reclevel       = 0;
		e.recskill       = 0;
		e.reqlevel       = 0;
		e.sellrate       = 0;
		e.size           = 0;
		e.skillmodtype   = 0;
		e.skillmodvalue  = 0;
		e.slots          = 0;
		e.clickeffect    = 0;
		e.tradeskills    = 0;
		e.weight         = 0;
		e.booktype       = 0;
		e.recastdelay    = 0;
		e.recasttype     = 0;
		e.updated        = 0;
		e.comment        = "";
		e.stacksize      = 0;
		e.stackable      = 0;
		e.proceffect     = 0;
		e.proctype       = 0;
		e.proclevel2     = 0;
		e.proclevel      = 0;
		e.worneffect     = 0;
		e.worntype       = 0;
		e.wornlevel2     = 0;
		e.wornlevel      = 0;
		e.focustype      = 0;
		e.focuslevel2    = 0;
		e.focuslevel     = 0;
		e.scrolleffect   = 0;
		e.scrolltype     = 0;
		e.scrolllevel2   = 0;
		e.scrolllevel    = 0;
		e.serialized     = 0;
		e.verified       = 0;
		e.serialization  = "";
		e.source         = "";
		e.lorefile       = "";
		e.questitemflag  = 0;
		e.clickunk5      = 0;
		e.clickunk6      = "";
		e.clickunk7      = 0;
		e.procunk1       = 0;
		e.procunk2       = 0;
		e.procunk3       = 0;
		e.procunk4       = 0;
		e.procunk6       = "";
		e.procunk7       = 0;
		e.wornunk1       = 0;
		e.wornunk2       = 0;
		e.wornunk3       = 0;
		e.wornunk4       = 0;
		e.wornunk5       = 0;
		e.wornunk6       = "";
		e.wornunk7       = 0;
		e.focusunk1      = 0;
		e.focusunk2      = 0;
		e.focusunk3      = 0;
		e.focusunk4      = 0;
		e.focusunk5      = 0;
		e.focusunk6      = "";
		e.focusunk7      = 0;
		e.scrollunk1     = 0;
		e.scrollunk2     = 0;
		e.scrollunk3     = 0;
		e.scrollunk4     = 0;
		e.scrollunk5     = 0;
		e.scrollunk6     = "";
		e.scrollunk7     = 0;
		e.clickname      = "";
		e.procname       = "";
		e.wornname       = "";
		e.focusname      = "";
		e.scrollname     = "";
		e.created        = "";
		e.bardeffect     = 0;
		e.bardeffecttype = 0;
		e.bardlevel2     = 0;
		e.bardlevel      = 0;
		e.bardunk1       = 0;
		e.bardunk2       = 0;
		e.bardunk3       = 0;
		e.bardunk4       = 0;
		e.bardunk5       = 0;
		e.bardname       = "";
		e.bardunk7       = 0;
		e.gmflag         = 0;
		e.soulbound      = 0;

		return e;
	}

	static Items GetItems(
		const std::vector<Items> &itemss,
		int items_id
	)
	{
		for (auto &items : itemss) {
			if (items.id == items_id) {
				return items;
			}
		}

		return NewEntity();
	}

	static Items FindOne(
		Database& db,
		int items_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				items_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			Items e{};

			e.id             = static_cast<int32_t>(atoi(row[0]));
			e.minstatus      = static_cast<int16_t>(atoi(row[1]));
			e.Name           = row[2] ? row[2] : "";
			e.aagi           = static_cast<int32_t>(atoi(row[3]));
			e.ac             = static_cast<int32_t>(atoi(row[4]));
			e.acha           = static_cast<int32_t>(atoi(row[5]));
			e.adex           = static_cast<int32_t>(atoi(row[6]));
			e.aint           = static_cast<int32_t>(atoi(row[7]));
			e.asta           = static_cast<int32_t>(atoi(row[8]));
			e.astr           = static_cast<int32_t>(atoi(row[9]));
			e.awis           = static_cast<int32_t>(atoi(row[10]));
			e.bagsize        = static_cast<int32_t>(atoi(row[11]));
			e.bagslots       = static_cast<int32_t>(atoi(row[12]));
			e.bagtype        = static_cast<int32_t>(atoi(row[13]));
			e.bagwr          = static_cast<int32_t>(atoi(row[14]));
			e.banedmgamt     = static_cast<int32_t>(atoi(row[15]));
			e.banedmgbody    = static_cast<int32_t>(atoi(row[16]));
			e.banedmgrace    = static_cast<int32_t>(atoi(row[17]));
			e.bardtype       = static_cast<int32_t>(atoi(row[18]));
			e.bardvalue      = static_cast<int32_t>(atoi(row[19]));
			e.book           = static_cast<int32_t>(atoi(row[20]));
			e.casttime       = static_cast<int32_t>(atoi(row[21]));
			e.casttime_      = static_cast<int32_t>(atoi(row[22]));
			e.classes        = static_cast<int32_t>(atoi(row[23]));
			e.color          = static_cast<uint32_t>(strtoul(row[24], nullptr, 10));
			e.price          = static_cast<int32_t>(atoi(row[25]));
			e.cr             = static_cast<int32_t>(atoi(row[26]));
			e.damage         = static_cast<int32_t>(atoi(row[27]));
			e.deity          = static_cast<int32_t>(atoi(row[28]));
			e.delay          = static_cast<int32_t>(atoi(row[29]));
			e.dr             = static_cast<int32_t>(atoi(row[30]));
			e.clicktype      = static_cast<int32_t>(atoi(row[31]));
			e.clicklevel2    = static_cast<int32_t>(atoi(row[32]));
			e.elemdmgtype    = static_cast<int32_t>(atoi(row[33]));
			e.elemdmgamt     = static_cast<int32_t>(atoi(row[34]));
			e.factionamt1    = static_cast<int32_t>(atoi(row[35]));
			e.factionamt2    = static_cast<int32_t>(atoi(row[36]));
			e.factionamt3    = static_cast<int32_t>(atoi(row[37]));
			e.factionamt4    = static_cast<int32_t>(atoi(row[38]));
			e.factionmod1    = static_cast<int32_t>(atoi(row[39]));
			e.factionmod2    = static_cast<int32_t>(atoi(row[40]));
			e.factionmod3    = static_cast<int32_t>(atoi(row[41]));
			e.factionmod4    = static_cast<int32_t>(atoi(row[42]));
			e.filename       = row[43] ? row[43] : "";
			e.focuseffect    = static_cast<int32_t>(atoi(row[44]));
			e.fr             = static_cast<int32_t>(atoi(row[45]));
			e.fvnodrop       = static_cast<int32_t>(atoi(row[46]));
			e.clicklevel     = static_cast<int32_t>(atoi(row[47]));
			e.hp             = static_cast<int32_t>(atoi(row[48]));
			e.icon           = static_cast<int32_t>(atoi(row[49]));
			e.idfile         = row[50] ? row[50] : "";
			e.itemclass      = static_cast<int32_t>(atoi(row[51]));
			e.itemtype       = static_cast<int32_t>(atoi(row[52]));
			e.light          = static_cast<int32_t>(atoi(row[53]));
			e.lore           = row[54] ? row[54] : "";
			e.magic          = static_cast<int32_t>(atoi(row[55]));
			e.mana           = static_cast<int32_t>(atoi(row[56]));
			e.material       = static_cast<int32_t>(atoi(row[57]));
			e.maxcharges     = static_cast<int32_t>(atoi(row[58]));
			e.mr             = static_cast<int32_t>(atoi(row[59]));
			e.nodrop         = static_cast<int32_t>(atoi(row[60]));
			e.norent         = static_cast<int32_t>(atoi(row[61]));
			e.pr             = static_cast<int32_t>(atoi(row[62]));
			e.procrate       = static_cast<int32_t>(atoi(row[63]));
			e.races          = static_cast<int32_t>(atoi(row[64]));
			e.range          = static_cast<int32_t>(atoi(row[65]));
			e.reclevel       = static_cast<int32_t>(atoi(row[66]));
			e.recskill       = static_cast<int32_t>(atoi(row[67]));
			e.reqlevel       = static_cast<int32_t>(atoi(row[68]));
			e.sellrate       = strtof(row[69], nullptr);
			e.size           = static_cast<int32_t>(atoi(row[70]));
			e.skillmodtype   = static_cast<int32_t>(atoi(row[71]));
			e.skillmodvalue  = static_cast<int32_t>(atoi(row[72]));
			e.slots          = static_cast<int32_t>(atoi(row[73]));
			e.clickeffect    = static_cast<int32_t>(atoi(row[74]));
			e.tradeskills    = static_cast<int32_t>(atoi(row[75]));
			e.weight         = static_cast<int32_t>(atoi(row[76]));
			e.booktype       = static_cast<int32_t>(atoi(row[77]));
			e.recastdelay    = static_cast<int32_t>(atoi(row[78]));
			e.recasttype     = static_cast<int32_t>(atoi(row[79]));
			e.updated        = strtoll(row[80] ? row[80] : "-1", nullptr, 10);
			e.comment        = row[81] ? row[81] : "";
			e.stacksize      = static_cast<int32_t>(atoi(row[82]));
			e.stackable      = static_cast<int32_t>(atoi(row[83]));
			e.proceffect     = static_cast<int32_t>(atoi(row[84]));
			e.proctype       = static_cast<int32_t>(atoi(row[85]));
			e.proclevel2     = static_cast<int32_t>(atoi(row[86]));
			e.proclevel      = static_cast<int32_t>(atoi(row[87]));
			e.worneffect     = static_cast<int32_t>(atoi(row[88]));
			e.worntype       = static_cast<int32_t>(atoi(row[89]));
			e.wornlevel2     = static_cast<int32_t>(atoi(row[90]));
			e.wornlevel      = static_cast<int32_t>(atoi(row[91]));
			e.focustype      = static_cast<int32_t>(atoi(row[92]));
			e.focuslevel2    = static_cast<int32_t>(atoi(row[93]));
			e.focuslevel     = static_cast<int32_t>(atoi(row[94]));
			e.scrolleffect   = static_cast<int32_t>(atoi(row[95]));
			e.scrolltype     = static_cast<int32_t>(atoi(row[96]));
			e.scrolllevel2   = static_cast<int32_t>(atoi(row[97]));
			e.scrolllevel    = static_cast<int32_t>(atoi(row[98]));
			e.serialized     = strtoll(row[99] ? row[99] : "-1", nullptr, 10);
			e.verified       = strtoll(row[100] ? row[100] : "-1", nullptr, 10);
			e.serialization  = row[101] ? row[101] : "";
			e.source         = row[102] ? row[102] : "";
			e.lorefile       = row[103] ? row[103] : "";
			e.questitemflag  = static_cast<int32_t>(atoi(row[104]));
			e.clickunk5      = static_cast<int32_t>(atoi(row[105]));
			e.clickunk6      = row[106] ? row[106] : "";
			e.clickunk7      = static_cast<int32_t>(atoi(row[107]));
			e.procunk1       = static_cast<int32_t>(atoi(row[108]));
			e.procunk2       = static_cast<int32_t>(atoi(row[109]));
			e.procunk3       = static_cast<int32_t>(atoi(row[110]));
			e.procunk4       = static_cast<int32_t>(atoi(row[111]));
			e.procunk6       = row[112] ? row[112] : "";
			e.procunk7       = static_cast<int32_t>(atoi(row[113]));
			e.wornunk1       = static_cast<int32_t>(atoi(row[114]));
			e.wornunk2       = static_cast<int32_t>(atoi(row[115]));
			e.wornunk3       = static_cast<int32_t>(atoi(row[116]));
			e.wornunk4       = static_cast<int32_t>(atoi(row[117]));
			e.wornunk5       = static_cast<int32_t>(atoi(row[118]));
			e.wornunk6       = row[119] ? row[119] : "";
			e.wornunk7       = static_cast<int32_t>(atoi(row[120]));
			e.focusunk1      = static_cast<int32_t>(atoi(row[121]));
			e.focusunk2      = static_cast<int32_t>(atoi(row[122]));
			e.focusunk3      = static_cast<int32_t>(atoi(row[123]));
			e.focusunk4      = static_cast<int32_t>(atoi(row[124]));
			e.focusunk5      = static_cast<int32_t>(atoi(row[125]));
			e.focusunk6      = row[126] ? row[126] : "";
			e.focusunk7      = static_cast<int32_t>(atoi(row[127]));
			e.scrollunk1     = static_cast<int32_t>(atoi(row[128]));
			e.scrollunk2     = static_cast<int32_t>(atoi(row[129]));
			e.scrollunk3     = static_cast<int32_t>(atoi(row[130]));
			e.scrollunk4     = static_cast<int32_t>(atoi(row[131]));
			e.scrollunk5     = static_cast<int32_t>(atoi(row[132]));
			e.scrollunk6     = row[133] ? row[133] : "";
			e.scrollunk7     = static_cast<int32_t>(atoi(row[134]));
			e.clickname      = row[135] ? row[135] : "";
			e.procname       = row[136] ? row[136] : "";
			e.wornname       = row[137] ? row[137] : "";
			e.focusname      = row[138] ? row[138] : "";
			e.scrollname     = row[139] ? row[139] : "";
			e.created        = row[140] ? row[140] : "";
			e.bardeffect     = static_cast<int16_t>(atoi(row[141]));
			e.bardeffecttype = static_cast<int16_t>(atoi(row[142]));
			e.bardlevel2     = static_cast<int16_t>(atoi(row[143]));
			e.bardlevel      = static_cast<int16_t>(atoi(row[144]));
			e.bardunk1       = static_cast<int16_t>(atoi(row[145]));
			e.bardunk2       = static_cast<int16_t>(atoi(row[146]));
			e.bardunk3       = static_cast<int16_t>(atoi(row[147]));
			e.bardunk4       = static_cast<int16_t>(atoi(row[148]));
			e.bardunk5       = static_cast<int16_t>(atoi(row[149]));
			e.bardname       = row[150] ? row[150] : "";
			e.bardunk7       = static_cast<int16_t>(atoi(row[151]));
			e.gmflag         = static_cast<int8_t>(atoi(row[152]));
			e.soulbound      = static_cast<int8_t>(atoi(row[153]));

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int items_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				items_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const Items &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = " + std::to_string(e.id));
		v.push_back(columns[1] + " = " + std::to_string(e.minstatus));
		v.push_back(columns[2] + " = '" + Strings::Escape(e.Name) + "'");
		v.push_back(columns[3] + " = " + std::to_string(e.aagi));
		v.push_back(columns[4] + " = " + std::to_string(e.ac));
		v.push_back(columns[5] + " = " + std::to_string(e.acha));
		v.push_back(columns[6] + " = " + std::to_string(e.adex));
		v.push_back(columns[7] + " = " + std::to_string(e.aint));
		v.push_back(columns[8] + " = " + std::to_string(e.asta));
		v.push_back(columns[9] + " = " + std::to_string(e.astr));
		v.push_back(columns[10] + " = " + std::to_string(e.awis));
		v.push_back(columns[11] + " = " + std::to_string(e.bagsize));
		v.push_back(columns[12] + " = " + std::to_string(e.bagslots));
		v.push_back(columns[13] + " = " + std::to_string(e.bagtype));
		v.push_back(columns[14] + " = " + std::to_string(e.bagwr));
		v.push_back(columns[15] + " = " + std::to_string(e.banedmgamt));
		v.push_back(columns[16] + " = " + std::to_string(e.banedmgbody));
		v.push_back(columns[17] + " = " + std::to_string(e.banedmgrace));
		v.push_back(columns[18] + " = " + std::to_string(e.bardtype));
		v.push_back(columns[19] + " = " + std::to_string(e.bardvalue));
		v.push_back(columns[20] + " = " + std::to_string(e.book));
		v.push_back(columns[21] + " = " + std::to_string(e.casttime));
		v.push_back(columns[22] + " = " + std::to_string(e.casttime_));
		v.push_back(columns[23] + " = " + std::to_string(e.classes));
		v.push_back(columns[24] + " = " + std::to_string(e.color));
		v.push_back(columns[25] + " = " + std::to_string(e.price));
		v.push_back(columns[26] + " = " + std::to_string(e.cr));
		v.push_back(columns[27] + " = " + std::to_string(e.damage));
		v.push_back(columns[28] + " = " + std::to_string(e.deity));
		v.push_back(columns[29] + " = " + std::to_string(e.delay));
		v.push_back(columns[30] + " = " + std::to_string(e.dr));
		v.push_back(columns[31] + " = " + std::to_string(e.clicktype));
		v.push_back(columns[32] + " = " + std::to_string(e.clicklevel2));
		v.push_back(columns[33] + " = " + std::to_string(e.elemdmgtype));
		v.push_back(columns[34] + " = " + std::to_string(e.elemdmgamt));
		v.push_back(columns[35] + " = " + std::to_string(e.factionamt1));
		v.push_back(columns[36] + " = " + std::to_string(e.factionamt2));
		v.push_back(columns[37] + " = " + std::to_string(e.factionamt3));
		v.push_back(columns[38] + " = " + std::to_string(e.factionamt4));
		v.push_back(columns[39] + " = " + std::to_string(e.factionmod1));
		v.push_back(columns[40] + " = " + std::to_string(e.factionmod2));
		v.push_back(columns[41] + " = " + std::to_string(e.factionmod3));
		v.push_back(columns[42] + " = " + std::to_string(e.factionmod4));
		v.push_back(columns[43] + " = '" + Strings::Escape(e.filename) + "'");
		v.push_back(columns[44] + " = " + std::to_string(e.focuseffect));
		v.push_back(columns[45] + " = " + std::to_string(e.fr));
		v.push_back(columns[46] + " = " + std::to_string(e.fvnodrop));
		v.push_back(columns[47] + " = " + std::to_string(e.clicklevel));
		v.push_back(columns[48] + " = " + std::to_string(e.hp));
		v.push_back(columns[49] + " = " + std::to_string(e.icon));
		v.push_back(columns[50] + " = '" + Strings::Escape(e.idfile) + "'");
		v.push_back(columns[51] + " = " + std::to_string(e.itemclass));
		v.push_back(columns[52] + " = " + std::to_string(e.itemtype));
		v.push_back(columns[53] + " = " + std::to_string(e.light));
		v.push_back(columns[54] + " = '" + Strings::Escape(e.lore) + "'");
		v.push_back(columns[55] + " = " + std::to_string(e.magic));
		v.push_back(columns[56] + " = " + std::to_string(e.mana));
		v.push_back(columns[57] + " = " + std::to_string(e.material));
		v.push_back(columns[58] + " = " + std::to_string(e.maxcharges));
		v.push_back(columns[59] + " = " + std::to_string(e.mr));
		v.push_back(columns[60] + " = " + std::to_string(e.nodrop));
		v.push_back(columns[61] + " = " + std::to_string(e.norent));
		v.push_back(columns[62] + " = " + std::to_string(e.pr));
		v.push_back(columns[63] + " = " + std::to_string(e.procrate));
		v.push_back(columns[64] + " = " + std::to_string(e.races));
		v.push_back(columns[65] + " = " + std::to_string(e.range));
		v.push_back(columns[66] + " = " + std::to_string(e.reclevel));
		v.push_back(columns[67] + " = " + std::to_string(e.recskill));
		v.push_back(columns[68] + " = " + std::to_string(e.reqlevel));
		v.push_back(columns[69] + " = " + std::to_string(e.sellrate));
		v.push_back(columns[70] + " = " + std::to_string(e.size));
		v.push_back(columns[71] + " = " + std::to_string(e.skillmodtype));
		v.push_back(columns[72] + " = " + std::to_string(e.skillmodvalue));
		v.push_back(columns[73] + " = " + std::to_string(e.slots));
		v.push_back(columns[74] + " = " + std::to_string(e.clickeffect));
		v.push_back(columns[75] + " = " + std::to_string(e.tradeskills));
		v.push_back(columns[76] + " = " + std::to_string(e.weight));
		v.push_back(columns[77] + " = " + std::to_string(e.booktype));
		v.push_back(columns[78] + " = " + std::to_string(e.recastdelay));
		v.push_back(columns[79] + " = " + std::to_string(e.recasttype));
		v.push_back(columns[80] + " = FROM_UNIXTIME(" + (e.updated > 0 ? std::to_string(e.updated) : "null") + ")");
		v.push_back(columns[81] + " = '" + Strings::Escape(e.comment) + "'");
		v.push_back(columns[82] + " = " + std::to_string(e.stacksize));
		v.push_back(columns[83] + " = " + std::to_string(e.stackable));
		v.push_back(columns[84] + " = " + std::to_string(e.proceffect));
		v.push_back(columns[85] + " = " + std::to_string(e.proctype));
		v.push_back(columns[86] + " = " + std::to_string(e.proclevel2));
		v.push_back(columns[87] + " = " + std::to_string(e.proclevel));
		v.push_back(columns[88] + " = " + std::to_string(e.worneffect));
		v.push_back(columns[89] + " = " + std::to_string(e.worntype));
		v.push_back(columns[90] + " = " + std::to_string(e.wornlevel2));
		v.push_back(columns[91] + " = " + std::to_string(e.wornlevel));
		v.push_back(columns[92] + " = " + std::to_string(e.focustype));
		v.push_back(columns[93] + " = " + std::to_string(e.focuslevel2));
		v.push_back(columns[94] + " = " + std::to_string(e.focuslevel));
		v.push_back(columns[95] + " = " + std::to_string(e.scrolleffect));
		v.push_back(columns[96] + " = " + std::to_string(e.scrolltype));
		v.push_back(columns[97] + " = " + std::to_string(e.scrolllevel2));
		v.push_back(columns[98] + " = " + std::to_string(e.scrolllevel));
		v.push_back(columns[99] + " = FROM_UNIXTIME(" + (e.serialized > 0 ? std::to_string(e.serialized) : "null") + ")");
		v.push_back(columns[100] + " = FROM_UNIXTIME(" + (e.verified > 0 ? std::to_string(e.verified) : "null") + ")");
		v.push_back(columns[101] + " = '" + Strings::Escape(e.serialization) + "'");
		v.push_back(columns[102] + " = '" + Strings::Escape(e.source) + "'");
		v.push_back(columns[103] + " = '" + Strings::Escape(e.lorefile) + "'");
		v.push_back(columns[104] + " = " + std::to_string(e.questitemflag));
		v.push_back(columns[105] + " = " + std::to_string(e.clickunk5));
		v.push_back(columns[106] + " = '" + Strings::Escape(e.clickunk6) + "'");
		v.push_back(columns[107] + " = " + std::to_string(e.clickunk7));
		v.push_back(columns[108] + " = " + std::to_string(e.procunk1));
		v.push_back(columns[109] + " = " + std::to_string(e.procunk2));
		v.push_back(columns[110] + " = " + std::to_string(e.procunk3));
		v.push_back(columns[111] + " = " + std::to_string(e.procunk4));
		v.push_back(columns[112] + " = '" + Strings::Escape(e.procunk6) + "'");
		v.push_back(columns[113] + " = " + std::to_string(e.procunk7));
		v.push_back(columns[114] + " = " + std::to_string(e.wornunk1));
		v.push_back(columns[115] + " = " + std::to_string(e.wornunk2));
		v.push_back(columns[116] + " = " + std::to_string(e.wornunk3));
		v.push_back(columns[117] + " = " + std::to_string(e.wornunk4));
		v.push_back(columns[118] + " = " + std::to_string(e.wornunk5));
		v.push_back(columns[119] + " = '" + Strings::Escape(e.wornunk6) + "'");
		v.push_back(columns[120] + " = " + std::to_string(e.wornunk7));
		v.push_back(columns[121] + " = " + std::to_string(e.focusunk1));
		v.push_back(columns[122] + " = " + std::to_string(e.focusunk2));
		v.push_back(columns[123] + " = " + std::to_string(e.focusunk3));
		v.push_back(columns[124] + " = " + std::to_string(e.focusunk4));
		v.push_back(columns[125] + " = " + std::to_string(e.focusunk5));
		v.push_back(columns[126] + " = '" + Strings::Escape(e.focusunk6) + "'");
		v.push_back(columns[127] + " = " + std::to_string(e.focusunk7));
		v.push_back(columns[128] + " = " + std::to_string(e.scrollunk1));
		v.push_back(columns[129] + " = " + std::to_string(e.scrollunk2));
		v.push_back(columns[130] + " = " + std::to_string(e.scrollunk3));
		v.push_back(columns[131] + " = " + std::to_string(e.scrollunk4));
		v.push_back(columns[132] + " = " + std::to_string(e.scrollunk5));
		v.push_back(columns[133] + " = '" + Strings::Escape(e.scrollunk6) + "'");
		v.push_back(columns[134] + " = " + std::to_string(e.scrollunk7));
		v.push_back(columns[135] + " = '" + Strings::Escape(e.clickname) + "'");
		v.push_back(columns[136] + " = '" + Strings::Escape(e.procname) + "'");
		v.push_back(columns[137] + " = '" + Strings::Escape(e.wornname) + "'");
		v.push_back(columns[138] + " = '" + Strings::Escape(e.focusname) + "'");
		v.push_back(columns[139] + " = '" + Strings::Escape(e.scrollname) + "'");
		v.push_back(columns[140] + " = '" + Strings::Escape(e.created) + "'");
		v.push_back(columns[141] + " = " + std::to_string(e.bardeffect));
		v.push_back(columns[142] + " = " + std::to_string(e.bardeffecttype));
		v.push_back(columns[143] + " = " + std::to_string(e.bardlevel2));
		v.push_back(columns[144] + " = " + std::to_string(e.bardlevel));
		v.push_back(columns[145] + " = " + std::to_string(e.bardunk1));
		v.push_back(columns[146] + " = " + std::to_string(e.bardunk2));
		v.push_back(columns[147] + " = " + std::to_string(e.bardunk3));
		v.push_back(columns[148] + " = " + std::to_string(e.bardunk4));
		v.push_back(columns[149] + " = " + std::to_string(e.bardunk5));
		v.push_back(columns[150] + " = '" + Strings::Escape(e.bardname) + "'");
		v.push_back(columns[151] + " = " + std::to_string(e.bardunk7));
		v.push_back(columns[152] + " = " + std::to_string(e.gmflag));
		v.push_back(columns[153] + " = " + std::to_string(e.soulbound));

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

	static Items InsertOne(
		Database& db,
		Items e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.minstatus));
		v.push_back("'" + Strings::Escape(e.Name) + "'");
		v.push_back(std::to_string(e.aagi));
		v.push_back(std::to_string(e.ac));
		v.push_back(std::to_string(e.acha));
		v.push_back(std::to_string(e.adex));
		v.push_back(std::to_string(e.aint));
		v.push_back(std::to_string(e.asta));
		v.push_back(std::to_string(e.astr));
		v.push_back(std::to_string(e.awis));
		v.push_back(std::to_string(e.bagsize));
		v.push_back(std::to_string(e.bagslots));
		v.push_back(std::to_string(e.bagtype));
		v.push_back(std::to_string(e.bagwr));
		v.push_back(std::to_string(e.banedmgamt));
		v.push_back(std::to_string(e.banedmgbody));
		v.push_back(std::to_string(e.banedmgrace));
		v.push_back(std::to_string(e.bardtype));
		v.push_back(std::to_string(e.bardvalue));
		v.push_back(std::to_string(e.book));
		v.push_back(std::to_string(e.casttime));
		v.push_back(std::to_string(e.casttime_));
		v.push_back(std::to_string(e.classes));
		v.push_back(std::to_string(e.color));
		v.push_back(std::to_string(e.price));
		v.push_back(std::to_string(e.cr));
		v.push_back(std::to_string(e.damage));
		v.push_back(std::to_string(e.deity));
		v.push_back(std::to_string(e.delay));
		v.push_back(std::to_string(e.dr));
		v.push_back(std::to_string(e.clicktype));
		v.push_back(std::to_string(e.clicklevel2));
		v.push_back(std::to_string(e.elemdmgtype));
		v.push_back(std::to_string(e.elemdmgamt));
		v.push_back(std::to_string(e.factionamt1));
		v.push_back(std::to_string(e.factionamt2));
		v.push_back(std::to_string(e.factionamt3));
		v.push_back(std::to_string(e.factionamt4));
		v.push_back(std::to_string(e.factionmod1));
		v.push_back(std::to_string(e.factionmod2));
		v.push_back(std::to_string(e.factionmod3));
		v.push_back(std::to_string(e.factionmod4));
		v.push_back("'" + Strings::Escape(e.filename) + "'");
		v.push_back(std::to_string(e.focuseffect));
		v.push_back(std::to_string(e.fr));
		v.push_back(std::to_string(e.fvnodrop));
		v.push_back(std::to_string(e.clicklevel));
		v.push_back(std::to_string(e.hp));
		v.push_back(std::to_string(e.icon));
		v.push_back("'" + Strings::Escape(e.idfile) + "'");
		v.push_back(std::to_string(e.itemclass));
		v.push_back(std::to_string(e.itemtype));
		v.push_back(std::to_string(e.light));
		v.push_back("'" + Strings::Escape(e.lore) + "'");
		v.push_back(std::to_string(e.magic));
		v.push_back(std::to_string(e.mana));
		v.push_back(std::to_string(e.material));
		v.push_back(std::to_string(e.maxcharges));
		v.push_back(std::to_string(e.mr));
		v.push_back(std::to_string(e.nodrop));
		v.push_back(std::to_string(e.norent));
		v.push_back(std::to_string(e.pr));
		v.push_back(std::to_string(e.procrate));
		v.push_back(std::to_string(e.races));
		v.push_back(std::to_string(e.range));
		v.push_back(std::to_string(e.reclevel));
		v.push_back(std::to_string(e.recskill));
		v.push_back(std::to_string(e.reqlevel));
		v.push_back(std::to_string(e.sellrate));
		v.push_back(std::to_string(e.size));
		v.push_back(std::to_string(e.skillmodtype));
		v.push_back(std::to_string(e.skillmodvalue));
		v.push_back(std::to_string(e.slots));
		v.push_back(std::to_string(e.clickeffect));
		v.push_back(std::to_string(e.tradeskills));
		v.push_back(std::to_string(e.weight));
		v.push_back(std::to_string(e.booktype));
		v.push_back(std::to_string(e.recastdelay));
		v.push_back(std::to_string(e.recasttype));
		v.push_back("FROM_UNIXTIME(" + (e.updated > 0 ? std::to_string(e.updated) : "null") + ")");
		v.push_back("'" + Strings::Escape(e.comment) + "'");
		v.push_back(std::to_string(e.stacksize));
		v.push_back(std::to_string(e.stackable));
		v.push_back(std::to_string(e.proceffect));
		v.push_back(std::to_string(e.proctype));
		v.push_back(std::to_string(e.proclevel2));
		v.push_back(std::to_string(e.proclevel));
		v.push_back(std::to_string(e.worneffect));
		v.push_back(std::to_string(e.worntype));
		v.push_back(std::to_string(e.wornlevel2));
		v.push_back(std::to_string(e.wornlevel));
		v.push_back(std::to_string(e.focustype));
		v.push_back(std::to_string(e.focuslevel2));
		v.push_back(std::to_string(e.focuslevel));
		v.push_back(std::to_string(e.scrolleffect));
		v.push_back(std::to_string(e.scrolltype));
		v.push_back(std::to_string(e.scrolllevel2));
		v.push_back(std::to_string(e.scrolllevel));
		v.push_back("FROM_UNIXTIME(" + (e.serialized > 0 ? std::to_string(e.serialized) : "null") + ")");
		v.push_back("FROM_UNIXTIME(" + (e.verified > 0 ? std::to_string(e.verified) : "null") + ")");
		v.push_back("'" + Strings::Escape(e.serialization) + "'");
		v.push_back("'" + Strings::Escape(e.source) + "'");
		v.push_back("'" + Strings::Escape(e.lorefile) + "'");
		v.push_back(std::to_string(e.questitemflag));
		v.push_back(std::to_string(e.clickunk5));
		v.push_back("'" + Strings::Escape(e.clickunk6) + "'");
		v.push_back(std::to_string(e.clickunk7));
		v.push_back(std::to_string(e.procunk1));
		v.push_back(std::to_string(e.procunk2));
		v.push_back(std::to_string(e.procunk3));
		v.push_back(std::to_string(e.procunk4));
		v.push_back("'" + Strings::Escape(e.procunk6) + "'");
		v.push_back(std::to_string(e.procunk7));
		v.push_back(std::to_string(e.wornunk1));
		v.push_back(std::to_string(e.wornunk2));
		v.push_back(std::to_string(e.wornunk3));
		v.push_back(std::to_string(e.wornunk4));
		v.push_back(std::to_string(e.wornunk5));
		v.push_back("'" + Strings::Escape(e.wornunk6) + "'");
		v.push_back(std::to_string(e.wornunk7));
		v.push_back(std::to_string(e.focusunk1));
		v.push_back(std::to_string(e.focusunk2));
		v.push_back(std::to_string(e.focusunk3));
		v.push_back(std::to_string(e.focusunk4));
		v.push_back(std::to_string(e.focusunk5));
		v.push_back("'" + Strings::Escape(e.focusunk6) + "'");
		v.push_back(std::to_string(e.focusunk7));
		v.push_back(std::to_string(e.scrollunk1));
		v.push_back(std::to_string(e.scrollunk2));
		v.push_back(std::to_string(e.scrollunk3));
		v.push_back(std::to_string(e.scrollunk4));
		v.push_back(std::to_string(e.scrollunk5));
		v.push_back("'" + Strings::Escape(e.scrollunk6) + "'");
		v.push_back(std::to_string(e.scrollunk7));
		v.push_back("'" + Strings::Escape(e.clickname) + "'");
		v.push_back("'" + Strings::Escape(e.procname) + "'");
		v.push_back("'" + Strings::Escape(e.wornname) + "'");
		v.push_back("'" + Strings::Escape(e.focusname) + "'");
		v.push_back("'" + Strings::Escape(e.scrollname) + "'");
		v.push_back("'" + Strings::Escape(e.created) + "'");
		v.push_back(std::to_string(e.bardeffect));
		v.push_back(std::to_string(e.bardeffecttype));
		v.push_back(std::to_string(e.bardlevel2));
		v.push_back(std::to_string(e.bardlevel));
		v.push_back(std::to_string(e.bardunk1));
		v.push_back(std::to_string(e.bardunk2));
		v.push_back(std::to_string(e.bardunk3));
		v.push_back(std::to_string(e.bardunk4));
		v.push_back(std::to_string(e.bardunk5));
		v.push_back("'" + Strings::Escape(e.bardname) + "'");
		v.push_back(std::to_string(e.bardunk7));
		v.push_back(std::to_string(e.gmflag));
		v.push_back(std::to_string(e.soulbound));

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
		const std::vector<Items> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.minstatus));
			v.push_back("'" + Strings::Escape(e.Name) + "'");
			v.push_back(std::to_string(e.aagi));
			v.push_back(std::to_string(e.ac));
			v.push_back(std::to_string(e.acha));
			v.push_back(std::to_string(e.adex));
			v.push_back(std::to_string(e.aint));
			v.push_back(std::to_string(e.asta));
			v.push_back(std::to_string(e.astr));
			v.push_back(std::to_string(e.awis));
			v.push_back(std::to_string(e.bagsize));
			v.push_back(std::to_string(e.bagslots));
			v.push_back(std::to_string(e.bagtype));
			v.push_back(std::to_string(e.bagwr));
			v.push_back(std::to_string(e.banedmgamt));
			v.push_back(std::to_string(e.banedmgbody));
			v.push_back(std::to_string(e.banedmgrace));
			v.push_back(std::to_string(e.bardtype));
			v.push_back(std::to_string(e.bardvalue));
			v.push_back(std::to_string(e.book));
			v.push_back(std::to_string(e.casttime));
			v.push_back(std::to_string(e.casttime_));
			v.push_back(std::to_string(e.classes));
			v.push_back(std::to_string(e.color));
			v.push_back(std::to_string(e.price));
			v.push_back(std::to_string(e.cr));
			v.push_back(std::to_string(e.damage));
			v.push_back(std::to_string(e.deity));
			v.push_back(std::to_string(e.delay));
			v.push_back(std::to_string(e.dr));
			v.push_back(std::to_string(e.clicktype));
			v.push_back(std::to_string(e.clicklevel2));
			v.push_back(std::to_string(e.elemdmgtype));
			v.push_back(std::to_string(e.elemdmgamt));
			v.push_back(std::to_string(e.factionamt1));
			v.push_back(std::to_string(e.factionamt2));
			v.push_back(std::to_string(e.factionamt3));
			v.push_back(std::to_string(e.factionamt4));
			v.push_back(std::to_string(e.factionmod1));
			v.push_back(std::to_string(e.factionmod2));
			v.push_back(std::to_string(e.factionmod3));
			v.push_back(std::to_string(e.factionmod4));
			v.push_back("'" + Strings::Escape(e.filename) + "'");
			v.push_back(std::to_string(e.focuseffect));
			v.push_back(std::to_string(e.fr));
			v.push_back(std::to_string(e.fvnodrop));
			v.push_back(std::to_string(e.clicklevel));
			v.push_back(std::to_string(e.hp));
			v.push_back(std::to_string(e.icon));
			v.push_back("'" + Strings::Escape(e.idfile) + "'");
			v.push_back(std::to_string(e.itemclass));
			v.push_back(std::to_string(e.itemtype));
			v.push_back(std::to_string(e.light));
			v.push_back("'" + Strings::Escape(e.lore) + "'");
			v.push_back(std::to_string(e.magic));
			v.push_back(std::to_string(e.mana));
			v.push_back(std::to_string(e.material));
			v.push_back(std::to_string(e.maxcharges));
			v.push_back(std::to_string(e.mr));
			v.push_back(std::to_string(e.nodrop));
			v.push_back(std::to_string(e.norent));
			v.push_back(std::to_string(e.pr));
			v.push_back(std::to_string(e.procrate));
			v.push_back(std::to_string(e.races));
			v.push_back(std::to_string(e.range));
			v.push_back(std::to_string(e.reclevel));
			v.push_back(std::to_string(e.recskill));
			v.push_back(std::to_string(e.reqlevel));
			v.push_back(std::to_string(e.sellrate));
			v.push_back(std::to_string(e.size));
			v.push_back(std::to_string(e.skillmodtype));
			v.push_back(std::to_string(e.skillmodvalue));
			v.push_back(std::to_string(e.slots));
			v.push_back(std::to_string(e.clickeffect));
			v.push_back(std::to_string(e.tradeskills));
			v.push_back(std::to_string(e.weight));
			v.push_back(std::to_string(e.booktype));
			v.push_back(std::to_string(e.recastdelay));
			v.push_back(std::to_string(e.recasttype));
			v.push_back("FROM_UNIXTIME(" + (e.updated > 0 ? std::to_string(e.updated) : "null") + ")");
			v.push_back("'" + Strings::Escape(e.comment) + "'");
			v.push_back(std::to_string(e.stacksize));
			v.push_back(std::to_string(e.stackable));
			v.push_back(std::to_string(e.proceffect));
			v.push_back(std::to_string(e.proctype));
			v.push_back(std::to_string(e.proclevel2));
			v.push_back(std::to_string(e.proclevel));
			v.push_back(std::to_string(e.worneffect));
			v.push_back(std::to_string(e.worntype));
			v.push_back(std::to_string(e.wornlevel2));
			v.push_back(std::to_string(e.wornlevel));
			v.push_back(std::to_string(e.focustype));
			v.push_back(std::to_string(e.focuslevel2));
			v.push_back(std::to_string(e.focuslevel));
			v.push_back(std::to_string(e.scrolleffect));
			v.push_back(std::to_string(e.scrolltype));
			v.push_back(std::to_string(e.scrolllevel2));
			v.push_back(std::to_string(e.scrolllevel));
			v.push_back("FROM_UNIXTIME(" + (e.serialized > 0 ? std::to_string(e.serialized) : "null") + ")");
			v.push_back("FROM_UNIXTIME(" + (e.verified > 0 ? std::to_string(e.verified) : "null") + ")");
			v.push_back("'" + Strings::Escape(e.serialization) + "'");
			v.push_back("'" + Strings::Escape(e.source) + "'");
			v.push_back("'" + Strings::Escape(e.lorefile) + "'");
			v.push_back(std::to_string(e.questitemflag));
			v.push_back(std::to_string(e.clickunk5));
			v.push_back("'" + Strings::Escape(e.clickunk6) + "'");
			v.push_back(std::to_string(e.clickunk7));
			v.push_back(std::to_string(e.procunk1));
			v.push_back(std::to_string(e.procunk2));
			v.push_back(std::to_string(e.procunk3));
			v.push_back(std::to_string(e.procunk4));
			v.push_back("'" + Strings::Escape(e.procunk6) + "'");
			v.push_back(std::to_string(e.procunk7));
			v.push_back(std::to_string(e.wornunk1));
			v.push_back(std::to_string(e.wornunk2));
			v.push_back(std::to_string(e.wornunk3));
			v.push_back(std::to_string(e.wornunk4));
			v.push_back(std::to_string(e.wornunk5));
			v.push_back("'" + Strings::Escape(e.wornunk6) + "'");
			v.push_back(std::to_string(e.wornunk7));
			v.push_back(std::to_string(e.focusunk1));
			v.push_back(std::to_string(e.focusunk2));
			v.push_back(std::to_string(e.focusunk3));
			v.push_back(std::to_string(e.focusunk4));
			v.push_back(std::to_string(e.focusunk5));
			v.push_back("'" + Strings::Escape(e.focusunk6) + "'");
			v.push_back(std::to_string(e.focusunk7));
			v.push_back(std::to_string(e.scrollunk1));
			v.push_back(std::to_string(e.scrollunk2));
			v.push_back(std::to_string(e.scrollunk3));
			v.push_back(std::to_string(e.scrollunk4));
			v.push_back(std::to_string(e.scrollunk5));
			v.push_back("'" + Strings::Escape(e.scrollunk6) + "'");
			v.push_back(std::to_string(e.scrollunk7));
			v.push_back("'" + Strings::Escape(e.clickname) + "'");
			v.push_back("'" + Strings::Escape(e.procname) + "'");
			v.push_back("'" + Strings::Escape(e.wornname) + "'");
			v.push_back("'" + Strings::Escape(e.focusname) + "'");
			v.push_back("'" + Strings::Escape(e.scrollname) + "'");
			v.push_back("'" + Strings::Escape(e.created) + "'");
			v.push_back(std::to_string(e.bardeffect));
			v.push_back(std::to_string(e.bardeffecttype));
			v.push_back(std::to_string(e.bardlevel2));
			v.push_back(std::to_string(e.bardlevel));
			v.push_back(std::to_string(e.bardunk1));
			v.push_back(std::to_string(e.bardunk2));
			v.push_back(std::to_string(e.bardunk3));
			v.push_back(std::to_string(e.bardunk4));
			v.push_back(std::to_string(e.bardunk5));
			v.push_back("'" + Strings::Escape(e.bardname) + "'");
			v.push_back(std::to_string(e.bardunk7));
			v.push_back(std::to_string(e.gmflag));
			v.push_back(std::to_string(e.soulbound));

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

	static std::vector<Items> All(Database& db)
	{
		std::vector<Items> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Items e{};

			e.id             = static_cast<int32_t>(atoi(row[0]));
			e.minstatus      = static_cast<int16_t>(atoi(row[1]));
			e.Name           = row[2] ? row[2] : "";
			e.aagi           = static_cast<int32_t>(atoi(row[3]));
			e.ac             = static_cast<int32_t>(atoi(row[4]));
			e.acha           = static_cast<int32_t>(atoi(row[5]));
			e.adex           = static_cast<int32_t>(atoi(row[6]));
			e.aint           = static_cast<int32_t>(atoi(row[7]));
			e.asta           = static_cast<int32_t>(atoi(row[8]));
			e.astr           = static_cast<int32_t>(atoi(row[9]));
			e.awis           = static_cast<int32_t>(atoi(row[10]));
			e.bagsize        = static_cast<int32_t>(atoi(row[11]));
			e.bagslots       = static_cast<int32_t>(atoi(row[12]));
			e.bagtype        = static_cast<int32_t>(atoi(row[13]));
			e.bagwr          = static_cast<int32_t>(atoi(row[14]));
			e.banedmgamt     = static_cast<int32_t>(atoi(row[15]));
			e.banedmgbody    = static_cast<int32_t>(atoi(row[16]));
			e.banedmgrace    = static_cast<int32_t>(atoi(row[17]));
			e.bardtype       = static_cast<int32_t>(atoi(row[18]));
			e.bardvalue      = static_cast<int32_t>(atoi(row[19]));
			e.book           = static_cast<int32_t>(atoi(row[20]));
			e.casttime       = static_cast<int32_t>(atoi(row[21]));
			e.casttime_      = static_cast<int32_t>(atoi(row[22]));
			e.classes        = static_cast<int32_t>(atoi(row[23]));
			e.color          = static_cast<uint32_t>(strtoul(row[24], nullptr, 10));
			e.price          = static_cast<int32_t>(atoi(row[25]));
			e.cr             = static_cast<int32_t>(atoi(row[26]));
			e.damage         = static_cast<int32_t>(atoi(row[27]));
			e.deity          = static_cast<int32_t>(atoi(row[28]));
			e.delay          = static_cast<int32_t>(atoi(row[29]));
			e.dr             = static_cast<int32_t>(atoi(row[30]));
			e.clicktype      = static_cast<int32_t>(atoi(row[31]));
			e.clicklevel2    = static_cast<int32_t>(atoi(row[32]));
			e.elemdmgtype    = static_cast<int32_t>(atoi(row[33]));
			e.elemdmgamt     = static_cast<int32_t>(atoi(row[34]));
			e.factionamt1    = static_cast<int32_t>(atoi(row[35]));
			e.factionamt2    = static_cast<int32_t>(atoi(row[36]));
			e.factionamt3    = static_cast<int32_t>(atoi(row[37]));
			e.factionamt4    = static_cast<int32_t>(atoi(row[38]));
			e.factionmod1    = static_cast<int32_t>(atoi(row[39]));
			e.factionmod2    = static_cast<int32_t>(atoi(row[40]));
			e.factionmod3    = static_cast<int32_t>(atoi(row[41]));
			e.factionmod4    = static_cast<int32_t>(atoi(row[42]));
			e.filename       = row[43] ? row[43] : "";
			e.focuseffect    = static_cast<int32_t>(atoi(row[44]));
			e.fr             = static_cast<int32_t>(atoi(row[45]));
			e.fvnodrop       = static_cast<int32_t>(atoi(row[46]));
			e.clicklevel     = static_cast<int32_t>(atoi(row[47]));
			e.hp             = static_cast<int32_t>(atoi(row[48]));
			e.icon           = static_cast<int32_t>(atoi(row[49]));
			e.idfile         = row[50] ? row[50] : "";
			e.itemclass      = static_cast<int32_t>(atoi(row[51]));
			e.itemtype       = static_cast<int32_t>(atoi(row[52]));
			e.light          = static_cast<int32_t>(atoi(row[53]));
			e.lore           = row[54] ? row[54] : "";
			e.magic          = static_cast<int32_t>(atoi(row[55]));
			e.mana           = static_cast<int32_t>(atoi(row[56]));
			e.material       = static_cast<int32_t>(atoi(row[57]));
			e.maxcharges     = static_cast<int32_t>(atoi(row[58]));
			e.mr             = static_cast<int32_t>(atoi(row[59]));
			e.nodrop         = static_cast<int32_t>(atoi(row[60]));
			e.norent         = static_cast<int32_t>(atoi(row[61]));
			e.pr             = static_cast<int32_t>(atoi(row[62]));
			e.procrate       = static_cast<int32_t>(atoi(row[63]));
			e.races          = static_cast<int32_t>(atoi(row[64]));
			e.range          = static_cast<int32_t>(atoi(row[65]));
			e.reclevel       = static_cast<int32_t>(atoi(row[66]));
			e.recskill       = static_cast<int32_t>(atoi(row[67]));
			e.reqlevel       = static_cast<int32_t>(atoi(row[68]));
			e.sellrate       = strtof(row[69], nullptr);
			e.size           = static_cast<int32_t>(atoi(row[70]));
			e.skillmodtype   = static_cast<int32_t>(atoi(row[71]));
			e.skillmodvalue  = static_cast<int32_t>(atoi(row[72]));
			e.slots          = static_cast<int32_t>(atoi(row[73]));
			e.clickeffect    = static_cast<int32_t>(atoi(row[74]));
			e.tradeskills    = static_cast<int32_t>(atoi(row[75]));
			e.weight         = static_cast<int32_t>(atoi(row[76]));
			e.booktype       = static_cast<int32_t>(atoi(row[77]));
			e.recastdelay    = static_cast<int32_t>(atoi(row[78]));
			e.recasttype     = static_cast<int32_t>(atoi(row[79]));
			e.updated        = strtoll(row[80] ? row[80] : "-1", nullptr, 10);
			e.comment        = row[81] ? row[81] : "";
			e.stacksize      = static_cast<int32_t>(atoi(row[82]));
			e.stackable      = static_cast<int32_t>(atoi(row[83]));
			e.proceffect     = static_cast<int32_t>(atoi(row[84]));
			e.proctype       = static_cast<int32_t>(atoi(row[85]));
			e.proclevel2     = static_cast<int32_t>(atoi(row[86]));
			e.proclevel      = static_cast<int32_t>(atoi(row[87]));
			e.worneffect     = static_cast<int32_t>(atoi(row[88]));
			e.worntype       = static_cast<int32_t>(atoi(row[89]));
			e.wornlevel2     = static_cast<int32_t>(atoi(row[90]));
			e.wornlevel      = static_cast<int32_t>(atoi(row[91]));
			e.focustype      = static_cast<int32_t>(atoi(row[92]));
			e.focuslevel2    = static_cast<int32_t>(atoi(row[93]));
			e.focuslevel     = static_cast<int32_t>(atoi(row[94]));
			e.scrolleffect   = static_cast<int32_t>(atoi(row[95]));
			e.scrolltype     = static_cast<int32_t>(atoi(row[96]));
			e.scrolllevel2   = static_cast<int32_t>(atoi(row[97]));
			e.scrolllevel    = static_cast<int32_t>(atoi(row[98]));
			e.serialized     = strtoll(row[99] ? row[99] : "-1", nullptr, 10);
			e.verified       = strtoll(row[100] ? row[100] : "-1", nullptr, 10);
			e.serialization  = row[101] ? row[101] : "";
			e.source         = row[102] ? row[102] : "";
			e.lorefile       = row[103] ? row[103] : "";
			e.questitemflag  = static_cast<int32_t>(atoi(row[104]));
			e.clickunk5      = static_cast<int32_t>(atoi(row[105]));
			e.clickunk6      = row[106] ? row[106] : "";
			e.clickunk7      = static_cast<int32_t>(atoi(row[107]));
			e.procunk1       = static_cast<int32_t>(atoi(row[108]));
			e.procunk2       = static_cast<int32_t>(atoi(row[109]));
			e.procunk3       = static_cast<int32_t>(atoi(row[110]));
			e.procunk4       = static_cast<int32_t>(atoi(row[111]));
			e.procunk6       = row[112] ? row[112] : "";
			e.procunk7       = static_cast<int32_t>(atoi(row[113]));
			e.wornunk1       = static_cast<int32_t>(atoi(row[114]));
			e.wornunk2       = static_cast<int32_t>(atoi(row[115]));
			e.wornunk3       = static_cast<int32_t>(atoi(row[116]));
			e.wornunk4       = static_cast<int32_t>(atoi(row[117]));
			e.wornunk5       = static_cast<int32_t>(atoi(row[118]));
			e.wornunk6       = row[119] ? row[119] : "";
			e.wornunk7       = static_cast<int32_t>(atoi(row[120]));
			e.focusunk1      = static_cast<int32_t>(atoi(row[121]));
			e.focusunk2      = static_cast<int32_t>(atoi(row[122]));
			e.focusunk3      = static_cast<int32_t>(atoi(row[123]));
			e.focusunk4      = static_cast<int32_t>(atoi(row[124]));
			e.focusunk5      = static_cast<int32_t>(atoi(row[125]));
			e.focusunk6      = row[126] ? row[126] : "";
			e.focusunk7      = static_cast<int32_t>(atoi(row[127]));
			e.scrollunk1     = static_cast<int32_t>(atoi(row[128]));
			e.scrollunk2     = static_cast<int32_t>(atoi(row[129]));
			e.scrollunk3     = static_cast<int32_t>(atoi(row[130]));
			e.scrollunk4     = static_cast<int32_t>(atoi(row[131]));
			e.scrollunk5     = static_cast<int32_t>(atoi(row[132]));
			e.scrollunk6     = row[133] ? row[133] : "";
			e.scrollunk7     = static_cast<int32_t>(atoi(row[134]));
			e.clickname      = row[135] ? row[135] : "";
			e.procname       = row[136] ? row[136] : "";
			e.wornname       = row[137] ? row[137] : "";
			e.focusname      = row[138] ? row[138] : "";
			e.scrollname     = row[139] ? row[139] : "";
			e.created        = row[140] ? row[140] : "";
			e.bardeffect     = static_cast<int16_t>(atoi(row[141]));
			e.bardeffecttype = static_cast<int16_t>(atoi(row[142]));
			e.bardlevel2     = static_cast<int16_t>(atoi(row[143]));
			e.bardlevel      = static_cast<int16_t>(atoi(row[144]));
			e.bardunk1       = static_cast<int16_t>(atoi(row[145]));
			e.bardunk2       = static_cast<int16_t>(atoi(row[146]));
			e.bardunk3       = static_cast<int16_t>(atoi(row[147]));
			e.bardunk4       = static_cast<int16_t>(atoi(row[148]));
			e.bardunk5       = static_cast<int16_t>(atoi(row[149]));
			e.bardname       = row[150] ? row[150] : "";
			e.bardunk7       = static_cast<int16_t>(atoi(row[151]));
			e.gmflag         = static_cast<int8_t>(atoi(row[152]));
			e.soulbound      = static_cast<int8_t>(atoi(row[153]));

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<Items> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<Items> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			Items e{};

			e.id             = static_cast<int32_t>(atoi(row[0]));
			e.minstatus      = static_cast<int16_t>(atoi(row[1]));
			e.Name           = row[2] ? row[2] : "";
			e.aagi           = static_cast<int32_t>(atoi(row[3]));
			e.ac             = static_cast<int32_t>(atoi(row[4]));
			e.acha           = static_cast<int32_t>(atoi(row[5]));
			e.adex           = static_cast<int32_t>(atoi(row[6]));
			e.aint           = static_cast<int32_t>(atoi(row[7]));
			e.asta           = static_cast<int32_t>(atoi(row[8]));
			e.astr           = static_cast<int32_t>(atoi(row[9]));
			e.awis           = static_cast<int32_t>(atoi(row[10]));
			e.bagsize        = static_cast<int32_t>(atoi(row[11]));
			e.bagslots       = static_cast<int32_t>(atoi(row[12]));
			e.bagtype        = static_cast<int32_t>(atoi(row[13]));
			e.bagwr          = static_cast<int32_t>(atoi(row[14]));
			e.banedmgamt     = static_cast<int32_t>(atoi(row[15]));
			e.banedmgbody    = static_cast<int32_t>(atoi(row[16]));
			e.banedmgrace    = static_cast<int32_t>(atoi(row[17]));
			e.bardtype       = static_cast<int32_t>(atoi(row[18]));
			e.bardvalue      = static_cast<int32_t>(atoi(row[19]));
			e.book           = static_cast<int32_t>(atoi(row[20]));
			e.casttime       = static_cast<int32_t>(atoi(row[21]));
			e.casttime_      = static_cast<int32_t>(atoi(row[22]));
			e.classes        = static_cast<int32_t>(atoi(row[23]));
			e.color          = static_cast<uint32_t>(strtoul(row[24], nullptr, 10));
			e.price          = static_cast<int32_t>(atoi(row[25]));
			e.cr             = static_cast<int32_t>(atoi(row[26]));
			e.damage         = static_cast<int32_t>(atoi(row[27]));
			e.deity          = static_cast<int32_t>(atoi(row[28]));
			e.delay          = static_cast<int32_t>(atoi(row[29]));
			e.dr             = static_cast<int32_t>(atoi(row[30]));
			e.clicktype      = static_cast<int32_t>(atoi(row[31]));
			e.clicklevel2    = static_cast<int32_t>(atoi(row[32]));
			e.elemdmgtype    = static_cast<int32_t>(atoi(row[33]));
			e.elemdmgamt     = static_cast<int32_t>(atoi(row[34]));
			e.factionamt1    = static_cast<int32_t>(atoi(row[35]));
			e.factionamt2    = static_cast<int32_t>(atoi(row[36]));
			e.factionamt3    = static_cast<int32_t>(atoi(row[37]));
			e.factionamt4    = static_cast<int32_t>(atoi(row[38]));
			e.factionmod1    = static_cast<int32_t>(atoi(row[39]));
			e.factionmod2    = static_cast<int32_t>(atoi(row[40]));
			e.factionmod3    = static_cast<int32_t>(atoi(row[41]));
			e.factionmod4    = static_cast<int32_t>(atoi(row[42]));
			e.filename       = row[43] ? row[43] : "";
			e.focuseffect    = static_cast<int32_t>(atoi(row[44]));
			e.fr             = static_cast<int32_t>(atoi(row[45]));
			e.fvnodrop       = static_cast<int32_t>(atoi(row[46]));
			e.clicklevel     = static_cast<int32_t>(atoi(row[47]));
			e.hp             = static_cast<int32_t>(atoi(row[48]));
			e.icon           = static_cast<int32_t>(atoi(row[49]));
			e.idfile         = row[50] ? row[50] : "";
			e.itemclass      = static_cast<int32_t>(atoi(row[51]));
			e.itemtype       = static_cast<int32_t>(atoi(row[52]));
			e.light          = static_cast<int32_t>(atoi(row[53]));
			e.lore           = row[54] ? row[54] : "";
			e.magic          = static_cast<int32_t>(atoi(row[55]));
			e.mana           = static_cast<int32_t>(atoi(row[56]));
			e.material       = static_cast<int32_t>(atoi(row[57]));
			e.maxcharges     = static_cast<int32_t>(atoi(row[58]));
			e.mr             = static_cast<int32_t>(atoi(row[59]));
			e.nodrop         = static_cast<int32_t>(atoi(row[60]));
			e.norent         = static_cast<int32_t>(atoi(row[61]));
			e.pr             = static_cast<int32_t>(atoi(row[62]));
			e.procrate       = static_cast<int32_t>(atoi(row[63]));
			e.races          = static_cast<int32_t>(atoi(row[64]));
			e.range          = static_cast<int32_t>(atoi(row[65]));
			e.reclevel       = static_cast<int32_t>(atoi(row[66]));
			e.recskill       = static_cast<int32_t>(atoi(row[67]));
			e.reqlevel       = static_cast<int32_t>(atoi(row[68]));
			e.sellrate       = strtof(row[69], nullptr);
			e.size           = static_cast<int32_t>(atoi(row[70]));
			e.skillmodtype   = static_cast<int32_t>(atoi(row[71]));
			e.skillmodvalue  = static_cast<int32_t>(atoi(row[72]));
			e.slots          = static_cast<int32_t>(atoi(row[73]));
			e.clickeffect    = static_cast<int32_t>(atoi(row[74]));
			e.tradeskills    = static_cast<int32_t>(atoi(row[75]));
			e.weight         = static_cast<int32_t>(atoi(row[76]));
			e.booktype       = static_cast<int32_t>(atoi(row[77]));
			e.recastdelay    = static_cast<int32_t>(atoi(row[78]));
			e.recasttype     = static_cast<int32_t>(atoi(row[79]));
			e.updated        = strtoll(row[80] ? row[80] : "-1", nullptr, 10);
			e.comment        = row[81] ? row[81] : "";
			e.stacksize      = static_cast<int32_t>(atoi(row[82]));
			e.stackable      = static_cast<int32_t>(atoi(row[83]));
			e.proceffect     = static_cast<int32_t>(atoi(row[84]));
			e.proctype       = static_cast<int32_t>(atoi(row[85]));
			e.proclevel2     = static_cast<int32_t>(atoi(row[86]));
			e.proclevel      = static_cast<int32_t>(atoi(row[87]));
			e.worneffect     = static_cast<int32_t>(atoi(row[88]));
			e.worntype       = static_cast<int32_t>(atoi(row[89]));
			e.wornlevel2     = static_cast<int32_t>(atoi(row[90]));
			e.wornlevel      = static_cast<int32_t>(atoi(row[91]));
			e.focustype      = static_cast<int32_t>(atoi(row[92]));
			e.focuslevel2    = static_cast<int32_t>(atoi(row[93]));
			e.focuslevel     = static_cast<int32_t>(atoi(row[94]));
			e.scrolleffect   = static_cast<int32_t>(atoi(row[95]));
			e.scrolltype     = static_cast<int32_t>(atoi(row[96]));
			e.scrolllevel2   = static_cast<int32_t>(atoi(row[97]));
			e.scrolllevel    = static_cast<int32_t>(atoi(row[98]));
			e.serialized     = strtoll(row[99] ? row[99] : "-1", nullptr, 10);
			e.verified       = strtoll(row[100] ? row[100] : "-1", nullptr, 10);
			e.serialization  = row[101] ? row[101] : "";
			e.source         = row[102] ? row[102] : "";
			e.lorefile       = row[103] ? row[103] : "";
			e.questitemflag  = static_cast<int32_t>(atoi(row[104]));
			e.clickunk5      = static_cast<int32_t>(atoi(row[105]));
			e.clickunk6      = row[106] ? row[106] : "";
			e.clickunk7      = static_cast<int32_t>(atoi(row[107]));
			e.procunk1       = static_cast<int32_t>(atoi(row[108]));
			e.procunk2       = static_cast<int32_t>(atoi(row[109]));
			e.procunk3       = static_cast<int32_t>(atoi(row[110]));
			e.procunk4       = static_cast<int32_t>(atoi(row[111]));
			e.procunk6       = row[112] ? row[112] : "";
			e.procunk7       = static_cast<int32_t>(atoi(row[113]));
			e.wornunk1       = static_cast<int32_t>(atoi(row[114]));
			e.wornunk2       = static_cast<int32_t>(atoi(row[115]));
			e.wornunk3       = static_cast<int32_t>(atoi(row[116]));
			e.wornunk4       = static_cast<int32_t>(atoi(row[117]));
			e.wornunk5       = static_cast<int32_t>(atoi(row[118]));
			e.wornunk6       = row[119] ? row[119] : "";
			e.wornunk7       = static_cast<int32_t>(atoi(row[120]));
			e.focusunk1      = static_cast<int32_t>(atoi(row[121]));
			e.focusunk2      = static_cast<int32_t>(atoi(row[122]));
			e.focusunk3      = static_cast<int32_t>(atoi(row[123]));
			e.focusunk4      = static_cast<int32_t>(atoi(row[124]));
			e.focusunk5      = static_cast<int32_t>(atoi(row[125]));
			e.focusunk6      = row[126] ? row[126] : "";
			e.focusunk7      = static_cast<int32_t>(atoi(row[127]));
			e.scrollunk1     = static_cast<int32_t>(atoi(row[128]));
			e.scrollunk2     = static_cast<int32_t>(atoi(row[129]));
			e.scrollunk3     = static_cast<int32_t>(atoi(row[130]));
			e.scrollunk4     = static_cast<int32_t>(atoi(row[131]));
			e.scrollunk5     = static_cast<int32_t>(atoi(row[132]));
			e.scrollunk6     = row[133] ? row[133] : "";
			e.scrollunk7     = static_cast<int32_t>(atoi(row[134]));
			e.clickname      = row[135] ? row[135] : "";
			e.procname       = row[136] ? row[136] : "";
			e.wornname       = row[137] ? row[137] : "";
			e.focusname      = row[138] ? row[138] : "";
			e.scrollname     = row[139] ? row[139] : "";
			e.created        = row[140] ? row[140] : "";
			e.bardeffect     = static_cast<int16_t>(atoi(row[141]));
			e.bardeffecttype = static_cast<int16_t>(atoi(row[142]));
			e.bardlevel2     = static_cast<int16_t>(atoi(row[143]));
			e.bardlevel      = static_cast<int16_t>(atoi(row[144]));
			e.bardunk1       = static_cast<int16_t>(atoi(row[145]));
			e.bardunk2       = static_cast<int16_t>(atoi(row[146]));
			e.bardunk3       = static_cast<int16_t>(atoi(row[147]));
			e.bardunk4       = static_cast<int16_t>(atoi(row[148]));
			e.bardunk5       = static_cast<int16_t>(atoi(row[149]));
			e.bardname       = row[150] ? row[150] : "";
			e.bardunk7       = static_cast<int16_t>(atoi(row[151]));
			e.gmflag         = static_cast<int8_t>(atoi(row[152]));
			e.soulbound      = static_cast<int8_t>(atoi(row[153]));

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

#endif //EQEMU_BASE_ITEMS_REPOSITORY_H
