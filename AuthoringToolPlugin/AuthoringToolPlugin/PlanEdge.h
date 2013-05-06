#pragma once
#include "Edge.h"

#include <vector>


class PlanEdge :
	public Edge
{
	//These fields are specific to the kind of edge only used in floorPlans
private:
	int profileType;
	int index; 
	int leftCornerIndex;
	int rightCornerIndex;

public:
	//Various constructors
	PlanEdge(void);
	PlanEdge(glm::vec3 _startPoint, glm::vec3 _endPoint, int _profileType);

	//Note - this is likely the one that we will always use in this case
	PlanEdge(glm::vec3 _startPoint, glm::vec3 __endPoint, int profileType, int _anchorType);

	//Getters/Setters
	int getProfileType();
	void setProfileType(int p);

	int getLeftCornerIndex();
	int getRightCornerIndex();
	void setLeftCornerIndex(int l);
	void setRightCornerIndex(int r);
	//********************************
	//Maybe not using this
	std::vector<Anchor> getAnchors(); 
	void addAnchor(Anchor &a);
	void setAnchors(std::vector<Anchor> a);
	//********************************
	void setIndex(int index); 
	int getIndex(); 
	//bool operator()( PlanEdge const& lhs, PlanEdge const& rhs ) const; 

	void setStartPoint(glm::vec3 start);

	void setEndPoint(glm::vec3 end);

	~PlanEdge(void);
};

