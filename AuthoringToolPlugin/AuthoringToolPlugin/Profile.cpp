#include "Profile.h"


Profile::Profile(void)
{
}


Profile::~Profile(void)
{
}

Profile::Profile(std::vector<Edge> e, int length){
	edgeList = e;
	numPoints = length;
}

//Getters - Don't expect to be modifying these fields in the future
std::vector<Edge> Profile::getEdgeList(){
	return edgeList;
}
int Profile::getNumPoints(){
	return numPoints;
}