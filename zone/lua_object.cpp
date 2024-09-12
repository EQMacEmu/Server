#ifdef LUA_EQEMU

#include "lua.hpp"
#include <luabind/luabind.hpp>

#include "object.h"
#include "lua_object.h"

void Lua_Object::Depop() {
	Lua_Safe_Call_Void();
	self->Depop();
}

void Lua_Object::DepopWithTimer() {
	Lua_Safe_Call_Void();
	self->DepopWithTimer();
}

void Lua_Object::Repop() {
	Lua_Safe_Call_Void();
	self->Repop();
}

void Lua_Object::SetModelName(const char *name) {
	Lua_Safe_Call_Void();
	self->SetModelName(name);
}

const char *Lua_Object::GetModelName() {
	Lua_Safe_Call_String();
	return self->GetModelName();
}

float Lua_Object::GetX() {
	Lua_Safe_Call_Real();
	return self->GetX();
}

float Lua_Object::GetY() {
	Lua_Safe_Call_Real();
	return self->GetY();
}

float Lua_Object::GetZ() {
	Lua_Safe_Call_Real();
	return self->GetZ();
}

float Lua_Object::GetHeading() {
	Lua_Safe_Call_Real();
	float h = 0.0f;
	self->GetHeading(&h);
	return h;
}

void Lua_Object::SetX(float x) {
	Lua_Safe_Call_Void();
	self->SetX(x);
}

void Lua_Object::SetY(float y) {
	Lua_Safe_Call_Void();
	self->SetY(y);
}

void Lua_Object::SetZ(float z) {
	Lua_Safe_Call_Void();
	self->SetZ(z);
}

void Lua_Object::SetHeading(float h) {
	Lua_Safe_Call_Void();
	self->SetHeading(h);
}

void Lua_Object::SetLocation(float x, float y, float z) {
	Lua_Safe_Call_Void();
	self->SetLocation(x, y, z);
}

void Lua_Object::SetItemID(uint32 item_id) {
	Lua_Safe_Call_Void();
	self->SetItemID(item_id);
}

int16 Lua_Object::GetItemID() {
	Lua_Safe_Call_Int();
	return self->GetItemID();
}

void Lua_Object::SetIcon(uint32 icon) {
	Lua_Safe_Call_Void();
	self->SetIcon(icon);
}

uint32 Lua_Object::GetIcon() {
	Lua_Safe_Call_Int();
	return self->GetIcon();
}

void Lua_Object::SetType(uint32 type) {
	Lua_Safe_Call_Void();
	self->SetType(type);
}

uint32 Lua_Object::GetType() {
	Lua_Safe_Call_Int();
	return self->GetType();
}

uint32 Lua_Object::GetDBID() {
	Lua_Safe_Call_Int();
	return self->GetDBID();
}

void Lua_Object::ClearUser() {
	Lua_Safe_Call_Void();
	self->ClearUser();
}

void Lua_Object::SetID(int user) {
	Lua_Safe_Call_Void();
	self->SetID(user);
}

int Lua_Object::GetID() {
	Lua_Safe_Call_Int();
	return self->GetID();
}

bool Lua_Object::Save() {
	Lua_Safe_Call_Int();
	return self->Save();
}

uint32 Lua_Object::VarSave() {
	Lua_Safe_Call_Int();
	return self->VarSave();
}

void Lua_Object::DeleteItem(int index) {
	Lua_Safe_Call_Void();
	self->DeleteItem(index);
}

void Lua_Object::StartDecay() {
	Lua_Safe_Call_Void();
	self->StartDecay();
}

void Lua_Object::Delete() {
	Lua_Safe_Call_Void();
	self->Delete();
}

void Lua_Object::Delete(bool reset_state) {
	Lua_Safe_Call_Void();
	self->Delete(reset_state);
}

bool Lua_Object::IsGroundSpawn() {
	Lua_Safe_Call_Int();
	return self->IsGroundSpawn();
}

void Lua_Object::Close() {
	Lua_Safe_Call_Void();
	self->Close();
}

bool Lua_Object::ClearEntityVariables() {
	Lua_Safe_Call_Bool();
	return self->ClearEntityVariables();
}

bool Lua_Object::DeleteEntityVariable(std::string variable_name) {
	Lua_Safe_Call_Bool();
	return self->DeleteEntityVariable(variable_name);
}

std::string Lua_Object::GetEntityVariable(std::string variable_name) {
	Lua_Safe_Call_String();
	return self->GetEntityVariable(variable_name);
}

luabind::object Lua_Object::GetEntityVariables(lua_State* L) {
	auto t = luabind::newtable(L);
	if (d_) {
		auto self = reinterpret_cast<NativeType*>(d_);
		auto l = self->GetEntityVariables();
		auto i = 0;
		for (const auto& v : l) {
			t[i] = v;
			i++;
		}
	}

	return t;
}

