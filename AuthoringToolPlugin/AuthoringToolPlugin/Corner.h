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
    Corner(Edge _previousEdge, Edge _nextEdge, glm::vec3 _pt, std::vector<Corner> parents);
	Corner(glm::vec3 p, std::vector<Corner> parents);

	//
	void setNextEdge(Edge e);
	void setPreviousEdge(Edge e);

	Edge getPreviousEdge();
	Edge getNextEdge();
	glm::vec3 getPt();
	int getIndex(); 
	void setIndex(int ind); 
	std::vector<Corner> getSource();
	void addToSource(Corner c);

	Corner(void);
	~Corner(void);
};

