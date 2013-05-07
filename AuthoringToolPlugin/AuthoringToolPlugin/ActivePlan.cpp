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
	if (activePlan.size() >= 4){
		for(int i = 0; i<activePlan.size(); i++){
			Corner c = activePlan.at(i);
			Corner pruneCorner = activePlan.at(c.getRightEdge().getRightCornerIndex());
			Corner testCorner = activePlan.at(pruneCorner.getRightEdge().getRightCornerIndex());

			std::cout<<"Index of the first corner is : "<<c.getIndex()<<std::endl;
			std::cout<<"Index of the prune corner is : "<<pruneCorner.getIndex()<<std::endl;
			std::cout<<"Index of the final corner is : "<<testCorner.getIndex()<<std::endl;
			
			glm::vec3 B = testCorner.getPt();
			glm::vec3 A = pruneCorner.getPt();
			glm::vec3 P = c.getPt();


			//****THIS IS CODE FROM STACK OVERFLOW http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
		//	float minimum_distance(vec2 v, vec2 w, vec2 p) {
  //// Return minimum distance between line segment vw and point p
  //const float l2 = length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
  //if (l2 == 0.0) return distance(p, v);   // v == w case
  //// Consider the line extending the segment, parameterized as v + t (w - v).
  //// We find projection of point p onto the line. 
  //// It falls where t = [(p-v) . (w-v)] / |w-v|^2
  //const float t = dot(p - v, w - v) / l2;
  //if (t < 0.0) return distance(p, v);       // Beyond the 'v' end of the segment
  //else if (t > 1.0) return distance(p, w);  // Beyond the 'w' end of the segment
  //const vec2 projection = v + t * (w - v);  // Projection falls on the segment
  //return distance(p, projection);

			float distance=0.0;
			glm::vec3 proj;
			float l2 = pow(glm::length(A-B),2);
			if(l2 < 0.001){
				distance = glm::length(P-A);
			}
			float t = glm::dot(P-B,A-B)/l2;
			if (t<0.0){
				distance = glm::length(P-B);
			}
			else if (t>1.0){
				distance = glm::length(P-A);
			}
			else {
				proj = B + (t*(A-B));
				distance = glm::length(P-proj);
			}

			std::cout<<"T value is: "<<t<<std::endl;
			std::cout<<"The Distance is: "<<distance<<std::endl;
			std::cout<<"The location of point A(pruneCorner) is: "<<A.x<<" "<<A.y<<" "<<A.z<<std::endl;
			std::cout<<"The location of point B is: "<<B.x<<" "<<B.y<<" "<<B.z<<std::endl;
			std::cout<<"The location of point P is: "<<P.x<<" "<<P.y<<" "<<P.z<<std::endl;
			std::cout<<"The location of projected point is: "<<proj.x<<" "<<proj.y<<" "<<proj.z<<std::endl;
			
			/*float xValue = (P.x-B.x)/(A.x-B.x + pow(10.0,-6));
			float yValue = (P.y-B.y)/(A.y-B.y+ pow(10.0,-6));
			float zValue = (P.z-B.z)/(A.z-B.z+ pow(10.0,-6));

			std::cout<<"The 3 values are x: "<<xValue<<" y: "<<yValue<<" z: "<<zValue<<std::endl;*/

			float ERROR = 0.01;

			if(c.getSkipped()){
				continue;
			}

			//This means we need to do us some prunin!
			if(distance < ERROR){
				std::cout<<"Pruned!"<<std::endl;
				//We need to check that it isn't involved in an intersection!
				

					//We need to fix the edges connecting them for indices
				/*	PlanEdge rightEdge = activePlan.at(i).getRightEdge();
					rightEdge.setRightCornerIndex(testCorner.getIndex());
					PlanEdge leftEdge = testCorner.getLeftEdge();
					leftEdge.setLeftCornerIndex(i);
					activePlan.at(i).setRightEdge(rightEdge);
					activePlan.at(testCorner.getIndex()).setLeftEdge(leftEdge);*/

					//Now we need to make sure the extra corner doesn't get in the way
					activePlan.at(pruneCorner.getIndex()).setSkipped(true);
				
			}

		}
	}

}

