#include "Anchor.h"

int A_DEBUG = 1;
Anchor::Anchor(void)
{
}


Anchor::~Anchor(void)
{
}

Anchor::Anchor(int i, float edgeNumPercent, int profile, float profileNumPercent, std::vector<Profile>profiles)
{
	index = i; 
	floorPlanPercent = edgeNumPercent - (int) edgeNumPercent; 
	floorPlanIndex = (int) edgeNumPercent; 

	profilePercent = profileNumPercent - (int) profileNumPercent; 
	profileIndex = (int) profileNumPercent;
	profileNum = profile; 

	//Calc the height from the profile
	Profile p = profiles.at(i); 
	ProfileEdge e = p.getEdgeList().at(profileIndex);
	glm::vec3 dir = e.getEndPoint() - e.getStartPoint();
	height = (e.getStartPoint() + dir * profilePercent).y; 

	if (A_DEBUG == 1)
	{
		std::cout<<"Floor plan index: "<<floorPlanIndex<<" Percent: "<<floorPlanPercent<<std::endl;
		std::cout<<"Profile index: "<<profileIndex<<" Percent: "<<profilePercent<<std::endl;
	}
}

//Getters & setters
int Anchor::getIndex()
{
	return index; 
}

float Anchor::getFloorPlanPercent()
{
	return floorPlanPercent; 
}

float Anchor::getProfilePercent()
{
	return profilePercent;
}

int Anchor::getFloorPlanIndex()
{
	return floorPlanIndex; 
}

int Anchor::getProfileIndex()
{
	return profileIndex; 
}

int Anchor::getProfileNum()
{
	return profileNum; 
}

float Anchor::getHeight()
{
	return height;
}

void Anchor::setTranslation(float x, float y, float z)
{
	translation.x = x;
	translation.y = y;
	translation.z = z; 
}

void Anchor::setTranslation(glm::vec3 trans)
{
	translation = trans; 
}


void Anchor::setRotY(float rot)
{
	rotY = rot; 
}
