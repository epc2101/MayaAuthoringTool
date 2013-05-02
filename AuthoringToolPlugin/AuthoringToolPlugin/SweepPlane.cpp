#include "SweepPlane.h"
#include <iostream>
#include "maya/MFnMesh.h"
#include "maya/MFloatPointArray.h"
#include "maya/MPointArray.h"

using namespace std;
float EPSILON = 0.0001;
float RADIUS =  0.000001;


int DEBUG = 0; 
int DEBUG_ANCHOR = 0; 
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
	
	//Preprocessing to kill the algo 
	maxProfileHeight = 0; 
	for (int i = 0; i < pList.size(); i++) 
	{
		ProfileEdge pe = pList.at(i).getEdgeList().at(pList.at(i).getEdgeList().size() - 1); 
		if (pe.getEndPoint().y > maxProfileHeight) maxProfileHeight = pe.getEndPoint().y; 
	}
	std::vector<PlanEdge> pe = plan.getEdgeList();
	avgPlanCenter = glm::vec3(0); 
	for (int i = 0; i < pe.size(); i++) 
	{
		avgPlanCenter += pe.at(i).getEndPoint(); 
	}
	avgPlanCenter /= (float) pe.size(); 
	cout<<"The avg plan center is : "<<avgPlanCenter.x<<" "<<avgPlanCenter.y<<" "<<avgPlanCenter.z<<endl;
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
			//cout<<"In plan : "<<i<<endl;
			ActivePlan temp = activePlanQueue.front();
			for(int j = 0; j<temp.getActivePlan().size(); j++){
				glm::vec3 tempVec = temp.getActivePlan().at(j).getPt();
				//cout<<"Point is "<<tempVec.x<< " "<<tempVec.y<<" " <<tempVec.z<<endl;
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
			//cout<<"Active plan stack is not empty"<<endl;
			ActivePlan temp = activePlanStack.top();
			activePlanStack.pop();
			stackLevel--;
			//Check if this is the floorplan
			if(temp.getActivePlan().at(0).getSource().size()==0){
				//cout<<"Taking care of the floorplan"<<endl;
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
			//cout<<"The point "<<pointArray[i].x<<" "<<pointArray[i].y<<" "<<pointArray[i].z<<endl;
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

	//Ghetto fix!!
	if (posArray.length() == 0) posArray.append(0);
	if (rotArray.length() == 0) rotArray.append(0); 

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
	float HEIGHTTEST = 0.0001;
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

			cout<<"The current height is "<<height<<" and it's being compared to profile end pt "<<edge.getEndPoint().y<<" and start pt "<<edge.getStartPoint().y<<endl;
			/*if (height >= edge.getEndPoint().y){
			continue;
			}*/

			if (edge.getIsTop() == true){
				//cout<<"Found top edge at height: "<<edge.getEndPoint().y<<endl;
				if (edge.getEndPoint().y == theLastHeight) {
					cout<<"The edge is a top & it is the same height as it was the last time."<<endl;
					killTheSweep = true; 
					break;
				}
				if (height >= edge.getEndPoint().y && edge.getEndPoint().y != theLastHeight){
					//cout<<"Adding the last edge height"<<endl;
					currentProfileFromHeight.push_back(edge);
					prof.getEdgesUsed().at(j) = true;
					break;
				} 
			} 
			
			if (height >= edge.getStartPoint().y && height < (edge.getEndPoint().y)){
				//We test the current edge and see if the one below it has been taken if it is horizontal.
				//if(prof.getEdgesUsed().at(j)==false && j > 0){
				//	//We can see if we skipped over the previous edge 
				//	if (prof.getEdgesUsed().at(j-1) == false && edge.getIsHorizontal()){
				//		cout<<"The lower edge is horizontal!!!!"<<endl;
				//		ProfileEdge horizEdge = prof.getEdgeList().at(j-1);
				//		currentProfileFromHeight.push_back(horizEdge);
				//		prof.getEdgesUsed().at(j-1) = true;
				//		break;
				//	}

				//}
				//cout<<"Adding general profile change event"<<endl;
				currentProfileFromHeight.push_back(edge);
				prof.getEdgesUsed().at(j) = true;
				break;
			}	
		}
	}

	float topHeight = 0.0;
	for(int i = 0; i<currentProfileFromHeight.size(); i++){
		if(currentProfileFromHeight.at(i).getEndPoint().y > topHeight){
			topHeight = currentProfileFromHeight.at(i).getEndPoint().y;
		}
	}

	if (height > topHeight){
		cout<<"KILLING - because the height is greater than the top height"<<endl;
		killTheSweep = true;
	}
	//cout<<"AT THE END OF GET PROFILES FROM HEIGHT"<<endl;
	return currentProfileFromHeight; 

}


