#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include "../common/misc_functions.h"
#include <string>
#include <memory>
#include <iostream>
#include <stdio.h>

#include "pathfinder_waypoint.h"
#include "zone.h"
#include "client.h"

extern Zone *zone;

#pragma pack(1)
struct NeighbourNode {
	int16 id;
	float distance;
	uint8 Teleport;
	int16 DoorID;
};

struct PathNode {
	uint16 id;
	glm::vec3 v;
	float bestz;
	NeighbourNode Neighbours[PATHNODENEIGHBOURS];
};

struct PathFileHeader {
	uint32 version;
	uint32 PathNodeCount;
};
#pragma pack()

struct Edge
{
	float distance;
	bool teleport;
	int door_id;
};

struct Node
{
	int id;
	glm::vec3 v;
	float bestz;
	std::map<int, Edge> edges;
};

struct PathNodeSortStruct
{
	int id;
	float Distance;
};

struct AStarNode
{
	int PathNodeID;
	int Parent;
	float HCost;
	float GCost;
	bool Teleport;
};

template <class Graph, class CostType, class NodeMap>
class distance_heuristic : public boost::astar_heuristic<Graph, CostType>
{
public:
	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;

	distance_heuristic(NodeMap n, Vertex goal)
		: m_node(n), m_goal(goal) {}
	CostType operator()(Vertex u)
	{
		CostType dx = m_node[m_goal].v.x - m_node[u].v.x;
		CostType dy = m_node[m_goal].v.y - m_node[u].v.y;
		CostType dz = m_node[m_goal].v.z - m_node[u].v.z;
		return ::sqrt(dx * dx + dy * dy + dz * dz);
	}
private:
	NodeMap m_node;
	Vertex m_goal;
};

struct found_goal {};
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor
{
public:
	astar_goal_visitor(Vertex goal) : m_goal(goal) {}
	template <class Graph>
	void examine_vertex(Vertex u, Graph& g) {
		if (u == m_goal)
			throw found_goal();
	}
private:
	Vertex m_goal;
};

typedef boost::geometry::model::point<float, 3, boost::geometry::cs::cartesian> boostPoint;
typedef std::pair<boostPoint, unsigned int> RTreeValue;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_weight_t, float>> GraphType;
typedef boost::property_map<GraphType, boost::edge_weight_t>::type WeightMap;

struct PathfinderWaypoint::Implementation {
	bool PathFileValid;
	std::vector<Node> Nodes;
	std::vector< std::vector< int16 > > path_tree;
	std::vector< std::vector< bool > > teleports;
	int *ClosedListFlag;
	std::string FileName;
	int HeadVersion;
};

PathfinderWaypoint::PathfinderWaypoint(const std::string &path)
{
	m_impl.reset(new Implementation());
	m_impl->PathFileValid = false;
	m_impl->FileName = path;
	m_impl->ClosedListFlag = nullptr;
	Load(path);
}

PathfinderWaypoint::~PathfinderWaypoint()
{
	safe_delete_array(m_impl->ClosedListFlag);
}

IPathfinder::IPath PathfinderWaypoint::FindRoute(const glm::vec3 &start, const glm::vec3 &end, bool &partial, bool &stuck, int flags)
{
	stuck = false;
	partial = false;
	std::vector<RTreeValue> result_start_n;

	int j = FindNearestPathNode(start);
	if (j == -1) {
		IPath Route;
		Route.push_back(start);
		Route.push_back(end);
		return Route;
	}

	int k = FindNearestPathNode(end);
	if (k == -1) {
		IPath Route;
		Route.push_back(start);
		Route.push_back(end);
		return Route;
	}

	std::deque<int>node_list;
	if (m_impl->HeadVersion == 4)
		node_list = FindRouteV4(j, k);
	else
		node_list = FindRouteV2(j, k);

	if (node_list.empty()) {
		IPath Route;
		Route.push_back(start);
		Route.push_back(end);
		return Route;
	}

	auto &nearest_start = node_list.front();
	auto &nearest_end = node_list.back();

	if (nearest_start == nearest_end) {
		IPath Route;
		Route.push_back(start);
		if (m_impl->Nodes[nearest_start].id == nearest_start)
			Route.push_back(m_impl->Nodes[nearest_start].v);
		Route.push_back(end);
		return Route;
	}

	IPath Route;

	Route.push_back(start);
	if (node_list.size() > 0) {
		for (auto &node : node_list) {
			Route.push_back(m_impl->Nodes[node].v);
		}
		node_list.clear();
	}
	Route.push_back(end);
	return Route;

}


