#pragma once
#include "PlanEdge.h"
#include <vector>


class FloorPlan
{
private:
	//This contains data for the points, anchortypes and anchor positions
	std::vector<PlanEdge> edgeList;
	
	int numPoints;
	//TODO: Will replace this with a pointer to a profile object when that class is complete
	std::vector<int> profileList;
	

public:
	//Constructors - vectors will be pre-created in main compute function before being passed in
	FloorPlan(void);
	FloorPlan(std::vector<PlanEdge> e, int length, std::vector<int> p);

	//Getters - not likely that these will need to be reset in this context
	std::vector<PlanEdge> getEdgeList();
	int getNumPoints();
	std::vector<int> getProfileList();

	~FloorPlan(void);
};