//Updates the vectors from the active plan's corners based off of the profile's intersections
void SweepPlane::updateIntersectionVectors(float height)
{
	std::vector<ProfileEdge> currentProfileFromHeight = getProfileEdgesAtHeight(height);
	std::vector<bool> aboveTheProfile;
	aboveTheProfile.resize(0);
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
		cout<<"Killing because its too high"<<endl;
		killTheSweep = true;
		return;
	}

	//If we are down to a single point in the plane, we kill the algorithm
	if (thePlan.getActivePlan().size() == 1){
			cout<<"Killing because we only have one point in the plane"<<endl;
			killTheSweep = true;
			return;
	}

	cout<<"TESTING UPDATE INTERSECTION VECTORS.."<<endl; 
	//Iterate through the active plan and calculate the varying vectors based on the different edge plans.
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		
		//At cach corner we need the current vector associated
		Corner corner = thePlan.getActivePlan().at(i);
		PlanEdge firstEdge = corner.getLeftEdge();
		PlanEdge secondEdge = corner.getRightEdge();		
		ProfileEdge firstProfileEdge = currentProfileFromHeight.at(firstEdge.getProfileType());
		//if (firstEdge.getStartPoint().x < -100) {
			cout<<"*************************************************"<<endl;
			cout<<"In corner: "<<corner.getIndex()<<endl;
			cout<<"First edge start: "<<firstEdge.getStartPoint().x<<" "<<firstEdge.getStartPoint().y<<" "<<firstEdge.getStartPoint().z<<" " <<endl;
			cout<<"First edge end: "<<firstEdge.getEndPoint().x<<" "<<firstEdge.getEndPoint().y<<" "<<firstEdge.getEndPoint().z<<" " <<endl;
			cout<<"Second edge start: "<<secondEdge.getStartPoint().x<<" "<<secondEdge.getStartPoint().y<<" "<<secondEdge.getStartPoint().z<<" " <<endl;
			cout<<"Second edge end: "<<secondEdge.getEndPoint().x<<" "<<secondEdge.getEndPoint().y<<" "<<secondEdge.getEndPoint().z<<" " <<endl;

		//Next we test whether the edge are the same!
		if (glm::length(firstEdge.getStartPoint() - secondEdge.getStartPoint()) < EPSILON && glm::length(firstEdge.getEndPoint() - secondEdge.getEndPoint()) < EPSILON)
		{
			cout<<"KILLING - because the edges start points are the same."<<endl;
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

		

		glm::vec3 edgeVec1 = firstEdge.getEndPoint()-firstEdge.getStartPoint();
		glm::vec3 profVec1 = firstProfileEdge.getEndPoint()-firstProfileEdge.getStartPoint();
			
		glm::vec3 edgeVec2 = secondEdge.getEndPoint()-secondEdge.getStartPoint();
		glm::vec3 profVec2 = secondProfileEdge.getEndPoint()-secondProfileEdge.getStartPoint();
 
		thePlan.setIntersectionVector(generateIntersection(edgeVec1,edgeVec2,profVec1,profVec2));
	}
	cout<<"AT THE END OF INTERSECTION VECTOR"<<endl;
}