void Lua_Object::SetEntityVariable(std::string variable_name, std::string variable_value) {
	Lua_Safe_Call_Void();
	self->SetEntityVariable(variable_name, variable_value);
}

bool Lua_Object::EntityVariableExists(std::string variable_name) {
	Lua_Safe_Call_Int();
	return self->EntityVariableExists(variable_name);
}

luabind::scope lua_register_object() {
	return luabind::class_<Lua_Object, Lua_Entity>("Object")
		.def(luabind::constructor<>())
		.property("null", &Lua_Object::Null)
		.property("valid", &Lua_Object::Valid)
		.def("Depop", (void(Lua_Object::*)(void)) & Lua_Object::Depop)
		.def("DepopWithTimer", (void(Lua_Object::*)(void)) & Lua_Object::DepopWithTimer)
		.def("Repop", (void(Lua_Object::*)(void)) & Lua_Object::Repop)
		.def("SetModelName", (void(Lua_Object::*)(const char*)) & Lua_Object::SetModelName)
		.def("GetModelName", (const char* (Lua_Object::*)(void)) & Lua_Object::GetModelName)
		.def("GetX", (float(Lua_Object::*)(void)) & Lua_Object::GetX)
		.def("GetY", (float(Lua_Object::*)(void)) & Lua_Object::GetY)
		.def("GetZ", (float(Lua_Object::*)(void)) & Lua_Object::GetZ)
		.def("GetHeading", (float(Lua_Object::*)(void)) & Lua_Object::GetHeading)
		.def("SetX", (void(Lua_Object::*)(float)) & Lua_Object::SetX)
		.def("SetY", (void(Lua_Object::*)(float)) & Lua_Object::SetY)
		.def("SetZ", (void(Lua_Object::*)(float)) & Lua_Object::SetZ)
		.def("SetHeading", (void(Lua_Object::*)(float)) & Lua_Object::SetHeading)
		.def("SetLocation", (void(Lua_Object::*)(float, float, float)) & Lua_Object::SetLocation)
		.def("SetItemID", (void(Lua_Object::*)(uint32)) & Lua_Object::SetItemID)
		.def("GetItemID", (int16(Lua_Object::*)(void)) & Lua_Object::GetItemID)
		.def("SetIcon", (void(Lua_Object::*)(uint32)) & Lua_Object::SetIcon)
		.def("GetIcon", (uint32(Lua_Object::*)(void)) & Lua_Object::GetIcon)
		.def("SetType", (void(Lua_Object::*)(uint32)) & Lua_Object::SetType)
		.def("GetType", (uint32(Lua_Object::*)(void)) & Lua_Object::GetType)
		.def("GetDBID", (uint32(Lua_Object::*)(void)) & Lua_Object::GetDBID)
		.def("ClearUser", (void(Lua_Object::*)(void)) & Lua_Object::ClearUser)
		.def("SetID", (void(Lua_Object::*)(int)) & Lua_Object::SetID)
		.def("GetID", (int(Lua_Object::*)(void)) & Lua_Object::GetID)
		.def("Save", (bool(Lua_Object::*)(void)) & Lua_Object::Save)
		.def("VarSave", (uint32(Lua_Object::*)(void)) & Lua_Object::VarSave)
		.def("DeleteItem", (void(Lua_Object::*)(int)) & Lua_Object::DeleteItem)
		.def("StartDecay", (void(Lua_Object::*)(void)) & Lua_Object::StartDecay)
		.def("Delete", (void(Lua_Object::*)(void)) & Lua_Object::Delete)
		.def("Delete", (void(Lua_Object::*)(bool)) & Lua_Object::Delete)
		.def("IsGroundSpawn", (bool(Lua_Object::*)(void)) & Lua_Object::IsGroundSpawn)
		.def("Close", (void(Lua_Object::*)(void)) & Lua_Object::Close)
		.def("ClearEntityVariables", (bool(Lua_Object::*)(void)) & Lua_Object::ClearEntityVariables)
		.def("DeleteEntityVariable", (bool(Lua_Object::*)(std::string)) & Lua_Object::DeleteEntityVariable)
		.def("GetEntityVariable", (std::string(Lua_Object::*)(std::string)) & Lua_Object::GetEntityVariable)
		.def("GetEntityVariables", (luabind::object(Lua_Object::*)(lua_State*)) & Lua_Object::GetEntityVariables)
		.def("SetEntityVariable", (void(Lua_Object::*)(std::string, std::string)) & Lua_Object::SetEntityVariable)
		.def("EntityVariableExists", (bool(Lua_Object::*)(std::string)) & Lua_Object::EntityVariableExists);
}

#endif
