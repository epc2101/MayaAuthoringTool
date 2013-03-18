#include "Corner.h"


Corner::Corner(void)
{
}

Corner::Corner(Edge _previousEdge, Edge _nextEdge, glm::vec3 _pt)
{
	previousEdge = _previousEdge;
	nextEdge = _nextEdge;
	pt = _pt;

}

Corner::Corner(Edge _previousEdge, Edge _nextEdge, glm::vec3 _pt, std::vector<Corner> parents)
{
	previousEdge = _previousEdge;
	nextEdge = _nextEdge;
	pt = _pt; 
	srcCorners = parents; 
}

Edge Corner::getPreviousEdge()
{
	return previousEdge;
}

Edge Corner::getNextEdge()
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
void Corner::setNextEdge(Edge e)
{
	nextEdge = e;
}
void Corner::setPreviousEdge(Edge e)
{
	previousEdge = e;
}