/*
Goes through the active plan and figures out the intersection events.  It then stuffs them into the priority queue!
*/
void SweepPlane::fillQueueWithIntersections(float height)
{
	//if (height > 8  && height < 11)
	//{
		//cout<<"**************************************************************************"<<endl;
		//cout<<"**************************************************************************"<<endl;
		//cout<<"**************************************************************************"<<endl;
	//	cout<<"In fille q with intersections"<<endl;
	//}

	bool foundIntersections = false; 
	//This is where the intersection method should really start
	if (DEBUG == 1) { 
		cout<<"Active plan size: "<<thePlan.getActivePlan().size()<<endl;
		cout<<"Calculated vectors.  Size of intersection vecotrs: "<<thePlan.getIntersectionVectors().size()<<endl;
	}
	//Compare each corner to all the other ones and determine the possible intersection events
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		for (int j = 1; j < thePlan.getActivePlan().size(); j++) {

			//We skip if the vector is being compared to itself
			if (i != j) {

				glm::vec3 firstVec, secondVec;
				Corner firstCorner, secondCorner;
				//We will calculate the intersection with the corner and its next neighbor - need to handle the end
				firstVec = thePlan.getIntersectionVectors().at(i);
				secondVec = thePlan.getIntersectionVectors().at(j);

				firstCorner = thePlan.getActivePlan().at(i);
				secondCorner = thePlan.getActivePlan().at(j);

				firstVec = glm::normalize(firstVec);
				secondVec = glm::normalize(secondVec);

				//cout<<"***Intersection Corner Indices****"<<endl; 
				//cout<<"***i = "<<i<<"***j = "<<j<<endl;
				//cout<<"First: "<< firstCorner.getIndex()<<" Second: "<<secondCorner.getIndex()<<endl; 
				//	cout<<"Starting from first: "<<firstCorner.getIndex()<<" ->"<<firstStartPoint.x<<" "<<firstStartPoint.y<<" "<<firstStartPoint.z<<endl;
				//	cout<<"Starting from second: "<<secondCorner.getIndex()<<" ->"<<secondStartPoint.x<<" "<<secondStartPoint.y<<" "<<secondStartPoint.z<<endl;


				if (DEBUG == 1){
					if ((firstCorner.getIndex() == 5 || firstCorner.getIndex() == 6) && height > 8) {
						if (firstVec.y < 0.0){
							cout<<"First vec is negative"<<endl;
						}
						if (secondVec.y < 0.0){
							cout<<"Second vec is negative"<<endl;
						}
						cout<<endl;

						cout<<"The first vector is "<<firstVec.x<<" "<<firstVec.y<<" "<<firstVec.z<<endl;
						cout<<"The second vector is "<<secondVec.x<<" "<<secondVec.y<<" "<<secondVec.z<<endl;
						cout<<endl;
					}
				}

				//Test if the vectors are parallel (might want to utilize an epsilon value for equality to deal with floating point issues
				//We extend the vectors in both directions from their current points and perform a line intersection test
				glm::vec3 firstStartPoint, secondStartPoint, firstBelowPoint, secondBelowPoint, firstTopPoint, secondTopPoint, intersectionPoint;

				//These are the initial starting points
				firstStartPoint = firstCorner.getPt();
				secondStartPoint = secondCorner.getPt();

				float t = 10000.0;

				//The top points are multiplied by a large t value (around 10000)
				firstTopPoint = firstStartPoint + (firstVec*t);
				secondTopPoint = secondStartPoint + (secondVec*t);

				//if (height > 5 && height < 11) {
				//if (firstCorner.getIndex() == 5) {
				//	cout<<"****************INDEX 5 & 6**********************"<<endl;
				//	cout<<"Starting from first: "<<firstCorner.getIndex()<<" ->"<<firstStartPoint.x<<" "<<firstStartPoint.y<<" "<<firstStartPoint.z<<endl;
				//	cout<<"Starting from second: "<<secondCorner.getIndex()<<" ->"<<secondStartPoint.x<<" "<<secondStartPoint.y<<" "<<secondStartPoint.z<<endl;
				//}
				//}

				bool intTest1 = intersectionTest(firstStartPoint,firstTopPoint,secondStartPoint,secondTopPoint,intersectionPoint);
				float mua, mub; 
				glm::vec3 pa, pb; 
				bool intTest2 = shortestDistTest(firstStartPoint,firstTopPoint,secondStartPoint,secondTopPoint, mua, mub, pa, pb);
				bool addToQ = true; 
				if (intTest1 || intTest2 ){
					//Check if the direct intersection test has missed & see if the shortest dist is close enough to count as intersection
					if (intTest1 == false) {
						//Check if the y value is greater than the current height
						if (pa.y > (height) && fabs(pa.x - pb.x) < 0.25f && fabs(pa.y - pb.y) < 0.25f && fabs(pa.z - pb.z) < 0.25f) {
							intersectionPoint = ((pa + pb) /2.f); 
						} else {
							addToQ = false; 
						}
					}
					//Make sure that we have a valid point
				
					if (intersectionPoint.y > maxProfileHeight || glm::distance(intersectionPoint, avgPlanCenter) > 50){
						addToQ = false; 
					}

					if (addToQ) {
						//This is the code to create the intersection event and push it onto the queue
						cout<<"The intersection point generated is: "<<intersectionPoint.x<<" "<<intersectionPoint.y<<" "<<intersectionPoint.z<<endl;
						cout<<"The distance between the points is: "<<glm::distance(intersectionPoint, avgPlanCenter)<<endl;
						cout<<"The avg plan center is : "<<avgPlanCenter.x<<" "<<avgPlanCenter.y<<" "<<avgPlanCenter.z<<endl;
						
						foundIntersections = true;
						std::vector<Corner> source;
						source.push_back(firstCorner);
						source.push_back(secondCorner);
						Event intersect = Event(intersectionPoint.y, intersectionPoint,source, Event::INTERSECTION);

						q.push(intersect);
					}
				}
			}
		}
	}


}

