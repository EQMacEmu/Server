#include "../client.h"

void command_rules(Client *c, const Seperator *sep){
	//super-command for managing rules settings
	if (sep->arg[1][0] == '\0' || !strcasecmp(sep->arg[1], "help")) {
		c->Message(CC_Default, "Syntax: #rules [subcommand].");
		c->Message(CC_Default, "-- Rule Set Manipulation --");
		c->Message(CC_Default, "...listsets - List avaliable rule sets");
		c->Message(CC_Default, "...current - gives the name of the ruleset currently running in this zone");
		c->Message(CC_Default, "...reload - Reload the selected ruleset in this zone");
		c->Message(CC_Default, "...switch (ruleset name) - Change the selected ruleset and load it");
		c->Message(CC_Default, "...load (ruleset name) - Load a ruleset in just this zone without changing the selected set");
		//too lazy to write this right now:
		//		c->Message(CC_Default, "...wload (ruleset name) - Load a ruleset in all zones without changing the selected set");
		c->Message(CC_Default, "...store [ruleset name] - Store the running ruleset as the specified name");
		c->Message(CC_Default, "---------------------");
		c->Message(CC_Default, "-- Running Rule Manipulation --");
		c->Message(CC_Default, "...reset - Reset all rules to their default values");
		c->Message(CC_Default, "...get [rule] - Get the specified rule's local value");
		c->Message(CC_Default, "...set (rule) (value) - Set the specified rule to the specified value locally only");
		c->Message(CC_Default, "...setdb (rule) (value) - Set the specified rule to the specified value locally and in the DB");
		c->Message(CC_Default, "...list [catname] - List all rules in the specified category (or all categiries if omitted)");
		c->Message(CC_Default, "...values [catname] - List the value of all rules in the specified category");
		return;
	}

	if (!strcasecmp(sep->arg[1], "current")) {
		c->Message(CC_Default, "Currently running ruleset '%s' (%d)", RuleManager::Instance()->GetActiveRuleset(),
			RuleManager::Instance()->GetActiveRulesetID());
	}
	else if (!strcasecmp(sep->arg[1], "listsets")) {
		std::map<int, std::string> sets;
		if (!RuleManager::Instance()->ListRulesets(&database, sets)) {
			c->Message(CC_Red, "Failed to list rule sets!");
			return;
		}

		c->Message(CC_Default, "Avaliable rule sets:");
		std::map<int, std::string>::iterator cur, end;
		cur = sets.begin();
		end = sets.end();
		for (; cur != end; ++cur) {
			c->Message(CC_Default, "(%d) %s", cur->first, cur->second.c_str());
		}
	}
	else if (!strcasecmp(sep->arg[1], "reload")) {
		RuleManager::Instance()->LoadRules(&database, RuleManager::Instance()->GetActiveRuleset());
		c->Message(CC_Default, "The active ruleset (%s (%d)) has been reloaded", RuleManager::Instance()->GetActiveRuleset(),
			RuleManager::Instance()->GetActiveRulesetID());
	}
	else if (!strcasecmp(sep->arg[1], "switch")) {
		//make sure this is a valid rule set..
		int rsid = RuleManager::Instance()->GetRulesetID(&database, sep->arg[2]);
		if (rsid < 0) {
			c->Message(CC_Red, "Unknown rule set '%s'", sep->arg[2]);
			return;
		}
		if (!database.SetVariable("RuleSet", sep->arg[2])) {
			c->Message(CC_Red, "Failed to update variables table to change selected rule set");
			return;
		}

		//TODO: we likely want to reload this ruleset everywhere...
		RuleManager::Instance()->LoadRules(&database, sep->arg[2]);

		c->Message(CC_Default, "The selected ruleset has been changed to (%s (%d)) and reloaded locally", sep->arg[2], rsid);
	}
	else if (!strcasecmp(sep->arg[1], "load")) {
		//make sure this is a valid rule set..
		int rsid = RuleManager::Instance()->GetRulesetID(&database, sep->arg[2]);
		if (rsid < 0) {
			c->Message(CC_Red, "Unknown rule set '%s'", sep->arg[2]);
			return;
		}
		RuleManager::Instance()->LoadRules(&database, sep->arg[2]);
		c->Message(CC_Default, "Loaded ruleset '%s' (%d) locally", sep->arg[2], rsid);
	}
	else if (!strcasecmp(sep->arg[1], "store")) {
		if (sep->argnum == 1) {
			//store current rule set.
			RuleManager::Instance()->SaveRules(&database);
			c->Message(CC_Default, "Rules saved");
		}
		else if (sep->argnum == 2) {
			RuleManager::Instance()->SaveRules(&database, sep->arg[2]);
			int prersid = RuleManager::Instance()->GetActiveRulesetID();
			int rsid = RuleManager::Instance()->GetRulesetID(&database, sep->arg[2]);
			if (rsid < 0) {
				c->Message(CC_Red, "Unable to query ruleset ID after store, it most likely failed.");
			}
			else {
				c->Message(CC_Default, "Stored rules as ruleset '%s' (%d)", sep->arg[2], rsid);
				if (prersid != rsid) {
					c->Message(CC_Default, "Rule set %s (%d) is now active in this zone", sep->arg[2], rsid);
				}
			}
		}
		else {
			c->Message(CC_Red, "Invalid argument count, see help.");
			return;
		}
	}
	else if (!strcasecmp(sep->arg[1], "reset")) {
		RuleManager::Instance()->ResetRules();
		c->Message(CC_Default, "The running ruleset has been set to defaults");

	}
	else if (!strcasecmp(sep->arg[1], "get")) {
		if (sep->argnum != 2) {
			c->Message(CC_Red, "Invalid argument count, see help.");
			return;
		}
		std::string value;
		if (!RuleManager::Instance()->GetRule(sep->arg[2], value))
			c->Message(CC_Red, "Unable to find rule %s", sep->arg[2]);
		else
			c->Message(CC_Default, "%s - %s", sep->arg[2], value.c_str());

	}
	else if (!strcasecmp(sep->arg[1], "set")) {
		if (sep->argnum != 3) {
			c->Message(CC_Red, "Invalid argument count, see help.");
			return;
		}
		if (!RuleManager::Instance()->SetRule(sep->arg[2], sep->arg[3])) {
			c->Message(CC_Red, "Failed to modify rule");
		}
		else {
			c->Message(CC_Default, "Rule modified locally.");
		}
	}
	else if (!strcasecmp(sep->arg[1], "setdb")) {
		if (sep->argnum != 3) {
			c->Message(CC_Red, "Invalid argument count, see help.");
			return;
		}
		if (!RuleManager::Instance()->SetRule(sep->arg[2], sep->arg[3], &database, true)) {
			c->Message(CC_Red, "Failed to modify rule");
		}
		else {
			c->Message(CC_Default, "Rule modified locally and in the database.");
		}
	}
	else if (!strcasecmp(sep->arg[1], "list")) {
		if (sep->argnum == 1) {
			std::vector<const char *> rule_list;
			if (!RuleManager::Instance()->ListCategories(rule_list)) {
				c->Message(CC_Red, "Failed to list categories!");
				return;
			}
			c->Message(CC_Default, "Rule Categories:");
			std::vector<const char *>::iterator cur, end;
			cur = rule_list.begin();
			end = rule_list.end();
			for (; cur != end; ++cur) {
				c->Message(CC_Default, " %s", *cur);
			}
		}
		else if (sep->argnum == 2) {
			const char *catfilt = nullptr;
			if (std::string("all") != sep->arg[2])
				catfilt = sep->arg[2];
			std::vector<const char *> rule_list;
			if (!RuleManager::Instance()->ListRules(catfilt, rule_list)) {
				c->Message(CC_Red, "Failed to list rules!");
				return;
			}
			c->Message(CC_Default, "Rules in category %s:", sep->arg[2]);
			std::vector<const char *>::iterator cur, end;
			cur = rule_list.begin();
			end = rule_list.end();
			for (; cur != end; ++cur) {
				c->Message(CC_Default, " %s", *cur);
			}
		}
		else {
			c->Message(CC_Red, "Invalid argument count, see help.");
		}
	}
	else if (!strcasecmp(sep->arg[1], "values")) {
		if (sep->argnum != 2) {
			c->Message(CC_Red, "Invalid argument count, see help.");
			return;
		}
		else {
			const char *catfilt = nullptr;
			if (std::string("all") != sep->arg[2])
				catfilt = sep->arg[2];
			std::vector<const char *> rule_list;
			if (!RuleManager::Instance()->ListRules(catfilt, rule_list)) {
				c->Message(CC_Red, "Failed to list rules!");
				return;
			}
			c->Message(CC_Default, "Rules & values in category %s:", sep->arg[2]);
			std::vector<const char *>::iterator cur, end;
			cur = rule_list.begin();
			end = rule_list.end();
			for (std::string tmp_value; cur != end; ++cur) {
				if (RuleManager::Instance()->GetRule(*cur, tmp_value))
					c->Message(CC_Default, " %s - %s", *cur, tmp_value.c_str());
			}
		}

	}
	else {
		c->Message(CC_Yellow, "Invalid action specified. use '#rules help' for help");
	}
}

