#pragma once
#include "Corner.h"
#include "Edge.h"
#include "FloorPlan.h"
#include <deque>
#include <vector>

class ActivePlan
{
private:
	std::deque<Corner> activePlan;
	std::vector<glm::vec3> intersectionVectors;

public:
	std::deque<Corner> getActivePlan();
	void setIndexNums(); 
	ActivePlan(FloorPlan thePlan);
	ActivePlan(std::deque<Corner> newPlan); 

	std::vector<glm::vec3> getIntersectionVectors();
	void setIntersectionVector(glm::vec3 v);

	ActivePlan(std::vector<Corner> cornerPlan);
	ActivePlan(void);
	~ActivePlan(void);
};

