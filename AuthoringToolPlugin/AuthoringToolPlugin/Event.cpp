#include "Event.h"


Event::Event(void)
{
}


Event::~Event(void)
{
}

Event::Event(float h, glm::vec3 p, std::vector<Corner> source, EventType t)
{
	height = h;
	point = p;
	sourceCorners = source;
	eventType = t;
}

Event::Event(float h, glm::vec3 p, EventType t, int ai)
{
	height = h;
	point = p;
	eventType = t;
	anchorIndex = ai; 
}

float Event::getHeight()
{
	return height;
}
std::vector<Corner> Event::getCorners()
{
	return sourceCorners;
}
glm::vec3 Event::getPoint()
{
	return point;
}

Event::EventType Event::getType()
{
	return eventType;
}