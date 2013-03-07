#include "FloorPlan.h"



FloorPlan::FloorPlan(void)
{
}


FloorPlan::~FloorPlan(void)
{
}

FloorPlan::FloorPlan(std::vector<Edge> e, int length, std::vector<int> p){
	edgeList = e;
	numPoints = length;
	profileList = p;
}

//Getters - not likely that these will need to be reset in this context
std::vector<Edge> FloorPlan::getEdgeList(){
	return edgeList;
}
int FloorPlan::getNumPoints(){
	return numPoints;
}
std::vector<int> FloorPlan::getProfileList(){
	return profileList;
}