IPathfinder::IPath PathfinderWaypoint::FindPath(const glm::vec3& start, const glm::vec3& end, bool& partial, bool& stuck, const PathfinderOptions &opts)
{
	stuck = false;
	partial = false;

	if (Distance(start, end) < 200.0f && zone->zonemap->CheckLoS(start, end)) {
		if (zone->HasWaterMap() && (zone->watermap->InLiquid(start) || zone->IsWaterZone(start.z)) && (zone->watermap->InLiquid(end) || zone->IsWaterZone(end.z))) {
			IPath Route;
			Route.push_back(start);
			Route.push_back(end);
			return Route;
		}
		if (zone->zonemap->NoHazardsAccurate(start, end, 6.0, 40, 5.0)) {
			IPath Route;
			Route.push_back(start);
			Route.push_back(end);
			return Route;
		}
	}

	int j = FindNearestPathNode(start);
	if (j == -1) {
		IPath Route;
		Route.push_back(start);
		Route.push_back(end);
		return Route;
	}

	int k = FindNearestPathNode(end);
	if (k == -1) {
		IPath Route;
		Route.push_back(start);
		Route.push_back(end);
		return Route;
	}

	std::deque<int>node_list;
	if (m_impl->HeadVersion == 4)
		node_list = FindRouteV4(j, k);
	else
		node_list = FindRouteV2(j, k);

	if (node_list.empty()) {
		IPath Route;
		Route.push_back(start);
		Route.push_back(end);
		return Route;
	}

	auto &nearest_start = node_list.front();
	auto &nearest_end = node_list.back();

	if (nearest_start == nearest_end) {
		IPath Route;
		Route.push_back(start);
		if (m_impl->Nodes[nearest_start].id == nearest_start)
			Route.push_back(m_impl->Nodes[nearest_start].v);
		Route.push_back(end);
		return Route;
	}

	IPath Route;

	Route.push_back(start);
	if (node_list.size() > 0) {
		for (auto &node : node_list) {
			Route.push_back(m_impl->Nodes[node].v);
		}
		node_list.clear();
	}
	Route.push_back(end);

	if (Route.size() > 2) {


		int CulledNodes = 0;

		// cull nodes off end
		while ((Route.size() > 2) && (CulledNodes < 2))
		{
			auto First = Route.end();
			--First;
			--First;
			auto Second = First;
			--Second;

			if (!zone->zonemap->LineIntersectsZone(end, Second->pos, 1.0f, nullptr)
				&& zone->zonemap->NoHazardsAccurate(end, Second->pos, 6.0f, 20, 5.0f))
			{
				Route.erase(First);
				++CulledNodes;
			}
			else
				break;
		}

		CulledNodes = 0;
		while ((Route.size() > 2) && (CulledNodes < 2))
		{
			auto First = Route.begin();
			++First;
			auto Second = First;
			++Second;

			if (!zone->zonemap->LineIntersectsZone(start, Second->pos, 1.0f, nullptr)
				&& zone->zonemap->NoHazardsAccurate(start, Second->pos, 6.0f, 20, 5.0f))
			{
				Route.erase(First);
				++CulledNodes;
			}
			else
				break;
		}
	}
		
	return Route;
}

