#include "PlanEdge.h"


PlanEdge::PlanEdge(void)
{
}


PlanEdge::~PlanEdge(void)
{
}


PlanEdge::PlanEdge(glm::vec3 _startPoint, glm::vec3 _endPoint, int _profileType)
{
	startPoint = _startPoint;
	endPoint = _endPoint;
	profileType = _profileType;
	anchorType = -1;


}

	//Note - this is likely the one that we will always use in this case
PlanEdge::PlanEdge(glm::vec3 _startPoint, glm::vec3 _endPoint, int _profileType, int _anchorType)
{
	startPoint = _startPoint;
	endPoint = _endPoint;
	profileType = _profileType;
	anchorType = _anchorType;
}

	//Getters/Setters
int PlanEdge::getProfileType()
{
	return profileType;

}

void PlanEdge::setProfileType(int p)
{
	profileType = p;
}

std::vector<Anchor> PlanEdge::getAnchors()
{
	return anchorsFp;
}

void PlanEdge::addAnchor(Anchor a)
{
	anchorsFp.push_back(a);
}

void PlanEdge::setAnchors(std::vector<Anchor> a)
{
	anchorsFp = a;
}