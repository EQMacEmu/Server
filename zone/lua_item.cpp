#ifdef LUA_EQEMU

#include "lua.hpp"
#include <luabind/luabind.hpp>

#include "masterentity.h"
#include "lua_item.h"

Lua_Item::Lua_Item(uint32 item_id) {
	const EQ::ItemData *t = database.GetItem(item_id);
	SetLuaPtrData(t);
}

int Lua_Item::GetMinStatus() {
	Lua_Safe_Call_Int();
	return self->MinStatus;
}

int Lua_Item::GetItemClass() {
	Lua_Safe_Call_Int();
	return self->ItemClass;
}

const char *Lua_Item::GetName() {
	Lua_Safe_Call_String();
	return self->Name;
}

const char *Lua_Item::GetLore() {
	Lua_Safe_Call_String();
	return self->Lore;
}

const char *Lua_Item::GetIDFile() {
	Lua_Safe_Call_String();
	return self->IDFile;
}

uint32 Lua_Item::GetID() {
	Lua_Safe_Call_Int();
	return self->ID;
}

int Lua_Item::GetWeight() {
	Lua_Safe_Call_Int();
	return self->Weight;
}

int Lua_Item::GetNoRent() {
	Lua_Safe_Call_Int();
	return self->NoRent;
}

int Lua_Item::GetNoDrop() {
	Lua_Safe_Call_Int();
	return self->NoDrop;
}

int Lua_Item::GetSize() {
	Lua_Safe_Call_Int();
	return self->Size;
}

uint32 Lua_Item::GetSlots() {
	Lua_Safe_Call_Int();
	return self->Slots;
}

uint32 Lua_Item::GetPrice() {
	Lua_Safe_Call_Int();
	return self->Price;
}

uint32 Lua_Item::GetIcon() {
	Lua_Safe_Call_Int();
	return self->Icon;
}

int Lua_Item::GetFVNoDrop() {
	Lua_Safe_Call_Int();
	return self->FVNoDrop;
}

int Lua_Item::GetBagType() {
	Lua_Safe_Call_Int();
	return self->BagType;
}

int Lua_Item::GetBagSlots() {
	Lua_Safe_Call_Int();
	return self->BagSlots;
}

int Lua_Item::GetBagSize() {
	Lua_Safe_Call_Int();
	return self->BagSize;
}

int Lua_Item::GetBagWR() {
	Lua_Safe_Call_Int();
	return self->BagWR;
}

bool Lua_Item::GetTradeskills() {
	Lua_Safe_Call_Bool();
	return self->Tradeskills;
}

int Lua_Item::GetCR() {
	Lua_Safe_Call_Int();
	return self->CR;
}

int Lua_Item::GetDR() {
	Lua_Safe_Call_Int();
	return self->DR;
}

int Lua_Item::GetPR() {
	Lua_Safe_Call_Int();
	return self->PR;
}

int Lua_Item::GetMR() {
	Lua_Safe_Call_Int();
	return self->MR;
}

int Lua_Item::GetFR() {
	Lua_Safe_Call_Int();
	return self->FR;
}

int Lua_Item::GetAStr() {
	Lua_Safe_Call_Int();
	return self->AStr;
}

int Lua_Item::GetASta() {
	Lua_Safe_Call_Int();
	return self->ASta;
}

int Lua_Item::GetAAgi() {
	Lua_Safe_Call_Int();
	return self->AAgi;
}

int Lua_Item::GetADex() {
	Lua_Safe_Call_Int();
	return self->ADex;
}

int Lua_Item::GetACha() {
	Lua_Safe_Call_Int();
	return self->ACha;
}

int Lua_Item::GetAInt() {
	Lua_Safe_Call_Int();
	return self->AInt;
}

int Lua_Item::GetAWis() {
	Lua_Safe_Call_Int();
	return self->AWis;
}

int Lua_Item::GetHP() {
	Lua_Safe_Call_Int();
	return self->HP;
}

int Lua_Item::GetMana() {
	Lua_Safe_Call_Int();
	return self->Mana;
}

int Lua_Item::GetAC() {
	Lua_Safe_Call_Int();
	return self->AC;
}

uint32 Lua_Item::GetDeity() {
	Lua_Safe_Call_Int();
	return self->Deity;
}

int Lua_Item::GetSkillModValue() {
	Lua_Safe_Call_Int();
	return self->SkillModValue;
}

uint32 Lua_Item::GetSkillModType() {
	Lua_Safe_Call_Int();
	return self->SkillModType;
}

uint32 Lua_Item::GetBaneDmgRace() {
	Lua_Safe_Call_Int();
	return self->BaneDmgRace;
}

int Lua_Item::GetBaneDmgAmt() {
	Lua_Safe_Call_Int();
	return self->BaneDmgAmt;
}

