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

//Constructs the inputplan as a double linked list
void FloorPlan::buildInputPlan()
{
	for(int i = 0; i<edgeList.size(); i++){
		inputPlan.push_back(edgeList.at(i).getStartPoint());
		if (i == (edgeList.size()-1)){
			inputPlan.push_back(edgeList.at(i).getEndPoint());
		}
	}
}