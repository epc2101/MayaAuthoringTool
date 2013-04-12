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
	void setFloorPlanPercent(float p); 
	int getFloorPlanIndex();
	int getProfileIndex();
	int getProfileNum(); 
	int getIndex(); 
	float getHeight();
	void setHeight(float h); 
	void setTranslation(float x, float y, float z); 
	void setTranslation(glm::vec3 trans);
	glm::vec3 getTranslation();
	void setRotY(float rot); 
	float getRotY(); 
	void setID(int id); 
	int getID(); 
	

private:
	int ID; 
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

