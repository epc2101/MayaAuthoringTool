#pragma once
#include "Corner.h"
#include "Edge.h"
#include "FloorPlan.h"
#include <deque>
#include <vector>
#include <map>	

class ActivePlan
{
private:
	std::deque<Corner> activePlan;

	//These are the vectors that come from each corner (used to find interections and locations of edge direction change)
	std::vector<glm::vec3> intersectionVectors;


public:
	std::map<int, Anchor> edgeAnchorMap;
	std::deque<Corner> getActivePlan();
	void setIndexNums(); 
	ActivePlan(FloorPlan thePlan);
	ActivePlan(std::deque<Corner> newPlan); 

	std::vector<glm::vec3> getIntersectionVectors();
	void setIntersectionVector(glm::vec3 v);
	void cleanIntersectionVectors();
	std::vector<PlanEdge> getEdgeList(); 
	void setCornerMeshIndex(int cornerIndex, int meshIndex);
	void setCornerIndex(int i);
	void updateCornerIndices();


	int getLeftEdgeLeftIndex(int index);
	int getLeftEdgeRightIndex(int index);

	int getRightEdgeLeftIndex(int index);
	int getRightEdgeRightIndex(int index);

	std::vector<Corner> cleanPrunedPlan();

	void pruneExcessPoints();

	void setCornerLeft(int index, bool value);
	void setCornerRight(int index, bool value);

	ActivePlan(std::vector<Corner> cornerPlan);
	ActivePlan(void);
	~ActivePlan(void);

	//New Methods to update the mesh creation
	void updateEdges(std::deque<Corner> &cornPlan);

	void setActivePlan(std::vector<Corner> corn);

	bool determineShittyPoints(float height);
	std::vector<Corner> removeShittyPoints();
};