std::deque<int> PathfinderWaypoint::FindRouteV4(int startID, int endID)
{
	std::deque<int>Route;
	int curid = endID;
	int previd = -1;
	Route.push_back(endID);
	bool tele = false;
	while (curid != startID && curid != -1)
	{
		previd = m_impl->path_tree[startID][curid];
		if (previd == -1) {
			Route.clear();
		}
		else {
			if (m_impl->teleports[previd][curid])
				Route.push_front(-1);
			Route.push_front(previd);
		}
		curid = previd;
	}
	return Route;
}

std::deque<int> PathfinderWaypoint::FindRouteV2(int startID, int endID)
{
	std::deque<int>Route;
	// simple case - direct neighbors
	auto &n = m_impl->Nodes[startID];
	for (auto &edge : n.edges) {
		if (edge.first == endID) {
			Route.push_back(startID);
			if (edge.second.teleport)
				Route.push_back(-1);
			Route.push_back(endID);
			return Route;
		}
	}

	memset(m_impl->ClosedListFlag, 0, sizeof(int) * m_impl->Nodes.size());

	std::deque<AStarNode> OpenList, ClosedList;
	AStarNode AStarEntry, CurrentNode;

	AStarEntry.PathNodeID = startID;
	AStarEntry.Parent = -1;
	AStarEntry.HCost = 0;
	AStarEntry.GCost = 0;
	AStarEntry.Teleport = false;

	OpenList.push_back(AStarEntry);

	while (!OpenList.empty())
	{
		// The OpenList is maintained in sorted order, lowest to highest cost.
		CurrentNode = (*OpenList.begin());
		ClosedList.push_back(CurrentNode);
		m_impl->ClosedListFlag[CurrentNode.PathNodeID] = true;

		OpenList.pop_front();

		auto &n = m_impl->Nodes[CurrentNode.PathNodeID];
		for (auto &edge : n.edges) {
			if (edge.first == CurrentNode.Parent)
				continue;

			if (edge.first == endID) {
				Route.push_back(CurrentNode.PathNodeID);
				Route.push_back(endID);

				std::deque<AStarNode>::iterator RouteIterator;
				while (CurrentNode.PathNodeID != startID)
				{
					for (RouteIterator = ClosedList.begin(); RouteIterator != ClosedList.end(); ++RouteIterator)
					{
						if ((*RouteIterator).PathNodeID == CurrentNode.Parent)
						{
							if (CurrentNode.Teleport)
								Route.insert(Route.begin(), -1);

							CurrentNode = (*RouteIterator);
							Route.insert(Route.begin(), CurrentNode.PathNodeID);
							break;
						}
					}
				}
				return Route;
			}
			if (m_impl->ClosedListFlag[edge.first])
				continue;

			AStarEntry.PathNodeID = edge.first;

			AStarEntry.Parent = CurrentNode.PathNodeID;

			AStarEntry.Teleport = edge.second.teleport;

			// HCost is the estimated cost to get from this node to the end.
			AStarEntry.HCost = Distance(m_impl->Nodes[edge.first].v, m_impl->Nodes[endID].v);
			AStarEntry.GCost = CurrentNode.GCost + edge.second.distance;

			float FCost = AStarEntry.HCost + AStarEntry.GCost;
#ifdef PATHDEBUG
			Log(Logs::General, Logs::Pathing, "Node: %i, Open Neighbour %i has HCost %8.3f, GCost %8.3f (Total Cost: %8.3f)\n",
				CurrentNode.PathNodeID,
				PathNodes[CurrentNode.PathNodeID].Neighbours[i].id,
				AStarEntry.HCost,
				AStarEntry.GCost,
				AStarEntry.HCost + AStarEntry.GCost);
#endif

			bool AlreadyInOpenList = false;

			std::deque<AStarNode>::iterator OpenListIterator, InsertionPoint = OpenList.end();

			for (OpenListIterator = OpenList.begin(); OpenListIterator != OpenList.end(); ++OpenListIterator)
			{
				if ((*OpenListIterator).PathNodeID == edge.first)
				{
					AlreadyInOpenList = true;

					float GCostToNode = CurrentNode.GCost + edge.second.distance;

					if (GCostToNode < (*OpenListIterator).GCost)
					{
						(*OpenListIterator).Parent = CurrentNode.PathNodeID;

						(*OpenListIterator).GCost = GCostToNode;

						(*OpenListIterator).Teleport = edge.second.teleport;
					}
					break;
				}
				else if ((InsertionPoint == OpenList.end()) && (((*OpenListIterator).HCost + (*OpenListIterator).GCost) > FCost))
				{
					InsertionPoint = OpenListIterator;
				}
			}
			if (!AlreadyInOpenList)
				OpenList.insert(InsertionPoint, AStarEntry);
		}
	}
	Log(Logs::Detail, Logs::Pathing, "Unable to find a route.");
	return Route;
}