void SweepPlane::fillQueueWithEdgeDirectionChanges(float height){
	std::vector<ProfileEdge> currentEdges = getProfileEdgesAtHeight(height); 
	if (thePlan.getActivePlan().size()==1){
		return;
	}

	//We will prevent overlaps by only useing the right edge
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		Corner tempCorner = thePlan.getActivePlan().at(i);
		glm::vec3 cornerVec = thePlan.getIntersectionVectors().at(i);

		PlanEdge edge1 = tempCorner.getRightEdge();
		ProfileEdge profEdge = currentEdges.at(edge1.getProfileType());
		if (profEdge.getIsHorizontal()){
			continue;
		}
		
		float difference = abs(profEdge.getEndPoint().y - tempCorner.getPt().y);
		float multiply = difference/(cornerVec.y+pow(10.0,-6.0));
		glm::vec3 newPoint = tempCorner.getPt()+(cornerVec*multiply);
		//newPoint.y = abs(newPoint.y);
		
		std::vector<Corner> parentCorner;
		parentCorner.push_back(tempCorner);
		//TODO********************************************************************
		//TODO - THIS IS A HACKY FIX BC WE SHOULD NEVERRRR GET A POINT THAT IS NOT ACTUALLY ON THE PROFILE...
		if (glm::distance(newPoint, avgPlanCenter) < 50) {
		//TODO********************************************************************
			//We are still going to do this for each of the corners for each of the edges they are associated with
			Event edgeChange = Event(profEdge.getEndPoint().y,newPoint,parentCorner,Event::PROFILE);
			q.push(edgeChange);
		}
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
		
		float difference = abs(profEdge.getEndPoint().y - tempCorner.getPt().y);
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

	if (glm::dot(dc, glm::cross(da,db)) != 0.0){
		return false;
	}

	glm::vec3 tempCross = glm::cross(da,db);
	float norm = tempCross.x*tempCross.x + tempCross.y * tempCross.y + tempCross.z*tempCross.z;

	float s = glm::dot(glm::cross(dc,db),glm::cross(da,db))/norm;
	//cout<<"The s value is "<<s<<endl;
	if (s >= -.0001 && s <= 1.0001){
		cout<<"Detected an intersection at height "<<intersection.y<<endl;
		intersection = line1S + da*abs(s);
		return true;
	}
	return false;
}

