/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2002 EQEMu Development Team (http://eqemu.org)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "client.h"
#include "entity.h"
#include "groups.h"
#include "mob.h"
#include "raids.h"

#include "../common/rulesys.h"

#include "hate_list.h"
#include "quest_parser_collection.h"
#include "zone.h"
#include "water_map.h"

#include <stdlib.h>
#include <list>

extern Zone *zone;

HateList::HateList()
{
	owner = nullptr;
	combatRangeBonus = 0;
	sitInsideBonus = 0;
	sitOutsideBonus = 0;
	rememberDistantMobs = false;
	nobodyInMeleeRange = false;
	allHatersIgnored = false;
	aggroDeaggroTime = 0xFFFFFFFF;
	ignoreStuckCount = 0;
}

HateList::~HateList()
{
}

void HateList::SetOwner(Mob *newOwner)
{
	owner = newOwner;

	// see http://www.eqemulator.org/forums/showthread.php?t=39819
	// for the data this came from.  This is accurate to EQ Live as of 2015

	int32 hitpoints = owner->GetMaxHP();

	// melee range hate bonus
	combatRangeBonus = 100;
	if (hitpoints > 60000)
	{
		combatRangeBonus = 250 + hitpoints / 100;
		if (combatRangeBonus > 2250)
		{
			combatRangeBonus = 2250;
		}
	}
	else if (hitpoints > 4000)
	{
		combatRangeBonus = (hitpoints - 4000) / 75 + 100;
	}

	// inside melee range sitting hate bonus
	sitInsideBonus = 15;
	if (hitpoints > 13500)
	{
		sitInsideBonus = -hitpoints / 100 + 1000;
		if (sitInsideBonus < 0)
		{
			sitInsideBonus = 0;
		}
	}
	else if (hitpoints > 225)
	{
		sitInsideBonus = hitpoints / 15;
	}

	// ouside melee range sitting hate bonus
	sitOutsideBonus = 15;
	if (hitpoints > 225)
	{
		sitOutsideBonus = hitpoints / 15;
		if (sitOutsideBonus > 1000)
		{
			sitOutsideBonus = 1000;
		}
	}
}

// added for frenzy support
// checks if target still is in frenzy mode
void HateList::CheckFrenzyHate()
{
	auto iterator = list.begin();
	while(iterator != list.end())
	{
		if ((*iterator)->ent->GetHPRatio() >= 20.0f)
			(*iterator)->bFrenzy = false;
		++iterator;
	}
}

void HateList::Wipe()
{
	if (list.size() > 0)
		aggroDeaggroTime = Timer::GetCurrentTime();

	auto iterator = list.begin();

	while(iterator != list.end())
	{
		Mob* m = (*iterator)->ent;
		if(m)
		{
			parse->EventNPC(EVENT_HATE_LIST, owner->CastToNPC(), m, "0", 0);
		}
		delete (*iterator);
		iterator = list.erase(iterator);

	}
	ignoreStuckCount = 0;
}

bool HateList::IsOnHateList(Mob *mob)
{
	if(Find(mob))
		return true;
	return false;
}

tHateEntry *HateList::Find(Mob *ent)
{
	auto iterator = list.begin();
	while(iterator != list.end())
	{
		if((*iterator)->ent == ent)
			return (*iterator);
		++iterator;
	}
	return nullptr;
}

void HateList::Set(Mob* other, int32 in_hate, int32 in_dam)
{
	tHateEntry *p = Find(other);
	if(p)
	{
		if(in_dam > -1)
			p->damage = in_dam;

		if(in_hate > -1)
			p->hate = in_hate;
	}
	else
	{
		Add(other, in_hate, in_dam);
	}
}

// adds up all the damage from the pets of ent
int32 HateList::GetEntPetDamage(Mob* ent)
{
	int32 dmg = 0;
	Mob* m = nullptr;
	Mob* pet_owner = nullptr;
	SwarmPet* swarm_info = nullptr;

	auto iterator = list.begin();
	while (iterator != list.end())
	{
		m = (*iterator)->ent;

		if (m->IsNPC() && (m->IsPet() || m->CastToNPC()->GetSwarmInfo()))
		{
			pet_owner = m->GetOwner();
			swarm_info = m->CastToNPC()->GetSwarmInfo();
			
			if ((pet_owner && pet_owner == ent) || (swarm_info && swarm_info->GetOwner() && swarm_info->GetOwner() == ent))
			{
				dmg += (*iterator)->damage;
			}
		}
		++iterator;
	}
	return dmg;
}

Mob* HateList::GetDamageTop(int32& return_dmg, bool combine_pet_dmg, bool clients_only)
{
	Mob* top_mob = nullptr;
	Mob* m = nullptr;
	Group* grp = nullptr;
	Raid* r = nullptr;
	int32 dmg;
	int32 top_dmg = 0;
	int32 npc_dmg = 0;
	int32 top_npc_dmg = 0;
	Mob* top_npc = nullptr;

	auto iterator = list.begin();
	while (iterator != list.end())
	{
		grp = nullptr;
		r = nullptr;
		m = (*iterator)->ent;
		dmg = (*iterator)->damage;
		
		if (!m)
		{
			++iterator;
			continue;
		}

		if (m->IsClient())
		{
			r = entity_list.GetRaidByClient(m->CastToClient());
			grp = entity_list.GetGroupByMob(m);
			
			if (m->CastToClient()->IsFeigned() && (!owner->IsFleeing() || owner->IsRooted() || (Distance(owner->GetPosition(), m->GetPosition()) > 100.0f)))
				dmg = 0;
		}

		if (r)
		{
			if (r->GetTotalRaidDamage(owner) >= top_dmg)
			{
				top_mob = m;
				top_dmg = r->GetTotalRaidDamage(owner);
			}
		}
		else if (grp)
		{
			if (grp->GetTotalGroupDamage(owner) >= top_dmg)
			{
				top_mob = m;
				top_dmg = grp->GetTotalGroupDamage(owner);
			}
		}
		else if (m->IsClient())
		{
			// solo clients

			if (combine_pet_dmg)
				dmg += GetEntPetDamage(m);

			if (dmg > top_dmg)
			{
				top_mob = m;
				top_dmg = dmg;
			}

		}
		else if (m->IsNPC())
		{
			if (m->GetOwner() && m->GetOwner()->IsClient())
			{
				// player pets

				if (!combine_pet_dmg)
				{
					if (dmg > top_dmg)
					{
						top_mob = m;
						top_dmg = dmg;
					}
				}
			}
			else if (!clients_only)
			{
				// non client pet NPCs
				npc_dmg += dmg;				// npc damage added together as if they were grouped
				if (top_npc_dmg < dmg)
				{
					top_npc_dmg = dmg;
					top_npc = m;
				}
			}
		}
		++iterator;
	}

	if (top_npc && npc_dmg >= top_dmg)
	{
		top_mob = top_npc;
		top_dmg = npc_dmg;
	}

	return_dmg = top_dmg;
	return top_mob;
}

bool HateList::IsClientOnHateList()
{
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		if ((*iterator)->ent != nullptr && ((*iterator)->ent->IsClient() || (*iterator)->ent->IsPlayerOwned()))
		{
			return true;
		}
		++iterator;
	}
	return false;
}

