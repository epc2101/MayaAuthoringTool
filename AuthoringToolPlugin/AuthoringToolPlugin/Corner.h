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

	//These will be used in generating the mesh from the active plan
	bool rightEdgeMesh;
	bool leftEdgeMesh;
	int meshIndex;

	bool skipped;
	bool isShitty;


public:
	Corner(PlanEdge _previousEdge, PlanEdge _nextEdge, glm::vec3 _pt);
    Corner(PlanEdge _previousEdge, PlanEdge _nextEdge, glm::vec3 _pt, std::vector<Corner> parents);
	Corner(glm::vec3 p, std::vector<Corner> parents);

	//
	void setLeftEdge(PlanEdge e);
	void setRightEdge(PlanEdge e); //TODO setRightEdge
	PlanEdge getRightEdge();
	PlanEdge getLeftEdge();

	void setLeftEdgeIndex(int i);
	void setRightEdgeIndex(int i);

	glm::vec3 getPt();
	int getIndex(); 
	void setIndex(int ind); 
	std::vector<Corner> getSource();
	void addToSource(Corner c);
	void addSourceToFront(Corner c);

	int getMeshIndex();
	void setMeshIndex(int i);

	bool getRightMesh();
	bool getLeftMesh();
	void setRightMesh(bool b);
	void setLeftMesh(bool b);

	bool getSkipped();
	void setSkipped(bool b);

	bool getIsShitty();
	void setIsShitty(bool b);

	Corner(void);
	~Corner(void);
};

