#include "Profile.h"
#include <iostream>
float DIFF = 0.001;

Profile::Profile(void)
{
}


Profile::~Profile(void)
{
}

Profile::Profile(std::vector<ProfileEdge> e, int length){
	
	
	edgeList = normalizeProfile(e);
	numPoints = length;
}

//Getters - Don't expect to be modifying these fields in the future
std::vector<ProfileEdge> Profile::getEdgeList(){
	return edgeList;
}
int Profile::getNumPoints(){
	return numPoints;
}

/*
This translate the points on the 2d profile to come from the origin (this is important for accurate height detection)
*/
std::vector<ProfileEdge> Profile::normalizeProfile(std::vector<ProfileEdge> e)
{
	//We get the first point to see it's distance from the origin
	ProfileEdge edge = e.at(0);
	std::vector<ProfileEdge> tempEdgeList;

	glm::vec3 start = edge.getStartPoint();
	float heightChange = start.y;
	float transChange = start.x;
	std::cout<<"Height change is: "<<heightChange<<std::endl;

	for(int i = 0; i<e.size(); i++){
		ProfileEdge temp = e.at(i);
		glm::vec3 startPoint = temp.getStartPoint();
		glm::vec3 endPoint = temp.getEndPoint();
		int anchor = temp.getAnchorType();
		

		startPoint.x -= transChange;
		startPoint.y -= heightChange;

		endPoint.x -= transChange;
		endPoint.y -= heightChange;

		//Check if horizontal and if it is the topmost point
		bool isTop, isHorizontal;

		if (i == e.size()-1){
			isTop = true;
		}
		else {
			isTop = false;
		}

		if((startPoint.y - endPoint.y) < DIFF){
			isHorizontal = true;
		}
		else{
			isHorizontal = false;
		}

		ProfileEdge newEdge = ProfileEdge(startPoint,endPoint,isTop,isHorizontal,anchor);
		tempEdgeList.push_back(newEdge);	
	}
	
	return tempEdgeList;
}