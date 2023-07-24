/*
	EQEMu:  Everquest Server Emulator

	Copyright (C) 2001-2014 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef ZONE_MAP_H
#define ZONE_MAP_H

#include "position.h"
#include <stdio.h>

#include "zone_config.h"

#define BEST_Z_INVALID -99999

extern const ZoneConfig *Config;


uint32 InflateData(const char* buffer, uint32 len, char* out_buffer, uint32 out_len_max);

class Map
{
public:
	Map();
	~Map();

	float FindBestZ(glm::vec3 &start, glm::vec3 *result) const;
	float FindBestZ(glm::vec3 &start, glm::vec3 *result, float limit, float offset = 0.0f) const;
	float FindClosestZ(glm::vec3 &start, glm::vec3 *result, float z_offset) const;
	float FindCeiling(glm::vec3 &start, glm::vec3 *result) const;
	float FindGround(glm::vec3 &start, glm::vec3 *result) const;
	bool LineIntersectsZone(glm::vec3 start, glm::vec3 end, float step, glm::vec3 *hitLocation, glm::vec3 *hitNormal = nullptr, float *hitDistance = nullptr) const;
	bool LineIntersectsZoneCeiling(glm::vec3 start, glm::vec3 end, float step, glm::vec3 *hitLocation, glm::vec3 *hitNormal = nullptr, float *hitDistance = nullptr) const;
	bool LineIntersectsZoneNoZLeaps(glm::vec3 start, glm::vec3 end, float step_mag, glm::vec3 *result) const;
	bool CheckLoS(glm::vec3 myloc, glm::vec3 oloc) const;
	bool DoCollisionCheck(glm::vec3 myloc, glm::vec3 oloc, glm::vec3& outnorm, float& distance) const;
	bool Load(const std::string& filename);
	static Map *LoadMapFile(std::string file);
	bool NoHazardsAccurate(glm::vec3 From, glm::vec3 To, float size = 6.0f, int max_steps = 20, float interval = 5.0f);
private:
	void RotateVertex(glm::vec3 &v, float rx, float ry, float rz);
	void ScaleVertex(glm::vec3 &v, float sx, float sy, float sz);
	void TranslateVertex(glm::vec3 &v, float tx, float ty, float tz);
	bool LoadV1(FILE *f);
	bool LoadV2(FILE *f);

	struct impl;
	impl *imp;
};

#endif
