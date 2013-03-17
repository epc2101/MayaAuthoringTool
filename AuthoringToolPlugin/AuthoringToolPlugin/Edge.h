#pragma once

#include "glm/glm.hpp"
class Edge
{
private:
	glm::vec3 startPoint;
	glm::vec3 endPoint;
	glm::vec3 anchor;
	int anchorType ;
	int profileType;
	


public:
	//Constructor
	Edge(void);
	Edge(glm::vec3 start, glm::vec3 end); 
	Edge(glm::vec3 start, glm::vec3 end, int prof); 
	Edge(glm::vec3 start, glm::vec3 end, int a, int prof);
	
	//No setting of this after construction so only getters
	//Possibly use this when creating the mesh object so this notion may have to be revisited
	glm::vec3 getStartPoint();
	glm::vec3 getEndPoint();
	glm::vec3 getAnchorPoint();
	int getProfileType();
	void setProfileType(int p);
	int getAnchorType();
	virtual ~Edge(void);
};

