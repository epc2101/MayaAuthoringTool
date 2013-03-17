#pragma once
#include "Edge.h"
#include "FloorPlan.h"
#include "Profile.h"
#include "ActivePlan.h"
#include "Event.h"
#include <maya/MGlobal.h>
#include <vector>
#include <queue>
#include <stack>

/* This class is reponsible for holding all the key data for the mesh creation and 
eventually outputting a workable mesh object*/
class SweepPlane
{
private:
	FloorPlan plan;
	std::vector<Profile> profileList;
	std::stack<ActivePlan> activePlanStack;
	ActivePlan thePlan;
	std::priority_queue<Event> q;
	
public:
	//Constructor
	SweepPlane(void);
	SweepPlane(FloorPlan p, std::vector<Profile> pList);

	//Aspects of the algorithm - right now these will just be stubs
	MObject createMesh(MObject& outData, MStatus& stat);

	void updateApStack(ActivePlan); 

	void fillQueueWithIntersections(float height);

	void edgeEvent();
	void splitEvent();

	//Logging and output info
	void validateData();

	~SweepPlane(void);
};

