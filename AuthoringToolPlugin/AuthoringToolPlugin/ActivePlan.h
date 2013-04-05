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

	ActivePlan(std::vector<Corner> cornerPlan);
	ActivePlan(void);
	~ActivePlan(void);
};

