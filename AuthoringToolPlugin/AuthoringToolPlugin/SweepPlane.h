#pragma once
#include "Edge.h"
#include "FloorPlan.h"
#include "Profile.h"
#include "ActivePlan.h"
#include "ProfileEdge.h"
#include "PlanEdge.h"
#include "Event.h"
#include <maya/MGlobal.h>
#include <maya\MFnDoubleArrayData.h>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include "CompareHeight.h"
#include "CompareParent.h"
#include "CompareIndex.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Anchor.h"

/* This class is reponsible for holding all the key data for the mesh creation and 
eventually outputting a workable mesh object*/
class SweepPlane
{
private:
	FloorPlan plan;
	std::vector<Profile> profileList;
	std::vector<Anchor> anchorList; 
	std::stack<ActivePlan> activePlanStack;
	std::queue<ActivePlan> activePlanQueue;
	std::priority_queue<Anchor, std::vector<Anchor>, CompareIndex> outputAnchors;
	bool killTheSweep;
	float maxProfileHeight;
	glm::vec3 avgPlanCenter;

	ActivePlan thePlan;
	std::priority_queue<Event,std::vector<Event>,CompareHeight> q;
	std::priority_queue<Event, std::vector<Event>, CompareHeight> anchorQ; 
	float theLastHeight; 
	int theLastHeightCount; 

public:
	//Constructor
	SweepPlane(void);
	SweepPlane(FloorPlan p, std::vector<Profile> pList, std::vector<Anchor> aList);

	//Final output creators
	MObject createMesh(MObject& outData, MStatus& stat);
	void createAnchors(MObject& anchorPosData, MObject& anchorRotData, MStatus& stat);

	//Anchor processing
	void calcAnchorTransforms(Anchor &a); 
	void findAnchorEvents(std::vector<Event> e); 
	std::queue<Anchor> getAnchors(); 
	void addAnchorsToFloorPlan(); 

	//Preprocessing helpers (before event generation)
	void updateIntersectionVectors(float height);
	std::vector<ProfileEdge> getProfileEdgesAtHeight(float height); 
	void addEdgeIndex(); 
	
	//Event generation methods
	void fillQueueWithIntersections(float height);
	void fillQueueWithEdgeDirectionChanges(float height);
	void fillQueueWithAnchors(float height); 
	void fillQueueWithHorizontalChanges(float height);
	bool intersectionTest(glm::vec3 line1S, glm::vec3 line1E, glm::vec3 line2S, glm::vec3 line2E, glm::vec3 &intersection);
	bool shortestDistTest(glm::vec3  p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, float &mua, float &mub, glm::vec3 &pa, glm::vec3 &pb);


	

	//Queue processing helper functions
	std::priority_queue<Corner,std::vector<Corner>, CompareParent> preprocessNewPlanQ(std::vector<Event> events);
	void  updateNewPlanEdges(std::vector<Corner> &tempActivePlan);
	std::vector<Corner> processClusters(std::vector<Corner> &tempActivePlan); 
	//std::vector<Corner> processInterClusters(std::vector<std::vector<Corner>> clusters, std::vector<Corner> &tempActivePlan);
	


	//Main control loops
	void processQueue();
	void buildIt();

	//Logging and output info
	void validateData();

	//For testing
	ActivePlan getThePlan();
	void setThePlan(FloorPlan plan);

	glm::vec3 rotateVector(glm::vec3 testVec);
	glm::vec3 generateIntersection(glm::vec3 planEdge1, glm::vec3 planEdge2, glm::vec3 profileEdge1, glm::vec3 profileEdge2);

	std::priority_queue<Anchor, std::vector<Anchor>, CompareIndex> getOutputAnchors() {return outputAnchors;}
	void popOutputAnchors() { outputAnchors.pop(); }

	~SweepPlane(void);
};