Mob* HateList::GetClosest(Mob *hater)
{
	Mob* close_entity = nullptr;
	float close_distance = 99999.9f;
	float this_distance;

	if (!hater && owner)
		hater = owner;

	if (!hater)
		return nullptr;

	auto iterator = list.begin();
	while(iterator != list.end())
	{
		if ((*iterator)->ent != nullptr) {
			this_distance = DistanceSquaredNoZ((*iterator)->ent->GetPosition(), hater->GetPosition());

			if(this_distance <= close_distance && !(*iterator)->ent->DivineAura()
				&& (!(*iterator)->ent->IsClient() || !(*iterator)->ent->CastToClient()->IsFeigned() || owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity))
			)
			{
				close_distance = this_distance;
				close_entity = (*iterator)->ent;
			}
		}
		++iterator;
	}

	if (!close_entity && hater->IsNPC())
		close_entity = hater->CastToNPC()->GetHateTop();

	return close_entity;
}

Mob* HateList::GetClosestClient(Mob *hater)
{
	Mob* close_entity = nullptr;
	float close_distance = 99999.9f;
	float this_distance;

	if (!hater && owner)
		hater = owner;

	if (!hater)
		return nullptr;

	auto iterator = list.begin();
	while(iterator != list.end())
	{
		if ((*iterator)->ent != nullptr && (*iterator)->ent->IsClient()) {
			this_distance = DistanceSquaredNoZ((*iterator)->ent->GetPosition(), hater->GetPosition());

			float ignoreDistance = 200.0f;
			if (owner->IsNPC())
				ignoreDistance = owner->CastToNPC()->GetIgnoreDistance();

			if (owner->GetOwner() && owner->GetOwner()->IsClient() && owner->GetPetType() != petHatelist)
				ignoreDistance = RuleR(Pets, AttackCommandRange);

			if ((*iterator)->ent->CastToClient()->IsFeigned() && !owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity))
			{
				++iterator;
				continue;
			}

			// ignore players farther away than distance specified in the database.
			if (!rememberDistantMobs && ignoreDistance > 0.0f && this_distance > (ignoreDistance*ignoreDistance)
				// exception for damaged summoning NPCs
				&& (!owner->GetSpecialAbility(SpecialAbility::Summon) || owner->GetHPRatio() > 90.0f))
			{
				++iterator;
				continue;
			}

			if(this_distance <= close_distance && !(*iterator)->ent->DivineAura())
			{
				close_distance = this_distance;
				close_entity = (*iterator)->ent;
			}
		}
		++iterator;
	}

	return close_entity;
}

