#pragma once
#include <vector>
#include "Edge.h"
#include "PlanEdge.h"


class Corner
{

private:
	PlanEdge previousEdge;
	PlanEdge nextEdge;
	glm::vec3 pt;
	int index;
	std::vector<Corner> srcCorners;

public:
	Corner(PlanEdge _previousEdge, PlanEdge _nextEdge, glm::vec3 _pt);
    Corner(PlanEdge _previousEdge, PlanEdge _nextEdge, glm::vec3 _pt, std::vector<Corner> parents);
	Corner(glm::vec3 p, std::vector<Corner> parents);

	//
	void setLeftEdge(PlanEdge e);
	void setRightEdge(PlanEdge e); //TODO setRightEdge
	PlanEdge getRightEdge();
	PlanEdge getLeftEdge();

	glm::vec3 getPt();
	int getIndex(); 
	void setIndex(int ind); 
	std::vector<Corner> getSource();
	void addToSource(Corner c);

	Corner(void);
	~Corner(void);
};

