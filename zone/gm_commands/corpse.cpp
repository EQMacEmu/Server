#include "../client.h"
#include "../corpse.h"

void command_corpse(Client *c, const Seperator *sep)
{
	std::string help0 = "#Corpse commands usage:";
	std::string help1 = "  #corpse buriedcount - Get the target's total number of buried player corpses.";
	std::string help2 = "  #corpse buriedsummon - Summons the target's oldest buried corpse, if any exist.";
	std::string help3 = "  #corpse charid [charid] - Change player corpse's owner.";
	std::string help4 = "  #corpse delete - Delete targetted corpse.";
	std::string help5 = "  #corpse deletenpccorpses - Delete all NPC corpses.";
	std::string help6 = "  #corpse deleteplayercorpses - Delete all player corpses.";
	std::string help7 = "  #corpse depop - Depops single target corpse. Optional arg [bury].";
	std::string help8 = "  #corpse depopall - Depops all target player's corpses. Optional arg [bury].";
	std::string help9 = "  - Set bury to 0 to skip burying the corpses for the above.";
	std::string help10 = "  #corpse inspect - Inspect contents of target corpse.";
	std::string help11 = "  #corpse list - List corpses for target.";
	std::string help12 = "  #corpse locate - Locates targetted player corpses. zone and loc.";
	std::string help13 = "  #corpse lock - Locks targetted corpse. Only GMs can loot locked corpses.";
	std::string help14 = "  #corpse unlock - Unlocks targetted corpse. Only GMs can loot locked corpses.";
	std::string help15 = "  #corpse removecash - Removes cash from targetted corpse.";
	std::string help16 = "  - To remove items from corpses, lock and loot them.";
	std::string help17 = "  #corpse reset - Resets looter status on targetted corpse for debugging.";
	std::string help18 = "  #corpse backups - List of current target's corpse backups.";
	std::string help19 = "  #corpse restore [corpse_id] - Summons the specified corpse from a player's backups.";

	std::string help[] = { help0, help1, help2, help3, help4, help5, help6, help7, help8, help9, help10, help11, help12, help13, help14, help15, help16, help17, help18, help19 };

	Mob *target = c->GetTarget();

	if (strcasecmp(sep->arg[1], "help") == 0)
	{
		int size = sizeof(help) / sizeof(std::string);
		for (int i = 0; i < size; i++)
		{
			c->Message(Chat::White, help[i].c_str());
		}
	}
	else if (strcasecmp(sep->arg[1], "buriedcount") == 0)
	{
		Client *t = c;

		if (c->GetTarget() && c->GetTarget()->IsClient() && c->GetGM())
			t = c->GetTarget()->CastToClient();
		else
		{
			c->Message(Chat::White, "You must first select a target!");
			return;
		}

		uint32 CorpseCount = database.GetCharacterBuriedCorpseCount(t->CharacterID());

		if (CorpseCount > 0)
			c->Message(Chat::White, "Your target has a total of %u buried corpses.", CorpseCount);
		else
			c->Message(Chat::White, "Your target doesn't have any buried corpses.");

		return;
	}
	else if (strcasecmp(sep->arg[1], "buriedsummon") == 0)
	{
		if (c->Admin() >= commandEditPlayerCorpses)
		{
			Client *t = c;

			if (c->GetTarget() && c->GetTarget()->IsClient() && c->GetGM())
				t = c->GetTarget()->CastToClient();
			else
			{
				c->Message(Chat::White, "You must first turn your GM flag on and select a target!");
				return;
			}

			Corpse* PlayerCorpse = database.SummonBuriedCharacterCorpses(t->CharacterID(), t->GetZoneID(), t->GetPosition());

			if (!PlayerCorpse)
				c->Message(Chat::White, "Your target doesn't have any buried corpses.");

			return;
		}
		else
			c->Message(Chat::White, "Insufficient status to summon buried corpses.");
	}
	else if (strcasecmp(sep->arg[1], "charid") == 0)
	{
		if (c->Admin() >= commandEditPlayerCorpses)
		{
			if (target == 0 || !target->IsPlayerCorpse())
				c->Message(Chat::White, "Error: Target must be a player corpse to set ID.");
			else if (!sep->IsNumber(2))
				c->Message(Chat::White, "Error: charid must be a number.");
			else
				c->Message(Chat::White, "Setting CharID=%u on PlayerCorpse '%s'", target->CastToCorpse()->SetCharID(atoi(sep->arg[2])), target->GetName());
		}
		else
			c->Message(Chat::White, "Insufficient status to change corpse owner.");
	}
	else if (strcasecmp(sep->arg[1], "delete") == 0)
	{
		if (target == 0 || !target->IsCorpse())
			c->Message(Chat::White, "Error: Target the corpse you wish to delete");
		else if (target->IsNPCCorpse())
		{
			c->Message(Chat::White, "Depoping %s.", target->GetName());
			target->CastToCorpse()->DepopNPCCorpse();
		}
		else if (c->Admin() >= commandEditPlayerCorpses)
		{
			c->Message(Chat::White, "Deleting %s.", target->GetName());
			target->CastToCorpse()->Delete();
		}
		else
			c->Message(Chat::White, "Insufficient status to delete player corpse.");
	}
	else if (strcasecmp(sep->arg[1], "deletenpccorpses") == 0)
	{
		int32 tmp = entity_list.DeleteNPCCorpses();
		if (tmp >= 0)
			c->Message(Chat::White, "%d corpses deleted.", tmp);
		else
			c->Message(Chat::White, "DeletePlayerCorpses Error #%d", tmp);
	}
	else if (strcasecmp(sep->arg[1], "deleteplayercorpses") == 0)
	{
		if (c->Admin() >= commandEditPlayerCorpses)
		{
			int32 tmp = entity_list.DeletePlayerCorpses();
			if (tmp >= 0)
				c->Message(Chat::White, "%i corpses deleted.", tmp);
			else
				c->Message(Chat::White, "DeletePlayerCorpses Error #%i", tmp);
		}
		else
			c->Message(Chat::White, "Insufficient status to delete player corpse.");
	}
	else if (strcasecmp(sep->arg[1], "depop") == 0)
	{
		if (target == 0 || !target->IsPlayerCorpse())
			c->Message(Chat::White, "Error: Target must be a player corpse to depop.");
		else if (c->Admin() >= commandEditPlayerCorpses && target->IsPlayerCorpse())
		{
			c->Message(Chat::White, "Depoping %s.", target->GetName());
			target->CastToCorpse()->DepopPlayerCorpse();
			if (!sep->arg[2][0] || atoi(sep->arg[2]) != 0)
				target->CastToCorpse()->Bury();
		}
		else
			c->Message(Chat::White, "Insufficient status to depop player corpse.");
	}
	else if (strcasecmp(sep->arg[1], "depopall") == 0)
	{
		if (target == 0 || !target->IsClient())
			c->Message(Chat::White, "Error: Target must be a player to depop their corpses.");
		else if (c->Admin() >= commandEditPlayerCorpses && target->IsClient())
		{
			c->Message(Chat::White, "Depoping %s\'s corpses.", target->GetName());
			target->CastToClient()->DepopAllCorpses();
			if (!sep->arg[2][0] || atoi(sep->arg[2]) != 0)
				target->CastToClient()->BuryPlayerCorpses();
		}
		else
			c->Message(Chat::White, "Insufficient status to depop player corpses.");
	}
	else if (strcasecmp(sep->arg[1], "inspect") == 0)
	{
		if (target == 0 || !target->IsCorpse())
			c->Message(Chat::White, "Error: Target must be a corpse to inspect.");
		else
			target->CastToCorpse()->QueryLoot(c);
	}
	else if (strcasecmp(sep->arg[1], "list") == 0)
	{
		if(!target || (target && target->IsNPC()))
		{
			entity_list.ListNPCCorpses(c);
		}
		else if(target && target->IsClient())
		{
			entity_list.ListPlayerCorpses(c);
		}
		else
			c->Message(Chat::Yellow, "Please select a NPC or Client to list corpses of that type.");
			
	}
	else if (strcasecmp(sep->arg[1], "locate") == 0)
	{
		if (target == 0 || !target->IsClient())
			c->Message(Chat::White, "Error: Target must be a player to locate their corpses.");
		else
		{
			c->Message(Chat::Red, "CorpseID : Zone , x , y , z , Buried");
			std::string query = StringFormat("SELECT id, zone_id, x, y, z, is_buried FROM character_corpses WHERE charid = %d", target->CastToClient()->CharacterID());
			auto results = database.QueryDatabase(query);

			if (!results.Success() || results.RowCount() == 0)
			{
				c->Message(Chat::Red, "No corpses exist for %s with ID: %i.", target->GetName(), target->CastToClient()->CharacterID());
				return;
			}

			for (auto row = results.begin(); row != results.end(); ++row)
			{

				c->Message(Chat::Yellow, " %s:	%s, %s, %s, %s, (%s)", row[0], ZoneName(atoi(row[1])), row[2], row[3], row[4], row[5]);
			}
		}
	}
	else if (strcasecmp(sep->arg[1], "lock") == 0)
	{
		if (target == 0 || !target->IsCorpse())
			c->Message(Chat::White, "Error: Target must be a corpse in order to lock.");
		else {
			target->CastToCorpse()->Lock();
			c->Message(Chat::White, "Locking %s...", target->GetName());
		}
	}
	else if (strcasecmp(sep->arg[1], "unlock") == 0)
	{
		if (target == 0 || !target->IsCorpse())
			c->Message(Chat::White, "Error: Target must be a corpse in order to unlock.");
		else {
			target->CastToCorpse()->UnLock();
			c->Message(Chat::White, "Unlocking %s...", target->GetName());
		}
	}
	else if (strcasecmp(sep->arg[1], "removecash") == 0)
	{
		if (target == 0 || !target->IsCorpse())
			c->Message(Chat::White, "Error: Target the corpse you wish to remove the cash from");
		else if (!target->IsPlayerCorpse() || c->Admin() >= commandEditPlayerCorpses)
		{
			c->Message(Chat::White, "Removing Cash from %s.", target->GetName());
			target->CastToCorpse()->RemoveCash();
		}
		else
			c->Message(Chat::White, "Insufficient status to modify cash on player corpse.");
	}
	else if (strcasecmp(sep->arg[1], "reset") == 0)
	{
		if (target == 0 || !target->IsCorpse())
			c->Message(Chat::White, "Error: Target the corpse you wish to reset");
		else
			target->CastToCorpse()->ResetLooter();
	}
	else if (strcasecmp(sep->arg[1], "backups") == 0)
	{
		if (target == 0 || !target->IsClient())
			c->Message(Chat::White, "Error: Target must be a player to list their backups.");
		else
		{
			c->Message(Chat::Red, "CorpseID : Zone , x , y , z , Items");
			std::string query = StringFormat("SELECT id, zone_id, x, y, z FROM character_corpses_backup WHERE charid = %d", target->CastToClient()->CharacterID());
			auto results = database.QueryDatabase(query);

			if (!results.Success() || results.RowCount() == 0)
			{
				c->Message(Chat::Red, "No corpse backups exist for %s with ID: %i.", target->GetName(), target->CastToClient()->CharacterID());
				return;
			}

			for (auto row = results.begin(); row != results.end(); ++row)
			{
				std::string ic_query = StringFormat("SELECT COUNT(*) FROM character_corpse_items_backup WHERE corpse_id = %d", atoi(row[0]));
				auto ic_results = database.QueryDatabase(ic_query);
				auto ic_row = ic_results.begin();

				c->Message(Chat::Yellow, " %s:	%s, %s, %s, %s, (%s)", row[0], ZoneName(atoi(row[1])), row[2], row[3], row[4], ic_row[0]);
			}
		}
	}
	else if (strcasecmp(sep->arg[1], "restore") == 0)
	{
		if (c->Admin() >= commandEditPlayerCorpses)
		{
			uint32 corpseid;
			Client *t = c;

			if (c->GetTarget() && c->GetTarget()->IsClient() && c->GetGM())
				t = c->GetTarget()->CastToClient();
			else
			{
				c->Message(Chat::White, "You must first turn your GM flag on and select a target!");
				return;
			}

			if (!sep->IsNumber(2))
			{
				c->Message(Chat::White, "Usage: #corpse restore [corpse_id].");
				return;
			}
			else
				corpseid = atoi(sep->arg[2]);

			if(!database.IsValidCorpseBackup(corpseid))
			{
				c->Message(Chat::Red, "Backup corpse %i not found.", corpseid);
				return;
			}
			else if(database.IsValidCorpse(corpseid))
			{
				c->Message(Chat::Red, "Corpse %i has been found! Please summon or delete it before attempting to restore from a backup.", atoi(sep->arg[2]));
				return;
			}
			else if(!database.IsCorpseBackupOwner(corpseid, t->CharacterID()))
			{
				c->Message(Chat::Red, "Targetted player is not the owner of the specified corpse!");
				return;
			}
			else
			{
				if(database.CopyBackupCorpse(corpseid))
				{
					Corpse* PlayerCorpse = database.SummonCharacterCorpse(corpseid, t->CharacterID(), t->GetZoneID(), t->GetPosition());

					if (!PlayerCorpse)
						c->Message(Chat::White, "Summoning of backup corpse failed. Please escalate this issue.");

					return;
				}
				else
				{
					c->Message(Chat::Red, "There was an error copying corpse %i. Please contact a DB admin.", corpseid);
					return;
				}
			}
		}
		else
		{
			c->Message(Chat::White, "Insufficient status to summon backup corpses.");
		}
	}
	else
	{
		int size = sizeof(help) / sizeof(std::string);
		for (int i = 0; i < size; i++)
		{
			c->Message(Chat::White, help[i].c_str());
		}
	}
}

