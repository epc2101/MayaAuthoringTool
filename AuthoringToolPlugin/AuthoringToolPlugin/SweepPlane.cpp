#include "SweepPlane.h"
#include <iostream>
#include "maya/MFnMesh.h"
#include "maya/MFloatPointArray.h"
#include "maya/MPointArray.h"

using namespace std;

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

	for(int i = 0; i<plan.getNumPoints(); i++){
		glm::vec3 point = plan.getEdgeList().at(i).getStartPoint();
		MPoint tempPoint = MPoint(point.x, point.y,point.z);
		pointArray.append(tempPoint);
		faceConnects.append(i);
	}

	numVertices = pointArray.length();
	faceCounts.append(faceConnects.length());
	
			
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
			if (height >= edge.getStartPoint().y && height < edge.getEndPoint().y){
				currentProfileFromHeight.push_back(edge);
				continue;
			}	
		}
	}


	//Iterate through the whole active plan and create intersection possibilities for adjacent corners and (non adjacent)
	for(int i = 0; i<thePlan.getActivePlan().size(); i++){
		//At cach corner we need the current vector associated
		Corner corner = thePlan.getActivePlan().at(i);
		Edge firstEdge = corner.getNextEdge();
		Edge secondEdge = corner.getPreviousEdge();
		//Edge firstEdgeProfileSegment = firstEdge.
		

	}

}


void SweepPlane::updateApStack(ActivePlan activePlan)
{
	activePlanStack.push(activePlan);
}

void SweepPlane::edgeEvent()
{
	return;
}

void SweepPlane::splitEvent()
{
	return;
}

SweepPlane::~SweepPlane(void)
{
}
