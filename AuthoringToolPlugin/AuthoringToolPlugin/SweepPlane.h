#pragma once
#include "Edge.h"
#include "FloorPlan.h"
#include "Profile.h"
#include "ActivePlan.h"
#include "ProfileEdge.h"
#include "PlanEdge.h"
#include "Event.h"
#include <maya/MGlobal.h>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include "CompareHeight.h"
#include "CompareParent.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Anchor.h"

/* This class is reponsible for holding all the key data for the mesh creation and 
eventually outputting a workable mesh object*/
class SweepPlane
{
private:
	FloorPlan plan;
	std::vector<Profile> profileList;
	std::queue<Anchor> anchorList; 
	std::stack<ActivePlan> activePlanStack;
	std::queue<ActivePlan> activePlanQueue;

	ActivePlan thePlan;
	std::priority_queue<Event,std::vector<Event>,CompareHeight> q;


	
public:
	//Constructor
	SweepPlane(void);
	SweepPlane(FloorPlan p, std::vector<Profile> pList, std::vector<Anchor> aList);

	//Aspects of the algorithm - right now these will just be stubs
	MObject createMesh(MObject& outData, MStatus& stat);

	//Anchor processing
	std::queue<Anchor> getAnchors(); 

	//Preprocessing helpers (before event generation)
	void updateIntersectionVectors(float height);
	std::vector<ProfileEdge> getProfileEdgesAtHeight(float height); 
	
	//Event generation methods
	void fillQueueWithIntersections(float height);
	void fillQueueWithEdgeDirectionChanges(float height);
	bool intersectionTest(glm::vec3 line1S, glm::vec3 line1E, glm::vec3 line2S, glm::vec3 line2E, glm::vec3 &intersection);

	//Queue processing helper functions
	std::priority_queue<Corner,std::vector<Corner>, CompareParent> preprocessNewPlanQ(std::vector<Event> events);
	void updateNewPlanEdges(std::vector<Corner> &tempActivePlan);
	std::vector<Corner> processClusters(std::vector<Corner> &tempActivePlan); 
	//std::vector<Corner> processInterClusters(std::vector<std::vector<Corner>> clusters, std::vector<Corner> &tempActivePlan);

	//Main control loops
	void processQueue();
	void buildIt();

	//Logging and output info
	void validateData();

	~SweepPlane(void);
};