glm::vec3 PathfinderWaypoint::GetRandomLocation(const glm::vec3 &start, int flags)
{
	if (m_impl->Nodes.size() > 0) {

		auto idx = zone->random.Int(0, (int)m_impl->Nodes.size() - 1);
		auto &node = m_impl->Nodes[idx];
		return node.v;
	}
	
	return glm::vec3(0.0f);
}

void PathfinderWaypoint::DebugCommand(Client *c, const Seperator *sep)
{
	if(sep->arg[1][0] == '\0' || !strcasecmp(sep->arg[1], "help"))
	{
		c->Message(CC_Yellow, "This zone is using path nodes from the .path file.");
		c->Message(0, "Syntax: #path shownodes: Spawns a npc to represent every npc node.");
		c->Message(0, "#path reload: Reload the path file.");
		c->Message(0, "#path info: Gives information about node info (requires shownode target).");
		return;
	}
	
	if(!strcasecmp(sep->arg[1], "shownodes"))
	{
		ShowNodes();	
		return;
	}
	
	if (!strcasecmp(sep->arg[1], "reload"))
	{
		Load(m_impl->FileName);
		return;
	}
	
	if (!strcasecmp(sep->arg[1], "info"))
	{
		NodeInfo(c);
		return;
	}
}

void PathfinderWaypoint::Load(const std::string &filename) {

	PathFileHeader Head;
	Head.PathNodeCount = 0;
	Head.version = 2;
	
	FILE *pathfile = fopen(filename.c_str(), "rb");
	if (pathfile) {
		char Magic[10];
		size_t fread_var = 0;
		fread_var = fread(&Magic, 9, 1, pathfile);
	
		if (strncmp(Magic, "EQEMUPATH", 9)) {
			LogError("Bad Magic String in .path file");
			fclose(pathfile);
			return;
		}
		fread_var = fread(&Head, sizeof(Head), 1, pathfile);
	
		Log(Logs::General, Logs::Pathing, "Path File Header: Version %d, PathNodes %d",
			(long)Head.version, (long)Head.PathNodeCount);

		if (Head.version != 2 && Head.version != 3 && Head.version != 4) {
			LogError("Unsupported path file version.");
			fclose(pathfile);
			return;
		}
	
		LoadPath(pathfile, Head);
		return;
	}
}

