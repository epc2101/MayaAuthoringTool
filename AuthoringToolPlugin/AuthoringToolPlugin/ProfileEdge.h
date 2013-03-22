#pragma once
#include "edge.h"
class ProfileEdge :
	public Edge
{
	//These parameters are specific to the profile edge(knowing which one is the top one, whether it is horizontal
private:
	bool isTop;
	bool isHorizontal;

public:
	ProfileEdge(void);
	//Since we should only be constructing this one during the initial parse - this should take care of itself
	ProfileEdge(glm::vec3 _startPoint, glm::vec3 _endPoint, bool _isTop, bool _isHorizontal, int _anchorType);
	
	
	~ProfileEdge(void);

	//Getters and setters
	bool getIsTop();
	void setIsTop(bool b);

	bool getIsHorizontal();
	void setIsHorizontal(bool b);


};

