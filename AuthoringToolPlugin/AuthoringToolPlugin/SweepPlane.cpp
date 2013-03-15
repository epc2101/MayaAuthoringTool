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
