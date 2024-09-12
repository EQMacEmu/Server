/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2004 EQEMu Development Team (http://eqemu.org)

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
#include "../common/global_define.h"
#ifdef _EQDEBUG
#include <iostream>
#endif

#include "../common/features.h"
#include "../common/rulesys.h"
#include "../common/strings.h"
#include "../common/misc_functions.h"
#include "../common/data_verification.h"

#include "map.h"
#include "npc.h"
#include "quest_parser_collection.h"
#include "water_map.h"
#include "mob_movement_manager.h"

#include <math.h>
#include <stdlib.h>

struct wp_distance
{
	float dist;
	int index;
};

void NPC::AI_SetRoambox(float iRoamDist, uint32 iDelay, uint32 iMinDelay) 
{
	AI_SetRoambox(GetX()+iRoamDist, GetX()-iRoamDist, GetY()+iRoamDist, GetY()-iRoamDist, iDelay, iMinDelay);
}

void NPC::AI_SetRoambox(float iMaxX, float iMinX, float iMaxY, float iMinY, uint32 iDelay, uint32 iMinDelay) 
{
	roambox_max_x = iMaxX;
	roambox_min_x = iMinX;
	roambox_max_y = iMaxY;
	roambox_min_y = iMinY;

	if (roambox_max_x != 0 || roambox_min_x != 0 || roambox_max_y != 0 || roambox_min_y != 0)
	{
		roambox_distance = true;
	}
	roambox_ceil = 5000.0f;
	if (zone && zone->zonemap) {
		glm::vec3 start(GetPosition());
		glm::vec3 dest;

		float ceil = zone->zonemap->FindCeiling(start, &dest);
		if (ceil != BEST_Z_INVALID)
			roambox_ceil = ceil - 1.0f;
		}

	roambox_movingto_x = roambox_max_x + 1; // this will trigger a recalc
	roambox_delay = iDelay;
	roambox_min_delay = iMinDelay;
}

void NPC::DisplayWaypointInfo(Client *c) {

	SpawnGroup* sg = zone->spawn_group_list.GetSpawnGroup(GetSpawnGroupId());
	std::string spawn2 = GetSpawnedString();

	//Mob is on a roambox.
	if(sg && GetGrid() == 0 && roambox_distance)
	{
		c->Message(Chat::White, "Mob in spawn group %d is on a roambox.", GetSpawnGroupId());
		c->Message(Chat::White, "MinX: %0.2f MaxX: %0.2f MinY: %0.2f MaxY: %0.2f", sg->roambox[1], sg->roambox[0], sg->roambox[3], sg->roambox[2]);
		c->Message(Chat::White, "MinDelay: %d Delay: %d", sg->min_delay, sg->delay);
		c->Message(Chat::White, "Spawned Type/Point: %s", spawn2.c_str());
		if (roambox_movingto_x < roambox_max_x)
			c->Message(Chat::White, "Currently moving toward: x %.2f y %.2f", roambox_movingto_x, roambox_movingto_y);
		if (AIwalking_timer->Enabled()) {
			uint32 time_remaining = AIwalking_timer->GetRemainingTime();
			if (time_remaining > 0 && time_remaining != 0xFFFFFFFF)
				c->Message(Chat::White, "Currently paused by WP timer %.3f sec remaining", (float)time_remaining / 1000.0f);
		}
		if (AIloiter_timer->Enabled()) {
			uint32 time_remaining = AIloiter_timer->GetRemainingTime();
			if (time_remaining > 0 && time_remaining != 0xFFFFFFFF)
				c->Message(Chat::White, "Currently loitering %.3f sec remaining", (float)time_remaining / 1000.0f);
		}
		return;
	}
	//Mob either doesn't have a grid, or has a quest assigned grid. 
	else if(GetGrid() == 0)
	{
		c->Message(Chat::White, "Mob with spawned type %s is not on a permanent grid.", spawn2.c_str());
	}
	//Mob is on a normal grid.
	else
	{
		int MaxWp = GetMaxWp();
		//If the Mob hasn't moved yet, this will be 0. Set to 1 so it looks correct.
		if(MaxWp == 0)
			MaxWp = 1;

		if (GetGrid() == 999999) // grids generated from scripts have this grid ID
		{
			c->Message(Chat::White, "Mob has script generated grid, in spawn group %d, on waypoint %d/%d",
				GetSpawnGroupId(), GetCurWp() + 1, MaxWp);
		}
		else
		{
			c->Message(Chat::White, "Mob is on grid %d, in spawn group %d, on waypoint %d/%d",
				GetGrid(),
				GetSpawnGroupId(),
				GetCurWp() + 1, //We start from 0 internally, but in the DB and lua functions we start from 1.
				MaxWp);
			std::string type = "unknown";
			switch (wandertype)
			{
			case GridCircular:
				type = "Circular";
				break;
			case GridRandom10:
				type = "Random10";
				break;
			case GridRandom:
				type = "Random";
				break;
			case GridPatrol:
				type = "Patrol";
				break;
			case GridOneWayRepop:
				type = "One-Way Repop";
				break;
			case GridRand5LoS:
				type = "Random 5 LoS";
				break;
			case GridOneWayDepop:
				type = "One-Way Depop";
				break;
			case GridCenterPoint:
				type = "Center Point";
				break;
			case GridRandomCenterPoint:
				type = "Random Center Point";
				break;
			case GridRandomPath:
				type = "Random Path.  Patrol to WP: ";
				type.append(std::to_string(patrol));
				break;
			}
			c->Message(Chat::White, "Mob is on patrol type %s", type.c_str());
		}
		c->Message(Chat::White, "Spawned Type/Point: %s", spawn2.c_str());
	}

	if (AIwalking_timer->Enabled()) {
		uint32 time_remaining = AIwalking_timer->GetRemainingTime();
		if (time_remaining > 0 && time_remaining != 0xFFFFFFFF)
			c->Message(Chat::White, "Currently paused by WP timer %.3f sec remaining", (float)time_remaining / 1000.0f);
	}
	if (AIloiter_timer->Enabled()) {
		uint32 time_remaining = AIloiter_timer->GetRemainingTime();
		if (time_remaining > 0 && time_remaining != 0xFFFFFFFF)
			c->Message(Chat::White, "Currently loitering %.3f sec remaining", (float)time_remaining / 1000.0f);
	}

	//Waypoints won't load into memory until the Mob moves.
	std::vector<wplist>::iterator cur, end;
	cur = Waypoints.begin();
	end = Waypoints.end();
	for(; cur != end; ++cur) {
		c->Message(Chat::White,"Waypoint %d: (%.2f,%.2f,%.2f,%.2f) pause %d",
				cur->index+1, //We start from 0 internally, but in the DB and lua functions we start from 1.
				cur->x,
				cur->y,
				cur->z,
				cur->heading,
				cur->pause );
	}
}

void NPC::StopWandering()
{	// stops a mob from wandering, takes him off grid and sends him back to spawn point
	roamer=false;
	CastToNPC()->SetGrid(0);
	StopNavigation();
	AIwalking_timer->Start(2000);
	Log(Logs::Detail, Logs::Pathing, "Stop Wandering requested.");
	return;
}

// causes wandering to continue - overrides loiter timer, waypoint pause timer and PauseWandering()
void NPC::ResumeWandering()
{
	if(!IsNPC())
		return;

	AIloiter_timer->Trigger();
	AIloiter_timer->Stop();
	
	if (GetGrid() != 0)
	{
		if (GetGrid() < 0)
		{	// we were paused by a quest
			AIwalking_timer->Disable();
			SetGrid( 0 - GetGrid());
			if (cur_wp == EQ::WaypointStatus::QuestControlGrid)
			{	// got here by a MoveTo()
				cur_wp=save_wp;
				UpdateWaypoint(cur_wp);	// have him head to last destination from here
			}
			Log(Logs::Detail, Logs::Pathing, "Resume Wandering requested. Grid %d, wp %d", GetGrid(), cur_wp);
		}
		else if (AIwalking_timer->Enabled())
		{	// we are at a waypoint paused normally
			Log(Logs::Detail, Logs::Pathing, "Resume Wandering on timed pause. Grid %d, wp %d", GetGrid(), cur_wp);
			AIwalking_timer->Trigger();	// disable timer to end pause now
		}
		else
		{
			Log(Logs::General, Logs::Error, "NPC not paused - can't resume wandering: %lu", (unsigned long)GetNPCTypeID());
			return;
		}
	}
	else
	{
		Log(Logs::General, Logs::Error, "NPC not on grid - can't resume wandering: %lu", (unsigned long)GetNPCTypeID());
	}
	return;
}

void NPC::PauseWandering(int pausetime)
{	// causes wandering to stop but is resumable
	// 0 pausetime means pause until resumed
	// otherwise automatically resume when time is up
	if (GetGrid() != 0)
	{
		Log(Logs::Detail, Logs::Pathing, "Paused Wandering requested for: %s. Grid %d. Resuming in %d ms (0=not until told)", GetName(), GetGrid(), pausetime*1000);
		if (pausetime<1)
		{	// negative grid number stops him dead in his tracks until ResumeWandering()
			SetGrid( 0 - GetGrid());
		}
		else
		{	// specified waiting time, he'll resume after that
			AIwalking_timer->Start(pausetime*1000); // set the timer
		}
		StopNavigation();
	}
	else if(roambox_distance)
	{
		roambox_movingto_x = roambox_max_x + 1; // force update
		AIloiter_timer->Reset();
		AIloiter_timer->Start(pausetime*1000);
		StopNavigation();
	}
	else 
	{
		Log(Logs::General, Logs::Error, "NPC not on grid - can't pause wandering: %lu", (unsigned long)GetNPCTypeID());
		return;
	}

	// this stops them from auto changing direction, in AI_DoMovement()
	AIhail_timer->Start((RuleI(NPC, SayPauseTimeInSec)-1)*1000);
	
}

void NPC::MoveTo(const glm::vec4& position, bool saveguardspot, uint32 delay)
{	// makes mob walk to specified location
	if (IsNPC() && GetGrid() != 0)
	{	// he is on a grid
		if (GetGrid() < 0)
		{	// currently stopped by a quest command
			SetGrid( 0 - GetGrid());	// get him moving again
			Log(Logs::Detail, Logs::AI, "MoveTo during quest wandering. Canceling quest wandering and going back to grid %d when MoveTo is done.", GetGrid());
		}
		AIwalking_timer->Disable();    // disable timer in case he is paused at a wp
		if (cur_wp >= 0)
		{	// we've not already done a MoveTo()
			save_wp = cur_wp;	// save the current waypoint
			cur_wp = EQ::WaypointStatus::QuestControlGrid;
		}
		Log(Logs::Detail, Logs::AI, "MoveTo %s, pausing regular grid wandering. Grid %d, save_wp %d",to_string(static_cast<glm::vec3>(position)).c_str(), -GetGrid(), save_wp);
	}
	else
	{	// not on a grid
		roamer = true;
		save_wp = 0;
		cur_wp = EQ::WaypointStatus::QuestControlNoGrid;
		Log(Logs::Detail, Logs::AI, "MoveTo %s without a grid.", to_string(static_cast<glm::vec3>(position)).c_str());
	}
	if (saveguardspot)
	{
		m_GuardPoint = position;
		m_GuardPoint.z = GetFixedZ(position);

		if (m_GuardPoint.w == -1.0f)
			m_GuardPoint.w = this->CalculateHeadingToTarget(position.x, position.y);

		m_GuardPoint.w = FixHeading(m_GuardPoint.w);
		if (m_GuardPoint.w == 0.0f)
			m_GuardPoint.w  = 0.0001f;		//hack to make IsGuarding simpler

		Log(Logs::Detail, Logs::AI, "Setting guard position to %s", to_string(static_cast<glm::vec3>(m_GuardPoint)).c_str());
	}

	m_CurrentWayPoint = position;
	m_CurrentWayPoint.z = GetFixedZ(position);
	cur_wp_pause = delay;
	AIloiter_timer->Reset();
	AIloiter_timer->Start(100);
	AIhail_timer->Disable();
	AIwalking_timer->Disable();
}

void NPC::UpdateWaypoint(int wp_index)
{
	if(wp_index >= static_cast<int>(Waypoints.size())) {
		Log(Logs::Detail, Logs::AI, "Update to waypoint %d failed. Not found.", wp_index);
		return;
	}
	std::vector<wplist>::iterator cur;
	cur = Waypoints.begin();
	cur += wp_index;
	cur_wp = wp_index;

	m_CurrentWayPoint = glm::vec4(cur->x, cur->y, cur->z, cur->heading);
	cur_wp_pause = cur->pause;
	Log(Logs::Detail, Logs::AI, "Next waypoint %d: (%.3f, %.3f, %.3f, %.3f)", wp_index, m_CurrentWayPoint.x, m_CurrentWayPoint.y, m_CurrentWayPoint.z, m_CurrentWayPoint.w);
}

void NPC::CalculateNewWaypoint()
{
	int old_wp = cur_wp;
	bool quest_controlled = (cur_wp == EQ::WaypointStatus::QuestControlGrid);
	bool reached_end = false;
	bool reached_beginning = false;
	if (cur_wp == max_wp - 1) //cur_wp starts at 0, max_xp starts at 1.
		reached_end = true;
	if (cur_wp == 0)
		reached_beginning = true;

	switch (wandertype)
	{
	case GridCircular:
	{
		if (reached_end)
			cur_wp = 0;
		else
			cur_wp = cur_wp + 1;
		break;
	}
	case GridRandom10:
	{
		std::list<wplist> closest;
		GetClosestWaypoints(closest, 10, glm::vec3(GetPosition()));
		auto iter = closest.begin();
		if (!closest.empty())
		{
			iter = closest.begin();
			std::advance(iter, zone->random.Int(0, closest.size() - 1));
			cur_wp = (*iter).index;
		}

		break;
	}
	case GridRandom:
	case GridCenterPoint:
	{
		if (wandertype == GridCenterPoint && !reached_beginning)
		{
			cur_wp = 0;
		}
		else
		{
			cur_wp = zone->random.Int(0, Waypoints.size() - 1);
			if (cur_wp == old_wp || (wandertype == GridCenterPoint && cur_wp == 0))
			{
				if (cur_wp == (Waypoints.size() - 1))
				{
					if (cur_wp > 0)
					{
						cur_wp--;
					}
				}
				else if (cur_wp == 0)
				{
					if ((Waypoints.size() - 1) > 0)
					{
						cur_wp++;
					}
				}
			}
		}

		break;
	}
	case GridRandomCenterPoint:
	{
		bool on_center = Waypoints[cur_wp].centerpoint;
		std::vector<wplist> random_waypoints;
		for (auto & wpl : Waypoints)
		{
			if (wpl.index != cur_wp &&
				((on_center && !wpl.centerpoint) || (!on_center && wpl.centerpoint)))
			{
				random_waypoints.push_back(wpl);
			}
		}

		if (random_waypoints.size() == 0)
		{
			cur_wp = 0;
		}
		else
		{
			int windex = zone->random.Roll0(random_waypoints.size());
			cur_wp = random_waypoints[windex].index;
		}

		break;
	}
	case GridPatrol:
	{
		if (reached_end)
			patrol = 1;
		else if (reached_beginning)
			patrol = 0;
		if (patrol == 1)
			cur_wp = cur_wp - 1;
		else
			cur_wp = cur_wp + 1;

		break;
	}
	case GridOneWayRepop:
	case GridOneWayDepop:
	{
		cur_wp = cur_wp + 1;
		break;
	}
	case GridRand5LoS:
	{
		std::list<wplist> closest;
		GetClosestWaypoints(closest, 5, glm::vec3(GetPosition()));

		auto iter = closest.begin();
		while (iter != closest.end())
		{
			if (CheckLosFN((*iter).x, (*iter).y, (*iter).z, GetSize()))
			{
				++iter;
			}
			else
			{
				iter = closest.erase(iter);
			}
		}

		if (closest.size() != 0)
		{
			iter = closest.begin();
			std::advance(iter, zone->random.Int(0, closest.size() - 1));
			cur_wp = (*iter).index;
		}
		break;
	}
	case GridRandomPath: // randomly select a waypoint but follow path to it instead of walk directly to it ignoring walls
	{
		if (Waypoints.size() == 0)
		{
			cur_wp = 0;
		}
		else
		{
			if (cur_wp == patrol) // reutilizing patrol member instead of making new member for this wander type; here we use it to save a random waypoint
			{
				if (!Waypoints[cur_wp].centerpoint)
				{
					// if we have arrived at a waypoint that is NOT a centerpoint, then check for the existence of any centerpoint waypoint
					// if any exists then randomly go to it otherwise go to one that exist.
					std::vector<wplist> random_centerpoints;
					for (auto& wpl : Waypoints)
					{
						if (wpl.index != cur_wp && wpl.centerpoint)
						{
							random_centerpoints.push_back(wpl);
						}
					}

					if (random_centerpoints.size() == 1)
					{
						patrol = random_centerpoints[0].index;
						break;
					}
					else if (random_centerpoints.size() > 1)
					{
						int windex = zone->random.Roll0(random_centerpoints.size());
						patrol = random_centerpoints[windex].index;
						break;
					}
				}

				while (patrol == cur_wp)
				{
					std::vector<wplist> random_waypoints;
					for (auto& wpl : Waypoints)
					{
						if (wpl.index != cur_wp && wpl.pause >= 0 && !wpl.centerpoint)
						{
							random_waypoints.push_back(wpl);
						}
					}
					int windex = zone->random.Roll0(random_waypoints.size());
					patrol = random_waypoints[windex].index;
				}
			}

			if (patrol > cur_wp)
				cur_wp = cur_wp + 1;
			else
				cur_wp = cur_wp - 1;
		}
	}
	}

	tar_ndx = 20;

	// Preserve waypoint setting for quest controlled NPCs
	if (quest_controlled && save_wp >= 0)
		cur_wp = save_wp;

	// Check to see if we need to update the waypoint.
	if (cur_wp >= 0 && cur_wp < Waypoints.size() && cur_wp != old_wp)
		UpdateWaypoint(cur_wp);

	if(IsNPC() && GetClass() == Class::Merchant && CastToNPC()->IsMerchantOpen())
		entity_list.SendMerchantEnd(this);
}

bool wp_distance_pred(const wp_distance& left, const wp_distance& right)
{
	return left.dist < right.dist;
}

int NPC::GetClosestWaypoint(const glm::vec3& location)
{
	if (Waypoints.size() <= 1)
		return 0;

	int closest = 0;
	float closestDist = 9999999.0f;
	float dist;

	for (int i = 0; i < Waypoints.size(); ++i)
	{
		dist = DistanceSquared(location, glm::vec3(Waypoints[i].x, Waypoints[i].y, Waypoints[i].z));

		if (dist < closestDist)
		{
			closestDist = dist;
			closest = i;
		}
	}
	return closest;
}

// fills wp_list with the closest count number of waypoints
void NPC::GetClosestWaypoints(std::list<wplist> &wp_list, int count, const glm::vec3& location)
{
	wp_list.clear();
	if(Waypoints.size() <= count)
	{
		for(int i = 0; i < Waypoints.size(); ++i)
		{
			wp_list.push_back(Waypoints[i]);
		}
		return;
	}

	std::list<wp_distance> distances;
	for(int i = 0; i < Waypoints.size(); ++i)
	{
		float cur_x = (Waypoints[i].x - location.x);
		cur_x *= cur_x;
		float cur_y = (Waypoints[i].y - location.y);
		cur_y *= cur_y;
		float cur_z = (Waypoints[i].z - location.z);
		cur_z *= cur_z;
		float cur_dist = cur_x + cur_y + cur_z;
		wp_distance w_dist;
		w_dist.dist = cur_dist;
		w_dist.index = i;
		distances.push_back(w_dist);
	}
	distances.sort([](const wp_distance& a, const wp_distance& b) {
		return a.dist < b.dist;
	});

	auto iter = distances.begin();
	for(int i = 0; i < count; ++i)
	{
		wp_list.push_back(Waypoints[(*iter).index]);
		++iter;
	}
}

void NPC::SetWaypointPause()
{
	//Declare time to wait on current WP

	if (cur_wp_pause == 0) {
		AIwalking_timer->Start(100);
		AIwalking_timer->Trigger();
	}
	else
	{
		switch (pausetype)
		{
			case 0: //Random Half
				AIwalking_timer->Start((cur_wp_pause - zone->random.Int(0, cur_wp_pause-1)/2)*1000);
				break;
			case 1: //Full
				AIwalking_timer->Start(cur_wp_pause*1000);
				break;
			case 2: //Random Full
				AIwalking_timer->Start(zone->random.Int(0, cur_wp_pause-1)*1000);
				break;
		}
		StopNavigation();
	}
}

void NPC::SaveGuardSpot(bool iClearGuardSpot)
{
	if (iClearGuardSpot) {
		Log(Logs::Detail, Logs::AI, "Clearing guard order.");
		m_GuardPoint = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else {

		m_GuardPoint = m_Position;
		m_GuardPoint.w = FixHeading(m_GuardPoint.w);

		if (m_GuardPoint.w == 0.0f)
			m_GuardPoint.w = 0.0001f;		//hack to make IsGuarding simpler
		Log(Logs::Detail, Logs::AI, "Setting guard position to {0}", to_string(static_cast<glm::vec3>(m_GuardPoint)));
	}
}

void NPC::SetGuardSpot(float x, float y, float z, float h)
{
	m_GuardPoint.x = x;
	m_GuardPoint.y = y;
	m_GuardPoint.z = z;
	m_GuardPoint.w = FixHeading(h);
}

void NPC::NextGuardPosition() {
	
	if (m_GuardPoint.x == m_Position.x && m_GuardPoint.y == m_Position.y && m_GuardPoint.z == m_Position.z) {
		if (HasOwner() && GetOwner()->IsClient()) {
			if (AIpetguard_timer->Check()) {
				// update heading to nearest NPC
				Mob* mob = nullptr;
				if (IsEngaged()) {
					mob = hate_list.GetMostHate();
				}
				if (!mob)
					mob = entity_list.GetNearestNPC(this);
				if (mob) {
					float new_heading = CalculateHeadingToTarget(mob->GetX(), mob->GetY());
					if (GetHeading() != new_heading) {
						RotateToRunning(new_heading);
					}
				}
				else {
					if (GetHeading() != m_GuardPoint.w && m_Delta.w == 0.0f) {
						RotateTo(m_GuardPoint.w);
					}
				}
			}
		} 
		else {
			if (GetHeading() != m_GuardPoint.w && m_Delta.w == 0.0f) {
				RotateTo(m_GuardPoint.w);
			}
			if (IsMoving() && m_Position == m_GuardPoint)
				StopNavigation();
		}
	}
	else {
		NavigateTo(m_GuardPoint.x, m_GuardPoint.y, m_GuardPoint.z);
	}
}

float Mob::CalculateDistance(float x, float y, float z) {
	return (float)sqrtf( ((m_Position.x-x)*(m_Position.x-x)) + ((m_Position.y-y)*(m_Position.y-y)) + ((m_Position.z-z)*(m_Position.z-z)) );
}

void Mob::WalkTo(float x, float y, float z)
{
	mMovementManager->NavigateTo(this, x, y, z, MovementWalking);
}

void Mob::RunTo(float x, float y, float z)
{
	mMovementManager->NavigateTo(this, x, y, z, MovementRunning);
}

void Mob::NavigateTo(float x, float y, float z)
{
	if (IsRunning()) {
		RunTo(x, y, z);
	}
	else {
		WalkTo(x, y, z);
	}
}

void Mob::RotateTo(float new_heading, bool at_guardpoint)
{
	if (IsRunning()) {
		RotateToRunning(new_heading, at_guardpoint);
	}
	else {
		RotateToWalking(new_heading, at_guardpoint);
	}
}

void Mob::RotateToWalking(float new_heading, bool at_guardpoint)
{
	mMovementManager->RotateTo(this, new_heading, MovementWalking, at_guardpoint);
}

void Mob::RotateToRunning(float new_heading, bool at_guardpoint)
{
	mMovementManager->RotateTo(this, new_heading, MovementRunning, at_guardpoint);
}

void Mob::StopNavigation(float new_heading) {
	mMovementManager->StopNavigation(this, new_heading);
}

float Mob::SetRunAnimation(float speed)
{
	SetCurrentSpeed(speed);
	pRunAnimSpeed = static_cast<uint8>(speed * 10.0f);
	if (IsClient()) {
		animation = static_cast<int8>(speed * 10.0f);
	}

	return speed;
}

void NPC::AssignWaypoints(int32 grid_id, int start_wp)
{
	if (grid_id == 0)
		return; // grid ID 0 not supported

	if (grid_id < 0) {
		// Allow setting negative grid values for pausing pathing
		this->CastToNPC()->SetGrid(grid_id);
		return;
	}

	Waypoints.clear();
	roamer = false;

	auto grid_entry = GridRepository::GetGrid(zone->grids, grid_id);
	if (grid_entry.id == 0) {
		return;
	}

	wandertype = grid_entry.type;
	pausetype = grid_entry.type2;

	SetGrid(grid_id);	// Assign grid number

	roamer = true;
	max_wp = 0;	// Initialize it; will increment it for each waypoint successfully added to the list

	for (auto& entry : zone->grid_entries) {
		if (entry.gridid == grid_id) {
			wplist new_waypoint{};
			new_waypoint.index = max_wp;
			new_waypoint.x = entry.x;
			new_waypoint.y = entry.y;
			new_waypoint.z = entry.z;

		if(zone->HasMap() && RuleB(Map, FixPathingZWhenLoading) && !IsBoat())
		{
			auto positon = glm::vec3(new_waypoint.x, new_waypoint.y, new_waypoint.z);
			if(!RuleB(Watermap, CheckWaypointsInWaterWhenLoading) || !zone->HasWaterMap() ||
				(zone->HasWaterMap() && !zone->watermap->InWater(positon)))
			{
				glm::vec3 dest(new_waypoint.x, new_waypoint.y, new_waypoint.z);

				float newz = zone->zonemap->FindBestZ(dest, nullptr);

				if( (newz != BEST_Z_INVALID) && std::abs(newz+GetZOffset()-dest.z) < (RuleR(Map, FixPathingZMaxDeltaLoading)+GetZOffset()))
				{
					new_waypoint.z = SetBestZ(newz);
				}
			}
		}

		new_waypoint.pause = entry.pause;
		new_waypoint.heading = entry.heading;
		new_waypoint.centerpoint = entry.centerpoint;

		LogPathing(
			"Loading Grid [{}] number [{}] name [{}]",
			grid_id,
			entry.number,
			GetCleanName()
		);

		Waypoints.push_back(new_waypoint);
		max_wp++;
		}
	}

	if(Waypoints.size() < 2) {
		roamer = false;
	}

	InitializeGrid(start_wp);
}

void NPC::InitializeGrid(int start_wp)
{
	cur_wp = start_wp;
	UpdateWaypoint(start_wp);
	if (Mob::GetAppearance() != eaDead && wandertype != GridOneWayDepop)
		cur_wp_pause = 0;
	SetWaypointPause();
	AIloiter_timer->Stop();

	if (wandertype == GridRandomPath)
	{
		cur_wp = GetClosestWaypoint(glm::vec3(GetPosition()));
		patrol = cur_wp;
	}

	if (wandertype == GridRandom10 || wandertype == GridRandom || wandertype == GridRand5LoS) {
		CalculateNewWaypoint();
	}
}

void NPC::RemoveWaypoints()
{
	Waypoints.clear();
	roamer = false;
	grid = 0;
	wandertype = 0;
	pausetype = 0;
	patrol = 0;
	cur_wp_pause = 0;
	cur_wp = 0;
	AIwalking_timer->Disable();
}

void NPC::EditWaypoint(int wp, float x, float y, float z, float h, int pause, bool centerpoint)
{
	if (Waypoints.size() <= wp)
		return;

	Waypoints[wp].x = x;
	Waypoints[wp].y = y;
	Waypoints[wp].z = z;
	Waypoints[wp].heading = h;
	Waypoints[wp].pause = pause;
	Waypoints[wp].centerpoint = centerpoint;
}

void NPC::AddWaypoint(float x, float y, float z, float h, int pause, bool centerpoint)
{
	wplist newwp;
	if (Waypoints.size() == 0)
	{
		max_wp = 0;
		grid = 999999;
	}

	newwp.index = max_wp;
	newwp.x = x;
	newwp.y = y;
	newwp.z = z;
	newwp.pause = pause;
	newwp.heading = h;
	newwp.centerpoint = centerpoint;
	Waypoints.push_back(newwp);
	max_wp++;

	if (Waypoints.size() < 2)
		roamer = false;
	else
		roamer = true;

	if (Waypoints.size() == 1)
		InitializeGrid(0);
}

void Mob::SendTo(float new_x, float new_y, float new_z) {
	if(IsNPC()) {
		entity_list.ProcessMove(CastToNPC(), new_x, new_y, new_z);
	}

	m_Position.x = new_x;
	m_Position.y = new_y;
	m_Position.z = new_z;
	Log(Logs::Detail, Logs::AI, "Sent To (%.3f, %.3f, %.3f)", new_x, new_y, new_z);

	if(flymode == GravityBehavior::Flying)
		return;

	bool skip_z = false;
	if (IsNPC())
	{
		Spawn2* spawn = CastToNPC()->respawn2;
		if (spawn)
		{
			if (spawn->GetForceZ())
			{
				skip_z = true;
			}
		}
	}

	//fix up pathing Z, this shouldent be needed IF our waypoints
	//are corrected instead
	if(zone->HasMap() && RuleB(Map, FixPathingZOnSendTo) && !IsBoat() && !skip_z)
	{
		if(!RuleB(Watermap, CheckForWaterOnSendTo) || !zone->HasWaterMap() ||
			!zone->watermap->InWater(glm::vec3(m_Position)))
		{
			glm::vec3 dest(m_Position.x, m_Position.y, m_Position.z);

			float newz = zone->zonemap->FindBestZ(dest, nullptr);

			Log(Logs::Detail, Logs::AI, "BestZ returned %4.3f at %4.3f, %4.3f, %4.3f", newz,m_Position.x,m_Position.y,m_Position.z);

			if ((newz != BEST_Z_INVALID) &&
			    std::abs(newz - dest.z) < RuleR(Map, FixPathingZMaxDeltaSendTo)) // Sanity check.
			{
				m_Position.z = SetBestZ(newz);
			}
		}
	}
	else
		m_Position.z += 0.1;
}

void Mob::SendToFixZ(float new_x, float new_y, float new_z) {

	m_Position.x = new_x;
	m_Position.y = new_y;
	m_Position.z = new_z + 0.1f;

	if (IsNPC()) {
		entity_list.ProcessMove(CastToNPC(), m_Position.x, m_Position.y, m_Position.z);
	}

	if (zone->HasMap() && RuleB(Map, FixPathingZOnSendTo) && !IsBoat())
	{
		if (!RuleB(Watermap, CheckForWaterOnSendTo) || !zone->HasWaterMap() ||
			!zone->watermap->InWater(glm::vec3(m_Position)))
		{
			glm::vec3 dest(m_Position.x, m_Position.y, m_Position.z);

			float newz = zone->zonemap->FindBestZ(dest, nullptr);

			if ((newz > -2000) && std::abs(newz - dest.z) < RuleR(Map, FixPathingZMaxDeltaSendTo)) // Sanity check.
				m_Position.z = newz + z_offset;
		}
	}
}

float Mob::GetFixedZ(const glm::vec3& destination, float z_find_offset) {
	BenchTimer timer;
	timer.reset();

	float new_z = destination.z;

	if (zone->HasMap()) {

		if (!IsClient() && (flymode == GravityBehavior::Flying || flymode == GravityBehavior::Levitating)) {
			return new_z;
		}

		if (zone->HasWaterMap() && zone->watermap->InLiquid(destination) || zone->IsWaterZone(destination.z))
			return new_z;

		/*
		 * Any more than 5 in the offset makes NPC's hop/snap to ceiling in small corridors
		 */
		glm::vec3 dest(destination.x, destination.y, destination.z);
		if (IsClient()) {
			new_z = zone->zonemap->FindBestZ(dest, nullptr);
		}
		else {
			new_z = zone->zonemap->FindBestZ(dest, nullptr, 20.0f);
			if (new_z == BEST_Z_INVALID)
				new_z = zone->zonemap->FindBestZ(dest, nullptr);
		}
		if (new_z != BEST_Z_INVALID) {
			new_z += this->GetZOffset();

			if (new_z < -2000.0f) {
				new_z = destination.z;
			}
			if (zone->HasWaterMap() && zone->watermap->InLiquid(glm::vec3(destination.x, destination.y, new_z)) || zone->IsWaterZone(new_z))
				new_z = destination.z;
		}
		else {
			new_z = destination.z;
		}

		auto duration = timer.elapsed();

		LogFixZ("[{}] returned [{}] at [{}] [{}] [{}] - Took [{}]",
			GetCleanName(),
			new_z,
			destination.x,
			destination.y,
			destination.z,
			duration);

	}

	return new_z;
}

void Mob::FixZ(bool force) {
	bool NPCFlyMode = false;
	float max_fix_z = 20.0f + GetZOffset();

	NPCFlyMode = (CastToNPC()->GetFlyMode() == 1 || CastToNPC()->GetFlyMode() == 2);
	bool underwater_mob = IsNPC() && (CastToNPC()->IsUnderwaterOnly() || zone->IsWaterZone(m_Position.z)
		|| (zone->HasWaterMap() && (zone->watermap->InLiquid(glm::vec3(m_Position.x, m_Position.y, m_Position.z)))));
	if (!underwater_mob && !NPCFlyMode && !IsBoat() && zone->HasMap()) {
		glm::vec3 dest(m_Position.x, m_Position.y, m_Position.z);
		float newz = zone->zonemap->FindBestZ(dest, nullptr, 20.0f, GetZOffset());
		if (newz == BEST_Z_INVALID)
			newz = zone->zonemap->FindBestZ(dest, nullptr);

		if (!zone->HasWaterMap()) {
			// no water map
			//Log(Logs::Detail, Logs::AI, "BestZ returned %4.3f at %4.3f, %4.3f, %4.3f", newz, m_Position.x, m_Position.y, m_Position.z);
			if (newz != BEST_Z_INVALID) {
				newz = SetBestZ(newz);
				if (force || std::abs(newz - dest.z) < max_fix_z || best_z_fail_count > 1) {
					m_Position.z = newz;
					best_z_fail_count = 0;
				}
				else {
					best_z_fail_count++;
				}
			}
		}
		else {
			if (newz != BEST_Z_INVALID) {
				newz = SetBestZ(newz);
				glm::vec3 new_loc(m_Position.x, m_Position.y, newz);
				bool in_liquid = zone->HasWaterMap() && zone->watermap->InLiquid(new_loc) || zone->IsWaterZone(newz);
				if (!in_liquid) {
					// new position is not in water
					if (force || std::abs(newz - dest.z) < max_fix_z || (zone->watermap->InLiquid(dest) || zone->IsWaterZone(dest.z)) || best_z_fail_count > 1) {
						m_Position.z = newz;
						best_z_fail_count = 0;
					}
					else {
						if (std::abs(newz - dest.z) > max_fix_z) {
							best_z_fail_count++;
						}
					}
				}
				else {
					// new position is in water
					glm::vec3 cur_loc(m_Position.x, m_Position.y, m_Position.z);
					bool in_liquid = zone->HasWaterMap() && zone->watermap->InLiquid(cur_loc) || zone->IsWaterZone(cur_loc.z);
					if (zone->zonemap->CheckLoS(cur_loc, new_loc) && !in_liquid) {
						// transition into water, and have LOS to new position
						if (cur_loc.z - new_loc.z > 10.0f) {
							// try smaller change
							for (int i = 0; i < 10 && cur_loc.z > new_loc.z; i++) {
								cur_loc.z -= 2.0f;
								if (zone->watermap->InLiquid(cur_loc) || zone->IsWaterZone(cur_loc.z)) {
									m_Position.z = cur_loc.z;
									best_z_fail_count = 0;
									break;
								}
							}
						}
						FixZInWater();
					}
					else {
						// no LOS to new position
						newz = zone->zonemap->FindClosestZ(cur_loc, nullptr, GetZOffset());
						if (newz != BEST_Z_INVALID) {
							newz = SetBestZ(newz);
							if (force || best_z_fail_count > 1 || std::abs(newz - dest.z) < (2.0f * GetZOffset())) {
								m_Position.z = newz;
								best_z_fail_count = 0;
							}
							else {
								best_z_fail_count++;
							}
						}
						FixZInWater();
					}
				}
			}
		}
	}
	else if (underwater_mob) {
		FixZInWater();
	}
}

void Mob::FixZInWater() {
	bool NPCFlyMode = false;
	bool underwater_mob = IsNPC() && CastToNPC()->IsUnderwaterOnly();
	if (!IsBoat() && zone->HasMap()) {
		if (underwater_mob || zone->HasWaterMap() && (zone->watermap->InLiquid(glm::vec3(m_Position.x, m_Position.y, m_Position.z)) || zone->IsWaterZone(m_Position.z))) {
			// in water
			glm::vec3 dest(m_Position.x, m_Position.y, m_Position.z);
			float ceiling = zone->zonemap->FindCeiling(dest, nullptr);
			float ground = zone->zonemap->FindGround(dest, nullptr);

			if (ground != BEST_Z_INVALID && m_Position.z < (ground + GetZOffset()))
				m_Position.z = ground + GetZOffset();
			if (ceiling != BEST_Z_INVALID && m_Position.z > ceiling) {
				m_Position.z = ceiling - 1.0f;
			}
		}
	}
}

int	ZoneDatabase::GetHighestGrid(uint32 zoneid) {

	std::string query = StringFormat("SELECT COALESCE(MAX(id), 0) FROM grid WHERE zoneid = %i", zoneid);
	auto results = QueryDatabase(query);
    if (!results.Success()) {
        return 0;
    }

	if (results.RowCount() != 1)
		return 0;

	auto row = results.begin();
	return atoi(row[0]);
}

uint8 ZoneDatabase::GetGridType2(uint32 grid, uint16 zoneid) {

	int type2 = 0;
	std::string query = StringFormat("SELECT type2 FROM grid WHERE id = %i AND zoneid = %i", grid, zoneid);
	auto results = QueryDatabase(query);
    if (!results.Success()) {
        return 0;
    }

	if (results.RowCount() != 1)
		return 0;

	auto row = results.begin();

	return atoi(row[0]);
}

bool ZoneDatabase::GetWaypoints(uint32 grid, uint16 zoneid, uint32 num, wplist* wp) {

	if (wp == nullptr)
		return false;

	std::string query = StringFormat("SELECT x, y, z, pause, heading FROM grid_entries "
                                    "WHERE gridid = %i AND number = %i AND zoneid = %i", grid, num, zoneid);
    auto results = QueryDatabase(query);
    if (!results.Success()) {
        return false;
    }

	if (results.RowCount() != 1)
		return false;

	auto row = results.begin();

	wp->x = atof(row[0]);
	wp->y = atof(row[1]);
	wp->z = atof(row[2]);
	wp->pause = atoi(row[3]);
	wp->heading = atof(row[4]);

	return true;
}

void ZoneDatabase::AssignGrid(Client *client, int grid, int spawn2id) {
	std::string query = StringFormat("UPDATE spawn2 SET pathgrid = %d WHERE id = %d", grid, spawn2id);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return;
	
	if (results.RowsAffected() != 1) {
		return;
	}

	client->Message(Chat::White, "Grid assign: spawn2 id = %d updated", spawn2id);
}


void ZoneDatabase::ModifyGrid(
	Client* c,
	bool remove,
	uint32 grid_id,
	uint8 type,
	uint8 type2,
	uint32 zone_id
)
{
	if (!remove) {
		GridRepository::InsertOne(
			*this,
			GridRepository::Grid{
				.id = static_cast<int32_t>(grid_id),
				.zoneid = static_cast<int32_t>(zone_id),
				.type = type,
				.type2 = type2
			}
		);

		return;
	}

	GridRepository::DeleteWhere(
		*this,
		fmt::format(
			"`id` = {} AND `zoneid` = {}",
			grid_id,
			zone_id
		)
	);

	GridEntriesRepository::DeleteWhere(
		*this,
		fmt::format(
			"`gridid` = {} AND `zoneid` = {}",
			grid_id,
			zone_id
		)
	);
}


bool ZoneDatabase::GridExistsInZone(uint32 zone_id, uint32 grid_id)
{
	const auto& l = GridRepository::GetWhere(
		*this,
		fmt::format(
			"`id` = {} AND `zoneid` = {}",
			grid_id,
			zone_id
		)
	);

	if (l.empty()) {
		return false;
	}

	return true;
}

/**************************************
* AddWP - Adds a new waypoint to a specific grid for a specific zone.
*/
void ZoneDatabase::AddWP(Client *client, uint32 gridid, uint32 wpnum, const glm::vec4& position, uint32 pause, uint16 zoneid)
{
	std::string query = StringFormat("INSERT INTO grid_entries (gridid, zoneid, `number`, x, y, z, pause, heading) "
									"VALUES (%i, %i, %i, %f, %f, %f, %i, %f)",
									gridid, zoneid, wpnum, position.x, position.y, position.z, pause, position.w);
	auto results = QueryDatabase(query);
	if (!results.Success()) {
		return;
	}

}


/**********
* ModifyWP() has been obsoleted. The #wp command either uses AddWP() or DeleteWaypoint()
***********/

/******************
* DeleteWaypoint - Removes a specific waypoint from the grid
*	grid_id:	The ID number of the grid whose wp is being deleted
*	wp_num:		The number of the waypoint being deleted
*	zoneid:		The ID number of the zone that contains the waypoint being deleted
*/
void ZoneDatabase::DeleteWaypoint(Client *client, uint32 grid_num, uint32 wp_num, uint16 zoneid)
{
	std::string query = StringFormat("DELETE FROM grid_entries WHERE "
									"gridid = %i AND zoneid = %i AND `number` = %i",
									grid_num, zoneid, wp_num);
	auto results = QueryDatabase(query);
	if(!results.Success()) {
		return;
	}

}


/******************
* AddWPForSpawn - Used by the #wpadd command - for a given spawn, this will add a new waypoint to whatever grid that spawn is assigned to.
* If there is currently no grid assigned to the spawn, a new grid will be created using the next available Grid ID number for the zone
* the spawn is in.
* Returns 0 if the function didn't have to create a new grid. If the function had to create a new grid for the spawn, then the ID of
* the created grid is returned.
*/
uint32 ZoneDatabase::AddWPForSpawn(Client *client, uint32 spawn2id, const glm::vec4& position, uint32 pause, int type1, int type2, uint16 zoneid) {

	uint32 grid_num;	 // The grid number the spawn is assigned to (if spawn has no grid, will be the grid number we end up creating)
	uint32 next_wp_num;	 // The waypoint number we should be assigning to the new waypoint
	bool createdNewGrid; // Did we create a new grid in this function?

	// See what grid number our spawn is assigned
	std::string query = StringFormat("SELECT pathgrid FROM spawn2 WHERE id = %i", spawn2id);
	auto results = QueryDatabase(query);
	if (!results.Success()) {
		// Query error
		return 0;
	}

	if (results.RowCount() == 0)
		return 0;

	auto row = results.begin();
	grid_num = atoi(row[0]);

	if (grid_num == 0)
	{ // Our spawn doesn't have a grid assigned to it -- we need to create a new grid and assign it to the spawn
		createdNewGrid = true;
		grid_num = GetFreeGrid(zoneid);
		if(grid_num == 0)	// There are no grids for the current zone -- create Grid #1
			grid_num = 1;

		query = StringFormat("INSERT INTO grid SET id = '%i', zoneid = %i, type ='%i', type2 = '%i'",
							grid_num, zoneid, type1, type2);
		QueryDatabase(query);

		query = StringFormat("UPDATE spawn2 SET pathgrid = '%i' WHERE id = '%i'", grid_num, spawn2id);
		QueryDatabase(query);

	}
	else	// NPC had a grid assigned to it
		createdNewGrid = false;

	// Find out what the next waypoint is for this grid
	query = StringFormat("SELECT max(`number`) FROM grid_entries WHERE zoneid = '%i' AND gridid = '%i'", zoneid, grid_num);

	results = QueryDatabase(query);
	if(!results.Success()) { // Query error
		return 0;
	}

	row = results.begin();
	if(row[0] != 0)
		next_wp_num = atoi(row[0]) + 1;
	else	// No waypoints in this grid yet
		next_wp_num = 1;

	query = StringFormat("INSERT INTO grid_entries(gridid, zoneid, `number`, x, y, z, pause, heading) "
						"VALUES (%i, %i, %i, %f, %f, %f, %i, %f)",
						grid_num, zoneid, next_wp_num, position.x, position.y, position.z, pause, position.w);
	results = QueryDatabase(query);

	return createdNewGrid? grid_num: 0;
}

uint32 ZoneDatabase::GetFreeGrid(uint16 zoneid) {

	std::string query = StringFormat("SELECT max(id) FROM grid WHERE zoneid = %i", zoneid);
	auto results = QueryDatabase(query);
	if (!results.Success()) {
        return 0;
	}

	if (results.RowCount() != 1)
		return 0;

    auto row = results.begin();
	uint32 freeGridID = row[0] ? atoi(row[0]) + 1 : 1;

    return freeGridID;
}

int ZoneDatabase::GetHighestWaypoint(uint32 zoneid, uint32 gridid) {

	std::string query = StringFormat("SELECT COALESCE(MAX(number), 0) FROM grid_entries "
									"WHERE zoneid = %i AND gridid = %i", zoneid, gridid);
	auto results = QueryDatabase(query);
	if (!results.Success()) {
        return 0;
	}

	if (results.RowCount() != 1)
		return 0;

	auto row = results.begin();
	return atoi(row[0]);
}

int ZoneDatabase::GetRandomWaypointLocFromGrid(glm::vec4 &loc, uint16 zoneid, int grid)
{
	loc.x = loc.y = loc.z = loc.w = 0.0f;

	// check grid cache before hitting DB
	auto grid_entry = GridRepository::GetGrid(zone->grids, grid);
	if (grid_entry.id != 0)
	{
		int count = -1, max_wp = 0, first = 0, last = 0;

		for (auto& entry : zone->grid_entries)
		{
			count++;
			if (entry.gridid == grid)
			{
				max_wp++;
				last = count;
				if (!first)
					first = count;
			}
		}

		int roll = zone->random.Int(1, max_wp);
		count = 0;

		auto it = zone->grid_entries.begin() + first;
		auto end = zone->grid_entries.end() - (zone->grid_entries.size() - last - 1);
		for (; it < end; it++)
		{
			if ((*it).gridid == grid)
			{
				count++;
				if (count == roll)
				{
					loc.x = (*it).x;
					loc.y = (*it).y;
					loc.z = (*it).z;
					loc.w = (*it).heading;
					break;
				}
			}
		}

		if (loc.x || loc.y || loc.z || loc.w)
			return roll - 1;
	}

	std::string query = StringFormat("SELECT `x`,`y`,`z`,`heading` "
		"FROM grid_entries WHERE `gridid` = %i AND `zoneid` = %u ORDER BY `number`", grid, zone->GetZoneID());
	auto results = database.QueryDatabase(query);
	if (!results.Success()) {
		Log(Logs::General, Logs::Error, "MySQL Error while trying get random waypoint loc from grid %i in zoneid %u;  %s", grid, zoneid, results.ErrorMessage().c_str());
		return 0;
	}

	if (results.RowCount() > 0)
	{
		int roll = zone->random.Int(0, results.RowCount() - 1);
		int i = 0;
		auto row = results.begin();
		while (i < roll)
		{
			row++;
			i++;
		}
		loc.x = atof(row[0]);
		loc.y = atof(row[1]);
		loc.z = atof(row[2]);
		loc.w = atof(row[3]);
		return i;
	}
	return 0;
}

void NPC::SaveGuardSpotCharm()
{
	m_GuardPointSaved = m_GuardPoint;
}

void NPC::RestoreGuardSpotCharm()
{
	m_GuardPoint = m_GuardPointSaved;
}

void NPC::SetSpawnPoint(float x, float y, float z, float h)
{
	m_SpawnPoint.x = x;
	m_SpawnPoint.y = y;
	m_SpawnPoint.z = z;
	m_SpawnPoint.w = FixHeading(h);
}
