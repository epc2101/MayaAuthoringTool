#pragma once
#include <vector>
#include "Edge.h"


class Corner
{

private:
	Edge previousEdge;
	Edge nextEdge;
	glm::vec3 pt;
	int index;
	std::vector<Corner> srcCorners;

public:
	Corner(Edge _previousEdge, Edge _nextEdge, glm::vec3 _pt);
	Edge getPreviousEdge();
	Edge getNextEdge();
	glm::vec3 getPt();
	int getIndex(); 
	void setIndex(int ind); 
	std::vector<Corner> getSource();

	Corner(void);
	~Corner(void);
};