Mob* HateList::GetClosestNPC(Mob *hater)
{
	Mob* close_entity = nullptr;
	float close_distance = 99999.9f;
	float this_distance;

	if (!hater && owner)
		hater = owner;

	if (!hater)
		return nullptr;

	auto iterator = list.begin();
	while(iterator != list.end())
	{
		if ((*iterator)->ent != nullptr && (*iterator)->ent->IsNPC()) {
			this_distance = DistanceSquaredNoZ((*iterator)->ent->GetPosition(), hater->GetPosition());

			float ignoreDistance = 200.0f;
			if (owner->IsNPC())
				ignoreDistance = owner->CastToNPC()->GetIgnoreDistance();

			if (owner->GetOwner() && owner->GetOwner()->IsClient() && owner->GetPetType() != petHatelist)
				ignoreDistance = RuleR(Pets, AttackCommandRange);

			// ignore players farther away than distance specified in the database.
			if (!rememberDistantMobs && ignoreDistance > 0.0f && this_distance > (ignoreDistance*ignoreDistance)
				// exception for damaged summoning NPCs
				&& (!owner->GetSpecialAbility(SpecialAbility::Summon) || owner->GetHPRatio() > 90.0f))
			{
				++iterator;
				continue;
			}

			if(this_distance <= close_distance && !(*iterator)->ent->DivineAura())
			{
				close_distance = this_distance;
				close_entity = (*iterator)->ent;
			}
		}
		++iterator;
	}

	return close_entity;
}

// this will process negative hate values fine (e.g. jolt)
void HateList::Add(Mob *ent, int32 in_hate, int32 in_dam, bool bFrenzy, bool iAddIfNotExist)
{
	if(!ent || ent == owner)
		return;

	if(ent->IsCorpse())
		return;

	if(ent->IsClient() && ent->CastToClient()->IsDead())
		return;

	tHateEntry *p = Find(ent);
	if (p)
	{
		if (in_dam > 0) {
			p->damage += in_dam;
			p->last_damage = Timer::GetCurrentTime();
		}
		p->hate += in_hate;
		p->bFrenzy = bFrenzy;
		Log(Logs::Detail, Logs::Aggro, "%s is adding %d damage and %d hate to %s hatelist.", ent->GetName(), in_dam, in_hate, owner->GetName());
	}
	else if (iAddIfNotExist)
	{
		if (owner->IsNPC())
		{
			// first to aggro this?
			if (GetNumHaters() == 0)
			{
				aggroDeaggroTime = Timer::GetCurrentTime();
				allHatersIgnored = false;
			}

			// the first-to-aggro hate on animals does nearly no hate, regardless of what it would normally be
			if (owner->CastToNPC()->IsAnimal())
				in_hate = 1;

			bool no_fte = owner->CastToNPC()->fte_charid == 0 && owner->CastToNPC()->raid_fte == 0 && owner->CastToNPC()->group_fte == 0;
			if (no_fte && (ent->IsClient() || ent->IsPlayerOwned()))
			{
				Mob* oos = ent->GetOwnerOrSelf();
				if (oos && oos->IsClient())
				{
					Client* c = oos->CastToClient();
					if (c)
					{
						owner->CastToNPC()->fte_charid = c->CharacterID();

						Raid *kr = entity_list.GetRaidByClient(c);
						Group *kg = entity_list.GetGroupByClient(c);
						if (kr)
						{
							owner->CastToNPC()->raid_fte = kr->GetID();
						}
						else if (kg)
						{
							owner->CastToNPC()->group_fte = kg->GetID();
						}
					}
				}
			}
		}

		p = new tHateEntry;
		p->ent = ent;
		if (in_dam > 0) {
			p->damage = in_dam;
			p->last_damage = Timer::GetCurrentTime();
		}
		else {
			p->damage = 0;
			p->last_damage = 0;
		}
		p->hate = in_hate;
		p->bFrenzy = bFrenzy;
		list.push_back(p);
		parse->EventNPC(EVENT_HATE_LIST, owner->CastToNPC(), ent, "1", 0);
		Log(Logs::Detail, Logs::Aggro, "%s is creating %d damage and %d hate on %s hatelist.", ent->GetName(), in_dam, in_hate, owner->GetName());
	}

	if (p)
	{
		// this prevents jolt spells from reducing hate below 1
		if (p->hate < 1)
			p->hate = 1;

		p->last_hate = Timer::GetCurrentTime();
	}
}

bool HateList::RemoveEnt(Mob *ent)
{
	if (!ent)
		return false;

	bool found = false;
	auto iterator = list.begin();

	while(iterator != list.end())
	{
		if((*iterator)->ent == ent)
		{
			if(ent)
			parse->EventNPC(EVENT_HATE_LIST, owner->CastToNPC(), ent, "0", 0);
			found = true;

			delete (*iterator);
			iterator = list.erase(iterator);

		}
		else
			++iterator;
	}
	if (GetNumHaters() == 0)
	{
		aggroDeaggroTime = Timer::GetCurrentTime();
	}
	return found;
}