std::vector<Corner> ActivePlan::cleanPrunedPlan()
{
	std::vector<Corner> newCornerPlan;
	int index = 0;
	std::cout<<"The size of the active plan pre-pruning is: "<<activePlan.size()<<std::endl;
	for(int i = 0; i<activePlan.size(); i++){
		std::cout<<"Times through the loop: "<<i<<std::endl;
		Corner c = activePlan.at(i);
		if (c.getSkipped()){
			std::cout<<"Skipping at original active plan index: "<<i<<std::endl;
			continue;
		}
		

		PlanEdge tEdge = c.getRightEdge();
		PlanEdge pruned = c.getLeftEdge();
		int prevInd = pruned.getLeftCornerIndex();
		int nextInd = tEdge.getRightCornerIndex();

		std::cout<<"The next index potentially to be pruned is: "<<nextInd<<std::endl;
		std::cout<<"The previous index potentially to be pruned is: "<<prevInd<<std::endl;
		
		//We know the next one is going to be pruned off so we handle it all here
		if(activePlan.at(nextInd).getSkipped()){
			PlanEdge keptEdge = activePlan.at(nextInd).getRightEdge();
			keptEdge.setStartPoint(c.getPt());
			keptEdge.setLeftCornerIndex(index);

			if(i == activePlan.size()-1){
				//This maybe should be 1
				keptEdge.setRightCornerIndex(0);
			} else {
				keptEdge.setRightCornerIndex(index+1);
			}
			c.setRightEdge(keptEdge);
			//c.setIndex(index);

		}
		if(activePlan.at(prevInd).getSkipped()){
			PlanEdge keptEdge = activePlan.at(prevInd).getLeftEdge();
			keptEdge.setEndPoint(activePlan.at(activePlan.at(prevInd).getLeftEdge().getLeftCornerIndex()).getPt());
			keptEdge.setLeftCornerIndex(index-1);
			keptEdge.setRightCornerIndex(index);
			c.setLeftEdge(keptEdge);
			//c.setIndex(index);
		}

		newCornerPlan.push_back(c);
		index++;
	}

	std::cout<<"The size of the post pruned plan is: "<<newCornerPlan.size()<<std::endl;
	for(int i = 0; i<newCornerPlan.size(); i++){
		std::cout<<"Index in new corner plan: "<<newCornerPlan.at(i).getIndex()<<std::endl;
		std::cout<<"The next index from the right edge is: "<<newCornerPlan.at(i).getRightEdge().getRightCornerIndex()<<std::endl;
		std::cout<<"The next index from the left edge is: "<<newCornerPlan.at(i).getLeftEdge().getLeftCornerIndex()<<std::endl;
		std::cout<<"The profile index of the left edge is: "<<newCornerPlan.at(i).getLeftEdge().getProfileType()<<" and the right is: "<<newCornerPlan.at(i).getRightEdge().getProfileType()<<std::endl;
	}

	return newCornerPlan;

}

void ActivePlan::setActivePlan(std::vector<Corner> corn)
{
	activePlan.clear();
	for(int i = 0; i<corn.size(); i++){
		activePlan.push_back(corn.at(i));
	}

}
bool ActivePlan::determineShittyPoints(float height)
{
	bool thereBeShittinessHere = false;
	for(int i = 0; i<activePlan.size(); i++){
		Corner c = activePlan.at(i);
		if(c.getPt().y != c.getPt().y){
			//This means we need to prune this fucker out of here
			std::cout<<"The actual height is: "<<height<<" and the corner height is: "<<c.getPt().y<<std::endl;
			activePlan.at(i).setIsShitty(true);
			thereBeShittinessHere = true;
			continue;
		}
	}

	return thereBeShittinessHere;
}

