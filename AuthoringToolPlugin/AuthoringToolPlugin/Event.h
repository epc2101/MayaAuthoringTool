#pragma once
#include "Corner.h"

class Event
{
public:
enum EventType  {INTERSECTION, PROFILE, ANCHOR, OVERHANG };

private:
	float height;
	glm::vec3 point;
	std::vector<Corner> sourceCorners;
	
	EventType eventType;

public:

	
	Event(void);
	Event(float h, glm::vec3 p, std::vector<Corner> source, EventType t);
	Event(float h, glm::vec3 p, EventType t); //Used for anchors
	float getHeight();
	std::vector<Corner> getCorners();
	glm::vec3 getPoint();
	EventType getType();

	
	~Event(void);
};

