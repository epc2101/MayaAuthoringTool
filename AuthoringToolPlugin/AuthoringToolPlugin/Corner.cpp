#include "Corner.h"


Corner::Corner(void)
{
}

Corner::Corner(PlanEdge _previousEdge, PlanEdge _nextEdge, glm::vec3 _pt)
{
	previousEdge = _previousEdge;
	nextEdge = _nextEdge;
	pt = _pt;

}

Corner::Corner(PlanEdge _previousEdge, PlanEdge _nextEdge, glm::vec3 _pt, std::vector<Corner> parents)
{
	previousEdge = _previousEdge;
	nextEdge = _nextEdge;
	pt = _pt; 
	srcCorners = parents; 
}

PlanEdge Corner::getRightEdge()
{
	return previousEdge;
}

PlanEdge Corner::getLeftEdge()
{
	return nextEdge;
}

glm::vec3 Corner::getPt()
{
	return pt;
}

int Corner::getIndex()
{
	return index;
}

void Corner::setIndex(int _index)
{
	index = _index;
}

std::vector<Corner> Corner::getSource()
{
	return srcCorners;
}

void Corner::addToSource(Corner c)
{
	srcCorners.push_back(c);
}

Corner::~Corner(void)
{
}

Corner::Corner(glm::vec3 p, std::vector<Corner> parents)
{
	pt = p;
	srcCorners = parents;
}
void Corner::setLeftEdge(PlanEdge e)
{
	nextEdge = e;
}
void Corner::setRightEdge(PlanEdge e)
{
	previousEdge = e;
}