//Calcs the shortest distance between two lines.  Use as backup if we are off by a small value
//Thanks to Paul Bourke http://paulbourke.net/geometry/pointlineplane/lineline.c
bool SweepPlane::shortestDistTest(glm::vec3  p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, float &mua, float &mub, glm::vec3 &pa, glm::vec3 &pb)
{
   glm::vec3 p13,p43,p21;
   float d1343,d4321,d1321,d4343,d2121;
   float numer,denom;

   p13.x = p1.x - p3.x;
   p13.y = p1.y - p3.y;
   p13.z = p1.z - p3.z;
   p43.x = p4.x - p3.x;
   p43.y = p4.y - p3.y;
   p43.z = p4.z - p3.z;
   if (abs(p43.x) < EPSILON && abs(p43.y) < EPSILON && abs(p43.z) < EPSILON)
      return(FALSE);
   p21.x = p2.x - p1.x;
   p21.y = p2.y - p1.y;
   p21.z = p2.z - p1.z;
   if (abs(p21.x) < EPSILON && abs(p21.y) < EPSILON && abs(p21.z) < EPSILON)
      return(FALSE);

   d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
   d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
   d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
   d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
   d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

   denom = d2121 * d4343 - d4321 * d4321;
   if (abs(denom) < EPSILON)
      return false;
   numer = d1343 * d4321 - d1321 * d4343;

   mua = numer / denom;
   mub = (d1343 + d4321 * (mua)) / d4343;

   pa.x = p1.x + mua * p21.x;
   pa.y = p1.y + mua * p21.y;
   pa.z = p1.z + mua * p21.z;
   pb.x = p3.x + mub * p43.x;
   pb.y = p3.y + mub * p43.y;
   pb.z = p3.z + mub * p43.z;

   return true;
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
			 if (corner.getPt().x < -100) 
				cout<<"We pushed an INTERSECTION on this at point: "<<" "<<corner.getPt().x<<corner.getPt().y<<endl;
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
				 if (corner.getPt().x < -100) 
					cout<<"We pushed an PROFILE on this at point: "<<" "<<corner.getPt().x<<corner.getPt().y<<endl;
				 cornerQ.push(corner);
				 flagPlan.at(parent.getIndex())=true;
			 } 
		 }
	}

	//This loop pushes any points that have not been brought up to the next height level, up to the next level
	//cout<<"The size of the sorting queue is "<<cornerQ.size()<<endl;
	int numPointsNotRaised = 0;
	for(int i = 0; i<flagPlan.size(); i++){
		if (flagPlan.at(i) == false){
			numPointsNotRaised++;
		}
	}
	//cout<<"The number of points not yet raised is: "<<numPointsNotRaised<<endl;

	//Raise the extra points that don't get no love!
		for (int i = 0; i<flagPlan.size(); i++){
			if (flagPlan.at(i) == false){
				cout<<"RAISING EXTRA POINTS: We shouldn't be in here!!!"<<endl;
				glm::vec3 vec = thePlan.getIntersectionVectors().at(i);
				//cout<<"Got vec "<<vec.x<<endl;
				Corner parentCorner = thePlan.getActivePlan().at(i);
				//cout<<"Got rents "<<parentCorner.getIndex()<<endl;
				float newActivePlanHeight;
				if (cornerQ.size() > 0) {
					newActivePlanHeight = cornerQ.top().getPt().y;
					//newActivePlanHeight = events.at(0).getHeight();
				}
				else  {
					newActivePlanHeight = events.at(0).getHeight();
				}
				cout<<"New height " <<newActivePlanHeight<<endl;

				float difference = abs(newActivePlanHeight - parentCorner.getPt().y);
				//cout<<"Dif: "<<difference<<endl;
				float multiply = difference/(vec.y+pow(10.0,-6.0));
				//cout<<"Mult: "<<multiply<<endl;
				glm::vec3 newPoint = parentCorner.getPt()+(vec*multiply);
				//cout<<"newPoint: "<<newPoint.y<<endl;
				std::vector<Corner> myParent;
				myParent.push_back(parentCorner);
				//cout<<"Pushing rent corner"<<endl;
				Corner finalCorner = Corner(newPoint,myParent);
				//cout<<"Adding final corner"<<endl;
				if (finalCorner.getPt().x < -100)
					cout<<"We RAISED A CORNER on this at point: "<<" "<<finalCorner.getPt().x<<finalCorner.getPt().y<<endl;
				cornerQ.push(finalCorner);
				//cout<<"Added to corner q"<<endl;
			}
		}
	cout<<"Got through process new plan q"<<endl;
	return cornerQ;
}


