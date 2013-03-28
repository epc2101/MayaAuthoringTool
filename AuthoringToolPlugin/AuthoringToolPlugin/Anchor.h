#pragma once

#include "glm/glm.hpp"
#include <iostream>

 

class Anchor
{
public:
	Anchor(void);
	Anchor(float edgeNumPercent, int profile, float profileNumPercent); 	
	~Anchor(void);

	//Getters & setters
	float getFloorPlanPercent();
	float getProfilePercent();
	int getFloorPlanIndex();
	int getProfileIndex();
	int getProfileNum(); 

	void setTranslation(float x, float y, float z); 
	void setTranslation(glm::vec3 trans);
	void setRotY(float rot); 

private:
	//Taken in directly from Maya
	float floorPlanPercent; 
	int floorPlanIndex; 
	int profileNum;
	float profilePercent; 
	int profileIndex; 

	//Updated once we have done sweep plane
	glm::vec3 translation; 
	float rotY; 
};