void HateList::DoFactionHits(int32 nfl_id, bool &success) {
	if (nfl_id <= 0)
		return;
	auto iterator = list.begin();
	while(iterator != list.end())
	{
		Client *p;

		if ((*iterator)->ent && (*iterator)->ent->IsClient())
			p = (*iterator)->ent->CastToClient();
		else
			p = nullptr;

		if (p)
		{
			if (!p->IsFeigned() || (owner->IsFleeing() && !owner->IsRooted() && (Distance(owner->GetPosition(), p->GetPosition()) < 100.0f)))
			p->SetFactionLevel(p->CharacterID(), nfl_id);
			success = true;
		}
		++iterator;
	}
}

int HateList::SummonedPetCount(Mob *hater) {

	//Function to get number of 'Summoned' pets on a targets hate list to allow calculations for certian spell effects.
	//Unclear from description that pets are required to be 'summoned body type'. Will not require at this time.
	int petcount = 0;
	auto iterator = list.begin();
	while(iterator != list.end()) {

		if((*iterator)->ent != nullptr && (*iterator)->ent->IsNPC() && 	((*iterator)->ent->CastToNPC()->IsPet() || ((*iterator)->ent->CastToNPC()->GetSwarmOwner() > 0)))
		{
			++petcount;
		}

		++iterator;
	}

	return petcount;
}

int32 HateList::GetHateBonus(tHateEntry *entry, bool combatRange, bool firstInRange, float distSquared)
{
	int32 bonus = 0;
	int32 lowHealthBonus = std::max(std::min(owner->GetHP(), 10000), 500);

	if (combatRange)
	{
		bonus += combatRangeBonus;
		if (firstInRange)
		{
			bonus += 35;
		}
	}

	if (entry->ent->IsClient() && entry->ent->CastToClient()->IsSitting() && entry->ent->CastToClient()->GetHorseId() == 0)
	{
		if (combatRange)
		{
			bonus += sitInsideBonus;
		}
		else
		{
			bonus += sitOutsideBonus;
		}
	}

	if (entry->bFrenzy || (entry->ent->GetMaxHP() > 0 && ((entry->ent->GetHP() * 100 / entry->ent->GetMaxHP()) < 20)))
	{
		bonus += lowHealthBonus;
	}

	// if nobody in melee range but entry is nearby, apply a bonus that scales with distance to target
	// this has the effect of the melee range bonus tapering off gradually over 100 distance
	if (!combatRange && nobodyInMeleeRange && list.size() > 1)
	{
		if (distSquared == -1.0f)
		{
			float distX = entry->ent->GetX() - owner->GetX();
			float distY = entry->ent->GetY() - owner->GetY();
			distSquared = distX * distX + distY * distY;
		}

		if (distSquared < 10000.0f)
		{
			float dist = sqrtf(distSquared);
			if (dist < 100 && dist > 0)
			{
				bonus += combatRangeBonus * static_cast<int32>(100.0f - dist) / 100;
			}
		}
	}
	return bonus;
}

