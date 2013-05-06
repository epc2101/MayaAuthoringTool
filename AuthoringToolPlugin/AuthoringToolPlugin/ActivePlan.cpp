#include "ActivePlan.h"
#include <iostream>


ActivePlan::ActivePlan(void)
{
}

ActivePlan::ActivePlan(FloorPlan thePlan)
{
	std::vector<Corner> cornerPlan;
	for (int i = 0; i < thePlan.getNumPoints(); i++)
	{
		PlanEdge e = thePlan.getEdgeList().at(i);
		//TODO - if the edge is a quad- get the profile from the previous edge below.
		e.setProfileType(thePlan.getProfileList().at(i));


		//std::cout<<"The profile index is "<<e.getProfileType()<<std::endl;
		glm::vec3 pt = e.getStartPoint();
		PlanEdge previousEdge, nextEdge;


		if (i == 0) {
			nextEdge = e;
			previousEdge = thePlan.getEdgeList().at(thePlan.getNumPoints()-1);
			previousEdge.setProfileType(thePlan.getProfileList().at(thePlan.getNumPoints()-1));

			previousEdge.setRightCornerIndex(0);
			previousEdge.setLeftCornerIndex(thePlan.getNumPoints()-1);

			nextEdge.setRightCornerIndex(1);
			nextEdge.setLeftCornerIndex(0);

		}  else {
			previousEdge = thePlan.getEdgeList().at(i-1);
			previousEdge.setProfileType(thePlan.getProfileList().at(i-1));


			previousEdge.setLeftCornerIndex(i-1);
			previousEdge.setRightCornerIndex(i);

			nextEdge = e;
			nextEdge.setLeftCornerIndex(i);

			if (i==thePlan.getNumPoints()-1){
				nextEdge.setRightCornerIndex(0);
			}
			else {
				nextEdge.setRightCornerIndex(i+1);

			}
		}
		//std::cout<<"Before adding edges to corner..."<<std::endl;
		//std::cout<<"Previous edge index is "<<previousEdge.getProfileType()<<std::endl;
		//std::cout<<"Next edge index is "<<nextEdge.getProfileType()<<std::endl;


		Corner c = Corner(nextEdge, previousEdge, pt); 
		//std::cout<<"The corner has the edge profile type of: "<<c.getLeftEdge().getProfileType()<<std::endl;

		c.setIndex(i);

		activePlan.push_back(c);
	}

}

void ActivePlan::pruneExcessPoints()
{
	//Somewhat arbitrary number selection right now
	if (activePlan.size() > 4){
		for(int i = 0; i<activePlan.size(); i++){
			Corner c = activePlan.at(i);
			Corner pruneCorner = activePlan.at(c.getRightEdge().getRightCornerIndex());
			Corner testCorner = activePlan.at(pruneCorner.getRightEdge().getRightCornerIndex());

			std::cout<<"Index of the first corner is : "<<c.getIndex()<<std::endl;
			std::cout<<"Index of the prune corner is : "<<pruneCorner.getIndex()<<std::endl;
			std::cout<<"Index of the final corner is : "<<testCorner.getIndex()<<std::endl;
			
			glm::vec3 P = testCorner.getPt();
			glm::vec3 A = pruneCorner.getPt();
			glm::vec3 B = c.getPt();

			float xValue = (P.x-B.x)/(A.x-B.x + pow(10.0,-6));
			float yValue = (P.y-B.y)/(A.y-B.y+ pow(10.0,-6));
			float zValue = (P.z-B.z)/(A.z-B.z+ pow(10.0,-6));

			std::cout<<"The 3 values are x: "<<xValue<<" y: "<<yValue<<" z: "<<zValue<<std::endl;

			float ERROR = 0.01;

			if(c.getSkipped()){
				continue;
			}

			//This means we need to do us some prunin!
			if(abs(xValue-yValue) < ERROR && abs(xValue-zValue) < ERROR && abs(yValue-zValue) < ERROR){
				std::cout<<"Pruned!"<<std::endl;
				//We need to check that it isn't involved in an intersection!
				if (A.x > P.x && B.x < P.x && A.y > P.y && B.y < P.y && A.z > P.z && B.z < P.z){

					if (abs(P.x)-abs(A.x)<ERROR && abs(P.y)-abs(A.y)<ERROR && abs(P.z)-abs(A.z)<ERROR){
						continue;
					}
					if (abs(P.x)-abs(B.x)<ERROR && abs(P.y)-abs(B.y)<ERROR && abs(P.z)-abs(B.z)<ERROR){
						continue;
					}

					//We need to fix the edges connecting them for indices
					PlanEdge rightEdge = activePlan.at(i).getRightEdge();
					rightEdge.setRightCornerIndex(testCorner.getIndex());
					PlanEdge leftEdge = testCorner.getLeftEdge();
					leftEdge.setLeftCornerIndex(i);
					activePlan.at(i).setRightEdge(rightEdge);
					activePlan.at(testCorner.getIndex()).setLeftEdge(leftEdge);

					//Now we need to make sure the extra corner doesn't get in the way
					activePlan.at(pruneCorner.getIndex()).setSkipped(true);
				}
			}

		}
	}

}