void PathfinderWaypoint::LoadPath(FILE *PathFile, const PathFileHeader &header)
{
	std::unique_ptr<PathNode[]> PathNodes(new PathNode[header.PathNodeCount]);
	size_t fread_var = 0;
	fread_var = fread(PathNodes.get(), sizeof(PathNode), header.PathNodeCount, PathFile);
	int MaxNodeID = header.PathNodeCount - 1;
	
	m_impl->PathFileValid = true;
	m_impl->Nodes.reserve(header.PathNodeCount);
	for (uint32 i = 0; i < header.PathNodeCount; ++i)
	{
		auto &n = PathNodes[i];
		Node node;
		node.id = i;
		node.v = n.v;
		node.bestz = n.bestz;
		m_impl->Nodes.push_back(node);
	}
	
	for (uint32 i = 0; i < header.PathNodeCount; ++i) {
		for (uint32 j = 0; j < 50; ++j)
		{
			auto &node = m_impl->Nodes[i];
			if (PathNodes[i].Neighbours[j].id > MaxNodeID)
			{
				Log(Logs::General, Logs::Error, "Path Node [%d], Neighbour %d (%d) out of range", i, j, PathNodes[i].Neighbours[j].id);
				m_impl->PathFileValid = false;
			}
	
			if (PathNodes[i].Neighbours[j].id > 0) {
				Edge edge;
				edge.distance = PathNodes[i].Neighbours[j].distance;
				edge.door_id = PathNodes[i].Neighbours[j].DoorID;
				edge.teleport = PathNodes[i].Neighbours[j].Teleport;
	
				node.edges[PathNodes[i].Neighbours[j].id] = edge;
			}
		}
	}
	m_impl->HeadVersion = header.version;
	m_impl->ClosedListFlag = new int[header.PathNodeCount];
	if (m_impl->PathFileValid) {
		LogInfo("Pathfile v{}d loaded.", header.version);
		if (header.version != 4) {
			RecalcDistances();
		}
		ResizePathingVectors();
		if (header.version == 4) {
			for (uint32 i = 0; i < header.PathNodeCount; i++) {
				fread_var = fread(&m_impl->path_tree[i][0], sizeof(int16) * header.PathNodeCount, 1, PathFile);
			}
			// update teleports matrix
			for (auto &node : m_impl->Nodes) {
				for (auto &edge : node.edges) {
					if (edge.second.teleport)
						m_impl->teleports[node.id][edge.first] = true;
				}
			}
		}
	}
	fclose(PathFile);
}

void PathfinderWaypoint::ShowNodes()
{
	for (size_t i = 0; i < m_impl->Nodes.size(); ++i)
	{
		ShowNode(m_impl->Nodes[i]);
	}
}

void PathfinderWaypoint::NodeInfo(Client *c)
{
	if (!c->GetTarget()) {
		return;
	}

	auto node = FindPathNodeByCoordinates(c->GetTarget()->GetX(), c->GetTarget()->GetY(), c->GetTarget()->GetZ());
	if (node == nullptr) {
		return;
	}

	c->Message(0, "Pathing node: %i at (%.2f, %.2f, %.2f) with bestz %.2f",
		node->id, node->v.x, node->v.y, node->v.z, node->bestz);

	for (auto &edge : node->edges) {
		c->Message(0, "id: %i, distance: %.2f, door id: %i, is teleport: %i",
			edge.first,
			edge.second.distance,
			edge.second.door_id,
			edge.second.teleport);
	}
}

Node *PathfinderWaypoint::FindPathNodeByCoordinates(float x, float y, float z)
{
	for (auto &node : m_impl->Nodes) {
		auto dist = Distance(glm::vec3(x, y, z), node.v);

		if (dist < 0.1) {
			return &node;
		}
	}

	return nullptr;
}

void PathfinderWaypoint::RecalcDistances()
{
	// update distances between nodes

	for (auto &node : m_impl->Nodes) {
		for (auto &edge : node.edges) {
			if (edge.first == -1)
				continue;
			auto &neighbor = m_impl->Nodes[edge.first];
			//PathNode* Neighbor = FindPathNodeById(edge.first);
			if (edge.second.teleport)
				edge.second.distance = 0.0f;
			else
				edge.second.distance = Distance(neighbor.v, node.v);
		}
	}
	if (m_impl->HeadVersion == 3)
		m_impl->HeadVersion = 2;
}