uint32 Lua_Item::GetBaneDmgBody() {
	Lua_Safe_Call_Int();
	return self->BaneDmgBody;
}

bool Lua_Item::GetMagic() {
	Lua_Safe_Call_Bool();
	return self->Magic;
}

int Lua_Item::GetCastTime_() {
	Lua_Safe_Call_Int();
	return self->CastTime_;
}

int Lua_Item::GetReqLevel() {
	Lua_Safe_Call_Int();
	return self->ReqLevel;
}

uint32 Lua_Item::GetBardType() {
	Lua_Safe_Call_Int();
	return self->BardType;
}

int Lua_Item::GetBardValue() {
	Lua_Safe_Call_Int();
	return self->BardValue;
}

int Lua_Item::GetLight() {
	Lua_Safe_Call_Int();
	return self->Light;
}

int Lua_Item::GetDelay() {
	Lua_Safe_Call_Int();
	return self->Delay;
}

int Lua_Item::GetRecLevel() {
	Lua_Safe_Call_Int();
	return self->RecLevel;
}

int Lua_Item::GetRecSkill() {
	Lua_Safe_Call_Int();
	return self->RecSkill;
}

int Lua_Item::GetElemDmgType() {
	Lua_Safe_Call_Int();
	return self->ElemDmgType;
}

int Lua_Item::GetElemDmgAmt() {
	Lua_Safe_Call_Int();
	return self->ElemDmgAmt;
}

int Lua_Item::GetRange() {
	Lua_Safe_Call_Int();
	return self->Range;
}

uint32 Lua_Item::GetDamage() {
	Lua_Safe_Call_Int();
	return self->Damage;
}

uint32 Lua_Item::GetColor() {
	Lua_Safe_Call_Int();
	return self->Color;
}

uint32 Lua_Item::GetClasses() {
	Lua_Safe_Call_Int();
	return self->Classes;
}

uint32 Lua_Item::GetRaces() {
	Lua_Safe_Call_Int();
	return self->Races;
}

int Lua_Item::GetMaxCharges() {
	Lua_Safe_Call_Int();
	return self->MaxCharges;
}

int Lua_Item::GetItemType() {
	Lua_Safe_Call_Int();
	return self->ItemType;
}

int Lua_Item::GetMaterial() {
	Lua_Safe_Call_Int();
	return self->Material;
}

double Lua_Item::GetSellRate() {
	Lua_Safe_Call_Real();
	return self->SellRate;
}

int Lua_Item::GetCastTime() {
	Lua_Safe_Call_Int();
	return self->CastTime;
}

int Lua_Item::GetProcRate() {
	Lua_Safe_Call_Int();
	return self->ProcRate;
}

int Lua_Item::GetFactionMod1() {
	Lua_Safe_Call_Int();
	return self->FactionMod1;
}

int Lua_Item::GetFactionMod2() {
	Lua_Safe_Call_Int();
	return self->FactionMod2;
}

int Lua_Item::GetFactionMod3() {
	Lua_Safe_Call_Int();
	return self->FactionMod3;
}

int Lua_Item::GetFactionMod4() {
	Lua_Safe_Call_Int();
	return self->FactionMod4;
}

int Lua_Item::GetFactionAmt1() {
	Lua_Safe_Call_Int();
	return self->FactionAmt1;
}

int Lua_Item::GetFactionAmt2() {
	Lua_Safe_Call_Int();
	return self->FactionAmt2;
}

int Lua_Item::GetFactionAmt3() {
	Lua_Safe_Call_Int();
	return self->FactionAmt3;
}

int Lua_Item::GetFactionAmt4() {
	Lua_Safe_Call_Int();
	return self->FactionAmt4;
}

uint32 Lua_Item::GetRecastDelay() {
	Lua_Safe_Call_Int();
	return self->RecastDelay;
}

uint32 Lua_Item::GetRecastType() {
	Lua_Safe_Call_Int();
	return self->RecastType;
}

bool Lua_Item::GetStackable() {
	Lua_Safe_Call_Bool();
	return self->Stackable;
}

bool Lua_Item::GetQuestItemFlag() {
	Lua_Safe_Call_Bool();
	return self->QuestItemFlag;
}

int Lua_Item::GetStackSize() {
	Lua_Safe_Call_Int();
	return self->StackSize;
}

int Lua_Item::GetClick_Effect() {
	Lua_Safe_Call_Int();
	return self->Click.Effect;
}

int Lua_Item::GetClick_Type() {
	Lua_Safe_Call_Int();
	return self->Click.Type;
}

int Lua_Item::GetClick_Level() {
	Lua_Safe_Call_Int();
	return self->Click.Level;
}

int Lua_Item::GetClick_Level2() {
	Lua_Safe_Call_Int();
	return self->Click.Level2;
}

