#include "Corner.h"


Corner::Corner(void)
{
}

Corner::Corner(Edge _previousEdge, Edge _nextEdge, glm::vec3 _pt)
{

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

Corner::~Corner(void)
{
}
