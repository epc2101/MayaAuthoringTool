#include "ActivePlan.h"
#include <iostream>


ActivePlan::ActivePlan(void)
{
}

ActivePlan::ActivePlan(FloorPlan thePlan)
{
	for (int i = 0; i < thePlan.getNumPoints(); i++)
	{
		PlanEdge e = thePlan.getEdgeList().at(i);
		//TODO - if the edge is a quad- get the profile from the previous edge below.
		e.setProfileType(thePlan.getProfileList().at(i));


		//std::cout<<"The profile index is "<<e.getProfileType()<<std::endl;
		glm::vec3 pt = e.getStartPoint();
		PlanEdge previousEdge, nextEdge;

		if (i == 0) {
			previousEdge = thePlan.getEdgeList().at(thePlan.getNumPoints()-1);
			previousEdge.setProfileType(thePlan.getProfileList().at(thePlan.getNumPoints()-1));
			nextEdge = e;
		}  else {
			previousEdge = thePlan.getEdgeList().at(i-1);
			previousEdge.setProfileType(thePlan.getProfileList().at(i-1));
			nextEdge = e;
		}
		//std::cout<<"Before adding edges to corner..."<<std::endl;
		//std::cout<<"Previous edge index is "<<previousEdge.getProfileType()<<std::endl;
		//std::cout<<"Next edge index is "<<nextEdge.getProfileType()<<std::endl;


		Corner c = Corner(previousEdge, nextEdge, pt); 
		//std::cout<<"The corner has the edge profile type of: "<<c.getLeftEdge().getProfileType()<<std::endl;

		c.setIndex(i);
		activePlan.push_back(c);
	}

}

/*
Updates the active plan by passing in the newly created plan.  
*/
ActivePlan::ActivePlan(std::deque<Corner> newPlan)
{
	activePlan = newPlan;
}

ActivePlan::~ActivePlan(void)
{

}

std::deque<Corner> ActivePlan::getActivePlan()
{
	return activePlan;
}

std::vector<glm::vec3> ActivePlan::getIntersectionVectors()
{
	return intersectionVectors;
}

void ActivePlan::setIntersectionVector(glm::vec3 v)
{
	intersectionVectors.push_back(v);
}
void ActivePlan::setIndexNums()
{
	for (int i = 0; i < activePlan.size(); i++)
	{
		activePlan.at(i).setIndex(i);
	}
}

ActivePlan::ActivePlan(std::vector<Corner> cornerPlan){
	for(int i = 0; i < cornerPlan.size(); i++){
		cornerPlan.at(i).setIndex(i);
		activePlan.push_back(cornerPlan.at(i));
	}
}

void ActivePlan::cleanIntersectionVectors()
{
	intersectionVectors.clear();
}

std::vector<PlanEdge> ActivePlan::getEdgeList()
{
	std::vector<PlanEdge> edgeList;
	for (int i = 0; i < activePlan.size(); i++)
	{
		edgeList.push_back(activePlan.at(i).getRightEdge()); 
	}
	return edgeList;
}