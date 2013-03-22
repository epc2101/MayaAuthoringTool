
#include "Edge.h"


Edge::Edge(void)
{
}

//Constructors
Edge::~Edge(void)
{
}

Edge::Edge(glm::vec3 start, glm::vec3 end, int a)
{
	startPoint = start;
	endPoint = end;
	anchorType = a;
	

	//Interpolate the point between the start and end point
	anchor = glm::vec3((start.x + end.x)/2.0, (start.y + end.y)/2.0, (start.z + end.z)/2.0);
}


	
Edge::Edge(glm::vec3 start, glm::vec3 end)
{
	startPoint = start;
	endPoint = end;
	anchorType = -1;
	

	anchor = glm::vec3((start.x + end.x)/2.0, (start.y + end.y)/2.0, (start.z + end.z)/2.0);
}
//Getters
glm::vec3 Edge::getStartPoint(){
	return startPoint;
}
glm::vec3 Edge::getEndPoint(){
	return endPoint;
}
glm::vec3 Edge::getAnchorPoint(){
	return anchor;
}
int Edge::getAnchorType(){
	return anchorType;
}