int Lua_Item::GetProc_Effect() {
	Lua_Safe_Call_Int();
	return self->Proc.Effect;
}

int Lua_Item::GetProc_Type() {
	Lua_Safe_Call_Int();
	return self->Proc.Type;
}

int Lua_Item::GetProc_Level() {
	Lua_Safe_Call_Int();
	return self->Proc.Level;
}

int Lua_Item::GetProc_Level2() {
	Lua_Safe_Call_Int();
	return self->Proc.Level2;
}

int Lua_Item::GetWorn_Effect() {
	Lua_Safe_Call_Int();
	return self->Worn.Effect;
}

int Lua_Item::GetWorn_Type() {
	Lua_Safe_Call_Int();
	return self->Worn.Type;
}

int Lua_Item::GetWorn_Level() {
	Lua_Safe_Call_Int();
	return self->Worn.Level;
}

int Lua_Item::GetWorn_Level2() {
	Lua_Safe_Call_Int();
	return self->Worn.Level2;
}

int Lua_Item::GetFocus_Effect() {
	Lua_Safe_Call_Int();
	return self->Focus.Effect;
}

int Lua_Item::GetFocus_Type() {
	Lua_Safe_Call_Int();
	return self->Focus.Type;
}

int Lua_Item::GetFocus_Level() {
	Lua_Safe_Call_Int();
	return self->Focus.Level;
}

int Lua_Item::GetFocus_Level2() {
	Lua_Safe_Call_Int();
	return self->Focus.Level2;
}

int Lua_Item::GetScroll_Effect() {
	Lua_Safe_Call_Int();
	return self->Scroll.Effect;
}

int Lua_Item::GetScroll_Type() {
	Lua_Safe_Call_Int();
	return self->Scroll.Type;
}

int Lua_Item::GetScroll_Level() {
	Lua_Safe_Call_Int();
	return self->Scroll.Level;
}

int Lua_Item::GetScroll_Level2() {
	Lua_Safe_Call_Int();
	return self->Scroll.Level2;
}

int Lua_Item::GetBard_Effect() {
	Lua_Safe_Call_Int();
	return self->Bard.Effect;
}

int Lua_Item::GetBard_Type() {
	Lua_Safe_Call_Int();
	return self->Bard.Type;
}

int Lua_Item::GetBard_Level() {
	Lua_Safe_Call_Int();
	return self->Bard.Level;
}

int Lua_Item::GetBard_Level2() {
	Lua_Safe_Call_Int();
	return self->Bard.Level2;
}

int Lua_Item::GetBook() {
	Lua_Safe_Call_Int();
	return self->Book;
}

uint32 Lua_Item::GetBookType() {
	Lua_Safe_Call_Int();
	return self->BookType;
}

const char *Lua_Item::GetFilename() {
	Lua_Safe_Call_String();
	return self->Filename;
}

