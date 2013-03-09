#pragma once
#include "Edge.h"
#include "FloorPlan.h"
#include "Profile.h"
//#include <maya/MGlobal.h>
#include <vector>

/* This class is reponsible for holding all the key data for the mesh creation and 
eventually outputting a workable mesh object*/
class SweepPlane
{
private:
	FloorPlan plan;
	std::vector<Profile> profileList;
	
public:
	//Constructor
	SweepPlane(void);
	SweepPlane(FloorPlan p, std::vector<Profile> pList);

	//Aspects of the algorithm - right now these will just be stubs
	void createMesh();
	void edgeEvent();
	void splitEvent();

	//Logging and output info
	void validateData();

	~SweepPlane(void);
};

