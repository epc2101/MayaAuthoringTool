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
	std::cout<<"Getting anchor list of size: "<<anchorsFp.size()<<std::endl;
	return anchorsFp;
}

void PlanEdge::addAnchor(Anchor &a)
{
	anchorsFp.push_back(a);
	std::cout<<"Pushing back anchor. New size: "<<anchorsFp.size()<<std::endl;
}

void PlanEdge::setAnchors(std::vector<Anchor> a)
{
	std::cout<<"In setting anchors"<<std::endl;
	anchorsFp = a;
}

void PlanEdge::setIndex(int i)
{
	index = i; 
}

int PlanEdge::getIndex()
{
	return index; 
}

//bool PlanEdge::operator()( PlanEdge const& lhs, PlanEdge const& rhs ) const
//{
//	
//	if (lhs.getIndex() < rhs.getIndex())
//		return true; 
//}