//Updates the temp active plan with the newly caclulated edges
void SweepPlane::updateNewPlanEdges(std::vector<Corner> &tempActivePlan) 
{
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
			//TODO - CHECK IF THIS IS CORRECT - FLIPPED 0 & i & i, i+1
			tempActivePlan.at(i).setRightEdge(edge); //TODO setRightEdge
			tempActivePlan.at(0).setLeftEdge(edge);
			//TODO - Deleted map update here...maybe add back?
		} else {
			startPoint = tempActivePlan.at(i).getPt();
			endPoint= tempActivePlan.at(i+1).getPt();
			std::vector<Corner> myCorn = tempActivePlan.at(i).getSource();
			profile = myCorn.at(myCorn.size()-1).getRightEdge().getProfileType();
			PlanEdge edge = PlanEdge(startPoint,endPoint,profile);			
			tempActivePlan.at(i).setRightEdge(edge); //TODO setRightEdge
			tempActivePlan.at(i+1).setLeftEdge(edge);
			//TODO - Deleted map update here...maybe add back?
		} 
		if (DEBUG == 1) {
			cout<<"We are looping to set up the new active plan.  Finished number "<<i<<endl;
			cout<<"Temp active plan pt "<<tempActivePlan.at(i).getPt().x<<" "<<tempActivePlan.at(i).getPt().y<<" "<<tempActivePlan.at(i).getPt().z<<" "<<endl;
		}
	}
}