Mob *HateList::GetTop()
{
	Mob* topMob = nullptr;
	int32 topHate = -1;
	bool mobInMeleeRange = false;
	bool clientInMeleeRange = false;
	bool wasIgnoringHaters = allHatersIgnored;
	allHatersIgnored = false;
	hasFeignedHaters = false;
	hasLandHaters = false;

	if (owner == nullptr || (list.size() == 0))
		return nullptr;

	Mob* topClient = nullptr;
	Mob* topMeleeClient = nullptr;
	Mob* closestMob = nullptr;
	float closestMobDist = 9999999.0f;
	int32 topClientHate = -1;
	int32 topMeleeClientHate = -1;
	bool firstInRangeBonusApplied = false;
	uint32 current_time = Timer::GetCurrentTime();
	bool ownerHasProxAggro = static_cast<bool>(owner->GetSpecialAbility(SpecialAbility::ProximityAggro));

	float ignoreDistance = 200.0f;
	if (owner->IsNPC())
		ignoreDistance = owner->CastToNPC()->GetIgnoreDistance();

	if (owner->GetOwner() && owner->GetOwner()->IsClient() && owner->GetPetType() != petHatelist)
		ignoreDistance = RuleR(Pets, AttackCommandRange);

	ignoreDistance *= ignoreDistance;

	tHateEntry *cur;
	auto iterator = list.begin();
	while (iterator != list.end()) {
		cur = (*iterator);
		if (!cur || !cur->ent)
		{
			safe_delete(*iterator);
			iterator = list.erase(iterator);
			continue;
		}
		// remove mobs that have not added hate in 10 minutes
		if (((current_time - cur->last_hate) > 600000) || cur->ent->HasDied())
		{
			if (cur && cur->ent) {
				parse->EventNPC(EVENT_HATE_LIST, owner->CastToNPC(), cur->ent, "0", 0);
				if (owner && !owner->HasDied())
					owner->RemoveFromRampageList(cur->ent, true);
			}
			safe_delete(*iterator);
			iterator = list.erase(iterator);
			continue;
		}
		cur->dist_squared = DistanceSquaredNoZ(cur->ent->GetPosition(), owner->GetPosition());
		int z_diff = std::abs(cur->ent->GetZ() - owner->GetZ());
		z_diff *= z_diff;
		// If NPC has ignore distance < 1000 (generally indoor zones) then add a 1000 unit Z check to ignore range.
		// I would use the real ignore range instead of 1000 but it was wonky in BoT towers due to the way our pathing
		// works so setting it to 1000 and relying on scripts to prevent exploits on certain NPCs
		if (ignoreDistance < 1000*1000 && z_diff > 1000*1000 && cur->dist_squared < z_diff)
			cur->dist_squared = z_diff;

		if (cur->dist_squared < closestMobDist)
		{
			closestMob = cur->ent;
			closestMobDist = cur->dist_squared;
		}
		++iterator;
	}
	if (list.size() == 0)
		return nullptr;

	iterator = list.begin();
	while (iterator != list.end())
	{
		cur = (*iterator);

		auto hateEntryPosition = glm::vec3(cur->ent->GetX(), cur->ent->GetY(), cur->ent->GetZ());

		// ignore players farther away than distance specified in the database.
		if (!rememberDistantMobs && cur->dist_squared > ignoreDistance)
		{
			++iterator;
			continue;
		}
		if (cur->ent->IsClient() && cur->ent->CastToClient()->IsFeigned() && !owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity))
		{
			hasFeignedHaters = true;
			++iterator;
			continue;
		}

		if (zone->HasWaterMap())
		{
			if (owner && owner->IsNPC() && owner->CastToNPC()->IsUnderwaterOnly())
			{
				bool in_liquid = zone->HasWaterMap() && zone->watermap->InLiquid(hateEntryPosition) || zone->IsWaterZone(hateEntryPosition.z);
				if (!in_liquid)
				{
					hasLandHaters = true;
					++iterator;
					continue;
				}
			}
		}

		if (cur->ent->DivineAura() || cur->ent->IsMezzed() || cur->ent->IsFearedNoFlee())
		{
			if (topHate == -1)
			{
				topMob = cur->ent;
				topHate = 0;
			}
			++iterator;
			continue;
		}

		bool isInMeleeRange = owner->IsInCombatRange(cur->ent, cur->dist_squared);
		if (isInMeleeRange)
		{
			mobInMeleeRange = true;
			if (cur->ent->IsClient())
				clientInMeleeRange = true;
		}

		// pets get no closest mob bonus - so set nobody in melee range, to skip that part of bonus
		if (owner->GetOwner()) {
			nobodyInMeleeRange = false;
			firstInRangeBonusApplied = true;
		}

		if (ownerHasProxAggro)		// prox aggro mobs give this small bonus to nearst target.  non-prox aggro give it to first on hate list in melee range
		{
			if (closestMob == cur->ent)
				firstInRangeBonusApplied = false;
			else
				firstInRangeBonusApplied = true;
		}

		int32 currentHate = cur->hate + GetHateBonus(cur, isInMeleeRange, !firstInRangeBonusApplied, cur->dist_squared);

		if (!firstInRangeBonusApplied && isInMeleeRange)
		{
			firstInRangeBonusApplied = true;
		}

		// favor targets inside 600 distance
		if (cur->dist_squared > 360000.0f)
		{
			currentHate = 0;
		}

		if (cur->ent->IsClient())
		{
			if (currentHate > topClientHate)
			{
				topClientHate = currentHate;
				topClient = cur->ent;
			}
			if (isInMeleeRange && currentHate > topMeleeClientHate)
			{
				topMeleeClientHate = currentHate;
				topMeleeClient = cur->ent;
			}
		}
		else if (cur->ent->IsCharmedPet() && !RuleB(AlKabor, AllowCharmPetRaidTanks))
		{
			// this makes NPCs ignore charmed pets if more than X players+pets get on the hate list
			if (list.size() > RuleI(AlKabor, MaxEntitiesCharmTanks))
				currentHate = 0;
		}

		if (currentHate > topHate)
		{
			topHate = currentHate;
			topMob = cur->ent;
		}
		if (wasIgnoringHaters)
			cur->last_hate = Timer::GetCurrentTime();

		++iterator;
	}
	nobodyInMeleeRange = !mobInMeleeRange;
	if (!topMob && list.size() > 0)
	{
		allHatersIgnored = true;
		if (!wasIgnoringHaters)
			ignoreStuckCount = ignoreStuckCount + 1;	// if NPC gets stuck due to ignore radius and geometry making it run away from the player
	}

	if (!clientInMeleeRange)
		return topMob;
	else
	{
		if (topMob == topClient)
			return topClient;
		else if (topMob && topMob->GetSpecialAbility(SpecialAbility::AllowedToTank))
			return topMob;
		else
			return topMeleeClient;
	}
}

