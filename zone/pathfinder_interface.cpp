#include "../common/seperator.h"
#include "client.h"
#include "pathfinder_null.h"
#include "pathfinder_nav_mesh.h"
#include "pathfinder_waypoint.h"
#include "../common/path_manager.h"
#include <fmt/format.h>
#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include <sys/stat.h>

IPathfinder *IPathfinder::Load(const std::string &zone) {
	struct stat statbuffer;

	std::string waypoint_path = fmt::format("{}/{}.path", PathManager::Instance()->GetMapsPath(), zone);
	if (stat(waypoint_path.c_str(), &statbuffer) == 0) {
		return new PathfinderWaypoint(waypoint_path);
	}

	std::string navmesh_path = fmt::format("{}/{}.nav", PathManager::Instance()->GetMapsPath(), zone);
	if (stat(navmesh_path.c_str(), &statbuffer) == 0) {
		return new PathfinderNavmesh(navmesh_path);
	}
	
	return new PathfinderNull();
}