luabind::scope lua_register_item() {
	return luabind::class_<Lua_Item>("Item")
		.def(luabind::constructor<>())
		.def(luabind::constructor<uint32>())
		.def("null", &Lua_Item::Null)
		.def("valid", &Lua_Item::Valid)
		.def("MinStatus", &Lua_Item::GetMinStatus)
		.def("ItemClass", &Lua_Item::GetItemClass)
		.def("Name", &Lua_Item::GetName)
		.def("Lore", &Lua_Item::GetLore)
		.def("IDFile", &Lua_Item::GetIDFile)
		.def("ID", &Lua_Item::GetID)
		.def("Weight", &Lua_Item::GetWeight)
		.def("NoRent", &Lua_Item::GetNoRent)
		.def("NoDrop", &Lua_Item::GetNoDrop)
		.def("Size", &Lua_Item::GetSize)
		.def("Slots", &Lua_Item::GetSlots)
		.def("Price", &Lua_Item::GetPrice)
		.def("Icon", &Lua_Item::GetIcon)
		.def("FVNoDrop", &Lua_Item::GetFVNoDrop)
		.def("BagType", &Lua_Item::GetBagType)
		.def("BagSlots", &Lua_Item::GetBagSlots)
		.def("BagSize", &Lua_Item::GetBagSize)
		.def("BagWR", &Lua_Item::GetBagWR)
		.def("Tradeskills", &Lua_Item::GetTradeskills)
		.def("CR", &Lua_Item::GetCR)
		.def("DR", &Lua_Item::GetDR)
		.def("PR", &Lua_Item::GetPR)
		.def("MR", &Lua_Item::GetMR)
		.def("FR", &Lua_Item::GetFR)
		.def("AStr", &Lua_Item::GetAStr)
		.def("ASta", &Lua_Item::GetASta)
		.def("AAgi", &Lua_Item::GetAAgi)
		.def("ADex", &Lua_Item::GetADex)
.def("ACha", &Lua_Item::GetACha)
.def("AInt", &Lua_Item::GetAInt)
.def("AWis", &Lua_Item::GetAWis)
.def("HP", &Lua_Item::GetHP)
.def("Mana", &Lua_Item::GetMana)
.def("AC", &Lua_Item::GetAC)
.def("Deity", &Lua_Item::GetDeity)
.def("SkillModValue", &Lua_Item::GetSkillModValue)
.def("SkillModType", &Lua_Item::GetSkillModType)
.def("BaneDmgRace", &Lua_Item::GetBaneDmgRace)
.def("BaneDmgAmt", &Lua_Item::GetBaneDmgAmt)
.def("BaneDmgBody", &Lua_Item::GetBaneDmgBody)
.def("Magic", &Lua_Item::GetMagic)
.def("CastTime_", &Lua_Item::GetCastTime_)
.def("ReqLevel", &Lua_Item::GetReqLevel)
.def("BardType", &Lua_Item::GetBardType)
.def("BardValue", &Lua_Item::GetBardValue)
.def("Light", &Lua_Item::GetLight)
.def("Delay", &Lua_Item::GetDelay)
.def("RecLevel", &Lua_Item::GetRecLevel)
.def("RecSkill", &Lua_Item::GetRecSkill)
.def("ElemDmgType", &Lua_Item::GetElemDmgType)
.def("ElemDmgAmt", &Lua_Item::GetElemDmgAmt)
.def("Range", &Lua_Item::GetRange)
.def("Damage", &Lua_Item::GetDamage)
.def("Color", &Lua_Item::GetColor)
.def("Classes", &Lua_Item::GetClasses)
.def("Races", &Lua_Item::GetRaces)
.def("MaxCharges", &Lua_Item::GetMaxCharges)
.def("ItemType", &Lua_Item::GetItemType)
.def("Material", &Lua_Item::GetMaterial)
.def("SellRate", &Lua_Item::GetSellRate)
.def("CastTime", &Lua_Item::GetCastTime)
.def("ProcRate", &Lua_Item::GetProcRate)
.def("FactionMod1", &Lua_Item::GetFactionMod1)
.def("FactionMod2", &Lua_Item::GetFactionMod2)
.def("FactionMod3", &Lua_Item::GetFactionMod3)
.def("FactionMod4", &Lua_Item::GetFactionMod4)
.def("FactionAmt1", &Lua_Item::GetFactionAmt1)
.def("FactionAmt2", &Lua_Item::GetFactionAmt2)
.def("FactionAmt3", &Lua_Item::GetFactionAmt3)
.def("FactionAmt4", &Lua_Item::GetFactionAmt4)
.def("RecastDelay", &Lua_Item::GetRecastDelay)
.def("RecastType", &Lua_Item::GetRecastType)
.def("Stackable", &Lua_Item::GetStackable)
.def("QuestItemFlag", &Lua_Item::GetQuestItemFlag)
.def("StackSize", &Lua_Item::GetStackSize)
.def("Click_Effect", &Lua_Item::GetClick_Effect)
.def("Click_Type", &Lua_Item::GetClick_Type)
.def("Click_Level", &Lua_Item::GetClick_Level)
.def("Click_Level2", &Lua_Item::GetClick_Level2)
.def("Proc_Effect", &Lua_Item::GetProc_Effect)
.def("Proc_Type", &Lua_Item::GetProc_Type)
.def("Proc_Level", &Lua_Item::GetProc_Level)
.def("Proc_Level2", &Lua_Item::GetProc_Level2)
.def("Worn_Effect", &Lua_Item::GetWorn_Effect)
.def("Worn_Type", &Lua_Item::GetWorn_Type)
.def("Worn_Level", &Lua_Item::GetWorn_Level)
.def("Worn_Level2", &Lua_Item::GetWorn_Level2)
.def("Focus_Effect", &Lua_Item::GetFocus_Effect)
.def("Focus_Type", &Lua_Item::GetFocus_Type)
.def("Focus_Level", &Lua_Item::GetFocus_Level)
.def("Focus_Level2", &Lua_Item::GetFocus_Level2)
.def("Scroll_Effect", &Lua_Item::GetScroll_Effect)
.def("Scroll_Type", &Lua_Item::GetScroll_Type)
.def("Scroll_Level", &Lua_Item::GetScroll_Level)
.def("Scroll_Level2", &Lua_Item::GetScroll_Level2)
.def("Bard_Effect", &Lua_Item::GetBard_Effect)
.def("Bard_Type", &Lua_Item::GetBard_Type)
.def("Bard_Level", &Lua_Item::GetBard_Level)
.def("Bard_Level2", &Lua_Item::GetBard_Level2)
.def("Book", &Lua_Item::GetBook)
.def("BookType", &Lua_Item::GetBookType)
.def("Filename", &Lua_Item::GetFilename);
}

#endif