void SweepPlane::calcAnchorTransforms(Anchor &a)
{
	if (DEBUG_ANCHOR) {
		cout<<"*************************IN CALC ANCHOR TRANSFORMS************************************"<<endl;
		cout<<"*************************************************************"<<endl;
		cout<<"*************************************************************"<<endl;
	}
	std::deque<Corner> ap = thePlan.getActivePlan();
	float height = a.getHeight(); 
	int id = a.getID(); 
	if (DEBUG_ANCHOR) {
		cout<<"PRINTING the plan edges: "<<endl;
		for (int i = 0; i < ap.size(); i++) {
			cout<<"Index: "<<ap.at(i).getIndex()<<endl;
			//cout<<"Start: "<<ap.at(i).getRightEdge().getStartPoint().x<<" "<<ap.at(i).getRightEdge().getStartPoint().z<<" End: "<<ap.at(i).getRightEdge().getEndPoint().x<<" "<<ap.at(i).getRightEdge().getEndPoint().z<<endl;
			cout<<"LEFT - Start: "<<ap.at(i).getLeftEdge().getStartPoint().x<<" "<<ap.at(i).getLeftEdge().getStartPoint().z<<" End: "<<ap.at(i).getLeftEdge().getEndPoint().x<<" "<<ap.at(i).getLeftEdge().getEndPoint().z<<endl;

		}
	}
	int edgeIndex = a.getFloorPlanIndex(); 
	float percentEdge = a.getFloorPlanPercent();
	//get the edge from the active plan and calc the angle from the point to the origin
	glm::vec3 start = ap.at(edgeIndex).getLeftEdge().getStartPoint();
	glm::vec3 end = ap.at(edgeIndex).getLeftEdge().getEndPoint();
	glm::vec3 dir = glm::normalize(end - start); 
	glm::vec3 point = start + percentEdge * dir * glm::distance(start, end);  
	
	//Translate the edge to the origin & find the angle from the end to the vector created by projecting the end to z = 0
	glm::vec3 translatedEdge = glm::normalize(end - start); 
	glm::vec3 anchorDirO(1, 0, 0); 
	float theta = glm::dot(translatedEdge, anchorDirO) / (glm::length(translatedEdge) * glm::length(anchorDirO));
	float angle; 
	angle = acos(theta) * 180.0f / 3.14159265359f;

	/*Since the anchors are always facing in pos z direction, 
	we must adjust the angle based on the direction of the edge so it faces outward*/
	//TODO - this will need to be changed if we start considering cw/ccw based on maya's system (looking from top view we are act going clockwise now)
	if (translatedEdge.x == 1) angle = 180;
	else if (translatedEdge.x == -1) angle = 0; 
	else if (translatedEdge.z < 0 ) angle -= 180; 
	else if (translatedEdge.z > 0 ) angle = -1.f * angle + 180;
	
	int profileNum = a.getProfileNum(); 
	int profileEdgeIndex = a.getProfileIndex();
	ProfileEdge profileEdge = profileList.at(profileNum).getEdgeList().at(profileEdgeIndex);

	glm::vec3 profileDir = glm::normalize(profileEdge.getEndPoint() - profileEdge.getStartPoint()); 
	glm::vec3 trans = point + profileDir * (a.getProfilePercent() * glm::distance(profileEdge.getEndPoint(), profileEdge.getStartPoint())); 
	if (DEBUG_ANCHOR) {
		cout<<"*************************ANCHOR PLAN EDGE************************************"<<endl;
		cout<<"vs the index stored in edge: "<<a.getFloorPlanIndex()<<" and percent "<<percentEdge<<endl;
		cout<<"The plan start edge is: "<<start.x<<" "<<start.y<<" "<<start.z<<" and the end is "<<end.x<<" "<<end.y<<" "<<end.z<<endl;
		cout<<"The starting point along the edge is: "<<point.x<<" "<<point.y<<" "<<point.z<<endl;
		cout<<"The direction of the edge is: "<<dir.x<<" "<<dir.y<<" "<<dir.z<<endl;
		cout<<"The translated x, y, z "<<translatedEdge.x<<" "<<translatedEdge.y<<" "<<translatedEdge.z<<endl;
		cout<<"The ROTATION of the edge is: "<<angle<<endl;
		cout<<"The calced x,y,z loca of the anchor is: "<<trans.x<<" "<<trans.y<<" " <<trans.z<<endl;

		cout<<"*************************ANCHOR PROFILE************************************"<<endl;
		cout<<"Our profile edge index is: "<<profileEdgeIndex<<" and percent is: "<<a.getProfilePercent()<<endl;
		cout<<"Profile edge start y: "<<profileEdge.getStartPoint().y<<" End: "<<profileEdge.getEndPoint().y<<endl;
		cout<<"The calced y location of the anchor is: "<<trans.y<<" vs. the orig calc of: "<<a.getHeight()<<endl;
		cout<<"The direction of the profile is: "<<profileDir.x<<" "<<profileDir.y<<" "<<profileDir.z<<endl;
	}
	trans.y = a.getHeight(); 
	Anchor a2 = Anchor(a); 
	a2.setRotY(angle);
	a2.setTranslation(trans); 
	outputAnchors.push(a2);	
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
					//cout<<"The parent's right edge index is: "<<tempSource.at(j).getRightEdge().getIndex()<<endl;
				}
				if (addParent == true) {
					parents.push_back(tempSource.at(j)); 
				}
			}
		}

		Corner newCorner = Corner(endEdge, startEdge, cluster.at(0).getPt(), parents);
		if(DEBUG == 1){
			cout<<"***********************"<<endl;
			cout<<"Parent indices at cluster.  The size of the sources are: "<<newCorner.getSource().size()<<endl;
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
					//cout<<"The size of the improved vector is: "<<improvedVector.size()<<endl;
					std::sort(improvedVector.begin(), improvedVector.end(), CompareParent());
					//cout<<"after sorting?"<<endl;
					newCorner = Corner(newCorner.getLeftEdge(), newCorner.getRightEdge(), newCorner.getPt(),improvedVector);
					//cout<<"After last for loop."<<endl;
				}
			}	
		}

		//Make sure that we are not adding "bad" corners (ones that are wayyy to far away) to the active plan
		bool addCorner = true; 
		if (glm::distance(avgPlanCenter, newCorner.getPt()) > 1000) 
		{
			addCorner = false; 
		}

		//if (addCorner) {
			postCluster.push_back(newCorner);
		//}
	}

	if (DEBUG == 1) cout<<"Got to end of process clusters."<<endl;

	return postCluster;
}

