#pragma once

#include "Edge.h"
#include <vector>

class Profile
{
private:
	std::vector<Edge> edgeList;
	int numPoints;

public:
	//Constructors
	Profile(void);
	Profile(std::vector<Edge> e, int length);

	//Getters - Don't expect to be modifying these fields in the future
	std::vector<Edge> getEdgeList();
	int getNumPoints();
	
	//During profile creation, this shifts the points over
	std::vector<Edge> normalizeProfile(std::vector<Edge> e);



	~Profile(void);
};

