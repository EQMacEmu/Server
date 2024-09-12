/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2006 EQEMu Development Team (http://eqemulator.net)

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

#include "../common/rulesys.h"

#include "map.h"
#include "pathfinder_interface.h"
#include "mob_movement_manager.h"
#include "string_ids.h"
#include "water_map.h"
#include "zone.h"

#ifdef _WINDOWS
#define snprintf	_snprintf
#endif

extern Zone* zone;

#define FEAR_PATHING_DEBUG

int Mob::GetFleeRatio(Mob* other)
{
	int specialFleeRatio = GetSpecialAbility(SpecialAbility::FleePercent);
	if (specialFleeRatio > 0)
	{
		return specialFleeRatio;
	}

	int fleeRatio = RuleI(Combat, FleeHPRatio);

	Mob *hate_top = GetHateTop();
	if (other != nullptr)
		hate_top = other;

	if (!hate_top)
	{
		return 0;
	}

	uint8 hateTopLevel = hate_top->GetLevel();
	if (GetLevel() <= hateTopLevel)
	{
		if (hate_top->GetLevelCon(GetLevel()) == CON_GREEN && GetLevel() <= DEEP_GREEN_LEVEL)
		{
			// green con 18 and under runs much earlier
			return 50;
		}
	}
	else
	{
		if (GetLevel() > (hateTopLevel + 2))
		{
			// red con
			return fleeRatio / 2;
		}
	}

	return fleeRatio;
}

//this is called whenever we are damaged to process possible fleeing
void Mob::CheckFlee() 
{

	if (IsPet() || IsCasting() || (IsNPC() && CastToNPC()->IsUnderwaterOnly()))
		return;

	//if were already fleeing, we only need to check speed.  Speed changes will trigger pathing updates.
	if (flee_mode && curfp) {
		float flee_speed = GetFearSpeed();
		if (flee_speed < 0.1f)
			flee_speed = 0.0f;
		SetRunAnimation(flee_speed);
		if (IsMoving() && flee_speed < 0.1f)
			StopNavigation();
		return;
	}

	//dont bother if we are immune to fleeing
	if(GetSpecialAbility(SpecialAbility::FleeingImmunity))
		return;
	
	//see if were possibly hurt enough
	float ratio = GetHPRatio();
	float fleeratio = static_cast<float>(GetFleeRatio());

	if(ratio > fleeratio)
		return;

	// hp cap so 1 million hp NPCs don't flee with 200,000 hp left
	if (!GetSpecialAbility(SpecialAbility::FleePercent) && GetHP() > 15000)
		return;

	//we might be hurt enough, check con now..
	Mob *hate_top = GetHateTop();
	if(!hate_top) {
		//this should never happen...
		StartFleeing();
		return;
	}

	if(hate_top->GetHP() < GetHP()) {
		// don't flee if target has less hp than us
		return;
	}

	if (RuleB(Combat, FleeIfNotAlone) ||
		GetSpecialAbility(SpecialAbility::AlwaysFlee) ||
		(GetSpecialAbility(SpecialAbility::AlwaysFleeLowCon) && hate_top->GetLevelCon(GetLevel()) == CON_GREEN) ||
		(!RuleB(Combat, FleeIfNotAlone) && entity_list.FleeAllyCount(hate_top, this) == 0)
		)
	{
		StartFleeing();
	}
}

void Mob::StopFleeing()
{
	if (!flee_mode)
		return;

	flee_mode = false;

	//see if we are legitimately feared or blind now
	if (!IsFearedNoFlee() && !IsBlind())
	{
		curfp = false;
		StopNavigation();
	}
}

void Mob::FleeInfo(Mob* client)
{
	float other_ratio = client->GetHPRatio();
	bool wontflee = false;
	std::string reason;
	std::string flee;

	int allycount = entity_list.FleeAllyCount(client, this);

	if (flee_mode && curfp)
	{
		wontflee = true;
		reason = "NPC is already fleeing!";
	}
	else if (GetSpecialAbility(SpecialAbility::FleeingImmunity))
	{
		wontflee = true;
		reason = "NPC is immune to fleeing.";
	}
	else if (client->GetHP() < GetHP())
	{
		wontflee = true;
		reason = "Player has less health.";
	}
	else if (GetSpecialAbility(SpecialAbility::AlwaysFlee))
	{
		flee = "NPC has ALWAYS_FLEE set.";
	}
	else if (GetSpecialAbility(SpecialAbility::AlwaysFleeLowCon) && client->GetLevelCon(GetLevel()) == CON_GREEN)
	{
		flee = "NPC has ALWAYS_FLEE_LOW_CON and is green to the player.";
	}
	else if (RuleB(Combat, FleeIfNotAlone) || (!RuleB(Combat, FleeIfNotAlone) && allycount == 0))
	{
		flee = "NPC has no allies nearby or the rule to flee when not alone is enabled.";
	}
	else
	{
		wontflee = true;
		reason = "NPC likely has allies nearby.";
	}


	if (!wontflee)
	{
		client->Message(Chat::Green, "%s will flee at %d percent because %s", GetName(), GetFleeRatio(client), flee.c_str());
	}
	else
	{
		client->Message(Chat::Red, "%s will not flee because %s", GetName(), reason.c_str());
	}

	client->Message(Chat::White, "NPC ally count %d", allycount);
}

void Mob::ProcessFlee()
{
	if (!flee_mode)
		return;

	//Stop fleeing if effect is applied after they start to run.
	//When ImmuneToFlee effect fades it will turn fear back on and check if it can still flee.
	// Stop flee if we've become a pet after we began fleeing.
	if (flee_mode && (GetSpecialAbility(SpecialAbility::FleeingImmunity) || IsCharmedPet()) && !IsFearedNoFlee() && !IsBlind())
	{
		curfp = false;
		return;
	}

	bool dying = GetHPRatio() < GetFleeRatio();
	// We have stopped fleeing for an unknown reason (couldn't find a node is possible) restart.
	if (flee_mode && !curfp)
	{
		if(dying)
			StartFleeing();
	}

	//see if we are still dying, if so, do nothing
	if (dying)
		return;

	//we are not dying anymore, check to make sure we're not blind or feared and cancel flee.
	StopFleeing();
}

void Mob::CalculateNewFearpoint()
{
	// blind waypoint logic isn't the same as fear's.  Has a chance to run toward the player
	// chance is very high if the player is moving, otherwise it's low
	if (IsBlind() && !IsFeared() && GetTarget())
	{
		int roll = 20;
		if (GetTarget()->GetCurrentSpeed() > 0.1f || (GetTarget()->IsClient() && GetTarget()->animation != 0))
			roll = 80;
		
		if (zone->random.Roll(roll))
		{
			m_FearWalkTarget = glm::vec3(GetTarget()->GetPosition());
			curfp = true;
			return;
		}
	}

	if (RuleB(Pathing, Fear) && zone->pathing) {
		glm::vec3 Node;
		int flags = PathingNotDisabled ^ PathingZoneLine;
		if (IsNPC() && CastToNPC()->IsUnderwaterOnly() && !zone->IsWaterZone(GetZ()))
			Node = glm::vec3(0.0f);
		else
			Node = zone->pathing->GetRandomLocation(glm::vec3(GetX(), GetY(), GetZ()), flags);

		if (Node.x != 0.0f || Node.y != 0.0f || Node.z != 0.0f) {
			Node.z = GetFixedZ(Node);
			PathfinderOptions opts;
			opts.smooth_path = true;
			opts.step_size = RuleR(Pathing, NavmeshStepSize);
			opts.offset = GetZOffset();
			opts.flags = flags;
			auto partial = false;
			auto stuck = false;
			auto route = zone->pathing->FindPath(
				glm::vec3(GetX(), GetY(), GetZ()),
				glm::vec3(Node.x, Node.y, Node.z),
				partial,
				stuck,
				opts
			);
			glm::vec3 last_good_loc = Node;
			int route_size = route.size();
			int route_count = 0;
			bool have_los = true;

			if (route_size == 2)
			{
				// FindPath() often fails to compute a route in some places, so to prevent running through walls we need to check LOS on all 2 node routes
				// size 2 route usually means FindPath() bugged out.  sometimes it returns locs outside the geometry
				if (CheckLosFN(Node.x, Node.y, Node.z, 6.0))
				{
					Log(Logs::Detail, Logs::Pathing, "Direct route to fearpoint %0.1f, %0.1f, %0.1f calculated for %s", last_good_loc.x, last_good_loc.y, last_good_loc.z, GetName());
					m_FearWalkTarget = last_good_loc;
					curfp = true;
					return;
				}
				else
				{
					Log(Logs::Detail, Logs::Pathing, "FindRoute() returned single hop route to destination without LOS: %0.1f, %0.1f, %0.1f for %s", last_good_loc.x, last_good_loc.y, last_good_loc.z, GetName());
				}
				// use fallback logic if LOS fails
			}
			else if (!stuck)
			{
				if (zone->pathing->IsUsingNavMesh())
				{
					// if nav mesh zone, check route for LOS failures to prevent mobs ending up outside of playable area
					// only checking the last few hops because LOS will often fail in a valid route which can result in mobs getting undesirably trapped
					auto iter = route.begin();
					glm::vec3 previous_pos(GetX(), GetY(), GetZ());
					while (iter != route.end() && have_los == true) {
						auto &current_node = (*iter);
						iter++;
						route_count++;

						if (iter == route.end()) {
							continue;
						}

						previous_pos = current_node.pos;
						auto &next_node = (*iter);

						if (next_node.teleport)
							continue;

						if ((route_size - route_count) < 5 && !zone->zonemap->CheckLoS(previous_pos, next_node.pos))
						{
							//Shout("Loc %0.1f, %0.1f, %0.1f TO %0.1f, %0.1f, %0.1f FAILED LOS", previous_pos.x, previous_pos.y, previous_pos.z, next_node.pos.x, next_node.pos.y, next_node.pos.z);
							have_los = false;
							break;
						}
						else
						{
							last_good_loc = next_node.pos;
							//Shout("Loc %0.1f, %0.1f, %0.1f TO %0.1f, %0.1f, %0.1f has LOS or not checking", previous_pos.x, previous_pos.y, previous_pos.z, next_node.pos.x, next_node.pos.y, next_node.pos.z);
						}
					}
				}

				if (have_los || route_count > 2)
				{
					if (have_los)
						Log(Logs::Detail, Logs::Pathing, "Route to fearpoint %0.1f, %0.1f, %0.1f calculated for %s; route size: %i", last_good_loc.x, last_good_loc.y, last_good_loc.z, GetName(), route_size);
					else
						Log(Logs::Detail, Logs::Pathing, "Using truncated route to fearpoint %0.1f, %0.1f, %0.1f for %s; node count: %i; route size %i", last_good_loc.x, last_good_loc.y, last_good_loc.z, GetName(), route_count, route_size);

					m_FearWalkTarget = last_good_loc;
					curfp = true;
					return;
				}
			}
		}
	}


	// fallback logic if pathing system can't be used
	bool inliquid = zone->HasWaterMap() && zone->watermap->InLiquid(glm::vec3(GetPosition())) || zone->IsWaterZone(GetZ());
	bool stay_inliquid = (inliquid && IsNPC() && CastToNPC()->IsUnderwaterOnly());
	bool levitating = IsClient() && (FindType(SE_Levitate) || flymode != GravityBehavior::Ground);
	bool open_outdoor_zone = !zone->CanCastDungeon() && !zone->IsCity();

	int loop = 0;
	float ranx, rany, ranz;
	curfp = false;
	glm::vec3 myloc(GetX(), GetY(), GetZ());
	glm::vec3 myceil = myloc;
	float ceil = zone->zonemap->FindCeiling(myloc, &myceil);
	if (ceil != BEST_Z_INVALID) {
		ceil -= 1.0f;
	}
	while (loop < 100) //Max 100 tries
	{
		int ran = 250 - (loop * 2);
		loop++;
		if (open_outdoor_zone && loop < 20) // try a distant loc first; other way will likely pick a close loc
		{
			ranx = zone->random.Int(0, ran);
			rany = zone->random.Int(0, ran);
			if (ranx + rany < 200)
				continue;

			ranx = GetX() + (zone->random.Int(0, 1) == 1 ? ranx : -ranx);
			rany = GetY() + (zone->random.Int(0, 1) == 1 ? rany : -rany);
		}
		else
		{
			ranx = GetX() + zone->random.Int(0, ran - 1) - zone->random.Int(0, ran - 1);
			rany = GetY() + zone->random.Int(0, ran - 1) - zone->random.Int(0, ran - 1);
		}
		ranz = BEST_Z_INVALID;
		glm::vec3 newloc(ranx, rany, ceil != BEST_Z_INVALID ? ceil : GetZ());

		if (stay_inliquid || levitating || (loop > 50 && inliquid)) {
			if (zone->zonemap->CheckLoS(myloc, newloc)) {
				ranz = GetZ();
				curfp = true;
				break;
			}
		}
		else {
			if (ceil != BEST_Z_INVALID)
				ranz = zone->zonemap->FindGround(newloc, &myceil);
			else
				ranz = zone->zonemap->FindBestZ(newloc, &myceil);
			if (ranz != BEST_Z_INVALID)
				ranz = SetBestZ(ranz);
		}
		if (ranz == BEST_Z_INVALID)
			continue;
		float fdist = ranz - GetZ();
		if (fdist >= -50 && fdist <= 50 && CheckCoordLosNoZLeaps(GetX(), GetY(), GetZ(), ranx, rany, ranz))
		{
			curfp = true;
			break;
		}
	}
	if (curfp)
	{
		m_FearWalkTarget = glm::vec3(ranx, rany, ranz);
		Log(Logs::Detail, Logs::Pathing, "Non-pathed fearpoint %0.1f, %0.1f, %0.1f selected for %s", ranx, rany, ranz, GetName());
	}
}

