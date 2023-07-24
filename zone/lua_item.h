#ifndef EQEMU_LUA_ITEM_H
#define EQEMU_LUA_ITEM_H
#ifdef LUA_EQEMU

#include "lua_ptr.h"

struct ItemData;

namespace luabind {
	struct scope;
}

luabind::scope lua_register_item();

class Lua_Item : public Lua_Ptr<const EQ::ItemData>
{
	typedef const EQ::ItemData NativeType;
public:
	Lua_Item(uint32 item_id);
	Lua_Item() : Lua_Ptr(nullptr) { }
	Lua_Item(const EQ::ItemData *d) : Lua_Ptr(d) { }
	virtual ~Lua_Item() { }

	operator const EQ::ItemData*() {
		return reinterpret_cast<const EQ::ItemData*>(GetLuaPtrData());
	}

	int GetMinStatus();
	int GetItemClass();
	const char *GetName();
	const char *GetLore();
	const char *GetIDFile();
	uint32 GetID();
	int GetWeight();
	int GetNoRent();
	int GetNoDrop();
	int GetSize();
	uint32 GetSlots();
	uint32 GetPrice();
	uint32 GetIcon();
	int GetFVNoDrop();
	int GetBagType();
	int GetBagSlots();
	int GetBagSize();
	int GetBagWR();
	bool GetTradeskills();
	int GetCR();
	int GetDR();
	int GetPR();
	int GetMR();
	int GetFR();
	int GetAStr();
	int GetASta();
	int GetAAgi();
	int GetADex();
	int GetACha();
	int GetAInt();
	int GetAWis();
	int GetHP();
	int GetMana();
	int GetAC();
	uint32 GetDeity();
	int GetSkillModValue();
	uint32 GetSkillModType();
	uint32 GetBaneDmgRace();
	int GetBaneDmgAmt();
	uint32 GetBaneDmgBody();
	bool GetMagic();
	int GetCastTime_();
	int GetReqLevel();
	uint32 GetBardType();
	int GetBardValue();
	int GetLight();
	int GetDelay();
	int GetRecLevel();
	int GetRecSkill();
	int GetElemDmgType();
	int GetElemDmgAmt();
	int GetRange();
	uint32 GetDamage();
	uint32 GetColor();
	uint32 GetClasses();
	uint32 GetRaces();
	int GetMaxCharges();
	int GetItemType();
	int GetMaterial();
	double GetSellRate();
	int GetCastTime();
	int GetProcRate();
	int GetFactionMod1();
	int GetFactionMod2();
	int GetFactionMod3();
	int GetFactionMod4();
	int GetFactionAmt1();
	int GetFactionAmt2();
	int GetFactionAmt3();
	int GetFactionAmt4();
	uint32 GetRecastDelay();
	uint32 GetRecastType();
	bool GetStackable();
	bool GetQuestItemFlag();
	int GetStackSize();
	int GetClick_Effect();
	int GetClick_Type();
	int GetClick_Level();
	int GetClick_Level2();
	int GetProc_Effect();
	int GetProc_Type();
	int GetProc_Level();
	int GetProc_Level2();
	int GetWorn_Effect();
	int GetWorn_Type();
	int GetWorn_Level();
	int GetWorn_Level2();
	int GetFocus_Effect();
	int GetFocus_Type();
	int GetFocus_Level();
	int GetFocus_Level2();
	int GetScroll_Effect();
	int GetScroll_Type();
	int GetScroll_Level();
	int GetScroll_Level2();
	int GetBard_Effect();
	int GetBard_Type();
	int GetBard_Level();
	int GetBard_Level2();
	int GetBook();
	uint32 GetBookType();
	const char *GetFilename();
};

#endif
#endif
