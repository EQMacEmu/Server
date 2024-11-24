#include "../../client.h"
#include "../../common/serverinfo.h"

void ShowServerInfo(Client *c, const Seperator *sep)
{
	auto os = EQ::GetOS();
	auto cpus = EQ::GetCPUs();
	auto pid = EQ::GetPID();
	auto rss = EQ::GetRSS();
	auto uptime = EQ::GetUptime();

	c->Message(0, "Operating System Information");
	c->Message(0, "==================================================");
	c->Message(0, "System: %s", os.sysname.c_str());
	c->Message(0, "Release: %s", os.release.c_str());
	c->Message(0, "Version: %s", os.version.c_str());
	c->Message(0, "Machine: %s", os.machine.c_str());
	c->Message(0, "Uptime: %.2f seconds", uptime);
	c->Message(0, "==================================================");
	c->Message(0, "CPU Information");
	c->Message(0, "==================================================");
	for (size_t i = 0; i < cpus.size(); ++i) {
		auto &cp = cpus[i];
		c->Message(0, "CPU #%i: %s, Speed: %.2fGhz", i, cp.model.c_str(), cp.speed);
	}
	c->Message(0, "==================================================");
	c->Message(0, "Process Information");
	c->Message(0, "==================================================");
	c->Message(0, "PID: %u", pid);
	c->Message(0, "RSS: %.2f MB", rss / 1048576.0);
	c->Message(0, "==================================================");
}

