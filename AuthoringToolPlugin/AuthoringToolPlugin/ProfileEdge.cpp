#include "ProfileEdge.h"


ProfileEdge::ProfileEdge(void)
{
}


ProfileEdge::~ProfileEdge(void)
{
}

//Since we should only be constructing this one during the initial parse - this should take care of itself
ProfileEdge::ProfileEdge(glm::vec3 _startPoint, glm::vec3 _endPoint, bool _isTop, bool _isHorizontal, int _anchorType)
{
	startPoint = _startPoint;
	endPoint = _endPoint;
	isTop = _isTop;
	isHorizontal = _isHorizontal;
	anchorType = _anchorType;
}

//Getters and setters
bool ProfileEdge::getIsTop()
{
	return isTop;
}
void ProfileEdge::setIsTop(bool b)
{
	isTop = b;
}

bool ProfileEdge::getIsHorizontal()
{
	return isHorizontal;
}

void ProfileEdge::setIsHorizontal(bool b)
{
	isHorizontal = b;
}