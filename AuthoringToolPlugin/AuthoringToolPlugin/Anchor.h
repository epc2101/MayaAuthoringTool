#pragma once

#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include "glm/gtc/matrix_transform.hpp"

class Anchor
{
public:
	Anchor(void);
	Anchor(int i, float edgeNumPercent, int profile, float profileNumPercent); 	
	~Anchor(void);

	//Getters & setters
	float getFloorPlanPercent();
	float getProfilePercent();
	int getFloorPlanIndex();
	int getProfileIndex();
	int getProfileNum(); 
	int getIndex(); 
	float getHeight();
	void setHeight(float h); 
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
	int index; 
	//Calculated from the profile
	float height; 

	//Updated once we have done sweep plane
	glm::vec3 translation; 
	float rotY; 
};