Mob *HateList::GetMostHate(bool includeBonus)
{
	Mob* topMob = nullptr;
	int32 topHate = -1;
	bool firstInRangeBonusApplied = false;
	tHateEntry *cur;
	auto iterator = list.begin();
	while(iterator != list.end())
	{
		cur = (*iterator);
		int32 bonus = 0;

		if (cur->ent->IsClient() && cur->ent->CastToClient()->IsFeigned() && !owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity))
		{
			++iterator;
			continue;
		}

		if (includeBonus)
		{
			bool combatRange = owner->IsInCombatRange(cur->ent);

			bonus = GetHateBonus(cur, combatRange, !firstInRangeBonusApplied);

			if (!firstInRangeBonusApplied && combatRange)
			{
				firstInRangeBonusApplied = true;
			}
		}

		if(cur->ent != nullptr && ((cur->hate + bonus) > topHate))
		{
			topMob = cur->ent;
			topHate = cur->hate + bonus;
		}
		++iterator;
	}
	return topMob;
}


Mob *HateList::GetRandom()
{
	int count = list.size();
	if (count == 0)
		return nullptr;

	auto iterator = list.begin();
	int random = zone->random.Int(0, count - 1);

	for (int i = 0; i < count; i++)
	{
		if (i < random)
			++iterator;
		else if ((*iterator)->ent->IsClient() && (*iterator)->ent->CastToClient()->IsFeigned() && !owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity))
			++iterator;
		else
			return (*iterator)->ent;
	}

	if (random > 0)
	{
		iterator = list.begin();
		for (int i = 0; i < random; i++)
		{
			if ((*iterator)->ent->IsClient() && (*iterator)->ent->CastToClient()->IsFeigned() && !owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity))
				++iterator;
			else
				return (*iterator)->ent;
		}
	}

	return nullptr;
}

Client *HateList::GetRandomClient(int32 max_dist)
{
	int count = list.size();
	if (count == 0)
		return nullptr;

	max_dist *= max_dist;
	auto iterator = list.begin();
	int random = zone->random.Int(0, count - 1);
	for (int i = 0; i < count; i++)
	{
		if (i < random || !(*iterator)->ent->IsClient())
			++iterator;
		else if ((*iterator)->ent->CastToClient()->IsFeigned() && !owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity))
			++iterator;
		else if (max_dist == 0 || (*iterator)->dist_squared < max_dist)
			return (*iterator)->ent->CastToClient();
		else
			++iterator;
	}

	if (random > 0)
	{
		iterator = list.begin();
		for (int i = 0; i < random; i++)
		{
			if (!(*iterator)->ent->IsClient())
				++iterator;
			if ((*iterator)->ent->CastToClient()->IsFeigned() && !owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity))
				++iterator;
			else if (max_dist == 0 || (*iterator)->dist_squared < max_dist)
				return (*iterator)->ent->CastToClient();
			else
				++iterator;
		}
	}

	return nullptr;
}

int32 HateList::GetEntHate(Mob *ent, bool includeBonus)
{
	bool firstInRangeBonusApplied = false;
	bool combatRange;
	tHateEntry *p;
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		p = (*iterator);

		if (!p)
		{
			++iterator;
			continue;
		}
		
		if (includeBonus)
		{
			combatRange = owner->CombatRange(p->ent);

			if (!firstInRangeBonusApplied && combatRange)
			{
				firstInRangeBonusApplied = true;
			}
		}

		if (p->ent == ent)
		{
			if (includeBonus)
			{
				return (p->hate + GetHateBonus(p, combatRange, !firstInRangeBonusApplied));
			}
			else
			{
				return p->hate;
			}
		}
		++iterator;
	}
	return 0;
}

int32 HateList::GetEntDamage(Mob *ent, bool combine_pet_dmg)
{
	int32 dmg = 0;
	tHateEntry *p;
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		p = (*iterator);

		if (!p)
		{
			++iterator;
			continue;
		}

		if (p->ent == ent)
		{
			if (ent->IsClient() && ent->CastToClient()->IsFeigned()
				&& (!owner->IsFleeing() || owner->IsRooted() || (Distance(owner->GetPosition(), ent->GetPosition()) > 100.0f))
			)
				dmg = 0;
			else
				dmg = p->damage;

			if (combine_pet_dmg)
				return dmg + GetEntPetDamage(p->ent);
			else
				return dmg;
		}
		++iterator;
	}
	return 0;
}

