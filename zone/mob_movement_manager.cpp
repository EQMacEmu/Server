#include "mob_movement_manager.h"
#include "client.h"
#include "mob.h"
#include "zone.h"
#include "position.h"
#include "water_map.h"
#include "../common/eq_packet_structs.h"
#include "../common/misc_functions.h"
#include "../common/data_verification.h"
#include "../common/fastmath.h"
#include "pathfinder_interface.h"
#include <vector>
#include <deque>
#include <map>
#include <stdlib.h>

extern double frame_time;
extern Zone   *zone;
extern FastMath g_Math;

class IMovementCommand {
public:
	IMovementCommand() = default;
	virtual ~IMovementCommand() = default;
	virtual bool Process(MobMovementManager *mob_movement_manager, Mob *mob) = 0;
	virtual bool Started() const = 0;
	virtual glm::vec3 CurrentDest() = 0;
};

class RotateToCommand : public IMovementCommand {
public:
	RotateToCommand(double rotate_to, double dir, MobMovementMode mob_movement_mode, bool at_guardpoint)
	{
		m_rotate_to      = rotate_to;
		m_rotate_to_dir  = dir;
		m_rotate_to_mode = mob_movement_mode;
		m_started        = false;
		m_at_guardpoint  = at_guardpoint;
	}

	virtual ~RotateToCommand()
	{

	}

	glm::vec3 CurrentDest() { return glm::vec3(0.0f); }

