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

#ifndef BEACON_H
#define BEACON_H

#include "mob.h"
#include "../common/types.h"
#include "../common/timer.h"
#include <chrono>

class Group;
class Raid;

enum BeaconTypes
{
	beaconTypeBeacon = 0,
	beaconTypeBolt = 1,
	beaconTypeProjectile = 2
};

class Beacon : public Mob
{
public:
	Beacon(Mob *at_mob, int lifetime);
	~Beacon();

	//abstract virtual function implementations requird by base abstract class
	virtual bool Death(Mob* killerMob, int32 damage, uint16 spell_id, EQ::skills::SkillType attack_skill, uint8 killedby = 0, bool bufftic = false) { return true; }
	virtual void Damage(Mob* from, int32 damage, uint16 spell_id, EQ::skills::SkillType attack_skill, bool avoidable = true, int8 buffslot = -1, bool iBuffTic = false) { return; }
	virtual bool Attack(Mob* other, int hand = EQ::invslot::slotPrimary, int damagePct = 100) { return false; }
	virtual bool HasRaid() { return false; }
	virtual bool HasGroup() { return false; }
	virtual Raid* GetRaid() { return 0; }
	virtual Group* GetGroup() { return 0; }
	virtual bool CombatRange(Mob* other, float dist_squared = 0.0f, bool check_z = false, bool pseudo_pos = false) { return false; }

	bool	IsBeacon()			const { return true; }
	bool	Process();
	virtual void	Depop(bool not_used = true)	{ remove_me = true; }
	void AELocationSpell(Mob *caster, uint16 cast_spell_id, int16 resist_adjust);
	void BoltSpell(Mob *caster, Mob *target, int16 cast_spell_id);
	void Projectile(Mob *attacker, Mob *target, EQ::skills::SkillType skill, int32 wep_dmg, int32 hate, uint32 reusetimer = 0, float pitch_angle = 128.0f);
	bool IsBolt() { return (beaconType == beaconTypeBolt); }
	bool IsBeacon() { return (beaconType == beaconTypeBeacon); }
	bool IsProjectile() { return (beaconType == beaconTypeProjectile); }
	uint8	GetTargetsHit() { return targets_hit; }
	void	SetTargetsHit(uint8 value) { targets_hit = value; } 
	bool CheckProjectileCollision(glm::vec3 oloc);
	void Tracer(int16 src_id, int16 tgt_id, glm::vec4 pos);

protected:
	Timer remove_timer;
	bool remove_me;

	uint16 spell_id;
	int16 resist_adjust;
	int spell_iterations;
	Timer spell_timer;
	uint32 timer_interval;

	float z_offset;
	float size;
	float base_size;
	glm::vec3 tarpos;
	glm::vec3 origin;

	uint16 caster_id;
	uint16 target_id;
	int32 caster_level;
	bool send_animation;
	float velocity;
	float pitch;
	uint8 targets_hit;
	uint8 beaconType;
	bool initial_cast;
	EQ::skills::SkillType skillinuse;
	int32 weapon_dmg;
	int32 projectile_hate;
	uint32 reuse_timer;
	double distance_traveled;
	double delay_duration;
	double delay_start;
	std::chrono::time_point<std::chrono::steady_clock> time_start;
	std::chrono::time_point<std::chrono::steady_clock> frame_prev;
private:
};

#endif
