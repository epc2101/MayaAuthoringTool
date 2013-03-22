#pragma once

#include "ProfileEdge.h"
#include <vector>

class Profile
{
private:
	std::vector<ProfileEdge> edgeList;
	int numPoints;

public:
	//Constructors
	Profile(void);
	Profile(std::vector<ProfileEdge> e, int length);

	//Getters - Don't expect to be modifying these fields in the future
	std::vector<ProfileEdge> getEdgeList();
	int getNumPoints();
	
	//During profile creation, this shifts the points over
	std::vector<ProfileEdge> normalizeProfile(std::vector<ProfileEdge> e);



	~Profile(void);
};

