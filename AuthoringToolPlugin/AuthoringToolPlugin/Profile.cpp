#include "Profile.h"
#include <iostream>


Profile::Profile(void)
{
}


Profile::~Profile(void)
{
}

Profile::Profile(std::vector<Edge> e, int length){
	
	
	edgeList = normalizeProfile(e);
	numPoints = length;
}

//Getters - Don't expect to be modifying these fields in the future
std::vector<Edge> Profile::getEdgeList(){
	return edgeList;
}
int Profile::getNumPoints(){
	return numPoints;
}

/*
This translate the points on the 2d profile to come from the origin (this is important for accurate height detection)
*/
std::vector<Edge> Profile::normalizeProfile(std::vector<Edge> e)
{
	//We get the first point to see it's distance from the origin
	Edge edge = e.at(0);
	std::vector<Edge> tempEdgeList;

	glm::vec3 start = edge.getStartPoint();
	float heightChange = start.y;
	float transChange = start.x;
	std::cout<<"Height change is: "<<heightChange<<std::endl;

	for(int i = 0; i<e.size(); i++){
		Edge temp = e.at(i);
		glm::vec3 startPoint = temp.getStartPoint();
		glm::vec3 endPoint = temp.getEndPoint();
		int anchor = temp.getAnchorType();
		

		startPoint.x -= transChange;
		startPoint.y -= heightChange;

		endPoint.x -= transChange;
		endPoint.y -= heightChange;

		Edge newEdge = Edge(startPoint,endPoint,anchor,-1);
		tempEdgeList.push_back(newEdge);	
	}
	
	return tempEdgeList;
}