//looking for any mob with hate > -1
bool HateList::IsEmpty() {
	return(list.size() == 0);
}

// Prints hate list to a client
void HateList::PrintToClient(Client *c)
{
	std::list<tHateEntry*> list2 = list;
	list2.sort([](tHateEntry* a, tHateEntry* b) { return a->hate > b->hate; });

	int32 bonusHate = 0;
	auto iterator = list2.begin();
	Mob* closestMob = GetClosest();
	Mob* firstInRange = GetFirstMobInRange();
	uint32 aggroTime = GetAggroDeaggroTime();
	int32 pets_dmg = 0;
	char buffer[20];
	char buffer2[15];

	if (IsEmpty())
	{
		if (aggroTime == 0xFFFFFFFF)
			c->Message(Chat::White, "Hatelist is empty. (never aggroed)");
		else
		{
			aggroTime /= 1000;
			c->Message(Chat::White, "Hatelist is empty. (last aggro %u hrs %u mins %u sec ago)", aggroTime / (60 * 60), (aggroTime / 60) % 60, aggroTime % 60);
		}

		if (owner->IsClient())
			c->Message(Chat::White, "NPCs targeting this client: %u", entity_list.GetTopHateCount(owner));

		return;
	}
	tHateEntry *e;
	while (iterator != list2.end())
	{
		e = (*iterator);
		uint32 timer = Timer::GetCurrentTime() - e->last_hate;
		if (timer > 0)
		{
			timer /= 1000;
		}
		bool combatRange = owner->CombatRange(e->ent);

		bonusHate = GetHateBonus(e, combatRange, e->ent == firstInRange);

		pets_dmg = GetEntPetDamage(e->ent);
		if (pets_dmg > 0)
			snprintf(buffer, 20, " (%i)", e->damage + pets_dmg);
		else
			buffer[0] = '\0';

		if (e->ent->IsClient() && e->ent->CastToClient()->IsFeigned())
			strcpy(buffer2, " (feigned)");
		else if (bonusHate > 0)
			snprintf(buffer2, 15, " (%i)", bonusHate);
		else
			buffer2[0] = '\0';

		c->Message(Chat::White, "- name: %s (%s), timer: %i, damage: %d%s, hate: %d%s",
			(e->ent && e->ent->GetName()) ? e->ent->GetName() : "(null)",
			GetClassIDName(e->ent->GetClass(), 1),
			timer, e->damage, buffer, e->hate, buffer2);

		++iterator;
	}

	if (owner->GetSpecialAbility(SpecialAbility::Rampage))
	{
		int entityID;
		Mob *mob;

		c->Message(Chat::White, " --- Rampage list top 10 (size: %i) ---", owner->GetRampageListSize());

		for (int slot = 0; slot < owner->GetRampageListSize(); slot++)
		{
			if (slot > 9)
				break;

			entityID = owner->GetRampageEntityID(slot);
			
			if (entityID < 0)
			{
				c->Message(Chat::White, " [%i] error: invalid slot", slot + 1);
				continue;
			}
			if (entityID == 0)
			{
				c->Message(Chat::White, " [%i] <empty>", slot + 1);
				continue;
			}

			mob = entity_list.GetMob(entityID);
			if (mob)
			{
				if (mob->IsClient() && mob->CastToClient()->IsFeigned() && !owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity))
					strcpy(buffer2, " (feigned)");
				else if (mob == owner->GetTarget())
					strcpy(buffer2, " (tank)");
				else
					buffer2[0] = '\0';

				c->Message(Chat::White, " [%i] %s (%s)%s Dist: %i", 
					slot + 1,
					mob->GetCleanName(),
					GetClassIDName(mob->GetClass(), 1),
					buffer2,
					static_cast<int>(Distance(mob->GetPosition(), owner->GetPosition()))
				);
			}
			else
			{
				c->Message(Chat::White, " [%i] error: invalid entity ID (%i)", slot + 1, entityID);
			}
		}
	}
	aggroTime /= 1000;
	c->Message(Chat::White, "Time aggro: %u hrs %u mins %u sec%s", aggroTime / (60 * 60), (aggroTime / 60) % 60, aggroTime % 60,
		IsIgnoringAllHaters() ? " (ignoring all haters)" : "");
}

