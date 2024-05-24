#include "../client.h"

void command_flagedit(Client *c, const Seperator *sep)
{
	//super-command for editing zone flags
	if (sep->arg[1][0] == '\0' || !strcasecmp(sep->arg[1], "help")) {
		c->Message(CC_Default, "Syntax: #flagedit [lockzone|unlockzone|listzones|give|take].");
		c->Message(CC_Default, "...lockzone [zone id/short] [flag name] - Set the specified flag name on the zone, locking the zone");
		c->Message(CC_Default, "...unlockzone [zone id/short] - Removes the flag requirement from the specified zone");
		c->Message(CC_Default, "...listzones - List all zones which require a flag, and their flag's name");
		c->Message(CC_Default, "...give [zone id/short] - Give your target the zone flag for the specified zone.");
		c->Message(CC_Default, "...take [zone id/short] - Take the zone flag for the specified zone away from your target");
		c->Message(CC_Default, "...Note: use #flags to view flags on a person");
		return;
	}

	if (!strcasecmp(sep->arg[1], "lockzone")) {
		uint32 zoneid = 0;
		if (sep->arg[2][0] != '\0') {
			zoneid = atoi(sep->arg[2]);
			if (zoneid < 1) {
				zoneid = database.GetZoneID(sep->arg[2]);
			}
		}
		if (zoneid < 1) {
			c->Message(CC_Red, "zone required. see help.");
			return;
		}

		char flag_name[128];
		if (sep->argplus[3][0] == '\0') {
			c->Message(CC_Red, "flag name required. see help.");
			return;
		}
		database.DoEscapeString(flag_name, sep->argplus[3], 64);
		flag_name[127] = '\0';

		std::string query = StringFormat("UPDATE zone SET flag_needed = '%s' "
			"WHERE zoneidnumber = %d",
			flag_name, zoneid);
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(CC_Red, "Error updating zone: %s", results.ErrorMessage().c_str());
			return;
		}

        c->Message(CC_Yellow, "Success! Zone %s now requires a flag, named %s", database.GetZoneName(zoneid), flag_name);
        return;
	}

	if (!strcasecmp(sep->arg[1], "unlockzone")) {
		uint32 zoneid = 0;
		if (sep->arg[2][0] != '\0') {
			zoneid = atoi(sep->arg[2]);
			if (zoneid < 1) {
				zoneid = database.GetZoneID(sep->arg[2]);
			}
		}

		if (zoneid < 1) {
			c->Message(CC_Red, "zone required. see help.");
			return;
		}

		std::string query = StringFormat("UPDATE zone SET flag_needed = '' "
			"WHERE zoneidnumber = %d",
			zoneid);
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
			c->Message(CC_Yellow, "Error updating zone: %s", results.ErrorMessage().c_str());
			return;
		}

		c->Message(CC_Yellow, "Success! Zone %s no longer requires a flag.", database.GetZoneName(zoneid));
		return;
	}

	if (!strcasecmp(sep->arg[1], "listzones")) {
		std::string query = "SELECT zoneidnumber, short_name, long_name, flag_needed FROM zone WHERE flag_needed != ''";
		auto results = database.QueryDatabase(query);
		if (!results.Success()) {
            return;
        }

		c->Message(CC_Default, "Zones which require flags:");
		for (auto row = results.begin(); row != results.end(); ++row)
			c->Message(CC_Default, "Zone %s (%s,%s) requires key %s", row[1], row[0], row[2], row[3]);

		return;
	}

	if (!strcasecmp(sep->arg[1], "give")) {
		uint32 zoneid = 0;
		if (sep->arg[2][0] != '\0') {
			zoneid = atoi(sep->arg[2]);
			if (zoneid < 1) {
				zoneid = database.GetZoneID(sep->arg[2]);
			}
		}
		if (zoneid < 1) {
			c->Message(CC_Red, "zone required. see help.");
			return;
		}

		Mob *t = c->GetTarget();
		if (t == nullptr || !t->IsClient()) {
			c->Message(CC_Red, "client target required");
			return;
		}

		t->CastToClient()->SetZoneFlag(zoneid);
		return;
	}

	if (!strcasecmp(sep->arg[1], "give")) {
		uint32 zoneid = 0;
		if (sep->arg[2][0] != '\0') {
			zoneid = atoi(sep->arg[2]);
			if (zoneid < 1) {
				zoneid = database.GetZoneID(sep->arg[2]);
			}
		}
		if (zoneid < 1) {
			c->Message(CC_Red, "zone required. see help.");
			return;
		}

		Mob *t = c->GetTarget();
		if (t == nullptr || !t->IsClient()) {
			c->Message(CC_Red, "client target required");
			return;
		}

		t->CastToClient()->ClearZoneFlag(zoneid);
		return;
	}

	c->Message(CC_Yellow, "Invalid action specified. use '#flagedit help' for help");
}