void PathfinderWaypoint::ResizePathingVectors()
{
	// this resizes pathing vectors.
	if (m_impl->Nodes.size() > 0) {
		//adjust distance vector size
		m_impl->path_tree.resize(m_impl->Nodes.size());
		m_impl->teleports.resize(m_impl->Nodes.size());
		for (uint32 i = 0; i < m_impl->Nodes.size(); ++i)
		{
			m_impl->path_tree[i].resize(m_impl->Nodes.size());
			m_impl->teleports[i].resize(m_impl->Nodes.size());
		}

		for (uint32 i = 0; i < m_impl->Nodes.size(); ++i)
			for (uint32 j = 0; j < m_impl->Nodes.size(); ++j)
				m_impl->teleports[i][j] = false;


	}
}
void PathfinderWaypoint::Optimize()
{
	// this converts a v2 pathfile to v4
	// SortNodes();
	// ResortConnections();
	ResizePathingVectors();
	if (m_impl->Nodes.size() > 0) {
		std::vector<float> distances;
		distances.resize(m_impl->Nodes.size());
		// initialize pathing tree
		for (uint32 i = 0; i < m_impl->Nodes.size(); i++)
		{
			for (uint32 j = 0; j < m_impl->Nodes.size(); j++) {
				m_impl->path_tree[i][j] = -1;
			}
		}
		// update teleports matrix
		for (auto &node : m_impl->Nodes) {
			for (auto &edge : node.edges) {
				if (edge.second.teleport)
					m_impl->teleports[node.id][edge.first] = true;
			}
		}

		int16 closestnode = -1;
		// calculate distances and paths between nodes
		for (uint32 i = 0; i < m_impl->Nodes.size(); i++) { // i
			memset(m_impl->ClosedListFlag, 0, sizeof(int) * m_impl->Nodes.size());
			for (uint32 j = 0; j < m_impl->Nodes.size(); j++)
				distances[j] = 999999.0f;
			distances[m_impl->Nodes[i].id] = 0.0f;
			int count = 0;
			while (count < m_impl->Nodes.size())
			{
				int mindist = 999999.0f;
				// find closest node
				for (int t = 0; t < m_impl->Nodes.size(); t++) {
					if (!m_impl->ClosedListFlag[m_impl->Nodes[t].id] && mindist >= distances[m_impl->Nodes[t].id]) {
						closestnode = m_impl->Nodes[t].id;
						mindist = distances[closestnode];
					}
				}
				m_impl->ClosedListFlag[closestnode] = true;
				auto node_closest = m_impl->Nodes[closestnode];
				for (auto &edge : node_closest.edges) {
					if (edge.first == -1)
						break;
					if (distances[edge.first] > (distances[closestnode] + edge.second.distance)) {
						distances[edge.first] = distances[closestnode] + edge.second.distance;
						m_impl->path_tree[i][edge.first] = closestnode;
					}
				}
				count++;
			}
		}
		m_impl->HeadVersion = 4;
	}
}


std::string DigitToWord(int i)
{
	std::string digit = std::to_string(i);
	std::string ret;
	for (size_t idx = 0; idx < digit.length(); ++idx) {
		if (!ret.empty()) {
			ret += "_";
		}

		switch (digit[idx]) {
		case '0':
			ret += "Zero";
			break;
		case '1':
			ret += "One";
			break;
		case '2':
			ret += "Two";
			break;
		case '3':
			ret += "Three";
			break;
		case '4':
			ret += "Four";
			break;
		case '5':
			ret += "Five";
			break;
		case '6':
			ret += "Six";
			break;
		case '7':
			ret += "Seven";
			break;
		case '8':
			ret += "Eight";
			break;
		case '9':
			ret += "Nine";
			break;
		default:
			break;
		}
	}

	return ret;
}

