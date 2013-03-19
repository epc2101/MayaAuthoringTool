#include "SweepPlane.h"
#include <iostream>
#include "maya/MFnMesh.h"
#include "maya/MFloatPointArray.h"
#include "maya/MPointArray.h"

using namespace std;
float EPSILON = 0.0001;
float RADIUS =  0.000001;;

SweepPlane::SweepPlane(void)
{
}

SweepPlane::SweepPlane(FloorPlan p, std::vector<Profile> pList)
{
	plan = p;
	profileList = pList;
}

/*Tests the data coming in from the file to make sure it's distributed correctly across structure*/
void SweepPlane::validateData()
{
	cout<<"Validate FloorPlan Data"<<endl;
	cout<<"Number of points in floor plan is: "<<plan.getNumPoints()<<endl;
	cout<<"Description of floor plan edges"<<endl;
	for(int i = 0; i<plan.getEdgeList().size(); i++){
		glm::vec3 start = plan.getEdgeList().at(i).getStartPoint(), 
			end = plan.getEdgeList().at(i).getEndPoint(),
			anchor = plan.getEdgeList().at(i).getAnchorPoint();
		int anchorType = plan.getEdgeList().at(i).getAnchorType();
		int profile = plan.getProfileList().at(i);
		cout<<"Edge "<<i<<": First Point: "<<start.x<<" "<<start.z<<" Second Point: "<<end.x<<" "<<end.z<<" Anchor Point: "<<anchor.x<<" "<<anchor.z<<" Anchor Type: "<<anchorType<<" Profile: "<<profile<<endl;
	}
	
	cout<<endl;
	cout<<"Number of profiles: "<<profileList.size()<<endl;
	cout<<"Description of profiles"<<endl;
	for(int i = 0; i<profileList.size(); i++){
		cout<<"Profile "<<i<<endl;
		for(int j = 0; j<profileList.at(i).getNumPoints()-1; j++){
			glm::vec3 start = profileList.at(i).getEdgeList().at(j).getStartPoint(),
				end = profileList.at(i).getEdgeList().at(j).getEndPoint(), 
				anchor = profileList.at(i).getEdgeList().at(j).getAnchorPoint();
			int anchorType = profileList.at(i).getEdgeList().at(j).getAnchorType();
			cout<<"Edge "<<j<<": First Point: "<<start.x<<" "<<start.z<<" Second Point: "<<end.x<<" "<<end.z<<" Anchor Point: "<<anchor.x<<" "<<anchor.z<<" Anchor Type: "<<anchorType<<endl;
		}
		cout<<endl;
	}
}

