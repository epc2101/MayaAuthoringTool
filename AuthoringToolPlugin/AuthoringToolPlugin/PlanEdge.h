#pragma once
#include "Edge.h"
#include "Anchor.h"
#include <vector>

class PlanEdge :
	public Edge
{
	//These fields are specific to the kind of edge only used in floorPlans
private:
	int profileType;

public:
	//Various constructors
	PlanEdge(void);
	PlanEdge(glm::vec3 _startPoint, glm::vec3 _endPoint, int _profileType);

	//Note - this is likely the one that we will always use in this case
	PlanEdge(glm::vec3 _startPoint, glm::vec3 __endPoint, int profileType, int _anchorType);

	//Getters/Setters
	int getProfileType();
	void setProfileType(int p);

	std::vector<Anchor> edgeAnchors;

	~PlanEdge(void);
};