void PathfinderWaypoint::ShowNode(const Node &n) 
{
	auto npc_type = new NPCType;
	memset(npc_type, 0, sizeof(NPCType));

	if (n.id < 10) {
		sprintf(npc_type->name, "%s", DigitToWord(n.id).c_str());
	}
	else if (n.id < 100) {
		sprintf(npc_type->name, "%s_%s", DigitToWord(n.id/10).c_str(), DigitToWord(n.id % 10).c_str());
	}
	else if (n.id < 1000) {
		sprintf(npc_type->name, "%s_%s_%s", DigitToWord(n.id / 100).c_str(), DigitToWord((n.id % 100) / 10).c_str(), DigitToWord(((n.id % 100) % 10)).c_str());
	}
	else {
		sprintf(npc_type->name, "%s_%s_%s_%s", DigitToWord(n.id / 1000).c_str(), DigitToWord((n.id % 1000) / 100).c_str(), DigitToWord(((n.id % 1000) % 100) / 10).c_str(), DigitToWord((((n.id % 1000) % 100) % 10)).c_str());
	}

	npc_type->cur_hp = 4000000;
	npc_type->max_hp = 4000000;
	npc_type->race = 151;
	npc_type->gender = 2;
	npc_type->class_ = 9;
	npc_type->deity = 1;
	npc_type->level = 75;
	npc_type->npc_id = 0;
	npc_type->loottable_id = 0;
	npc_type->texture = 1;
	npc_type->light = 0;
	npc_type->runspeed = 0;
	npc_type->d_melee_texture1 = 1;
	npc_type->d_melee_texture2 = 1;
	npc_type->merchanttype = 1;
	npc_type->bodytype = 1;
	npc_type->flymode = 1;

	npc_type->STR = 150;
	npc_type->STA = 150;
	npc_type->DEX = 150;
	npc_type->AGI = 150;
	npc_type->INT = 150;
	npc_type->WIS = 150;
	npc_type->CHA = 150;

	strcpy(npc_type->special_abilities, "19,1^20,1^24,1^35,1");

	auto position = glm::vec4(n.v.x, n.v.y, n.v.z, 0.0f);
	auto npc = new NPC(npc_type, nullptr, position, EQ::constants::Flying);

	entity_list.AddNPC(npc, true, true);
}

auto path_compare = [](const PathNodeSortStruct& a, const PathNodeSortStruct& b)
{
	return a.Distance < b.Distance;
};

int PathfinderWaypoint::FindNearestPathNode(glm::vec3 Position)
{

	// Find the nearest PathNode we have LOS to.
	//
	//
	int result = -1;

	float CandidateNodeRangeXY = RuleR(Pathing, CandidateNodeRangeXY);

	float CandidateNodeRangeZ = RuleR(Pathing, CandidateNodeRangeZ);

	if (zone->GetZoneID() == kedge || (zone->GetZoneID() == powater && Position.z < 0.0f) || (zone->HasWaterMap() && zone->watermap->InLiquid(Position)))
		CandidateNodeRangeZ = 100.0f;

	int ClosestPathNodeToStart = -1;

	std::deque<PathNodeSortStruct> SortedByDistance;

	PathNodeSortStruct TempNode;

	for (auto &node : m_impl->Nodes) {
		if ((std::abs(Position.x - node.v.x) <= CandidateNodeRangeXY) &&
			(std::abs(Position.y - node.v.y) <= CandidateNodeRangeXY) &&
			(std::abs(Position.z - node.v.z) <= CandidateNodeRangeZ)) {
			TempNode.id = node.id;
			TempNode.Distance = DistanceSquared(Position, node.v);
			SortedByDistance.push_back(TempNode);
		}
	}

	std::sort(SortedByDistance.begin(), SortedByDistance.end(), path_compare);
	int haz_check = 0;
	for (auto Iterator = SortedByDistance.begin(); Iterator != SortedByDistance.end(); ++Iterator)
	{
		//Log(Logs::Detail, Logs::Pathing, "Checking Reachability of Node %i from Start Position.", PathNodes[(*Iterator).id].id);

		if (!zone->zonemap->LineIntersectsZone(Position, m_impl->Nodes[(*Iterator).id].v, 1.0f, nullptr))
		{
			// limit how many hazard checks we are doing
			if (haz_check > 5)
				break;
			// check hazards to nearest nodes
			if (zone->zonemap->NoHazardsAccurate(Position, m_impl->Nodes[(*Iterator).id].v, 6.0f, 20, 5.0f)) {
				result = (*Iterator).id;
				break;
			}
			haz_check++;
		}
	}

	return result;
}