//Runs through the q and process the events into a new active plan stack
void SweepPlane::processQueue()
{
	cout<<"IN THE BEGINNING OF PROCESS Q"<<endl;
	std::vector<Event> events;
	std::vector<Event> anchorEvents;

	ActivePlan localPlan;
	std::vector<Corner> tempActivePlan;

	Event firstEvent = q.top();
	//if (fabs(firstEvent.getHeight() - theLastHeight) < EPSILON) {
	//	sameHeightCount++; 
	//	if (sameHeightCount > 100000) {
	//		cout<<"KILLING - killed because we are not finding any new heights"<<endl;
	//		killTheSweep = true;
	//		return;
	//	}
	//}

	theLastHeight = firstEvent.getHeight(); 
	cout<<"The first event type is: ";
	if (firstEvent.getType() == Event::ANCHOR) cout<<"anchor";
	else if (firstEvent.getType() == Event::INTERSECTION) cout<<"intersection";
	else if (firstEvent.getType() == Event::PROFILE) cout<<"Profile change";
	cout<<" and its height is: "<<firstEvent.getHeight(); 
	cout<<" and its point is: "<<firstEvent.getPoint().x<<" "<<firstEvent.getPoint().y<<" "<<firstEvent.getPoint().z<<endl;

	q.pop();
	events.push_back(firstEvent);

	//Get all anchor events less than the first event height
	bool hasAnchor = false;
	Event firstAnchorEvent;
	if (anchorQ.size() > 0) {
		cout<<"Our anchor queue is now size: "<<anchorQ.size()<<endl;
		firstAnchorEvent = anchorQ.top();
		anchorQ.pop(); 
		anchorEvents.push_back(firstAnchorEvent); 
		if (firstAnchorEvent.getHeight() <= firstEvent.getHeight()) hasAnchor = true; 
		while ((firstAnchorEvent.getHeight() <= firstEvent.getHeight()) && !anchorQ.empty())
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
	updateNewPlanEdges(tempActivePlan); 
	//Process the intraclusters & the interclusters
	thePlan = ActivePlan(processClusters(tempActivePlan));

	//Clean out the queue
	while (!q.empty()){
		q.pop();
	}
	cout<<"AT THE END OF PROCESS Q"<<endl;
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
	sameHeightCount = 0; 

	addAnchorsToFloorPlan();
	activePlanStack.push(thePlan);
	activePlanQueue.push(thePlan);
	float height = thePlan.getActivePlan().at(0).getPt().y;;
	updateIntersectionVectors(height);
	fillQueueWithIntersections(height);
	fillQueueWithEdgeDirectionChanges(height);
	fillQueueWithAnchors(height);

	cout<<"AFTER 1st run"<<endl;
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

			std::deque<Corner> p = thePlan.getActivePlan(); 
			//for (int i = 0; i < thePlan.getActivePlan().size(); i++) {
			//	cout<<"The corner is at: "<<p.at(i).getLeftEdge().getEndPoint().x<<" "<<p.at(i).getLeftEdge().getEndPoint().y<<" "<<p.at(i).getLeftEdge().getEndPoint().z<<endl;
			//}
		} else {
			break;
		}
		sameHeightCount++;
		if (sameHeightCount > 100) {cout<<"FORCING KILL"<<endl; break;}
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

glm::vec3 SweepPlane::rotateVector(glm::vec3 testVec)
{
	glm::mat4 tempMat = glm::mat4(1.0);
	glm::mat4 firstRot = glm::rotate(tempMat,-90.0f,glm::vec3(0,1,0));

	glm::vec4 rotVec = glm::vec4(testVec,1.0);
	rotVec = firstRot * rotVec;

	glm::vec3 newVector = glm::vec3(rotVec);
	newVector = glm::normalize(newVector);

	//cout<<"The new rotated vector is "<<newVector.x<<" "<<newVector.y<<" "<<newVector.z<<endl;

	return newVector;
}

glm::vec3 SweepPlane::generateIntersection(glm::vec3 planEdge1, glm::vec3 planEdge2, glm::vec3 profileEdge1, glm::vec3 profileEdge2)
{
	//Get the perpendicular edge of each edge on the corner
	glm::vec3 edgeRot1 = rotateVector(planEdge1);
	glm::vec3 edgeRot2 = rotateVector(planEdge2);

	//Now apply both rotations to profiles
	glm::vec3 newProf1(edgeRot1.x * profileEdge1.x, profileEdge1.y, edgeRot1.z*profileEdge1.x);
	glm::vec3 newProf2(edgeRot2.x * profileEdge2.x, profileEdge2.y, edgeRot2.z*profileEdge2.x);

	//Get the normal of both
	glm::vec3 normal1 = glm::cross(planEdge1,newProf1);
	glm::vec3 normal2 = glm::cross(planEdge2,newProf2);

	glm::vec3 final1 = glm::cross(normal1,normal2);
	glm::vec3 final2 = glm::cross(normal2,normal1);

	if (final1.y > 0){
		return glm::normalize(final1);
	}
	else {
		return glm::normalize(final2);
	}
}