	virtual bool Process(MobMovementManager *mob_movement_manager, Mob *mob)
	{
		if (!mob->IsAIControlled()) {
			return true;
		}

		if (m_rotate_to_mode == MovementRunning) {
			mob->SetHeading(FixHeading(m_rotate_to));
			mob->SetRunAnimation(0.0f);
			mob->SetDelta(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			mob->SetMoving(false);
			mob->SetMoved(false);
			mob->SendRealPosition();
			mob->SendPosUpdate(2);
			if (m_at_guardpoint) {
				mob->ClearBestZCount();
				mob->AI_SetLoiterTimer();
				mob->RemoveFeignedFromHateList();
				if (mob->IsNPC())
					mob->SetAppearance(mob->CastToNPC()->GetGuardPointAnim());
			}
			return true;
		}

		auto rotate_to_speed =  12.0f;

		auto from = FixHeading(mob->GetHeading());
		auto to   = FixHeading(m_rotate_to);
		auto diff = to - from;

		while (diff < 0.0f) {
			diff += 256.0f;
		}

		while (diff >= 256.0f) {
			diff -= 256.0f;
		}

		auto dist = std::abs(diff);

		if (!m_started) {
			m_started = true;
			mob->SetRunAnimation(0.0f);
			if (dist > 13.0f && rotate_to_speed > 0.0f && rotate_to_speed <= 25.0f) { //send basic rotation
				mob->SetDelta(glm::vec4(0.0f, 0.0f, 0.0f, static_cast<float>(m_rotate_to_dir * rotate_to_speed)));
				mob->SendPosUpdate();
				mob->SendPosUpdate(2);
				mob->SetMoving(true);
				mob->SetMoved(false);
				return false;
			}
			else {
				mob->SetHeading(to);
				mob->SetRunAnimation(0.0f);
				mob->SetDelta(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
				mob->SetMoving(false);
				mob->SetMoved(false);
				mob->SendRealPosition();
				mob->SendPosUpdate(2);
				if (m_at_guardpoint) {
					mob->ClearBestZCount();
					mob->AI_SetLoiterTimer();
					mob->RemoveFeignedFromHateList();
					if (mob->IsNPC())
						mob->SetAppearance(mob->CastToNPC()->GetGuardPointAnim());
				}
				return true;
			}
		}

		auto td = rotate_to_speed * 10.0f * frame_time;

		if (td >= dist) {
			mob->SetRunAnimation(0.0f);
			mob->SetDelta(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			mob->SetHeading(to);
			mob->SetMoving(false);
			mob->SetMoved(false);
			mob->SendRealPosition();
			mob->SendPosUpdate(2);
			if (m_at_guardpoint) {
				mob->ClearBestZCount();
				mob->AI_SetLoiterTimer();
				mob->RemoveFeignedFromHateList();
				if (mob->IsNPC())
					mob->SetAppearance(mob->CastToNPC()->GetGuardPointAnim());
			}
			return true;
		}

		from += td * m_rotate_to_dir;
		mob->SetMoved(false);
		mob->SetHeading(FixHeading(from));
		return false;
	}

	virtual bool Started() const
	{
		return m_started;
	}

private:
	double          m_rotate_to;
	double          m_rotate_to_dir;
	MobMovementMode m_rotate_to_mode;
	bool            m_started;
	bool			m_at_guardpoint;
};

class MoveToCommand : public IMovementCommand {
public:
	MoveToCommand(float x, float y, float z, MobMovementMode mob_movement_mode)
	{
		m_distance_moved_since_correction = 0.0;
		m_move_to_x                       = x;
		m_move_to_y                       = y;
		m_move_to_z                       = z;
		m_move_to_mode                    = mob_movement_mode;
		m_last_sent_time                  = 0.0;
		m_last_sent_speed                 = 0;
		m_started                         = false;
		m_total_h_dist                    = 0.0;
		m_total_v_dist                    = 0.0;
		m_total_h_dist_moved              = 0.0;
	}

	virtual ~MoveToCommand()
	{

	}

	glm::vec3 CurrentDest() {
		return glm::vec3((float)m_move_to_x, (float)m_move_to_y, (float)m_move_to_z); }

	/**
	 * @param mob_movement_manager
	 * @param mob
	 * @return
	 */
	virtual bool Process(MobMovementManager *mob_movement_manager, Mob *mob)
	{
		if (!mob->IsAIControlled()) {
			return true;
		}

		//Send a movement packet when you start moving		
		double current_time  = static_cast<double>(Timer::GetCurrentTime()) / 1000.0;
		float  current_float_speed = 0.0f;
		int current_speed = 0;
		float mob_speed = mob->GetCurrentSpeed();
		bool need_update = false;

		if (m_move_to_mode == MovementRunning) {
			if (mob->IsFeared()) {
				current_float_speed = mob->GetFearSpeed();
				current_speed = mob->GetIntFearSpeed();
			}
			else {
				current_float_speed = mob->GetRunspeed();
				current_speed = mob->GetIntRunspeed();
			}
		}
		else {
			current_float_speed = mob->GetWalkspeed();
			current_speed = mob->GetIntWalkspeed();
		}
		if (current_float_speed < 0.1f) {
			current_float_speed = 0.0f;
			current_speed = 0;
		}

		mob->SetRunAnimation(current_float_speed);

		if (mob->IsClient())
			current_speed *= 2;

		auto      &p = mob->GetPosition();
		glm::vec2 tar(m_move_to_x, m_move_to_y);
		glm::vec2 pos(p.x, p.y);
		double    len = glm::distance(pos, tar);

		if (!m_started) {
			m_started = true;
			bool currently_moving = mob->IsMoving();
			//rotate to the point
			if (current_speed > 0)
				mob->SetMoving(true);
			float cur_head = mob->GetHeading();
			mob->SetHeading(mob->CalculateHeadingToTarget(m_move_to_x, m_move_to_y));
			glm::vec2 dir = tar - pos;
			m_ndir = glm::normalize(dir);
			m_total_h_dist    = DistanceNoZ(mob->GetPosition(), glm::vec4(m_move_to_x, m_move_to_y, 0.0f, 0.0f));
			m_total_v_dist    = m_move_to_z - mob->GetZ();
			m_last_sent_speed = current_speed;
			if (!currently_moving || (currently_moving && current_speed == 0) || cur_head != mob->GetHeading()) {
				m_last_sent_time = current_time;
				if (RuleB(Map, FixZWhenPathing)) {
					m_distance_moved_since_correction = 0.0;
					mob->FixZ();
				}
				mob->SendPosUpdate();
				mob->SendPosUpdate(2);
				return false;
			}
		}
		if (m_last_sent_speed > 0.0f)
			mob->SetMoved(true);

		double    distance_moved = frame_time * static_cast<double>(m_last_sent_speed) * 0.4f * 1.45f;

		//When speed changes
		if (current_speed != m_last_sent_speed || mob_speed != current_float_speed) {
			need_update = true;
		}
		m_total_h_dist_moved += distance_moved;
		if (distance_moved >= len || m_total_h_dist_moved > m_total_h_dist) {
			if (mob->IsNPC()) {
				entity_list.ProcessMove(mob->CastToNPC(), m_move_to_x, m_move_to_y, m_move_to_z);
			}
			auto vec = glm::vec4(m_move_to_x, m_move_to_y, m_move_to_z, mob->GetHeading());
			mob->SetPosition(vec);
			if (RuleB(Map, FixZWhenPathing)) {
				m_distance_moved_since_correction = 0.0;
				mob->FixZ();
			}
			mob->SendPosUpdate();
			mob->SendPosUpdate(2);
			return true;
		}
		else {
			glm::vec2 npos = pos + (m_ndir * static_cast<float>(distance_moved));
			len -= distance_moved;
			double total_distance_traveled = m_total_h_dist - len;
			// z remaining to move
			
			double h_remaining = m_total_h_dist - total_distance_traveled;
			double z_at_pos = mob->GetZ();
			if (h_remaining > 0) {
				double v_remaining = m_move_to_z - mob->GetZ();
				z_at_pos += v_remaining * (distance_moved / h_remaining);
			}

			if (mob->IsNPC()) {
				entity_list.ProcessMove(mob->CastToNPC(), npos.x, npos.y, z_at_pos);
			}
			auto vec = glm::vec4(npos.x, npos.y, z_at_pos, mob->GetHeading());
			mob->SetPosition(vec);
			if (RuleB(Map, FixZWhenPathing)) {
				m_distance_moved_since_correction += distance_moved;
			}
		}
		//If x seconds have passed without sending an update.
		if ((m_distance_moved_since_correction > 0.0 || mob->IsClient()) && ((current_time - m_last_sent_time) >= 2.0))
			need_update = true;

		if ((need_update) || (m_distance_moved_since_correction > 10.0)) {
			if (RuleB(Map, FixZWhenPathing)) {
				m_distance_moved_since_correction = 0.0;
				mob->FixZ();
			}
			m_last_sent_speed = current_speed;
			m_last_sent_time = current_time;
			mob->SendPosUpdate();
			mob->SendPosUpdate(2);
		}

		return false;
	}

	virtual bool Started() const
	{
		return m_started;
	}

protected:
	double          m_distance_moved_since_correction;
	double          m_move_to_x;
	double          m_move_to_y;
	double          m_move_to_z;
	MobMovementMode m_move_to_mode;
	bool            m_started;
	glm::vec2		m_ndir;

	double m_last_sent_time;
	int    m_last_sent_speed;
	double m_total_h_dist;
	double m_total_v_dist;
	double m_total_h_dist_moved;
};

class FlyToCommand : public IMovementCommand {
public:
	FlyToCommand(float x, float y, float z, MobMovementMode mob_movement_mode)
	{
		m_distance_moved_since_correction = 0.0;
		m_move_to_x                       = x;
		m_move_to_y                       = y;
		m_move_to_z                       = z;
		m_move_to_mode                    = mob_movement_mode;
		m_last_sent_time                  = 0.0;
		m_last_sent_speed                 = 0;
		m_started                         = false;
		m_total_h_dist                    = 0.0;
		m_total_v_dist                    = 0.0;
		m_total_h_dist_moved = 0.0;
	}

	virtual ~FlyToCommand()
	{

	}

	glm::vec3 CurrentDest() { return glm::vec3((float)m_move_to_x, (float)m_move_to_y, (float)m_move_to_z); }

	/**
	 * @param mob_movement_manager
	 * @param mob
	 * @return
	 */
	virtual bool Process(MobMovementManager *mob_movement_manager, Mob *mob)
	{
		if (!mob->IsAIControlled()) {
			return true;
		}

		//Send a movement packet when you start moving
		double current_time  = static_cast<double>(Timer::GetCurrentTime()) / 1000.0;
		float    current_float_speed = 0.0f;
		int current_speed = 0;
		float mob_speed = mob->GetCurrentSpeed();

		if (m_move_to_mode == MovementRunning) {
			if (mob->IsFeared()) {
				current_float_speed = mob->GetFearSpeed();
				current_speed = mob->GetIntFearSpeed();
			}
			else {
				current_float_speed = mob->GetRunspeed();
				current_speed = mob->GetIntRunspeed();
			}
		}
		else {
			current_float_speed = mob->GetWalkspeed();
			current_speed = mob->GetIntWalkspeed();
		}

		if (current_float_speed < 0.1f) {
			current_float_speed = 0.0f;
			current_speed = 0;
		}

		mob->SetRunAnimation(current_float_speed);

		if (mob->IsClient())
			current_speed *= 2;

		if (!m_started) {
			m_started = true;
			//rotate to the point
			if (current_speed > 0)
				mob->SetMoving(true);
			mob->SetHeading(mob->CalculateHeadingToTarget(m_move_to_x, m_move_to_y));

			m_last_sent_speed = current_speed;
			m_last_sent_time  = current_time;
			m_total_h_dist    = DistanceNoZ(mob->GetPosition(), glm::vec4(m_move_to_x, m_move_to_y, 0.0f, 0.0f));
			m_total_v_dist    = m_move_to_z - mob->GetZ();
			mob->SendPosUpdate();
			mob->SendPosUpdate(2);
			return false;
		}

		//When speed changes
		if (current_speed != m_last_sent_speed || mob_speed != current_float_speed) {
			m_distance_moved_since_correction = 0.0;
			m_last_sent_speed = current_speed;
			m_last_sent_time  = current_time;
			mob->SendPosUpdate();
			mob->SendPosUpdate(2);
		}

		bool need_update = false;
		auto      &p  = mob->GetPosition();
		glm::vec2 tar(m_move_to_x, m_move_to_y);
		glm::vec2 pos(p.x, p.y);
		double    len = glm::distance(pos, tar);
		if (current_speed > 0)
			mob->SetMoved(true);

		glm::vec2 dir            = tar - pos;
		glm::vec2 ndir           = glm::normalize(dir);
		double    distance_moved = frame_time * current_speed * 0.4f * 1.45f;

		m_total_h_dist_moved += distance_moved;
		if (distance_moved >= len || m_total_h_dist_moved > m_total_h_dist) {
			if (mob->IsNPC()) {
				entity_list.ProcessMove(mob->CastToNPC(), m_move_to_x, m_move_to_y, m_move_to_z);
			}
			auto vec = glm::vec4(m_move_to_x, m_move_to_y, m_move_to_z, mob->GetHeading());
			mob->SetPosition(vec);
			mob->SendPosUpdate();
			mob->SendPosUpdate(2);
			return true;
		}
		else {
			glm::vec2 npos = pos + (ndir * static_cast<float>(distance_moved));

			len -= distance_moved;
			double total_distance_traveled = m_total_h_dist - len;
			double start_z                 = m_move_to_z - m_total_v_dist;
			double z_at_pos                = start_z + (m_total_v_dist * (total_distance_traveled / m_total_h_dist));

			if (mob->IsNPC()) {
				entity_list.ProcessMove(mob->CastToNPC(), npos.x, npos.y, z_at_pos);
			}
			auto vec = glm::vec4(npos.x, npos.y, z_at_pos, mob->GetHeading());
			mob->SetPosition(vec);
			m_distance_moved_since_correction += distance_moved;
		}
		//If x seconds have passed without sending an update.
		if ((current_time - m_last_sent_time >= 2.0) && (mob->IsClient() || (m_distance_moved_since_correction > 0.0))) {
			m_distance_moved_since_correction = 0.0;
			m_last_sent_speed = current_speed;
			m_last_sent_time = current_time;
			mob->SendPosUpdate();
			mob->SendPosUpdate(2);
		}

		return false;
	}

	virtual bool Started() const
	{
		return m_started;
	}

protected:
	double          m_distance_moved_since_correction;
	double          m_move_to_x;
	double          m_move_to_y;
	double          m_move_to_z;
	MobMovementMode m_move_to_mode;
	bool            m_started;

	double m_last_sent_time;
	int    m_last_sent_speed;
	double m_total_h_dist;
	double m_total_v_dist;
	double m_total_h_dist_moved;
};

class SwimToCommand : public MoveToCommand {
public:
	SwimToCommand(float x, float y, float z, MobMovementMode mob_movement_mode) : MoveToCommand(x, y, z, mob_movement_mode)
	{
		m_distance_moved_since_correction = 0.0;
		m_move_to_x = x;
		m_move_to_y = y;
		m_move_to_z = z;
		m_move_to_mode = mob_movement_mode;
		m_last_sent_time = 0.0;
		m_last_sent_speed = 0;
		m_started = false;
		m_total_h_dist = 0.0;
		m_total_v_dist = 0.0;
		m_total_h_dist_moved = 0.0;
	}

	glm::vec3 CurrentDest() { return glm::vec3((float)m_move_to_x, (float)m_move_to_y, (float)m_move_to_z); }

	virtual bool Process(MobMovementManager *mob_movement_manager, Mob *mob)
	{
		if (!mob->IsAIControlled()) {
			return true;
		}

		//Send a movement packet when you start moving		
		double current_time = static_cast<double>(Timer::GetCurrentTime()) / 1000.0;
		float  current_float_speed = 0;
		float mob_speed = mob->GetCurrentSpeed();
		int current_speed = 0;
		bool need_update = false;

		if (m_move_to_mode == MovementRunning) {
			if (mob->IsFeared()) {
				current_float_speed = mob->GetFearSpeed();
				current_speed = mob->GetIntFearSpeed();
			}
			else {
				current_float_speed = mob->GetRunspeed();
				current_speed = mob->GetIntRunspeed();
			}
		}
		else {
			current_float_speed = mob->GetWalkspeed();
			current_speed = mob->GetIntWalkspeed();
		}

		if (current_float_speed < 0.1f) {
			current_float_speed = 0.0f;
			current_speed = 0;
		}

		mob->SetRunAnimation(current_float_speed);

		if (mob->IsClient())
			current_speed *= 2;

		auto      &p = mob->GetPosition();
		glm::vec2 tar(m_move_to_x, m_move_to_y);
		glm::vec2 pos(p.x, p.y);
		double    len = glm::distance(pos, tar);

		if (!m_started) {
			m_started = true;
			bool currently_moving = mob->IsMoving();
			//rotate to the point
			if (current_speed > 0)
				mob->SetMoving(true);
			mob->SetHeading(mob->CalculateHeadingToTarget(m_move_to_x, m_move_to_y));

			glm::vec2 dir = tar - pos;
			m_ndir = glm::normalize(dir);

			m_total_h_dist = DistanceNoZ(mob->GetPosition(), glm::vec4(m_move_to_x, m_move_to_y, 0.0f, 0.0f));
			m_total_v_dist = m_move_to_z - mob->GetZ();
			m_last_sent_speed = current_speed;
			if (!currently_moving || (currently_moving && current_speed == 0)) {
				m_last_sent_time = current_time;
				mob->SendPosUpdate();
				mob->SendPosUpdate(2);
				return false;
			}
		}
		if (m_last_sent_speed > 0)
			mob->SetMoved(true);

		double    distance_moved = frame_time * static_cast<double>(m_last_sent_speed) * 0.4f * 1.45f;

		//When speed changes
		if (current_speed != m_last_sent_speed || mob_speed != current_float_speed) {
			need_update = true;
		}
		m_total_h_dist_moved += distance_moved;
		if (distance_moved >= len || m_total_h_dist_moved > m_total_h_dist) {
			if (mob->IsNPC()) {
				entity_list.ProcessMove(mob->CastToNPC(), m_move_to_x, m_move_to_y, m_move_to_z);
			}
			auto vec = glm::vec4(m_move_to_x, m_move_to_y, m_move_to_z, mob->GetHeading());
			mob->SetPosition(vec);
			mob->SendPosUpdate();
			mob->SendPosUpdate(2);
			return true;
		}
		else {
			glm::vec2 npos = pos + (m_ndir * static_cast<float>(distance_moved));

			len -= distance_moved;
			double total_distance_traveled = m_total_h_dist - len;
			double start_z = m_move_to_z - m_total_v_dist;
			double z_at_pos = start_z + (m_total_v_dist * (total_distance_traveled / m_total_h_dist));

			if (mob->IsNPC()) {
				entity_list.ProcessMove(mob->CastToNPC(), npos.x, npos.y, z_at_pos);
			}
			auto vec = glm::vec4(npos.x, npos.y, z_at_pos, mob->GetHeading());
			mob->SetPosition(vec);

			m_distance_moved_since_correction += distance_moved;

		}
		//If x seconds have passed without sending an update.
		if ((m_distance_moved_since_correction > 0.0 || mob->IsClient()) && ((current_time - m_last_sent_time) >= 2.0))
			need_update = true;

		if (need_update) {
			m_last_sent_speed = current_speed;
			m_last_sent_time = current_time;
			mob->SendPosUpdate();
			mob->SendPosUpdate(2);
		}

		return false;
	}
};

class TeleportToCommand : public IMovementCommand {
public:
	TeleportToCommand(float x, float y, float z, float heading)
	{
		m_teleport_to_x       = x;
		m_teleport_to_y       = y;
		m_teleport_to_z       = z;
		m_teleport_to_heading = heading;
	}

	virtual ~TeleportToCommand()
	{

	}

	glm::vec3 CurrentDest() { return glm::vec3(0.0f); }

	virtual bool Process(MobMovementManager *mob_movement_manager, Mob *mob)
	{
		if (!mob->IsAIControlled()) {
			return true;
		}

		if (mob->IsNPC()) {
			entity_list.ProcessMove(mob->CastToNPC(), m_teleport_to_x, m_teleport_to_y, m_teleport_to_z);
		}

		mob->SetPosition(glm::vec4(m_teleport_to_x, m_teleport_to_y, m_teleport_to_z, mob_movement_manager->FixHeading(m_teleport_to_heading)));
		mob->SetHeading(mob_movement_manager->FixHeading(m_teleport_to_heading));
		mob->SetDelta(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		mob->SetRunAnimation(0.0f);
		mob->SendPosUpdate();
		mob->SendPosUpdate(2);
		return true;
	}

	virtual bool Started() const
	{
		return false;
	}

private:

	double m_teleport_to_x;
	double m_teleport_to_y;
	double m_teleport_to_z;
	double m_teleport_to_heading;
};

class StopMovingCommand : public IMovementCommand {
public:
	StopMovingCommand()
	{
	}

	virtual ~StopMovingCommand()
	{

	}

	glm::vec3 CurrentDest() { return glm::vec3(0.0f); }

	virtual bool Process(MobMovementManager *mob_movement_manager, Mob *mob)
	{
		if (!mob->IsAIControlled()) {
			return true;
		}

		if (mob->IsMoving() || mob->GetDeltaHeading() != 0.0f) {
			if (RuleB(Map, FixZWhenPathing)) {
				mob->FixZ();
			}
			mob->SetMoving(false);
			mob->SetMoved(false);
			mob->SetRunAnimation(0.0f);
			mob->SetDelta(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			mob->SendRealPosition();
			mob->SendPosUpdate(2);
		}
		return true;
	}

	virtual bool Started() const
	{
		return false;
	}
};

class StopSwimmingCommand : public IMovementCommand {
public:
	StopSwimmingCommand()
	{
	}

	virtual ~StopSwimmingCommand()
	{

	}

	glm::vec3 CurrentDest() { return glm::vec3(0.0f); }

	virtual bool Process(MobMovementManager *mob_movement_manager, Mob *mob)
	{
		if (!mob->IsAIControlled()) {
			return true;
		}

		if (mob->IsMoving()) {
			mob->SetMoving(false);
			mob->SetMoved(false);
			mob->SetRunAnimation(0.0f);
			mob->SendRealPosition();
			mob->SendPosUpdate(2);
		}
		return true;
	}

	virtual bool Started() const
	{
		return false;
	}
};

class EvadeCombatCommand : public IMovementCommand {
public:
	EvadeCombatCommand()
	{
	}

	virtual ~EvadeCombatCommand()
	{

	}

	glm::vec3 CurrentDest() { return glm::vec3(0.0f); }

	virtual bool Process(MobMovementManager *mob_movement_manager, Mob *mob)
	{
		if (!mob->IsAIControlled()) {
			return true;
		}

		if (mob->IsMoving()) {
			mob->SetMoving(false);
			mob->SetRunAnimation(0.0f);
			mob->SendRealPosition();
			mob->SendPosUpdate(2);
		}

		mob->BuffFadeAll();
		mob->WipeHateList();
		mob->Heal();

		return true;
	}

	virtual bool Started() const
	{
		return false;
	}
};

struct MovementStats {
	MovementStats()
	{
		LastResetTime     = static_cast<double>(Timer::GetCurrentTime()) / 1000.0;
		TotalSent         = 0ULL;
		TotalSentMovement = 0ULL;
		TotalSentPosition = 0ULL;
		TotalSentHeading  = 0ULL;
	}

	double   LastResetTime;
	uint64_t TotalSent;
	uint64_t TotalSentMovement;
	uint64_t TotalSentPosition;
	uint64_t TotalSentHeading;
};

struct NavigateTo {
	NavigateTo()
	{
		navigate_to_x       = 0.0;
		navigate_to_y       = 0.0;
		navigate_to_z       = 0.0;
		navigate_to_speed   = 0.0f;
		last_set_time       = 0.0;
		navigate_to_mode = MovementWalking;
	}

	double navigate_to_x;
	double navigate_to_y;
	double navigate_to_z;

	double last_set_time;
	float navigate_to_speed;
	MobMovementMode navigate_to_mode;
};

struct MobMovementEntry {
	std::deque<std::unique_ptr<IMovementCommand>> Commands;
	NavigateTo                                    NavTo;
};

void AdjustRoute(std::list<IPathfinder::IPathNode> &nodes, Mob *who)
{
	if (!zone->HasMap() || !zone->HasWaterMap() || !who || who->IsBoat()) {
		return;
	}

	auto offset = who->GetZOffset();
	bool underwater_mob = who->IsNPC() && who->CastToNPC()->IsUnderwaterOnly();

	for (auto &node : nodes) {
		if (node.teleport)
			continue;
		if (underwater_mob || zone->IsWaterZone(node.pos.z) || zone->watermap->InLiquid(node.pos)) {
			float ceiling = zone->zonemap->FindCeiling(node.pos, nullptr);
			float ground = zone->zonemap->FindGround(node.pos, nullptr);

			if (ground != BEST_Z_INVALID && (node.pos.z < (ground + offset)))
				node.pos.z = ground + offset;
			if (ceiling != BEST_Z_INVALID && node.pos.z > ceiling) {
				node.pos.z = ceiling - 1.0f;
			}
		} else {
			auto best_z = zone->zonemap->FindBestZ(node.pos, nullptr, 20.0f);
			if (best_z == BEST_Z_INVALID)
				best_z = zone->zonemap->FindBestZ(node.pos, nullptr);
			if (best_z != BEST_Z_INVALID) {
				node.pos.z = best_z + offset;
			}
		}
	}
}

struct MobMovementManager::Implementation {
	std::map<Mob *, MobMovementEntry> Entries;
	std::vector<Client *>             Clients;
	MovementStats                     Stats;
};

MobMovementManager::MobMovementManager()
{
	_impl.reset(new Implementation());
}

MobMovementManager::~MobMovementManager()
{
}

void MobMovementManager::Process()
{
	for (auto &iter : _impl->Entries) {
		auto &ent      = iter.second;
		auto &commands = ent.Commands;

		while (true != commands.empty()) {
			auto &cmd = commands.front();
			auto r    = cmd->Process(this, iter.first);

			if (true != r) {
				break;
			}

			commands.pop_front();
		}
	}
}

/**
 * @param mob
 */
void MobMovementManager::AddMob(Mob *mob)
{
	_impl->Entries.insert(std::make_pair(mob, MobMovementEntry()));
}

/**
 * @param mob
 */
void MobMovementManager::RemoveMob(Mob *mob)
{
	_impl->Entries.erase(mob);
}

/**
 * @param client
 */
void MobMovementManager::AddClient(Client *client)
{
	_impl->Clients.push_back(client);
}

/**
 * @param client
 */
void MobMovementManager::RemoveClient(Client *client)
{
	auto iter = _impl->Clients.begin();
	while (iter != _impl->Clients.end()) {
		if (client == *iter) {
			_impl->Clients.erase(iter);
			return;
		}

		++iter;
	}
}

/**
 * @param who
 * @param to
 * @param mob_movement_mode
 */
void MobMovementManager::RotateTo(Mob *who, float to, MobMovementMode mob_movement_mode, bool at_guardpoint)
{
	auto iter = _impl->Entries.find(who);
	auto &ent = (*iter);

	if (true != ent.second.Commands.empty() && mob_movement_mode == MovementWalking) {
		return;
	}

	if (ent.second.Commands.empty() && mob_movement_mode == MovementRunning && who->GetHeading() != to)
		who->SetMoving(true);

	PushRotateTo(ent.second, who, to, mob_movement_mode, at_guardpoint);
}

/**
 * @param who
 * @param x
 * @param y
 * @param z
 * @param heading
 */
void MobMovementManager::Teleport(Mob *who, float x, float y, float z, float heading)
{
	auto iter = _impl->Entries.find(who);
	auto &ent = (*iter);

	ent.second.Commands.clear();

	PushTeleportTo(ent.second, x, y, z, heading);
}

/**
 * @param who
 * @param x
 * @param y
 * @param z
 * @param mode
 */
void MobMovementManager::NavigateTo(Mob *who, float x, float y, float z, MobMovementMode mode)
{
	if (IsPositionEqualWithinCertainZ(glm::vec3(x, y, z), glm::vec3(who->GetX(), who->GetY(), who->GetZ()), 6.0f)) {
		who->SetPosition(glm::vec4(x, y, z, who->GetHeading()));
		return;
	}

	auto iter = _impl->Entries.find(who);
	auto &ent = (*iter);
	auto &nav = ent.second.NavTo;

	double current_time = static_cast<double>(Timer::GetCurrentTime()) / 1000.0;
	float current_speed = who->GetCurrentSpeed();
	bool speed_changed = (who->GetCurrentSpeed() != nav.navigate_to_speed) || (mode != nav.navigate_to_mode);
	if ((current_time - nav.last_set_time) > 0.5 || speed_changed || ((ent.second.Commands.size() < 2) && (current_time - nav.last_set_time) > 0.2)) {
		//Can potentially recalc
		auto within = IsPositionWithinSimpleCylinder(
			glm::vec3(x, y, z),
			glm::vec3(nav.navigate_to_x, nav.navigate_to_y, nav.navigate_to_z),
			1.5f,
			6.0f
		);

		if (false == within || ent.second.Commands.size() == 0 || speed_changed) {

			// we are updating path to a new location
			// see what our current command was heading us toward.
			glm::vec3 last(0.0f);
			if (ent.second.Commands.size() > 1) {
				auto current_command = ent.second.Commands.begin();
				auto dest = current_command->get();
				last = dest->CurrentDest();
			}

			ent.second.Commands.clear();
			//Path is no longer valid, calculate a new path
			UpdatePath(who, x, y, z, mode, last.x, last.y, last.z);
			nav.navigate_to_x       = x;
			nav.navigate_to_y       = y;
			nav.navigate_to_z       = z;
			nav.navigate_to_speed = current_speed;
			nav.last_set_time       = current_time;
			nav.navigate_to_mode = mode;
		}
	}
}

/**
 * @param who
 */
void MobMovementManager::StopNavigation(Mob *who, float new_head)
{
	auto iter = _impl->Entries.find(who);
	auto &ent = (*iter);
	auto &nav = ent.second.NavTo;

	nav.navigate_to_x       = 0.0;
	nav.navigate_to_y       = 0.0;
	nav.navigate_to_z       = 0.0;
	nav.navigate_to_speed = 0.0f;

	if (new_head != -1.0f && who->GetHeading() != new_head) {
		who->SetHeading(new_head);
		who->SetMoving(true);
	}

	if (ent.second.Commands.empty()) {
		PushStopMoving(ent.second);
		return;
	}

	if (!who->IsMoving() && who->GetDeltaHeading() == 0.0f) {
		ent.second.Commands.clear();
		return;
	}

	ent.second.Commands.clear();
	PushStopMoving(ent.second);
}

/**
 * @param mob
 * @param delta_x
 * @param delta_y
 * @param delta_z
 * @param delta_heading
 * @param anim
 * @param range
 * @param single_client
 */
void MobMovementManager::SendCommandToClients(
	Mob *mob,
	float delta_x,
	float delta_y,
	float delta_z,
	float delta_heading,
	int anim,
	ClientRange range,
	Client* single_client,
	Client* ignore_client
)
{
	if (range == ClientRangeNone) {
		return;
	}

	EQApplicationPacket outapp(OP_ClientUpdate, sizeof(SpawnPositionUpdate_Struct));
	auto                *spu = (SpawnPositionUpdate_Struct*) outapp.pBuffer;

	FillCommandStruct(spu, mob, delta_x, delta_y, delta_z, delta_heading, anim);

		//float short_range = RuleR(Pathing, ShortMovementUpdateRange);
		//float long_range = zone->GetMaxMovementUpdateRange();

	for (auto& c : _impl->Clients) {
		if (single_client && c != single_client) {
			continue;
		}

		if (ignore_client && c == ignore_client) {
			continue;
		}

		bool match = true;
		if (match) {
			_impl->Stats.TotalSent++;

			if (anim != 0) {
				_impl->Stats.TotalSentMovement++;
			}
			else if (delta_heading != 0) {
				_impl->Stats.TotalSentHeading++;
			}
			else {
				_impl->Stats.TotalSentPosition++;
			}

			c->QueuePacket(&outapp, false);
		}
	}
}

/**
 * @param in
 * @return
 */
float MobMovementManager::FixHeading(float in)
{
	auto h = in;
	while (h >= 256.0) {
		h -= 256.0;
	}

	while (h < 0.0) {
		h += 256.0;
	}

	return h;
}

/**
 * @param client
 */
void MobMovementManager::DumpStats(Client *client)
{
	auto current_time = static_cast<double>(Timer::GetCurrentTime()) / 1000.0;
	auto total_time   = current_time - _impl->Stats.LastResetTime;

	client->Message(15, "Dumping Movement Stats:");
	client->Message(
		15,
		"Total Sent: %u (%.2f / sec)",
		_impl->Stats.TotalSent,
		static_cast<double>(_impl->Stats.TotalSent) / total_time
	);
	client->Message(
		15,
		"Total Heading: %u (%.2f / sec)",
		_impl->Stats.TotalSentHeading,
		static_cast<double>(_impl->Stats.TotalSentHeading) / total_time
	);
	client->Message(
		15,
		"Total Movement: %u (%.2f / sec)",
		_impl->Stats.TotalSentMovement,
		static_cast<double>(_impl->Stats.TotalSentMovement) / total_time
	);
	client->Message(
		15,
		"Total Position: %u (%.2f / sec)",
		_impl->Stats.TotalSentPosition,
		static_cast<double>(_impl->Stats.TotalSentPosition) / total_time
	);
}

void MobMovementManager::ClearStats()
{
	_impl->Stats.LastResetTime     = static_cast<double>(Timer::GetCurrentTime()) / 1000.0;
	_impl->Stats.TotalSent         = 0;
	_impl->Stats.TotalSentHeading  = 0;
	_impl->Stats.TotalSentMovement = 0;
	_impl->Stats.TotalSentPosition = 0;
}

/**
 * @param position_update
 * @param mob
 * @param delta_x
 * @param delta_y
 * @param delta_z
 * @param delta_heading
 * @param anim
 */
void MobMovementManager::FillCommandStruct(
	SpawnPositionUpdate_Struct *position_update,
	Mob *mob,
	float delta_x,
	float delta_y,
	float delta_z,
	float delta_heading,
	int anim
)
{
	memset(position_update, 0x00, sizeof(SpawnPositionUpdate_Struct));
	position_update->spawn_id      = mob->GetID();
	position_update->x_pos         = static_cast<int16>(mob->GetX());
	position_update->y_pos         = static_cast<int16>(mob->GetY());
	position_update->z_pos         = static_cast<int16>(mob->GetZ() * 10.0f);
	position_update->heading       = static_cast<int8>(mob->GetHeading());
	position_update->delta_yzx.SetValue(delta_x, delta_y, delta_z);
	position_update->delta_heading = static_cast<int8>(delta_heading);
	position_update->anim_type     = mob->GetRunAnimSpeed();
}

/**
 * @param who
 * @param x
 * @param y
 * @param z
 * @param mob_movement_mode
 */
void MobMovementManager::UpdatePath(Mob *who, float x, float y, float z, MobMovementMode mob_movement_mode, float last_x, float last_y, float last_z)
{
	Mob *target=who->GetTarget();

	if (!zone->HasMap()) {
		auto iter = _impl->Entries.find(who);
		auto &ent = (*iter);

		PushMoveTo(ent.second, x, y, z, mob_movement_mode);
		return;
	}

	if (who->IsBoat()) {
		UpdatePathBoat(who, x, y, z, mob_movement_mode);
	}
	else if ((who->IsNPC() && who->CastToNPC()->IsUnderwaterOnly()) || zone->IsWaterZone(who->GetZ())) {
		UpdatePathUnderwater(who, x, y, z, mob_movement_mode);
	}
	// If we can fly, and we have a target and we have LoS, simply fly to them.
	// if we ever lose LoS we go back to mesh run mode.
	else if (target && who->IsNPC() && (who->GetFlyMode() == 1 || who->GetFlyMode() == 2) &&
				who->CheckLosFN(x,y,z,target->GetSize())) {
		auto iter = _impl->Entries.find(who);
		auto &ent = (*iter);
		PushFlyTo(ent.second, x, y, z, mob_movement_mode);
		}
	else {
		UpdatePathGround(who, x, y, z, mob_movement_mode, last_x, last_y, last_z);
	}
}

/**
 * @param who
 * @param x
 * @param y
 * @param z
 * @param mode
 */
void MobMovementManager::UpdatePathGround(Mob *who, float x, float y, float z, MobMovementMode mode, float last_x, float last_y, float last_z)
{
	PathfinderOptions opts;
	opts.smooth_path = true;
	opts.step_size   = RuleR(Pathing, NavmeshStepSize);
	opts.offset      = who->GetZOffset();
	opts.flags       = PathingNotDisabled ^ PathingZoneLine;

	//This is probably pointless since the nav mesh tool currently sets zonelines to disabled anyway
	auto partial = false;
	auto stuck   = false;
	glm::vec3 begin(who->GetX(), who->GetY(), who->GetZ());
	bool in_liquid = zone->HasWaterMap() && zone->watermap->InLiquid(begin) || zone->IsWaterZone(begin.z);
	if (!in_liquid) {
		if (who->IsClient()) {
			float best_z = zone->zonemap->FindBestZ(begin, nullptr);
			if (best_z != BEST_Z_INVALID)
				begin.z = best_z + who->GetZOffset();
		}
		else {
			float best_z = zone->zonemap->FindBestZ(begin, nullptr, 20.0f);
			if (best_z == BEST_Z_INVALID)
				best_z = zone->zonemap->FindBestZ(begin, nullptr);
			if (best_z != BEST_Z_INVALID)
				begin.z = best_z + who->GetZOffset();
		}
	}

	glm::vec3 end(x, y, z);

	auto route   = zone->pathing->FindPath(
		begin,
		end,
		partial,
		stuck,
		opts
	);

	auto eiter = _impl->Entries.find(who);
	auto &ent  = (*eiter);

	if (route.size() == 0) {
		HandleStuckBehavior(who, x, y, z, mode);
		return;
	}

	AdjustRoute(route, who);

	//avoid doing any processing if the mob is stuck to allow normal stuck code to work.
	if (!stuck) {

		//there are times when the routes returned are no differen than where the mob is currently standing. What basically happens
		//is a mob will get 'stuck' in such a way that it should be moving but the 'moving' place is the exact same spot it is at.
		//this is a problem and creates an area of ground that if a mob gets to, will stay there forever. If socal this creates a
		//"Ball of Death" (tm). This code tries to prevent this by simply warping the mob to the requested x/y. Better to have a warp than
		//have stuck mobs.

		auto routeNode   = route.begin();
		bool noValidPath = true;
		while (routeNode != route.end() && noValidPath == true) {
			auto &currentNode = (*routeNode);

			if (routeNode == route.end()) {
				continue;
			}

			if (!(currentNode.pos.x == who->GetX() && currentNode.pos.y == who->GetY())) {
				//if one of the nodes to move to, is not our current node, pass it.
				noValidPath = false;
				break;
			}
			//move to the next node
			routeNode++;
		}
		if (noValidPath) {
			//we are 'stuck' in a path, lets just get out of this by 'teleporting' to the next position.
			PushTeleportTo(
				ent.second,
				x,
				y,
				z,
				CalculateHeadingAngleBetweenPositions(who->GetX(), who->GetY(), x, y)
			);
			return;
		}
		if (!(last_x == 0.0f && last_y != 0.0f && last_z != 0.0f)) {
			// if one of points in our route may be where we are already heading
			if (last_x != x && last_y != y) {
				routeNode = route.begin();
				int nodes_checked = 0;
				bool found_last = false;
				auto &currentNode = (*routeNode);
				// first node should be our location
				if (!(currentNode.pos.x == who->GetX() && currentNode.pos.y == who->GetY())) {
					nodes_checked = 5;
				}
				else {
					routeNode++;
				}
				while (routeNode != route.end() && found_last == false && nodes_checked < 5) {

					if (routeNode == route.end()) {
						continue;
					}

					if (currentNode.pos.x == last_x && currentNode.pos.y == last_y && currentNode.pos.z == last_z) {
						// we were previously moving and recalculated our route
						// the node we were heading to is in our new route.
						// lets continue on to that node next
						found_last = true;
						break;
					}
					//move to the next node
					nodes_checked++;
					routeNode++;
				}
				if (found_last) {
					routeNode = route.begin();
					bool done = false;
					routeNode++;
					while (routeNode != route.end() && done == false) {
						currentNode = (*routeNode);
						if (routeNode == route.end()) {
							continue;
						}
						if (currentNode.pos.x == last_x && currentNode.pos.y == last_y && currentNode.pos.z == last_z)
							done = true;
						else
							routeNode = route.erase(routeNode);
					}
				}
			}
		}
		
	}

	auto iter = route.begin();

	glm::vec3 previous_pos(who->GetX(), who->GetY(), who->GetZ());

	while (iter != route.end()) {
		auto &current_node = (*iter);

		iter++;

		if (iter == route.end()) {
			continue;
		}

		previous_pos = current_node.pos;
		auto &next_node = (*iter);

		if (next_node.teleport)
			continue;

		//move to / teleport to node + 1
		if (current_node.teleport && next_node.pos.x != 0.0f && next_node.pos.y != 0.0f) {
			PushTeleportTo(
				ent.second,
				next_node.pos.x,
				next_node.pos.y,
				next_node.pos.z,
				CalculateHeadingAngleBetweenPositions(
					current_node.pos.x,
					current_node.pos.y,
					next_node.pos.x,
					next_node.pos.y
				)
			);
		}
		else {
			bool in_liquid = zone->HasWaterMap() && zone->watermap->InLiquid(previous_pos) || zone->IsWaterZone(previous_pos.z);
			if (in_liquid || who->IsBoat()) {
				PushSwimTo(ent.second, next_node.pos.x, next_node.pos.y, next_node.pos.z, mode);
			}
			else {
				PushMoveTo(ent.second, next_node.pos.x, next_node.pos.y, next_node.pos.z, mode);
			}
		}
	}

	if (stuck) {
		HandleStuckBehavior(who, x, y, z, mode);
	}
}

/**
 * @param who
 * @param x
 * @param y
 * @param z
 * @param movement_mode
 */
void MobMovementManager::UpdatePathUnderwater(Mob *who, float x, float y, float z, MobMovementMode movement_mode)
{
	auto eiter = _impl->Entries.find(who);
	auto &ent  = (*eiter);
	bool underwater_mob = who->IsNPC() && who->CastToNPC()->IsUnderwaterOnly();
	if ((underwater_mob || (zone->IsWaterZone(who->GetZ()) && zone->IsWaterZone(z)) || (zone->watermap->InLiquid(who->GetPosition()) && zone->watermap->InLiquid(glm::vec3(x, y, z)))) &&
		zone->zonemap->CheckLoS(who->GetPosition(), glm::vec3(x, y, z))) {
		PushSwimTo(ent.second, x, y, z, movement_mode);
		return;
	}

	PathfinderOptions opts;
	opts.smooth_path = true;
	opts.step_size   = RuleR(Pathing, NavmeshStepSize);
	opts.offset      = who->GetZOffset();
	opts.flags       = PathingNotDisabled ^ PathingZoneLine;

	auto partial = false;
	auto stuck   = false;
	auto route   = zone->pathing->FindPath(
		glm::vec3(who->GetX(), who->GetY(), who->GetZ()),
		glm::vec3(x, y, z),
		partial,
		stuck,
		opts
	);

	if (route.size() == 0) {
		HandleStuckBehavior(who, x, y, z, movement_mode);
		return;
	}
	AdjustRoute(route, who);

	auto      iter = route.begin();
	glm::vec3 previous_pos(who->GetX(), who->GetY(), who->GetZ());

	if (route.size() == 0) {
		HandleStuckBehavior(who, x, y, z, movement_mode);
		return;
	}

	iter = route.begin();
	while (iter != route.end()) {
		auto &current_node = (*iter);

		iter++;

		if (iter == route.end()) {
			continue;
		}

		previous_pos = current_node.pos;
		auto &next_node = (*iter);

		if (next_node.teleport)
			continue;

		//move to / teleport to node + 1
		if (current_node.teleport && next_node.pos.x != 0.0f && next_node.pos.y != 0.0f) {
			PushTeleportTo(
				ent.second, next_node.pos.x, next_node.pos.y, next_node.pos.z,
				CalculateHeadingAngleBetweenPositions(
					current_node.pos.x,
					current_node.pos.y,
					next_node.pos.x,
					next_node.pos.y
				));
		}
		else {
			if (underwater_mob || (zone->IsWaterZone(previous_pos.z) && zone->IsWaterZone(next_node.pos.z)) || (zone->watermap->InLiquid(previous_pos) && zone->watermap->InLiquid(next_node.pos)))
				PushSwimTo(ent.second, next_node.pos.x, next_node.pos.y, next_node.pos.z, movement_mode);
			else
				PushMoveTo(ent.second, next_node.pos.x, next_node.pos.y, next_node.pos.z, movement_mode);
		}
	}

	if (stuck) {
		HandleStuckBehavior(who, x, y, z, movement_mode);
	}
}

/**
 * @param who
 * @param x
 * @param y
 * @param z
 * @param mode
 */
void MobMovementManager::UpdatePathBoat(Mob *who, float x, float y, float z, MobMovementMode mode)
{
	auto eiter = _impl->Entries.find(who);
	auto &ent  = (*eiter);

	PushSwimTo(ent.second, x, y, z, mode);
}

/**
 * @param ent
 * @param x
 * @param y
 * @param z
 * @param heading
 */
void MobMovementManager::PushTeleportTo(MobMovementEntry &ent, float x, float y, float z, float heading)
{
	ent.Commands.push_back(std::unique_ptr<IMovementCommand>(new TeleportToCommand(x, y, z, heading)));
}

/**
 * @param ent
 * @param x
 * @param y
 * @param z
 * @param mob_movement_mode
 */
void MobMovementManager::PushMoveTo(MobMovementEntry &ent, float x, float y, float z, MobMovementMode mob_movement_mode)
{
	ent.Commands.push_back(std::unique_ptr<IMovementCommand>(new MoveToCommand(x, y, z, mob_movement_mode)));
}

/**
 * @param ent
 * @param x
 * @param y
 * @param z
 * @param mob_movement_mode
 */
void MobMovementManager::PushSwimTo(MobMovementEntry &ent, float x, float y, float z, MobMovementMode mob_movement_mode)
{
	ent.Commands.push_back(std::unique_ptr<IMovementCommand>(new SwimToCommand(x, y, z, mob_movement_mode)));
}

/**
 * @param ent
 * @param who
 * @param to
 * @param mob_movement_mode
 */
void MobMovementManager::PushRotateTo(MobMovementEntry &ent, Mob *who, float to, MobMovementMode mob_movement_mode, bool at_guardpoint)
{
	auto from = FixHeading(who->GetHeading());
	to = FixHeading(to);

	float diff = to - from;

	while (diff < -128.0) {
		diff += 256.0;
	}

	while (diff > 128.0) {
		diff -= 256.0;
	}

	ent.Commands.push_back(std::unique_ptr<IMovementCommand>(new RotateToCommand(to, diff > 0 ? 1.0 : -1.0, mob_movement_mode, at_guardpoint)));
}

/**
 * @param ent
 * @param x
 * @param y
 * @param z
 * @param mob_movement_mode
 */
void MobMovementManager::PushFlyTo(MobMovementEntry &ent, float x, float y, float z, MobMovementMode mob_movement_mode)
{
	ent.Commands.push_back(std::unique_ptr<IMovementCommand>(new FlyToCommand(x, y, z, mob_movement_mode)));
}

/**
 * @param mob_movement_entry
 */
void MobMovementManager::PushStopMoving(MobMovementEntry &mob_movement_entry)
{
	mob_movement_entry.Commands.push_back(std::unique_ptr<IMovementCommand>(new StopMovingCommand()));
}

/**
* @param mob_movement_entry
*/
void MobMovementManager::PushStopSwimming(MobMovementEntry &mob_movement_entry)
{
	mob_movement_entry.Commands.push_back(std::unique_ptr<IMovementCommand>(new StopSwimmingCommand()));
}

/**
 * @param mob_movement_entry
 */
void MobMovementManager::PushEvadeCombat(MobMovementEntry &mob_movement_entry)
{
	mob_movement_entry.Commands.push_back(std::unique_ptr<IMovementCommand>(new EvadeCombatCommand()));
}

/**
 * @param who
 * @param x
 * @param y
 * @param z
 * @param mob_movement_mode
 */
void MobMovementManager::HandleStuckBehavior(Mob *who, float x, float y, float z, MobMovementMode mob_movement_mode)
{

	auto sb = who->GetStuckBehavior();
	MobStuckBehavior behavior = RunToTarget;

	if (sb >= 0 && sb < MaxStuckBehavior) {
		if (who->IsEngaged())
			behavior = (MobStuckBehavior) sb;
	}

	auto eiter = _impl->Entries.find(who);
	auto &ent = (*eiter);

	bool in_liquid = zone->HasWaterMap() && zone->watermap->InLiquid(who->GetPosition()) || zone->IsWaterZone(who->GetZ());
	bool dest_in_liquid = zone->HasWaterMap() && zone->watermap->InLiquid(glm::vec3(x, y, z)) || zone->IsWaterZone(z);

	switch (sb) {
		case RunToTarget:
			if (!in_liquid || !dest_in_liquid)
				PushMoveTo(ent.second, x, y, z, mob_movement_mode);
			else
				PushSwimTo(ent.second, x, y, z, mob_movement_mode);
			break;
		case WarpToTarget:
			PushTeleportTo(ent.second, x, y, z, 0.0f);
			break;
		case TakeNoAction:
			PushStopMoving(ent.second);
			break;
		case EvadeCombat:
			PushEvadeCombat(ent.second);
			break;
	}
}