//The following 3 methods are just stubs for now until we perform the algorithm
/*
TODO - we will have 5 cases here...see the board pics for details on how to draw
*/
MObject SweepPlane::createMesh(MObject& outData, MStatus& stat)
{

		int  numVertices, frame;
        MFnMesh  meshFS;

		MPointArray pointArray;
		MIntArray faceConnects;
		MIntArray faceCounts;

		vector<int> indexingHolder;
		
		int numbersToIndex = activePlanQueue.size();
		//We fill up the point array
		while(!activePlanQueue.empty()){
			ActivePlan temp = activePlanQueue.front();
			for(int j = 0; j<temp.getActivePlan().size(); j++){
				glm::vec3 tempVec = temp.getActivePlan().at(j).getPt();
				MPoint tempPoint = MPoint(tempVec.x, tempVec.y,tempVec.z);
				pointArray.append(tempPoint);
				activePlanQueue.pop();
			}
			indexingHolder.push_back(temp.getActivePlan().size());

		}

		
		numVertices = pointArray.length();
		int stackLevel = indexingHolder.size();

		//Move through the stack and start building the polygons

		while(!activePlanStack.empty()){
			ActivePlan temp = activePlanStack.top();
			activePlanStack.pop();
			stackLevel--;
			//Check if this is the floorplan
			if(temp.getActivePlan().at(0).getSource().size()==0){
				int polySize = temp.getActivePlan().size();
				faceCounts.append(polySize);
				for(int i = 0; i<polySize; i++){
					faceConnects.append(i);
				}
			}
			else {
				int currentPosition = 0,positionBelow = 0;
				
				for (int i = 0; i<stackLevel; i++){
					currentPosition+=indexingHolder.at(i);
				}
				positionBelow = currentPosition-indexingHolder.at(stackLevel-1);
				for(int i = 0; i<temp.getActivePlan().size(); i++){
					Corner corner = temp.getActivePlan().at(i);

					//We know it's a quad
					if (corner.getSource().size() == 1){
						int index1, index2,index3,index4;
						
						if (i== temp.getActivePlan().size()-1){
						index1 = currentPosition + corner.getIndex();
						index2 = positionBelow + corner.getSource().at(0).getIndex();
						index3 = positionBelow + temp.getActivePlan().at(0).getSource().at(0).getIndex();
						index4 = positionBelow + temp.getActivePlan().at(0).getIndex();
						faceCounts.append(4);
						faceConnects.append(index1);
						faceConnects.append(index2);
						faceConnects.append(index3);
						faceConnects.append(index4);
						}
						else {

						//THe first index is this corner
						index1 = currentPosition + corner.getIndex();
						index2 = positionBelow + corner.getSource().at(0).getIndex();
						index3 = positionBelow + corner.getSource().at(0).getIndex()+1;
						index4 = currentPosition + corner.getIndex() + 1;
						faceCounts.append(4);
						faceConnects.append(index1);
						faceConnects.append(index2);
						faceConnects.append(index3);
						faceConnects.append(index4);
						}
					}
					//This is going to be a triangle and a quad
					else if (corner.getSource().size()==2){
						int index1, index2, index3, index4;
						//Process the triangle first
						for (int k = 0; k<corner.getSource().size(); k++) {
						if(k < corner.getSource().size()-1) {
							index1 = currentPosition + corner.getIndex();
							index2 = positionBelow + corner.getSource().at(k).getIndex();
							index3 = positionBelow + corner.getSource().at(k).getIndex()+1;
						
							faceCounts.append(3);
							faceConnects.append(index1);
							faceConnects.append(index2);
							faceConnects.append(index3);

						}
						else {
						//Then process teh quad

								if (i== temp.getActivePlan().size()-1){
								index1 = currentPosition + corner.getIndex();
								index2 = positionBelow + corner.getSource().at(k).getIndex();
								index3 = positionBelow + temp.getActivePlan().at(0).getSource().at(0).getIndex();
								index4 = positionBelow + temp.getActivePlan().at(0).getIndex();
								faceCounts.append(4);
								faceConnects.append(index1);
								faceConnects.append(index2);
								faceConnects.append(index3);
								faceConnects.append(index4);
								}
								else {

								//THe first index is this corner
								index1 = currentPosition + corner.getIndex();
								index2 = positionBelow + corner.getSource().at(k).getIndex();
								index3 = positionBelow + corner.getSource().at(k).getIndex()+1;
								index4 = currentPosition + corner.getIndex() + 1;
								faceCounts.append(4);
								faceConnects.append(index1);
								faceConnects.append(index2);
								faceConnects.append(index3);
								faceConnects.append(index4);
								}
							}
						}
					}

				}
			}
		}

		

	//for(int i = 0; i<plan.getNumPoints(); i++){
	//	glm::vec3 point = plan.getEdgeList().at(i).getStartPoint();
	//	MPoint tempPoint = MPoint(point.x, point.y,point.z);
	//	pointArray.append(tempPoint);
	//	faceConnects.append(i);
	//}

	numVertices = pointArray.length();
	
	
			
		//MGlobal::executeCommand((MString)"print " + numVertices +"  ;",true, false);
		int numFaces = faceCounts.length();
		//MGlobal::executeCommand((MString)"print " + numFaces +"  ;",true, false);
		MObject newMesh = meshFS.create(numVertices, numFaces, pointArray,faceCounts, faceConnects, outData, &stat);

        return newMesh;


}

