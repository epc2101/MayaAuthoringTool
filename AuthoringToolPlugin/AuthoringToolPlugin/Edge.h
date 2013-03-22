#pragma once

#include "glm/glm.hpp"
class Edge
{
protected:
	glm::vec3 startPoint;
	glm::vec3 endPoint;
	glm::vec3 anchor;
	int anchorType ;
	
	


public:
	//Constructor
	Edge(void);
	Edge(glm::vec3 start, glm::vec3 end); 
	
	Edge(glm::vec3 start, glm::vec3 end, int a);
	
	//No setting of this after construction so only getters
	//Possibly use this when creating the mesh object so this notion may have to be revisited
	glm::vec3 getStartPoint();
	glm::vec3 getEndPoint();
	glm::vec3 getAnchorPoint();
	int getAnchorType();
	virtual ~Edge(void);
};

