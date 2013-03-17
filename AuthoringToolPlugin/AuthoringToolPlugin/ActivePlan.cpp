#include "ActivePlan.h"


ActivePlan::ActivePlan(void)
{
}

ActivePlan::ActivePlan(FloorPlan thePlan)
{
	for (int i = 0; i < thePlan.getNumPoints(); i++)
	{
		Edge e = thePlan.getEdgeList().at(i);
		//TODO - if the edge is a quad- get the profile from the previous edge below.
		e.setProfileType(thePlan.getProfileList().at(i));

		glm::vec3 pt = e.getStartPoint();
		Edge previousEdge, nextEdge;

		if (i == 0) {
			previousEdge = thePlan.getEdgeList().at(thePlan.getNumPoints()-1);
			nextEdge = e;
		}  else {
			previousEdge = thePlan.getEdgeList().at(i-1);
			nextEdge = e;
		}

		Corner c = Corner(previousEdge, nextEdge, pt); 
		activePlan.push_back(c);
	}
}

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

void ActivePlan::setIndexNums()
{
	for (int i = 0; i < activePlan.size(); i++)
	{
		activePlan.at(i).setIndex(i);
	}
}