/*
Goes through the active plan and figures out the intersection events.  It then stuffs them into the priority queue!
*/
void SweepPlane::fillQueueWithIntersections(float height)
{
	//Preprocess the correct profile edges based on the current height of the active plan
	//We just figure out what edge of the profile we are at here
	std::vector<Edge> currentProfileFromHeight;
	for (int i = 0; i<profileList.size(); i++){
		Profile prof = profileList.at(i);
		for(int j = 0; j<prof.getEdgeList().size(); j++){
			Edge edge = prof.getEdgeList().at(j);
			cout<<"The height of the start poin is "<<edge.getStartPoint().y<<endl;
			cout<<"The height of the end poin is "<<edge.getEndPoint().y<<endl;
			if (height >= edge.getStartPoint().y && height < edge.getEndPoint().y){
				
				currentProfileFromHeight.push_back(edge);
				continue;
			}	
		}
	}

	cout<<"Active plan size: "<<thePlan.getActivePlan().size()<<endl;
	
	cout<<"Current profile from height : "<<currentProfileFromHeight.size()<<endl;
	//Iterate through the active plan and calculate the varying vectors based on the different edge plans.
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		//At cach corner we need the current vector associated
		Corner corner = thePlan.getActivePlan().at(i);
		Edge firstEdge = corner.getNextEdge();
		Edge secondEdge = corner.getPreviousEdge();

		cout<<"The data of the first edge is..."<<endl;
		cout<<"First Edge Start Point is "<<firstEdge.getStartPoint().x<<" "<<firstEdge.getStartPoint().y<<" "<<firstEdge.getStartPoint().z<<endl;
		cout<<"First Edge End Point is "<<firstEdge.getEndPoint().x<<" "<<firstEdge.getEndPoint().y<<" "<<firstEdge.getEndPoint().z<<endl;

		cout<<"First Edge Profile Index is: "<<firstEdge.getProfileType()<<endl;
		cout<<"Second Edge Profile Index is: "<<secondEdge.getProfileType()<<endl;
		
		Edge firstProfileEdge = currentProfileFromHeight.at(firstEdge.getProfileType());

		cout<<"First Profile Edge Start Point is "<<firstProfileEdge.getStartPoint().x<<" "<<firstProfileEdge.getStartPoint().y<<" "<<firstProfileEdge.getStartPoint().z<<endl;
		cout<<"First Profile Edge End Point is "<<firstProfileEdge.getEndPoint().x<<" "<<firstProfileEdge.getEndPoint().y<<" "<<firstProfileEdge.getEndPoint().z<<endl;


		Edge secondProfileEdge = currentProfileFromHeight.at(secondEdge.getProfileType());

		//Calculate the normal of the first edge.
		glm::vec3 normal1, normal2, finalVector;

		glm::vec3 edgeVec1 = firstEdge.getEndPoint()-firstEdge.getStartPoint();
		glm::vec3 profVec1 = firstProfileEdge.getEndPoint()-firstProfileEdge.getStartPoint();

		
		
		glm::mat4 tempMat = glm::mat4(1.0);
		glm::mat4 firstRot = glm::rotate(tempMat,-90.0f,glm::vec3(0,1,0));

		glm::vec4 rotVec = glm::vec4(edgeVec1,1.0);
		rotVec = firstRot * rotVec;

		glm::vec3 newVector = glm::vec3(rotVec);
		newVector = glm::normalize(newVector);

		cout<<"New Vector 1 is "<<newVector.x<<" "<<newVector.y<<" "<<newVector.z<<endl;

		profVec1 = glm::vec3(newVector.x*profVec1.x,profVec1.y,newVector.z*profVec1.x);


		cout<<"First FloorPlan Edge Vector is "<<edgeVec1.x<<" "<<edgeVec1.y<<" "<<edgeVec1.z<<endl;
		

		cout<<"First Profile Edge Vectpr is "<<profVec1.x<<" "<<profVec1.y<<" "<<profVec1.z<<endl;
		

		normal1 = glm::cross(edgeVec1, profVec1);
		normal1 = glm::normalize(normal1);
		cout<<"First Normal is "<<normal1.x<<" "<<normal1.y<<" "<<normal1.z<<endl;

		//Calculate normal of the second edge
		glm::vec3 edgeVec2 = secondEdge.getEndPoint()-secondEdge.getStartPoint();
		glm::vec3 profVec2 = secondProfileEdge.getEndPoint()-secondProfileEdge.getStartPoint();

		firstRot = glm::rotate(tempMat,-90.0f,glm::vec3(0,1,0));
		rotVec = glm::vec4(edgeVec2,1.0);
		rotVec = firstRot * rotVec;

		newVector = glm::vec3(rotVec);
		newVector = glm::normalize(newVector);

		cout<<"New Vector 2 is "<<newVector.x<<" "<<newVector.y<<" "<<newVector.z<<endl;

		profVec2 = glm::vec3(newVector.x*profVec2.x,profVec2.y,newVector.z*profVec2.x);
		

		cout<<"Second FloorPlan Edge Vector is "<<edgeVec2.x<<" "<<edgeVec2.y<<" "<<edgeVec2.z<<endl;
		

		cout<<"Second Profile Edge Vectpr is "<<profVec2.x<<" "<<profVec2.y<<" "<<profVec2.z<<endl;

		normal2 = glm::cross(edgeVec2, profVec2);

		normal2 = glm::normalize(normal2);

		cout<<"Second Normal is "<<normal2.x<<" "<<normal2.y<<" "<<normal2.z<<endl;

		finalVector = glm::cross(normal1,normal2);

		cout<<"Final vector is "<<finalVector.x<<" "<<finalVector.y<<" "<<finalVector.z<<endl;

		finalVector = glm::normalize(finalVector);



		thePlan.setIntersectionVector(finalVector);
		cout<<"End of loop "<<i<<endl;
	}

	cout<<"Calculated vectors.  Size of intersection vecotrs: "<<thePlan.getIntersectionVectors().size()<<endl;
	//Compare each corner to all the other ones and determine the possible intersection events
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		//TODO - Add in intersection detection and handling for non adjacent corners
		glm::vec3 firstVec, secondVec;
		Corner firstCorner, secondCorner;
		//We will calculate the intersection with the corner and its next neighbor - need to handle the end
		if (i == thePlan.getActivePlan().size()-1){
			firstVec = thePlan.getIntersectionVectors().at(i);
			secondVec = thePlan.getIntersectionVectors().at(0);
			firstCorner = thePlan.getActivePlan().at(i);
			secondCorner = thePlan.getActivePlan().at(0);
		} else {
			firstVec = thePlan.getIntersectionVectors().at(i);
			secondVec = thePlan.getIntersectionVectors().at(i+1);
			firstCorner = thePlan.getActivePlan().at(i);
			secondCorner = thePlan.getActivePlan().at(i+1);
		}

		glm::normalize(firstVec);
		glm::normalize(secondVec);
		//Test if the vectors are parallel (might want to utilize an epsilon value for equality to deal with floating point issues
		//We extend the vectors in both directions from their current points and perform a line intersection test
		glm::vec3 firstStartPoint, secondStartPoint, firstBelowPoint, secondBelowPoint, firstTopPoint, secondTopPoint, intersectionPoint;

		//These are the initial starting points
		firstStartPoint = firstCorner.getPt();
		secondStartPoint = secondCorner.getPt();

		float t = 10000;

		//The top points are multiplied by a large t value (around 10000)
		firstTopPoint = firstStartPoint + (firstVec*t);
		secondTopPoint = secondStartPoint + (secondVec*t);


		if (intersectionTest(firstStartPoint,firstTopPoint,secondStartPoint,secondTopPoint,intersectionPoint)){
			//This is the code to create the intersection event and push it onto the queue
			std::vector<Corner> source;
			source.push_back(firstCorner);
			source.push_back(secondCorner);
			Event intersect = Event(intersectionPoint.y, intersectionPoint,source, Event::INTERSECTION);

			q.push(intersect);
			cout<<"The size of the queue is "<<q.size()<<endl;
		}
		else {
			cout<<"Didn't find intersection..."<<endl;
			continue;
		}		
	}

}

