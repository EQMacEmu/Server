#include "platform.h"

EQEmuExePlatform exe_platform = ExePlatformNone;

void RegisterExecutablePlatform(EQEmuExePlatform p) {
	exe_platform = p;
}

const EQEmuExePlatform& GetExecutablePlatform() {
	return exe_platform;
}

int GetExecutablePlatformInt(){
	return exe_platform;
}

/**
 * Returns platform name by string
 *
 * @return
 */
std::string GetPlatformName()
{
	switch (GetExecutablePlatformInt()) {
	case EQEmuExePlatform::ExePlatformWorld:
		return "WorldServer";
	case EQEmuExePlatform::ExePlatformQueryServ:
		return "QueryServer";
	case EQEmuExePlatform::ExePlatformZone:
		return "ZoneServer";
	case EQEmuExePlatform::ExePlatformUCS:
		return "UCS";
	case EQEmuExePlatform::ExePlatformLogin:
		return "LoginServer";
	case EQEmuExePlatform::ExePlatformSharedMemory:
		return "SharedMemory";
	case EQEmuExePlatform::ExePlatformClientImport:
		return "ClientImport";
	case EQEmuExePlatform::ExePlatformClientExport:
		return "ClientExport";
	case EQEmuExePlatform::ExePlatformLaunch:
		return "Launch";
	default:
		return "";
	}
}