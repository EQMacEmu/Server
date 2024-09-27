#include <vector>
#include "zone.h"
#include "../common/repositories/npc_faction_repository.h"
#include "../common/repositories/npc_faction_entries_repository.h"

void Zone::LoadNPCFactions(const std::vector<uint32> &npc_faction_ids)
{
	LogFaction(
		"Load for Faction IDs [{}]",
		Strings::Join(npc_faction_ids, ", ")
	);

	if (npc_faction_ids.empty()) {
		LogFactionDetail("No NPC factions to load.");
		return;
	}

	// Narrow the list sent in, to new npc_faction_ids that are being loaded
	// as the result of a new spawn.  Ignore those already loaded.

	std::vector<uint32> new_npc_faction_ids = { };

	for (const auto& e : npc_faction_ids) {
		bool found = false;

		for (const auto& nf : m_npc_factions) {
			found = (nf.id == e);
			if (found) {
				LogFaction("Already loaded npc_faction [{}]", nf.id);
				break;
			}
		}

		// This one is new
		if (!found) {
			new_npc_faction_ids.emplace_back(e);
		}
	}

	if (new_npc_faction_ids.empty()) {
		LogFactionDetail("No New NPC factions to load.");
		return;
	}

	auto npc_factions = NpcFactionRepository::GetWhere(
		database,
		fmt::format(
			"`id` IN ({})",
			Strings::Join(new_npc_faction_ids, ", ")
		)
	);

	auto npc_faction_entries = NpcFactionEntriesRepository::GetWhere(
		database,
		fmt::format(
			"`npc_faction_id` IN ({}) Order BY npc_faction_id, sort_order",
			Strings::Join(new_npc_faction_ids, ", ")
		)
	);

	for (const auto& e : npc_factions) {
		m_npc_factions.emplace_back(e);

		for (const auto& f : npc_faction_entries) {
			m_npc_faction_entries.emplace_back(f);
		}
	}

	if (npc_factions.size() > 1) {
		LogFaction("Loaded [{}] Factions", npc_factions.size());
	}
}

void Zone::LoadNPCFaction(const uint32 npc_faction_id)
{
	if (!npc_faction_id) {
		return;
	}

	LogFaction("LoadNPCFaction for [{}]", npc_faction_id);
	LoadNPCFactions({ npc_faction_id });
}

void Zone::ClearNPCFactions()
{
	m_npc_factions.clear();
	m_npc_faction_entries.clear();
}

void Zone::ReloadNPCFactions()
{
	LogFaction("Reloading NPC Factions");

	ClearNPCFactions();

	std::vector<uint32> npc_faction_ids = { };

	for (const auto& n : entity_list.GetNPCList()) {
		if (n.second->GetNPCFactionID() != 0) {
			if (
				std::find(
					npc_faction_ids.begin(),
					npc_faction_ids.end(),
					n.second->GetNPCFactionID()
				) == npc_faction_ids.end()
			) {
				npc_faction_ids.emplace_back(n.second->GetNPCFactionID());
			}
		}
	}

	LoadNPCFactions(npc_faction_ids);
}

NpcFactionRepository::NpcFaction* Zone::GetNPCFaction(const uint32 npc_faction_id)
{
	// Maybe we're being asked to load an npc_faction not yet used in the zone
	LoadNPCFaction(npc_faction_id);

	for (auto& e : m_npc_factions) {
		if (e.id == npc_faction_id) {
			return &e;
		}
	}

	return nullptr;
}

std::vector<NpcFactionEntriesRepository::NpcFactionEntries> Zone::GetNPCFactionEntries(const uint32 npc_faction_id) const
{
	std::vector<NpcFactionEntriesRepository::NpcFactionEntries> npc_faction_entries = {};

	std::vector<uint32> faction_ids;

	for (auto e : m_npc_faction_entries) {
		if (
			e.npc_faction_id == npc_faction_id &&
			std::find(
				faction_ids.begin(),
				faction_ids.end(),
				e.faction_id
			) == faction_ids.end()
		) {
			faction_ids.emplace_back(e.faction_id);
			npc_faction_entries.emplace_back(e);
		}
	}

	return npc_faction_entries;
}
