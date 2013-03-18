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
#include <set>
#include "CompareHeight.h"
#include "CompareParent.h"

/* This class is reponsible for holding all the key data for the mesh creation and 
eventually outputting a workable mesh object*/
class SweepPlane
{
private:
	FloorPlan plan;
	std::vector<Profile> profileList;
	std::stack<ActivePlan> activePlanStack;
	std::queue<ActivePlan> activePlanQueue;

	ActivePlan thePlan;
	std::priority_queue<Event,std::vector<Event>,CompareHeight> q;


	
public:
	//Constructor
	SweepPlane(void);
	SweepPlane(FloorPlan p, std::vector<Profile> pList);

	//Aspects of the algorithm - right now these will just be stubs
	MObject createMesh(MObject& outData, MStatus& stat);


	

	void fillQueueWithIntersections(float height);
	bool intersectionTest(glm::vec3 line1S, glm::vec3 line1E, glm::vec3 line2S, glm::vec3 line2E, glm::vec3 intersection);

	void updateApStack(ActivePlan); 
	void processQueue();
	void buildIt();

	//Logging and output info
	void validateData();

	~SweepPlane(void);
};

