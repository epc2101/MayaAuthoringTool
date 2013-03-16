
#include "Edge.h"


Edge::Edge(void)
{
}

//Constructors
Edge::~Edge(void)
{
}

Edge::Edge(glm::vec3 start, glm::vec3 end, int a, int prof)
{
	startPoint = start;
	endPoint = end;
	anchorType = a;
	profileType = prof;

	//Interpolate the point between the start and end point
	anchor = glm::vec3((start.x + end.x)/2.0, (start.y + end.y)/2.0, (start.z + end.z)/2.0);
}

Edge::Edge(glm::vec3 start, glm::vec3 end, int prof)
{
	startPoint = start;
	endPoint = end;
	anchorType = -1;
	profileType = prof;
}
	
Edge::Edge(glm::vec3 start, glm::vec3 end)
{
	startPoint = start;
	endPoint = end;
	anchorType = -1;
	profileType = -1;

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

int Edge::getProfileType(){
	return profileType;
}
void Edge::setProfileType(int p)
{
	profileType = p;
}