bool SweepPlane::intersectionTest(glm::vec3 line1S, glm::vec3 line1E, glm::vec3 line2S, glm::vec3 line2E, glm::vec3 intersection )
{
	cout<<"In intersection test"<<endl;
	glm::vec3 da = line1E-line1S, db = line2E-line2S, dc = line2S-line1S;

	if (glm::dot(glm::cross(da,db),dc) != 0.0){
		return false;
	}

	glm::vec3 tempCross = glm::cross(da,db);
	float norm = tempCross.x*tempCross.x + tempCross.y * tempCross.y + tempCross.z*tempCross.z;

	float s = glm::dot(glm::cross(dc,db),glm::cross(da,db))/norm;
	cout<<"The s value is "<<s<<endl;
	if (s >= 0.0 && s <= 1.0){

		intersection = line1S + da*s;
		return true;
	}


	return false;
}

//Runs through the q and process the events into a new active plan stack
void SweepPlane::processQueue()
{
	std::vector<Event> events;
	std::vector<vector<Corner>> clusters;
	ActivePlan localPlan;

	Event firstEvent = q.top();
	q.pop();

	events.push_back(firstEvent);

	//Get list of all events at the same height 
	while (q.top().getHeight() - firstEvent.getHeight() < EPSILON)
	{
		events.push_back(q.top());
		q.pop();
	}

	//TODO- check if we need to filter invalid events


	std::priority_queue<Corner,std::vector<Corner>, CompareParent> cornerQ;
	std::vector<Corner> tempActivePlan;
	std::vector<bool> flagPlan;
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		flagPlan.push_back(false);
	}

	//Interchain handling
	for (int i = 0; i < events.size(); i++)
	{
		 Event e = events.at(i);
		 Corner corner = Corner(e.getPoint(), e.getCorners());
		 cornerQ.push(corner);
	}



	//Now we connect the edge!
	for( int i = 0; i<tempActivePlan.size(); i++){
		Corner temp = tempActivePlan.at(i);
		for(int j = 0; j<temp.getSource().size(); j++){
			Corner parent = temp.getSource().at(j);
			flagPlan.at(parent.getIndex())=true;
		}
	}

	//Raise the extra points that don't get no love!
	for (int i = 0; i<flagPlan.size(); i++){
		if (flagPlan.at(i) == false){
			glm::vec3 vec = thePlan.getIntersectionVectors().at(i);
			Corner parentCorner = thePlan.getActivePlan().at(i);
			float newActivePlanHeight = tempActivePlan.at(0).getPt().y;
			float difference = newActivePlanHeight - parentCorner.getPt().y;
			float multiply = difference/vec.y;
			glm::vec3 newPoint = parentCorner.getPt()+(vec*multiply);
			std::vector<Corner> myParent;
			myParent.push_back(parentCorner);
			Corner finalCorner = Corner(newPoint,myParent);
			cornerQ.push(finalCorner);
		}
	}


	//Go thorugh the priority queue and set the edges then turn it into a regular active plan for clustering!
	for(int i = 0; i<cornerQ.size(); i++){
		tempActivePlan.push_back(cornerQ.top());
		cornerQ.pop();
	}

		for(int i = 0; tempActivePlan.size(); i++){
		glm::vec3 startPoint, endPoint;
		int profile;
		if (i == tempActivePlan.size()-1){	
			startPoint = tempActivePlan.at(i).getPt();
			endPoint = tempActivePlan.at(0).getPt();
			std::vector<Corner> myCorn = tempActivePlan.at(i).getSource();
			profile = myCorn.at(myCorn.size()-1).getPreviousEdge().getProfileType();
			Edge edge = Edge(startPoint,endPoint,profile);
			tempActivePlan.at(i).setPreviousEdge(edge);
			tempActivePlan.at(0).setNextEdge(edge);
			
		} else {
			startPoint = tempActivePlan.at(i).getPt();
			endPoint = tempActivePlan.at(i+1).getPt();
			std::vector<Corner> myCorn = tempActivePlan.at(i).getSource();
			profile = myCorn.at(myCorn.size()-1).getPreviousEdge().getProfileType();
			Edge edge = Edge(startPoint,endPoint,profile);
			tempActivePlan.at(i).setPreviousEdge(edge);
			tempActivePlan.at(i+1).setNextEdge(edge);
		}		
	}

	//Now we cluster and then we have our next active plan!
	//Cluster the events based on the radius
	std::vector<bool> cornerInCluster;
	for (int i = 0; i < tempActivePlan.size(); i++)
	{
		cornerInCluster.push_back(false);
	}

	for (int i = 0; i < tempActivePlan.size(); i++)
	{
		Corner temp = tempActivePlan.at(i);
		std::vector<Corner> temp2; 
		temp2.push_back(temp);
		cornerInCluster.at(i) = true; 

		for (int k = 0; k < events.size(); k++)
		{
			if (k == i ) continue;
			if (glm::distance(tempActivePlan.at(i).getPt(), tempActivePlan.at(k).getPt()) < RADIUS)
			{
				//TODO - think about pruning...we'll be getting multiple chains of the same thing
				if (cornerInCluster.at(k) == false) {
					temp2.push_back(tempActivePlan.at(k));
				}
			}
		}
		clusters.push_back(temp2);
	}

	//Process the interclusters
	std::vector<Corner> postCluster;
	for (int i = 0; i < clusters.size(); i++)
	{
		std::vector<Corner> cluster = clusters.at(i); 
		Edge startEdge = cluster.at(0).getNextEdge();
		Edge endEdge = cluster.at(cluster.size() - 1).getPreviousEdge();
		std::vector<Corner> parents;
		for (int k = 0; k < cluster.size(); k++)
		{
			std::vector<Corner> tempSource = cluster.at(k).getSource();
			for (int j = 0; j < tempSource.size(); j++) {
				bool addParent = true; 
				for (int p = 0; p < parents.size(); p++) {
					if (tempSource.at(j).getIndex() == parents.at(p).getIndex()) addParent = false; 				
				}
				if (addParent == true) {
					parents.push_back(tempSource.at(j)); 
				}
			}
		}
		Corner newCorner = Corner(endEdge, startEdge, cluster.at(0).getPt(), parents);
		postCluster.push_back(newCorner);
	}


	//Process the intraclusters
	thePlan = ActivePlan(postCluster);

	//Clean out the queue
	while (!q.empty()){
		q.pop();
	}
}


void SweepPlane::updateApStack(ActivePlan activePlan)
{
	activePlanStack.push(activePlan);
}

//The main method that checks for different events & builds the final stack from the active plan
void SweepPlane::buildIt()
{
	cout<<"Made it to buildIt method"<<endl;
	thePlan = ActivePlan(plan);
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		cout<<"The profile for each edge is "<<thePlan.getActivePlan().at(i).getPreviousEdge().getProfileType()<<endl;
	}
	activePlanStack.push(thePlan);
	activePlanQueue.push(thePlan);
	float height = 0;

	fillQueueWithIntersections(height);
	cout<<"Supposedly filled with events"<<endl;
	cout<<"The queue size is "<<q.size()<<endl;
	while (!q.empty())
	{
		cout<<"Made it into the first queue loop"<<endl;
		processQueue();
		activePlanStack.push(thePlan);
		activePlanQueue.push(thePlan);
		fillQueueWithIntersections(thePlan.getActivePlan().at(0).getPt().y);
		break;
	}


}

SweepPlane::~SweepPlane(void)
{
}