int HateList::AreaRampage(Mob *caster, Mob *target, int count, int damagePct)
{
	if(!target || !caster)
		return 0;

	int targetsHit = 0;
	float dist;

	// if the rampager dies during the rampge this hate list is wiped and its elements deleted, so make a copy of the list of hated mobs before starting the rampage
	std::list<Mob *> hated_mobs;
	std::for_each(list.begin(), list.end(), [&](tHateEntry *h) { hated_mobs.push_back(h->ent); });

	for(auto it = hated_mobs.begin(); it != hated_mobs.end() && !caster->HasDied() && targetsHit < count; ++it)
	{
		Mob *mob = *it;
		if (mob == caster || mob->HasDied() || (mob->IsClient() && mob->CastToClient()->IsFeigned() && !owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity)))
		{
			continue;
		}

		dist = Distance(caster->GetPosition(), mob->GetPosition());
		dist += 0.5f;	// allow melee to avoid AoE rampage on cornered mobs with some difficulty/luck
		dist *= dist;

		// Wild Ramp hits the tank if the hate list is 1 entity on Live.  Haven't verified this for AK yet
		if ((caster->GetRace() != caster->GetBaseRace() && target != mob && sqrt(dist) <= caster->GetBaseSize())	// crude way to prevent Ignite Bones spell from reducing AoE Rampage range very much
			|| (caster->CombatRange(mob, dist) && (hated_mobs.size() == 1 || target != mob)))
		{
			caster->DoMainHandRound(mob, damagePct);
			if (caster->IsDualWielding())
				caster->DoOffHandRound(mob, damagePct);

			++targetsHit;
		}
	}

	return targetsHit;
}

void HateList::SpellCast(Mob *caster, uint32 spell_id, float range, Mob* ae_center)
{
	if(!caster)
		return;

	Mob* center = caster;

	if (ae_center)
		center = ae_center;

	//this is slower than just iterating through the list but avoids
	//crashes when people kick the bucket in the middle of this call
	//that invalidates our iterator but there's no way to know sadly
	//So keep a list of entity ids and look up after
	std::list<uint32> id_list;
	range = range * range;
	float dist_targ = 0;
	tHateEntry *h;
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		h = (*iterator);
		if(range > 0)
		{
			dist_targ = DistanceSquared(center->GetPosition(), h->ent->GetPosition());
			if (dist_targ <= range)
			{
				id_list.push_back(h->ent->GetID());
			}
		}
		else
		{
			id_list.push_back(h->ent->GetID());
		}
		++iterator;
	}
	Mob *cur;
	auto iter = id_list.begin();
	while(iter != id_list.end())
	{
		cur = entity_list.GetMobID((*iter));
		if(cur)
		{
			caster->SpellOnTarget(spell_id, cur);
		}
		iter++;
	}
}

void HateList::ReportDmgTotals(Mob* mob, bool corpse, bool xp, bool faction, int32 dmg_amt) 
{
	if (!mob || mob->IsPlayerOwned())
		return;

	auto iterator = list.begin();
	while (iterator != list.end())
	{
		Client *p;

		if ((*iterator)->ent && (*iterator)->ent->IsClient())
			p = (*iterator)->ent->CastToClient();
		else if ((*iterator)->ent && (*iterator)->ent->IsPlayerOwned())
		{
			p = (*iterator)->ent->GetOwner()->CastToClient();
			if (mob && mob->IsNPC() && mob->CastToNPC()->IsOnHatelist(p))
			{
				// Owner is on the hatelist, so it will have its own entry. Set to null to prevent a double message.
				p = nullptr;
			}
		}
		else
			p = nullptr;

		if (p && p->Admin() >= 80)
			mob->ReportDmgTotals(p, corpse, xp, faction, dmg_amt);

		++iterator;
	}
}

// get hate for the Nth player on the list, in descending order.  Does not include bonuses
// returns -1 for invalid n
int HateList::GetHateN(int n)
{
	if (n < 1 || n > list.size() || list.size() == 0)
		return -1;

	std::list<tHateEntry*> list2 = list;
	list2.sort([](tHateEntry* a, tHateEntry* b) { return a->hate > b->hate; });

	auto iterator = list2.begin();
	std::advance(iterator, n-1);

	tHateEntry *entity = (*iterator);

	if (entity)
		return entity->hate;
	else
		return -1;
}

Mob* HateList::GetFirstMobInRange()
{
	auto iterator = list.begin();
	if (IsEmpty())
	{
		return nullptr;
	}
	tHateEntry *e;
	Mob* closestMob = GetClosest();

	if (!closestMob || !closestMob->CombatRange(owner))
		return nullptr;

	if (owner->GetSpecialAbility(SpecialAbility::ProximityAggro))
		return closestMob;

	while (iterator != list.end())
	{
		e = (*iterator);
		if (owner->CombatRange(e->ent) && (!e->ent->IsClient() || !e->ent->CastToClient()->IsFeigned() || owner->GetSpecialAbility(SpecialAbility::FeignDeathImmunity)))
			return e->ent;

		++iterator;
	}
	return nullptr;
}

void HateList::RemoveFeigned()
{
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		if ((*iterator)->ent && (*iterator)->ent->IsClient() && (*iterator)->ent->CastToClient()->IsFeigned())
		{
			safe_delete(*iterator);
			iterator = list.erase(iterator);
			continue;
		}
		++iterator;
	}

	if (owner->GetPet() && !owner->GetPet()->IsCharmed())
		owner->GetPet()->RemoveFeignedFromHateList();
}
