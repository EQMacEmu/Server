#include "../../client.h"
#include "../../titles.h"

void SetTitleSuffix(Client *c, const Seperator *sep){
	if (sep->arg[2][0] == 0)
		c->Message(Chat::White, "Usage: #titlesuffix [remove|text] [1 = create row in title table] - remove or set title suffix to 'text'");
	else {
		bool Save = (atoi(sep->arg[3]) == 1);

		Mob *target_mob = c->GetTarget();
		if (!target_mob)
			target_mob = c;
		if (!target_mob->IsClient()) {
			c->Message(Chat::Red, "#titlesuffix only works on players.");
			return;
		}
		Client *t = target_mob->CastToClient();

		if (strlen(sep->arg[2]) > 31) {
			c->Message(Chat::Red, "Title suffix must be 31 characters or less.");
			return;
		}

		bool removed = false;
		if (!strcasecmp(sep->arg[2], "remove")) {
			t->SetTitleSuffix("");
			removed = true;
		}
		else {
			for (unsigned int i = 0; i<strlen(sep->arg[2]); i++)
				if (sep->arg[2][i] == '_')
					sep->arg[2][i] = ' ';

			if (!Save)
				t->SetTitleSuffix(sep->arg[2]);
			else
				title_manager.CreateNewPlayerSuffix(t, sep->arg[2]);
		}

		t->Save();

		if (removed) {
			c->Message(Chat::Red, "%s's title suffix has been removed.", t->GetName(), sep->arg[2]);
			if (t != c)
				t->Message(Chat::Red, "Your title suffix has been removed.", sep->arg[2]);
		}
		else {
			c->Message(Chat::Red, "%s's title suffix has been changed to '%s'.", t->GetName(), sep->arg[2]);
			if (t != c)
				t->Message(Chat::Red, "Your title suffix has been changed to '%s'.", sep->arg[2]);
		}
	}
}

