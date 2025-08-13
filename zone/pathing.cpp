#include "../common/global_define.h"
#include "client.h"
#include "doors.h"
#include "water_map.h"
#include "zone.h"

#include <fstream>
#include <list>
#include <math.h>
#include <sstream>
#include <string.h>

#ifdef _WINDOWS
#define snprintf _snprintf
#endif

//#define PATHDEBUG 

extern Zone* zone;

void CullPoints(std::vector<FindPerson_Point>& points) {
	if (!zone->HasMap()) {
		return;
	}

	size_t i = 0;
	for (; i < points.size(); ++i) {
		auto& p = points[i];

		for (;;) {
			if (i + 2 >= points.size()) {
				return;
			}

			if (points.size() < 36) {
				return;
			}

			auto& p1 = points[i + 1];
			auto& p2 = points[i + 2];

			if (zone->zonemap->CheckLoS(glm::vec3(p.x, p.y, p.z), glm::vec3(p2.x, p2.y, p2.z))) {
				points.erase(points.begin() + i + 1);
				LogPathingDetail("Culled find path point [{}], connecting [{}]->[{}] instead", i + 1, i, i + 2);
			}
			else {
				break;
			}
		}
	}
}
