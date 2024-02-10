#pragma once

#include "pathfinder_interface.h"

#define PATHNODENEIGHBOURS 50

struct PathFileHeader;
struct Node;

class PathfinderWaypoint : public IPathfinder
{
public:
	PathfinderWaypoint(const std::string &path);
	virtual ~PathfinderWaypoint();

	bool IsUsingNavMesh() { return false; }
	virtual IPath FindRoute(const glm::vec3 &start, const glm::vec3 &end, bool &partial, bool &stuck, int flags = PathingNotDisabled);
	virtual IPath FindPath(const glm::vec3& start, const glm::vec3& end, bool& partial, bool& stuck, const PathfinderOptions& opts);
	virtual glm::vec3 GetRandomLocation(const glm::vec3 &start, int flags = PathingNotDisabled);
	virtual void DebugCommand(Client *c, const Seperator *sep);

private:
	void Load(const std::string &filename);
	void LoadPath(FILE *PathFile, const PathFileHeader &header);
	void ShowNodes();
	void NodeInfo(Client *c);
	Node *FindPathNodeByCoordinates(float x, float y, float z);
	void ResizePathingVectors();
	void RecalcDistances();
	void Optimize();
	void ShowNode(const Node &n);
	int FindNearestPathNode(glm::vec3 Position);
	std::deque<int> FindRouteV4(int startID, int endID);
	std::deque<int> FindRouteV2(int startID, int endID);

	struct Implementation;
	std::unique_ptr<Implementation> m_impl;
};
