#include "SweepPlane.h"
#include <iostream>
#include "maya/MFnMesh.h"
#include "maya/MFloatPointArray.h"
#include "maya/MPointArray.h"

using namespace std;
float EPSILON = 0.0001;
float RADIUS =  0.000001;


int DEBUG = 0; 
int DEBUG_ANCHOR = 1; 
int DEBUG_PROFILE = 0; 
int DEBUG_FLOORPLAN = 0; 

SweepPlane::SweepPlane(void)
{
}

SweepPlane::SweepPlane(FloorPlan p, std::vector<Profile> pList, std::vector<Anchor> aList)
{
	plan = p;
	profileList = pList;
	anchorList = aList; 
	killTheSweep = false;
	theLastHeight = -1.0;
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
			cout<<"Edge "<<j<<": First Point: "<<start.x<<" "<<start.y<<" Second Point: "<<end.x<<" "<<end.y<<" Anchor Point: "<<anchor.x<<" "<<anchor.z<<" Anchor Type: "<<anchorType<<endl;
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
		int i = 1;
		while(!activePlanQueue.empty()){
			cout<<"In plan : "<<i<<endl;
			ActivePlan temp = activePlanQueue.front();
			for(int j = 0; j<temp.getActivePlan().size(); j++){
				glm::vec3 tempVec = temp.getActivePlan().at(j).getPt();
				cout<<"Point is "<<tempVec.x<< " "<<tempVec.y<<" " <<tempVec.z<<endl;
				MPoint tempPoint = MPoint(tempVec.x, tempVec.y,tempVec.z);
				pointArray.append(tempPoint);
				
			}
			activePlanQueue.pop();
			indexingHolder.push_back(temp.getActivePlan().size());
			i++;

		}

		
		numVertices = pointArray.length();
		int stackLevel = indexingHolder.size();
		cout<<"We are checking the number of vertice at the top and the number is "<<numVertices<<endl;
		//Move through the stack and start building the polygons

		while(!activePlanStack.empty()){
			cout<<"Active plan stack is not empty"<<endl;
			ActivePlan temp = activePlanStack.top();
			activePlanStack.pop();
			stackLevel--;
			//Check if this is the floorplan
			if(temp.getActivePlan().at(0).getSource().size()==0){
				cout<<"Taking care of the floorplan"<<endl;
				int polySize = temp.getActivePlan().size();
				faceCounts.append(polySize);
				for(int i = 0; i<polySize; i++){
					faceConnects.append(i);
				}
			}
			//This is going to be just a pyrimid
			else if (temp.getActivePlan().size() == 1){
				int currentPosition = 0,positionBelow = 0;

				for (int i = 0; i<stackLevel; i++){
					currentPosition+=indexingHolder.at(i);
				}
				positionBelow = currentPosition-indexingHolder.at(stackLevel-1);
				Corner corner = temp.getActivePlan().at(0);
				int index1, index2,index3,index4;
				for (int k = 0; k<corner.getSource().size(); k++) {
					if(k < corner.getSource().size()-1) {
						index1 = currentPosition + corner.getIndex();
						cout<<"Position below is "<<positionBelow<<endl;
						index2 = positionBelow + corner.getSource().at(k).getIndex();
						index3 = positionBelow + corner.getSource().at(k+1).getIndex();

						faceCounts.append(3);
						faceConnects.append(index1);
						faceConnects.append(index2);
						faceConnects.append(index3);
					}
					else {
						index1 = currentPosition + corner.getIndex();
						cout<<"Position below is "<<positionBelow<<endl;
						index2 = positionBelow + corner.getSource().at(k).getIndex();
						index3 = positionBelow + corner.getSource().at(0).getIndex();

						faceCounts.append(3);
						faceConnects.append(index1);
						faceConnects.append(index2);
						faceConnects.append(index3);

					}
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
							index4 = currentPosition + temp.getActivePlan().at(0).getIndex();
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
					else if (corner.getSource().size() >= 2){
						cout<<"We should be at this part"<<endl;
						int index1, index2, index3, index4;
						//Process the triangle first
						for (int k = 0; k<corner.getSource().size(); k++) {
							if(k < corner.getSource().size()-1) {
								index1 = currentPosition + corner.getIndex();
								cout<<"Position below is "<<positionBelow<<endl;
								index2 = positionBelow + corner.getSource().at(k).getIndex();
								index3 = positionBelow + corner.getSource().at(k+1).getIndex();

								faceCounts.append(3);
								faceConnects.append(index1);
								faceConnects.append(index2);
								faceConnects.append(index3);

							}
							else {
								//Then process the quad

								if (i== temp.getActivePlan().size()-1){
									cout<<"Processing the last quad. The number of parents it has is: "<<corner.getSource().size()<<endl;
									cout<<"Position below is "<<positionBelow<<endl;
									index1 = currentPosition + corner.getIndex();
									index2 = positionBelow + corner.getSource().at(k).getIndex();
									index3 = positionBelow + temp.getActivePlan().at(0).getSource().at(0).getIndex();
									index4 = currentPosition + temp.getActivePlan().at(0).getIndex();
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
	int numFaces = faceCounts.length();

		cout<<"This is the general data of the mesh"<<endl<<endl;
		cout<<"The number of vertices is "<<numVertices<<endl;
		cout<<"The number of faces is "<<numFaces<<endl;
		for(int i = 0; i < pointArray.length(); i++){
			cout<<"The point "<<pointArray[i].x<<" "<<pointArray[i].y<<" "<<pointArray[i].z<<endl;
		}
		cout<<"The connections array is "<<endl;
		for (int i =0; i<faceConnects.length(); i++){
			cout<<faceConnects[i]<<" "<<endl;
		}
		
			
		//MGlobal::executeCommand((MString)"print " + numVertices +"  ;",true, false);
		
		//MGlobal::executeCommand((MString)"print " + numFaces +"  ;",true, false);
		MObject newMesh = meshFS.create(numVertices, numFaces, pointArray,faceCounts, faceConnects, outData, &stat);

        return newMesh;


}

//Creates the anchors that were calculated & sends them to the output object
void SweepPlane::createAnchors(MObject& anchorPosData, MObject& anchorRotData, MStatus& stat)
{
	//TODO - fill in with correct anchors...testing for now
	 MFnDoubleArrayData posData;
	 MDoubleArray posArray; 
	 MFnDoubleArrayData rotData;
	 MDoubleArray rotArray; 

	 //Run the the anchors that were created & assign the position & rotations 
	 //x, y, z position calculated for each anchor
	while (!outputAnchors.empty())
	{
		Anchor a = outputAnchors.front(); 
		outputAnchors.pop();
		posArray.append(a.getTranslation().x);
		posArray.append(a.getTranslation().y);
		posArray.append(a.getTranslation().z);
		rotArray.append(a.getRotY()); 
	}
	 //posArray.append(10);
	 //posArray.append(4);
	 //posArray.append(2);
	 //rotArray.append(90);

	cout<<"The lenght of the positions array is: "<<posArray.length()<<endl;
	for (int i = 0; i < posArray.length(); i++)
		cout<<"Anchor pos: "<<posArray[i]<<endl;
	for (int i = 0; i < rotArray.length(); i++)
		cout<<"Anchor rot: "<<rotArray[i]<<endl;
	 MObject p = posData.create(posArray, &stat);
	 anchorPosData = p; 
	 cout<<"Created anchor sucessfully"<<endl;
	 MObject r = rotData.create(rotArray, &stat);
	 anchorRotData = r; 
	 cout<<"Created anchor sucessfully2"<<endl;
}



//Preprocess the correct profile edges based on the current height of the active plan
//We just figure out what edge of the profile we are at here
std::vector<ProfileEdge> SweepPlane::getProfileEdgesAtHeight(float height)
{
	float HEIGHTTEST = 0.01;
	height += HEIGHTTEST;
	if (DEBUG == 1){
		cout<<endl;
		cout<<"The Height being passed into the getProfilesFunction is "<<height<<endl<<endl;
	}
	std::vector<ProfileEdge> currentProfileFromHeight;
	for (int i = 0; i<profileList.size(); i++){
		Profile prof = profileList.at(i);
		for(int j = 0; j<prof.getEdgeList().size(); j++){
			ProfileEdge edge = prof.getEdgeList().at(j);
			if (DEBUG == 1)
			{
				cout<<"The height of the start point is "<<edge.getStartPoint().y<<endl;
				cout<<"The height of the end point 0 "<<edge.getEndPoint().y<<endl;
			}


			cout<<"The current height is "<<height<<" and it's being compared to "<<edge.getEndPoint().y<<endl;
			/*if (height >= edge.getEndPoint().y){
			continue;
			}*/
			if (edge.getIsTop() == true){
				if (height >= edge.getEndPoint().y){
					currentProfileFromHeight.push_back(edge);
					prof.getEdgesUsed().at(j) = true;
					break;
				}
			} 
			
			if (height >= edge.getStartPoint().y && height < (edge.getEndPoint().y)){
				//We test the current edge and see if the one below it has been taken if it is horizontal.
				if(prof.getEdgesUsed().at(j)==false && j > 0){
					//We can see if we skipped over the previous edge 
					if (prof.getEdgesUsed().at(j-1) == false && edge.getIsHorizontal()){
						cout<<"The lower edge is horizontal!!!!"<<endl;
						ProfileEdge horizEdge = prof.getEdgeList().at(j-1);
						currentProfileFromHeight.push_back(horizEdge);
						prof.getEdgesUsed().at(j-1) = true;
						break;
					}

				}
				
				currentProfileFromHeight.push_back(edge);
				prof.getEdgesUsed().at(j) = true;
				break;
			}	
		}
	}
	return currentProfileFromHeight; 
}


//Updates the vectors from the active plan's corners based off of the profile's intersections
void SweepPlane::updateIntersectionVectors(float height)
{
	std::vector<ProfileEdge> currentProfileFromHeight = getProfileEdgesAtHeight(height);
	std::vector<bool> aboveTheProfile;
	for(int i = 0; i<currentProfileFromHeight.size(); i++){
		if (height > currentProfileFromHeight.at(i).getEndPoint().y){
			aboveTheProfile.push_back(true);
		}
		else {
			aboveTheProfile.push_back(false);
		}
	}
	bool isItTooHigh = true;
	for (int i = 0; i<aboveTheProfile.size(); i++){
		if (aboveTheProfile.at(i) == false){
			isItTooHigh = false;
			break;
		}
	}
	if (isItTooHigh){
		killTheSweep = true;
		return;
	}

	//Iterate through the active plan and calculate the varying vectors based on the different edge plans.
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		if (thePlan.getActivePlan().size() == 1){
			killTheSweep = true;
			break;
		}
		

		//At cach corner we need the current vector associated
		Corner corner = thePlan.getActivePlan().at(i);
		PlanEdge firstEdge = corner.getLeftEdge();
		PlanEdge secondEdge = corner.getRightEdge();		
		ProfileEdge firstProfileEdge = currentProfileFromHeight.at(firstEdge.getProfileType());

		//Next we test whether the edge are the same!
		if (glm::length(firstEdge.getStartPoint() - secondEdge.getStartPoint()) < EPSILON && glm::length(firstEdge.getEndPoint() - secondEdge.getEndPoint()) < EPSILON)
		{
			killTheSweep = true; 
			break; 
		}

		if (DEBUG ==1 ) {
			cout<<"Current profile from height : "<<currentProfileFromHeight.size()<<endl;
			cout<<"The data of the first edge is..."<<endl;
			cout<<"First Edge Start Point is "<<firstEdge.getStartPoint().x<<" "<<firstEdge.getStartPoint().y<<" "<<firstEdge.getStartPoint().z<<endl;
			cout<<"First Edge End Point is "<<firstEdge.getEndPoint().x<<" "<<firstEdge.getEndPoint().y<<" "<<firstEdge.getEndPoint().z<<endl;
			cout<<"First Edge Profile Index is: "<<firstEdge.getProfileType()<<endl;
			cout<<"Second Edge Profile Index is: "<<secondEdge.getProfileType()<<endl;
			cout<<"First Profile Edge Start Point is "<<firstProfileEdge.getStartPoint().x<<" "<<firstProfileEdge.getStartPoint().y<<" "<<firstProfileEdge.getStartPoint().z<<endl;
			cout<<"First Profile Edge End Point is "<<firstProfileEdge.getEndPoint().x<<" "<<firstProfileEdge.getEndPoint().y<<" "<<firstProfileEdge.getEndPoint().z<<endl;
		
		}
		ProfileEdge secondProfileEdge = currentProfileFromHeight.at(secondEdge.getProfileType());

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
		profVec1 = glm::vec3(newVector.x*profVec1.x,profVec1.y,newVector.z*profVec1.x);
		
		normal1 = glm::cross(edgeVec1, profVec1);
		normal1 = glm::normalize(normal1);
		
		if (DEBUG == 1) {
			cout<<endl;
			cout<<"New Vector 1 is "<<newVector.x<<" "<<newVector.y<<" "<<newVector.z<<endl;
			cout<<"First FloorPlan Edge Vector is "<<edgeVec1.x<<" "<<edgeVec1.y<<" "<<edgeVec1.z<<endl;
			cout<<"First Profile Edge Vectpr is "<<profVec1.x<<" "<<profVec1.y<<" "<<profVec1.z<<endl;
			cout<<"First Normal is "<<normal1.x<<" "<<normal1.y<<" "<<normal1.z<<endl;
			cout<<endl;
		}

		//Calculate normal of the second edge
		glm::vec3 edgeVec2 = secondEdge.getEndPoint()-secondEdge.getStartPoint();
		glm::vec3 profVec2 = secondProfileEdge.getEndPoint()-secondProfileEdge.getStartPoint();

		firstRot = glm::rotate(tempMat,-90.0f,glm::vec3(0,1,0));
		rotVec = glm::vec4(edgeVec2,1.0);
		rotVec = firstRot * rotVec;

		newVector = glm::vec3(rotVec);
		newVector = glm::normalize(newVector);

		cout<<"New Vector Of Second Edge is is "<<newVector.x<<" "<<newVector.y<<" "<<newVector.z<<endl;

		profVec2 = glm::vec3(newVector.x*profVec2.x,profVec2.y,newVector.z*profVec2.x);

		normal2 = glm::cross(edgeVec2, profVec2);
		normal2 = glm::normalize(normal2);

		//What if the normals were reversed?  Or at least one of them
		

		finalVector = glm::cross(normal1,normal2);
		


		if (DEBUG == 1) {
			cout<<endl;
			cout<<"Second FloorPlan Edge Vector is "<<edgeVec2.x<<" "<<edgeVec2.y<<" "<<edgeVec2.z<<endl;
			cout<<"Second Profile Edge Vectpr is "<<profVec2.x<<" "<<profVec2.y<<" "<<profVec2.z<<endl;
			cout<<"Second Normal is "<<normal2.x<<" "<<normal2.y<<" "<<normal2.z<<endl;
			cout<<"Final vector is "<<finalVector.x<<" "<<finalVector.y<<" "<<finalVector.z<<endl;
			cout<<endl;
		}
		//This is a hack that I'm going to attemp
		

		finalVector = glm::normalize(finalVector);
		finalVector.y = abs(finalVector.y);

		thePlan.setIntersectionVector(finalVector);
	}
}

/*
Goes through the active plan and figures out the intersection events.  It then stuffs them into the priority queue!
*/
void SweepPlane::fillQueueWithIntersections(float height)
{
	bool foundIntersections = false; 
	//This is where the intersection method should really start
	if (DEBUG == 1) { 
		cout<<"Active plan size: "<<thePlan.getActivePlan().size()<<endl;
		cout<<"Calculated vectors.  Size of intersection vecotrs: "<<thePlan.getIntersectionVectors().size()<<endl;
	}
	//Compare each corner to all the other ones and determine the possible intersection events
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		for (int j = 1; j < thePlan.getActivePlan().size(); j++) {
			glm::vec3 firstVec, secondVec;
			Corner firstCorner, secondCorner;
			//We will calculate the intersection with the corner and its next neighbor - need to handle the end
			firstVec = thePlan.getIntersectionVectors().at(i);
			secondVec = thePlan.getIntersectionVectors().at(j);
			firstCorner = thePlan.getActivePlan().at(i);
			secondCorner = thePlan.getActivePlan().at(j);
			firstVec = glm::normalize(firstVec);
			secondVec = glm::normalize(secondVec);

			if (DEBUG == 1){
				cout<<endl;
				cout<<"The first vector is "<<firstVec.x<<" "<<firstVec.y<<" "<<firstVec.z<<endl;
				cout<<"The second vector is "<<secondVec.x<<" "<<secondVec.y<<" "<<secondVec.z<<endl;
				cout<<endl;
			}

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
				//intersectionPoint.y+=firstCorner.getPt().y;
				foundIntersections = true;
				std::vector<Corner> source;
				source.push_back(firstCorner);
				source.push_back(secondCorner);
				Event intersect = Event(intersectionPoint.y, intersectionPoint,source, Event::INTERSECTION);

				q.push(intersect);
				if (DEBUG == 1) {
					cout<<"The size of the queue is "<<q.size()<<endl;
				}

			}
			else {
				if (DEBUG == 1) {
					cout<<"Didn't find intersection..."<<endl;
				}
				continue;
			}
		//}
	}
	}
	//if (foundIntersections == false) {
	//	killTheSweep = true; 
	//}
}

void SweepPlane::fillQueueWithEdgeDirectionChanges(float height){
	std::vector<ProfileEdge> currentEdges = getProfileEdgesAtHeight(height); 
	
	//We will prevent overlaps by only useing the right edge
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		Corner tempCorner = thePlan.getActivePlan().at(i);
		glm::vec3 cornerVec = thePlan.getIntersectionVectors().at(i);

		PlanEdge edge1 = tempCorner.getRightEdge();
		ProfileEdge profEdge = currentEdges.at(edge1.getProfileType());
		if (profEdge.getIsHorizontal()){
			continue;
		}
		
		float difference = profEdge.getEndPoint().y - tempCorner.getPt().y;
		float multiply = difference/(cornerVec.y+pow(10.0,-6.0));
		glm::vec3 newPoint = tempCorner.getPt()+(cornerVec*multiply);
		
		std::vector<Corner> parentCorner;
		parentCorner.push_back(tempCorner);

		//We are still going to do this for each of the corners for each of the edges they are associated with
		Event edgeChange = Event(profEdge.getEndPoint().y,newPoint,parentCorner,Event::PROFILE);
		q.push(edgeChange);
	}
}

void SweepPlane::fillQueueWithHorizontalChanges(float height){
	std::vector<ProfileEdge> currentEdges = getProfileEdgesAtHeight(height); 
	
	//We will prevent overlaps by only useing the right edge
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		Corner tempCorner = thePlan.getActivePlan().at(i);
		glm::vec3 cornerVec = thePlan.getIntersectionVectors().at(i);

		PlanEdge edge1 = tempCorner.getRightEdge();
		ProfileEdge profEdge = currentEdges.at(edge1.getProfileType());
		if (!profEdge.getIsHorizontal()){
			continue;
		}
		
		float difference = profEdge.getEndPoint().y - tempCorner.getPt().y;
		float multiply = difference/(cornerVec.y+pow(10.0,-6.0));
		glm::vec3 newPoint = tempCorner.getPt()+(cornerVec*multiply);
		
		std::vector<Corner> parentCorner;
		parentCorner.push_back(tempCorner);

		//We are still going to do this for each of the corners for each of the edges they are associated with
		Event edgeChange = Event(profEdge.getEndPoint().y,newPoint,parentCorner,Event::PROFILE);
		q.push(edgeChange);
	}
}

void SweepPlane::fillQueueWithAnchors(float height)
{
	for (int i = 0; i < anchorList.size(); i++)
	{
		float h = anchorList.at(i).getHeight(); 
		if (DEBUG_ANCHOR) 
			cout<<"****CHECKING ANCHOR EVENT****** at height"<<h<<endl;
		//TODO - should we calc the current position here instead?  i'm just sending in dummy x,z vals bc it seems silly if we aren't 
		//gunna use it for sure.
		if (h >= height) {
			cout<<"ADDING ANCHOR EVENT.."<<endl;
			Event anchorEvent = Event(h, glm::vec3(0, h, 0), Event::ANCHOR, i); 
			anchorQ.push(anchorEvent); 
		}
	}
}

bool SweepPlane::intersectionTest(glm::vec3 line1S, glm::vec3 line1E, glm::vec3 line2S, glm::vec3 line2E, glm::vec3 &intersection )
{
	glm::vec3 da = line1E-line1S, db = line2E-line2S, dc = line2S-line1S;

	if (glm::dot(glm::cross(da,db),dc) != 0.0){
		return false;
	}

	glm::vec3 tempCross = glm::cross(da,db);
	float norm = tempCross.x*tempCross.x + tempCross.y * tempCross.y + tempCross.z*tempCross.z;

	float s = glm::dot(glm::cross(dc,db),glm::cross(da,db))/norm;
	//cout<<"The s value is "<<s<<endl;
	if (s >= -.9990 && s <= 1.0001){

		intersection = line1S + da*s;
		return true;
	}


	return false;
}

//Generate the new priority q of the next active plan, sorted by the right-most parent index (ensures correct edge ordering) 
//This preprocesses our new floor plan so that the order of the new active plan follows that of the previous plan
std::priority_queue<Corner,std::vector<Corner>, CompareParent> SweepPlane::preprocessNewPlanQ(std::vector<Event> events)
{


	std::priority_queue<Corner,std::vector<Corner>, CompareParent> cornerQ;
	std::vector<bool> flagPlan;
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		flagPlan.push_back(false);
	}

	//Assuming that the intersections will automatically be at the front of the vector, these theoretically should be process first
	//Interchain handling
	for (int i = 0; i < events.size(); i++)
	{
		
		 Event e = events.at(i);
		 if (e.getType() == Event::ANCHOR) continue; 
		 if (e.getType() == Event::INTERSECTION){ 
			 Corner corner = Corner(e.getPoint(), e.getCorners());
			 for(int j = 0; j<corner.getSource().size(); j++){
				 Corner parent = corner.getSource().at(j);
				 flagPlan.at(parent.getIndex())=true;
			 }
			 cornerQ.push(corner);
		 }
		 else if (e.getType() == Event::PROFILE){
			 //This should only have 1 parent 
			 Corner corner = Corner(e.getPoint(), e.getCorners());
			 Corner parent = corner.getSource().at(0);
			 if (flagPlan.at(parent.getIndex())==true){
				 continue;
			 }
			 else {
				 cornerQ.push(corner);
				 flagPlan.at(parent.getIndex())=true;
			 } 
		 }
	}

	//This loop pushes any points that have not been brought up to the next height level, up to the next level
	cout<<"The size of the sorting queue is "<<cornerQ.size()<<endl;

	//Raise the extra points that don't get no love!
		for (int i = 0; i<flagPlan.size(); i++){
			if (flagPlan.at(i) == false){
				cout<<"We shouldn't be in here!!!"<<endl;
				glm::vec3 vec = thePlan.getIntersectionVectors().at(i);
				cout<<"Got vec "<<vec.x<<endl;
				Corner parentCorner = thePlan.getActivePlan().at(i);
				cout<<"Got rents "<<parentCorner.getIndex()<<endl;
				float newActivePlanHeight;
				if (cornerQ.size() > 0)
					newActivePlanHeight = cornerQ.top().getPt().y;
				else 
					newActivePlanHeight = events.at(0).getHeight();
				cout<<"New height " <<newActivePlanHeight<<endl;

				float difference = newActivePlanHeight - parentCorner.getPt().y;
				cout<<"Dif: "<<difference<<endl;
				float multiply = difference/(vec.y+pow(10.0,-6.0));
				cout<<"Mult: "<<multiply<<endl;
				glm::vec3 newPoint = parentCorner.getPt()+(vec*multiply);
				cout<<"newPoint: "<<newPoint.y<<endl;
				std::vector<Corner> myParent;
				myParent.push_back(parentCorner);
				cout<<"Pushing rent corner"<<endl;
				Corner finalCorner = Corner(newPoint,myParent);
				cout<<"Adding final corner"<<endl;
				cornerQ.push(finalCorner);
				cout<<"Added to corner q"<<endl;
			}
		}
	cout<<"Got through process new plan q"<<endl;
	return cornerQ;
}

//Updates the temp active plan with the newly caclulated edges
std::map<int, Anchor> SweepPlane::updateNewPlanEdges(std::vector<Corner> &tempActivePlan) 
{
	std::map<int, Anchor> tempMap; 
	if(DEBUG_ANCHOR) {
		cout<<"The current map size is : "<<thePlan.edgeAnchorMap.size()<<endl; 
		cout<<"The size: "<<tempActivePlan.size()<<endl;
	}

	for(int i = 0; i < tempActivePlan.size(); i++){
		glm::vec3 startPoint, endPoint;
		int profile;
		tempActivePlan.at(i).getRightEdge().setIndex(i); 

		if (i == tempActivePlan.size()-1){	
			startPoint = tempActivePlan.at(i).getPt();
			endPoint = tempActivePlan.at(0).getPt();
			std::vector<Corner> myCorn = tempActivePlan.at(i).getSource();
			profile = myCorn.at(myCorn.size()-1).getRightEdge().getProfileType();
			PlanEdge edge = PlanEdge(startPoint,endPoint,profile);
			tempActivePlan.at(i).setRightEdge(edge); //TODO setRightEdge
			tempActivePlan.at(0).setLeftEdge(edge);
			//Check anchors
			int edgeIndex = myCorn.at(myCorn.size()-1).getIndex();
			cout<<"The parent's corner index is: "<<edgeIndex; 
			if (thePlan.edgeAnchorMap.find(edgeIndex) != thePlan.edgeAnchorMap.end())
			{				
				//TODO - replace anchor insert w/a proper search
				if (tempMap.find(i) == tempMap.end()) {
					tempMap.insert(std::pair<int, Anchor>(i, anchorList.at(0)));
					if (DEBUG_ANCHOR) {
						cout<<"Found an anchor! at"<<edgeIndex<<endl;
					}
				}
			}

		} else {
			startPoint = tempActivePlan.at(i).getPt();
			endPoint = tempActivePlan.at(i+1).getPt();
			std::vector<Corner> myCorn = tempActivePlan.at(i).getSource();
			profile = myCorn.at(myCorn.size()-1).getRightEdge().getProfileType();
			PlanEdge edge = PlanEdge(startPoint,endPoint,profile);			
			tempActivePlan.at(i).setRightEdge(edge); //TODO setRightEdge
			tempActivePlan.at(i+1).setLeftEdge(edge);
			//Update edgeAnchorMap for new active plan
			int cornerIndex = myCorn.at(myCorn.size()-1).getIndex();
			cout<<"The parent's corner index is: "<<cornerIndex; 
			int edgeIndex = myCorn.at(myCorn.size()-1).getRightEdge().getIndex(); //TODO - check why this isn't working orjust delete
			if (thePlan.edgeAnchorMap.find(cornerIndex) != thePlan.edgeAnchorMap.end())
			{
				cout<<"Found an anchor at parent index "<<edgeIndex<<" Corresponding with new index"<<i<<endl;
				//TODO - replace anchor insert w/a proper search
				//Make sure we ignore dups in case multiple edges have the same rents
				if (tempMap.find(i) == tempMap.end()) {
					tempMap.insert(std::pair<int, Anchor>(i, anchorList.at(0)));
				}
			}
		} 
		if (DEBUG == 1) {
			cout<<"We are looping to set up the new active plan.  Finished number "<<i<<endl;
			cout<<"Temp active plan pt "<<tempActivePlan.at(i).getPt().x<<" "<<tempActivePlan.at(i).getPt().y<<" "<<tempActivePlan.at(i).getPt().z<<" "<<endl;
		}
	}
	return tempMap;
}

//Finds the anchor translation and rotation based on the current activeplan
void SweepPlane::calcAnchorTransforms(Anchor &a)
{
	std::deque<Corner> ap = thePlan.getActivePlan();
	float height = a.getHeight(); 
	int id = a.getID(); 
	if (DEBUG_ANCHOR) {
		cout<<thePlan.edgeAnchorMap.size()<<" Trying to find height: "<<height<<" and id "<<id<<endl;
	}
	for(map<int,Anchor>::iterator it =  thePlan.edgeAnchorMap.begin(); it !=  thePlan.edgeAnchorMap.end(); ++it) {

		if (it->second.getHeight() == height && it->second.getID() == id)
		{
			int edgeIndex = it->first; 
			float percentEdge = it->second.getFloorPlanPercent();
			//get the edge from the active plan and calc the angle from the point to the origin
			glm::vec3 start = ap.at(edgeIndex).getRightEdge().getStartPoint();
			glm::vec3 end = ap.at(edgeIndex).getRightEdge().getEndPoint();
			glm::vec3 dir = glm::normalize(end - start); 
			glm::vec3 point = start + percentEdge * dir;  
			//TODO - is there a better way to do this...not sure if it will always work
			glm::vec3 pointOnX(point.x, point.y, 0); 
			glm::vec3 angle = glm::atan(pointOnX / point); 
			if (DEBUG_ANCHOR) {
				cout<<"Checking: "<<it->second.getHeight()<<" and id: "<<it->second.getID()<<endl;
				cout<<"ROT ANGLE = "<<angle.x<< " " <<angle.y<<" "<<angle.z<<endl; 
			}
			float xDeg = angle.x * 180.0 / 3.14159265359;
		
			int profileNum = it->second.getProfileNum(); 
			int profileEdgeIndex = it->second.getProfileIndex();
			if (profileNum != ap.at(edgeIndex).getRightEdge().getProfileType())
				cout<<"UHOH! Our anchor edge/profile links don't match!! :("<<endl;
			ProfileEdge profileEdge = profileList.at(profileNum).getEdgeList().at(profileEdgeIndex);

			glm::vec3 profileDir = glm::normalize(profileEdge.getEndPoint() - profileEdge.getStartPoint()); 
			glm::vec3 trans = point + profileDir * it->second.getProfilePercent(); 
			if (DEBUG_ANCHOR) {
				cout<<"*************************ANCHOR PLAN EDGE************************************"<<endl;
				cout<<"Our plan edge is: "<<it->first<<" vs the index stored in edge: "<<it->second.getFloorPlanIndex()<<" and percent "<<percentEdge<<endl;
				cout<<"The plan start edge is: "<<start.x<<" "<<start.y<<" "<<start.z<<" and the end is "<<end.x<<" "<<end.y<<" "<<end.z<<endl;
				cout<<"The starting point along the edge is: "<<point.x<<" "<<point.y<<" "<<point.z<<endl;
				cout<<"The direction of the edge is: "<<dir.x<<" "<<dir.y<<" "<<dir.z<<endl;
				cout<<"The calced x,z loca of the anchor is: "<<trans.x<<" "<<trans.z<<endl;

				cout<<"*************************ANCHOR PROFILE************************************"<<endl;
				cout<<"Our profile edge index is: "<<profileEdgeIndex<<" and percent is: "<<it->second.getProfilePercent()<<endl;
				cout<<"Profile edge start y: "<<profileEdge.getStartPoint().y<<" End: "<<profileEdge.getEndPoint().y<<endl;
				cout<<"The calced y location of the anchor is: "<<trans.y<<" vs. the orig calc of: "<<it->second.getHeight()<<endl;
				cout<<"The direction of the profile is: "<<profileDir.x<<" "<<profileDir.y<<" "<<profileDir.z<<endl;

			}
			Anchor a = Anchor(it->second); 
			a.setRotY(xDeg);
			a.setTranslation(trans); 
			outputAnchors.push(a);
		}
	}
}

//Looks for anchor events to process
void SweepPlane::findAnchorEvents(std::vector<Event> e)
{
	if (DEBUG_ANCHOR) 
		cout<<"Finding anchor event in list size: "<<e.size()<<endl;
	for (int i = 0; i < e.size(); i++)
	{
		int index = e.at(i).getAnchorIndex(); 
		if (DEBUG_ANCHOR) {
			cout<<"FOUND ANCHOR EVENT!!! We are at height: "<<e.at(i).getHeight()<<endl;
			cout<<"The index is : "<<index<<endl;
		}

		calcAnchorTransforms(anchorList.at(index));
	}
}

//Handles all the event clustering (merges overlapping corners) 
std::vector<Corner> SweepPlane::processClusters(std::vector<Corner> &tempActivePlan) 
{
	//Now we cluster and then we have our next active plan!
	//Cluster the events based on the radius
	std::vector<bool> cornerInCluster;
	std::vector<vector<Corner>> clusters;
	std::vector<int> isSingle; 

	for (int i = 0; i < tempActivePlan.size(); i++)
	{
		cornerInCluster.push_back(false);
	}

	//Creates the clusters of points 
	for (int i = 0; i < tempActivePlan.size(); i++)
	{
		Corner temp = tempActivePlan.at(i);
		Edge e1 = temp.getLeftEdge();

		std::vector<Corner> temp2; 
		if (cornerInCluster.at(i) == false) {
			temp2.push_back(temp);
			cornerInCluster.at(i) = true; 
		}


		for (int k = 0; k < tempActivePlan.size(); k++)
		{
			if (k == i ) continue;
			if (glm::distance(tempActivePlan.at(i).getPt(), tempActivePlan.at(k).getPt()) < 0.1)
			{

				if (cornerInCluster.at(k) == false) {
					if (DEBUG == 1) cout<<"Found one that is close"<<endl;
					temp2.push_back(tempActivePlan.at(k));
					cornerInCluster.at(k) = true; 
				}
			}
		}
		clusters.push_back(temp2);
	}


	//Process the interclusters (merges them)
	std::vector<Corner> postCluster;
	for (int i = 0; i < clusters.size(); i++)
	{
		if (DEBUG == 1) cout<<"Number of points in cluster: "<<clusters.at(i).size()<<endl;
		//Keep track of whether there is 1 or more in the cluster for intercluster processing
		if (clusters.at(i).size() == 1) 
			isSingle.push_back(1);
		else 
			isSingle.push_back(0); 

		std::vector<Corner> cluster = clusters.at(i); 
		if (cluster.size() == 0) continue; 

		PlanEdge startEdge = cluster.at(0).getLeftEdge();
		PlanEdge endEdge = cluster.at(cluster.size() - 1).getRightEdge();
		std::vector<Corner> parents;

		for (int k = 0; k < cluster.size(); k++)
		{
			std::vector<Corner> tempSource = cluster.at(k).getSource();
			for (int j = 0; j < tempSource.size(); j++) {
				bool addParent = true; 
				for (int p = 0; p < parents.size(); p++) {
					if (tempSource.at(j).getIndex() == parents.at(p).getIndex()) addParent = false; 	
					cout<<"The parent's right edge index is: "<<tempSource.at(j).getRightEdge().getIndex()<<endl;
				}
				if (addParent == true) {
					parents.push_back(tempSource.at(j)); 
				}
			}
		}

		Corner newCorner = Corner(endEdge, startEdge, cluster.at(0).getPt(), parents);
		if(DEBUG == 1){
			cout<<"***********************"<<endl;
			cout<<"Parent indices at cluster"<<endl;
			for(int f = 0; f<newCorner.getSource().size(); f++){
				cout<<newCorner.getSource().at(f).getIndex()<<endl;
			}
		}

		//Need to deal with the case where 0 is involved and the numbers are mixed up.  This is a hack fix and could be improved by a priority queue???? 
		int indexToShift,previousIndex;
		for(int f = 0; f<newCorner.getSource().size(); f++){

			int currentIndex = newCorner.getSource().at(f).getIndex();
			if (f >= 1){
				previousIndex = newCorner.getSource().at(f-1).getIndex();
				if (currentIndex-previousIndex != 1){
					indexToShift = f;

					std::vector<Corner> improvedVector;
					//Push the mismatched ones onto the new corner - realize that this only works if almost sorted
					for(int g = indexToShift; g<newCorner.getSource().size(); g++){
						improvedVector.push_back(newCorner.getSource().at(g));
					}

					for(int g = 0; g<indexToShift; g++){
						improvedVector.push_back(newCorner.getSource().at(g));
					}
					std::sort(improvedVector.begin(), improvedVector.end(), CompareParent());
					newCorner = Corner(newCorner.getLeftEdge(), newCorner.getRightEdge(), newCorner.getPt(),improvedVector);
				}
			}	
		}

		

		postCluster.push_back(newCorner);
	}


	return postCluster;
}

//Runs through the q and process the events into a new active plan stack
void SweepPlane::processQueue()
{
	std::vector<Event> events;
	std::vector<Event> anchorEvents;

	ActivePlan localPlan;
	std::vector<Corner> tempActivePlan;

	Event firstEvent = q.top();
	if (firstEvent.getHeight() - theLastHeight < EPSILON) {
		killTheSweep = true;
		return;
	}
	theLastHeight = firstEvent.getHeight(); 
	q.pop();
	events.push_back(firstEvent);



	//Get all anchor events less than the first event height
	bool hasAnchor = false;
	bool updateAnchorMap = false; 
	Event firstAnchorEvent;
	if (anchorQ.size() > 0) {
		cout<<"Our anchor queue is now size: "<<anchorQ.size()<<endl;
		firstAnchorEvent = anchorQ.top();
		anchorQ.pop(); 
		anchorEvents.push_back(firstAnchorEvent); 
		if (firstAnchorEvent.getHeight() < firstEvent.getHeight()) hasAnchor = true; 
		if (firstAnchorEvent.getHeight() - firstEvent.getHeight() < EPSILON) updateAnchorMap = true; 
		while ((anchorQ.top().getHeight() - firstAnchorEvent.getHeight() < EPSILON) && !anchorQ.empty())
		{	
			anchorEvents.push_back(anchorQ.top()); 
			anchorQ.pop(); 
		}
	}


	//Get list of all events at the same height 
	while ((q.top().getHeight() - firstEvent.getHeight() < EPSILON) && !q.empty())
	{
		//cout<<"The height of the firs event is "<<firstEvent.getHeight()<<endl;
		//cout<<"The height of the event we are comparing is "<<q.top().getHeight()<<endl;
		events.push_back(q.top());
		q.pop();
	}

	//TODO- check if we need to filter invalid events (last section in intersection part of paper)
	//Generate the anchors if there are any on this level
	if (hasAnchor) 	findAnchorEvents(anchorEvents);  
	//Generate the new priority q of the next active plan, sorted by the right-most parent index (ensures correct edge ordering) 
	std::priority_queue<Corner,std::vector<Corner>, CompareParent> cornerQ;
	cornerQ = preprocessNewPlanQ(events); 

	//Go thorugh the priority queue and set the edges then turn it into a regular active plan for clustering!
	while(!cornerQ.empty()){
		if (DEBUG == 1) {
			cout<<"CornerQ pt "<<cornerQ.top().getPt().x<<" "<<cornerQ.top().getPt().y<<" "<<cornerQ.top().getPt().z<<" "<<endl;
		}
		tempActivePlan.push_back(cornerQ.top());
		cornerQ.pop();
	}

	//Updates the temp active plan to include its new edges 
	std::map<int, Anchor> temp = updateNewPlanEdges(tempActivePlan); 
	std::map<int, Anchor> temp2 = thePlan.edgeAnchorMap;
	//Process the intraclusters & the interclusters
	thePlan = ActivePlan(processClusters(tempActivePlan));

	if (updateAnchorMap) {
		thePlan.edgeAnchorMap.clear(); 
		thePlan.edgeAnchorMap = temp;
	} else {
		thePlan.edgeAnchorMap = temp2; 
	}


	//Clean out the queue
	while (!q.empty()){
		q.pop();
	}
}

void SweepPlane::addAnchorsToFloorPlan()
{
	//Add anchors to the current plan
	std::cout<<"The size of the anchors in sweepplane is: "<<anchorList.size()<<endl;
	for (int i = 0; i < anchorList.size(); i++)
	{
		cout<<"Adding anchor to map..."<<endl;
		thePlan.edgeAnchorMap.insert(std::pair<int, Anchor>(anchorList.at(i).getFloorPlanIndex(), anchorList.at(i)));
	}
}

void SweepPlane::addEdgeIndex()
{
	//Add the edge order
	for (int i = 0; i < thePlan.getActivePlan().size(); i++) {
		thePlan.getActivePlan().at(i).getRightEdge().setIndex(i); 
	}
}

//The main method that checks for different events & builds the final stack from the active plan
void SweepPlane::buildIt()
{
	int f = 1;
	thePlan = ActivePlan(plan);

	addAnchorsToFloorPlan();
	if (DEBUG == 0) {
		cout<<"THE CURRENT SIZE OF THE ANCHORS ARE: "<<thePlan.edgeAnchorMap.size()<<endl;
		cout<<"The value of the key should be: "<<thePlan.edgeAnchorMap.at(0).getFloorPlanIndex()<<endl;
		//for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		//	cout<<"In START OF BUILDIT: The index for each edge is "<<thePlan.getActivePlan().at(i).getIndex()<<endl;
		//}
	}
	activePlanStack.push(thePlan);
	activePlanQueue.push(thePlan);
	float height = thePlan.getActivePlan().at(0).getPt().y;;
	updateIntersectionVectors(height);
	fillQueueWithIntersections(height);
	fillQueueWithEdgeDirectionChanges(height);
	fillQueueWithAnchors(height);

	if (DEBUG == 1) {
		cout<<"Supposedly filled with events"<<endl;
		cout<<"The queue size is "<<q.size()<<endl;
	}
	while (!q.empty())
	{
		f++;
		processQueue();
		cout<<"THE CURRENT SIZE OF THE ANCHORS ARE: "<<thePlan.edgeAnchorMap.size()<<endl;
		activePlanStack.push(thePlan);
		activePlanQueue.push(thePlan);
		cout<<"In the main loop"<<endl;
		
		if (DEBUG == 1) {
			cout<<"Made it into the first queue loop"<<endl;
			cout<<"The stack has "<<activePlanStack.size()<<" and the queue has "<<activePlanQueue.size()<<endl;
		}

		 height = thePlan.getActivePlan().at(0).getPt().y;
		
		////This is an arbitrary height for testing
		//if (thePlan.getActivePlan().size() <= 2 || f == 4)
		//	break;

		//Clear out the current plan's vectors and recalculate them.
		thePlan.cleanIntersectionVectors();

		if (DEBUG == 1){
			cout<<"The new height of the active plan is: "<<height<<endl;
		}
		updateIntersectionVectors(height); 
		if (killTheSweep == false) {
			fillQueueWithIntersections(height);	
			fillQueueWithEdgeDirectionChanges(height);
			fillQueueWithAnchors(height);
		} else {
			break;
		}
	}
}

SweepPlane::~SweepPlane(void)
{
}

ActivePlan SweepPlane::getThePlan()
{
	return thePlan;
}

void SweepPlane::setThePlan(FloorPlan plan)
{
	thePlan = ActivePlan(plan);
}