std::vector<Corner> ActivePlan::removeShittyPoints()
{
		std::vector<Corner> newCornerPlan;
	int index = 0;
	std::cout<<"The size of the active plan pre-pruning is: "<<activePlan.size()<<std::endl;
	for(int i = 0; i<activePlan.size(); i++){
		std::cout<<"Times through the loop: "<<i<<std::endl;
		Corner c = activePlan.at(i);
		if (c.getIsShitty()){
			std::cout<<"Skipping at original active plan index: "<<i<<std::endl;
			continue;
		}
		

		PlanEdge tEdge = c.getRightEdge();
		PlanEdge pruned = c.getLeftEdge();
		int prevInd = pruned.getLeftCornerIndex();
		int nextInd = tEdge.getRightCornerIndex();

		std::cout<<"The next index potentially to be pruned is: "<<nextInd<<std::endl;
		std::cout<<"The previous index potentially to be pruned is: "<<prevInd<<std::endl;
		
		//We know the next one is going to be pruned off so we handle it all here
		if(activePlan.at(nextInd).getIsShitty()){
			PlanEdge keptEdge = activePlan.at(nextInd).getRightEdge();
			keptEdge.setStartPoint(c.getPt());
			keptEdge.setLeftCornerIndex(index);

			if(i == activePlan.size()-1){
				//This maybe should be 1
				keptEdge.setRightCornerIndex(0);
			} else {
				keptEdge.setRightCornerIndex(index+1);
			}
			c.addSourceToFront(activePlan.at(nextInd).getSource().at(0));
			c.setRightEdge(keptEdge);
			c.setIndex(index);

		}
		if(activePlan.at(prevInd).getIsShitty()){
			PlanEdge keptEdge = activePlan.at(prevInd).getLeftEdge();
			keptEdge.setEndPoint(activePlan.at(activePlan.at(prevInd).getLeftEdge().getLeftCornerIndex()).getPt());
			keptEdge.setLeftCornerIndex(index-1);
			keptEdge.setRightCornerIndex(index);
			//c.addToSource(activePlan.at(prevInd).getSource().at(0));
			c.setLeftEdge(keptEdge);
			c.setIndex(index);
		}

		newCornerPlan.push_back(c);
		index++;
	}

	
	if (newCornerPlan.size() == 2){

		PlanEdge edge1 = newCornerPlan.at(0).getLeftEdge();
		edge1.setLeftCornerIndex(1);
		PlanEdge edge2 = newCornerPlan.at(0).getRightEdge();
		edge2.setRightCornerIndex(1);
		PlanEdge edge3 = newCornerPlan.at(1).getLeftEdge();
		edge3.setLeftCornerIndex(0);
		PlanEdge edge4 = newCornerPlan.at(1).getRightEdge();
		edge4.setRightCornerIndex(0);

		newCornerPlan.at(0).setLeftEdge(edge1);
		newCornerPlan.at(0).setRightEdge(edge2);

		newCornerPlan.at(1).setLeftEdge(edge3);
		newCornerPlan.at(1).setRightEdge(edge4);

	}
	
	
	std::cout<<"The size of the post shitty plan is: "<<newCornerPlan.size()<<std::endl;
	for(int i = 0; i<newCornerPlan.size(); i++){
		std::cout<<"Index in new corner plan: "<<newCornerPlan.at(i).getIndex()<<std::endl;
		std::cout<<"The next index from the right edge is: "<<newCornerPlan.at(i).getRightEdge().getRightCornerIndex()<<std::endl;
		std::cout<<"The next index from the left edge is: "<<newCornerPlan.at(i).getLeftEdge().getLeftCornerIndex()<<std::endl;
		std::cout<<"The profile index of the left edge is: "<<newCornerPlan.at(i).getLeftEdge().getProfileType()<<" and the right is: "<<newCornerPlan.at(i).getRightEdge().getProfileType()<<std::endl;
		std::cout<<"The parents of each point are: ";
		for(int j = 0; j<newCornerPlan.at(i).getSource().size(); j++){
			std::cout<<newCornerPlan.at(i).getSource().at(j).getIndex()<<" ";

		}
		std::cout<<std::endl;
	}

	//This is hacky fix for the top section - might break in other circumstances
	
	return newCornerPlan;


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
	std::cout<<"The new values of the corner's left is: "<<activePlan.at(index).getLeftMesh()<<std::endl;
}
void ActivePlan::setCornerRight(int index, bool value)
{
	activePlan.at(index).setRightMesh(value);
	std::cout<<"The new values of the corner's right is: "<<activePlan.at(index).getRightMesh()<<std::endl;
}