/*
Goes through the active plan and updates the edges for what corners they point to
*/
void ActivePlan::updateEdges(std::deque<Corner> &cornPlan)
{
	
	for(int i = 0; i < cornPlan.size(); i++){
		glm::vec3 startPoint, endPoint;
		int profile;
		//tempActivePlan.at(i).getRightEdge().setIndex(i); 

		if (i == cornPlan.size()-1){	
			startPoint = cornPlan.at(i).getPt();
			endPoint = cornPlan.at(0).getPt();
			std::vector<Corner> myCorn = cornPlan.at(i).getSource();
			profile = myCorn.at(myCorn.size()-1).getRightEdge().getProfileType();
			PlanEdge edge = PlanEdge(startPoint,endPoint,profile);

			//Going to try to set the data here...
			edge.setLeftCornerIndex(i);
			edge.setRightCornerIndex(0);

			cornPlan.at(i).setRightEdge(edge); //TODO setRightEdge
			cornPlan.at(0).setLeftEdge(edge);
			//TODO - Deleted map update here...maybe add back?
		} else {
			startPoint = cornPlan.at(i).getPt();
			endPoint= cornPlan.at(i+1).getPt();
			std::vector<Corner> myCorn = cornPlan.at(i).getSource();
			profile = myCorn.at(myCorn.size()-1).getRightEdge().getProfileType();
			PlanEdge edge = PlanEdge(startPoint,endPoint,profile);	

			edge.setLeftCornerIndex(i);
			edge.setRightCornerIndex(i+1);

			cornPlan.at(i).setRightEdge(edge); //TODO setRightEdge
			cornPlan.at(i+1).setLeftEdge(edge);
		}

	}
}

int ActivePlan::getLeftEdgeLeftIndex(int index)
{
	return activePlan.at(index).getLeftEdge().getLeftCornerIndex();
}
int ActivePlan::getLeftEdgeRightIndex(int index)
{
	return activePlan.at(index).getLeftEdge().getRightCornerIndex();
}

int ActivePlan::getRightEdgeLeftIndex(int index)
{
	return activePlan.at(index).getRightEdge().getLeftCornerIndex();
}
int ActivePlan::getRightEdgeRightIndex(int index)
{
	return activePlan.at(index).getRightEdge().getRightCornerIndex();
}
/*
Updates the active plan by passing in the newly created plan.  
*/
ActivePlan::ActivePlan(std::deque<Corner> newPlan)
{
	activePlan = newPlan;
}

void ActivePlan::setCornerMeshIndex(int cornerIndex, int meshIndex)
{
	activePlan.at(cornerIndex).setMeshIndex(meshIndex);
}

void ActivePlan::setCornerIndex(int i)
{
	activePlan.at(i).setIndex(i);
}

ActivePlan::~ActivePlan(void)
{

}
void ActivePlan::updateCornerIndices(){
	for(int i =0; i<activePlan.size(); i++){
		setCornerIndex(i);
	}
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
	//First we loop through the corners and makes sure the indices are fixed
	for(int i = 0; i<cornerPlan.size(); i++){
		cornerPlan.at(i).setIndex(i);
	}
	//Then we go and fix the edges
	for(int i =0; i<cornerPlan.size();i++){
		if (i == cornerPlan.size()-1){	
			glm::vec3 startPoint = cornerPlan.at(i).getPt();
			glm::vec3 endPoint = cornerPlan.at(0).getPt();
			std::vector<Corner> myCorn = cornerPlan.at(i).getSource();
			int profile = myCorn.at(myCorn.size()-1).getRightEdge().getProfileType();
			PlanEdge edge = PlanEdge(startPoint,endPoint,profile);

			//Going to try to set the data here...
			edge.setLeftCornerIndex(i);
			edge.setRightCornerIndex(0);

			cornerPlan.at(i).setRightEdge(edge); //TODO setRightEdge
			cornerPlan.at(0).setLeftEdge(edge);
			//TODO - Deleted map update here...maybe add back?
		} else {
			glm::vec3 startPoint = cornerPlan.at(i).getPt();
			glm::vec3 endPoint = cornerPlan.at(i+1).getPt();
			std::vector<Corner> myCorn = cornerPlan.at(i).getSource();
			int profile = myCorn.at(myCorn.size()-1).getRightEdge().getProfileType();
			PlanEdge edge = PlanEdge(startPoint,endPoint,profile);	

			edge.setLeftCornerIndex(i);
			edge.setRightCornerIndex(i+1);

			cornerPlan.at(i).setRightEdge(edge); //TODO setRightEdge
			cornerPlan.at(i+1).setLeftEdge(edge);
			//TODO - Deleted map update here...maybe add back?
		} 
	
	}
	
	for(int i = 0; i < cornerPlan.size(); i++){
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

void ActivePlan::setCornerLeft(int index, bool value)
{
	activePlan.at(index).setLeftMesh(value);
	//std::cout<<"The new values of the corner's left is: "<<activePlan.at(index).getLeftMesh()<<std::endl;
}
void ActivePlan::setCornerRight(int index, bool value)
{
	activePlan.at(index).setRightMesh(value);
	//std::cout<<"The new values of the corner's right is: "<<activePlan.at(index).getRightMesh()<<std::endl;
}