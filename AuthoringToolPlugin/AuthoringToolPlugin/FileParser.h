#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "Edge.h"
#include "ProfileEdge.h"
#include "PlanEdge.h"
#include "FloorPlan.h"
#include "Profile.h"
#include "Anchor.h"

//This class exists to parse a text file 
class FileParser
{
private:
	std::string fileName;
	FloorPlan plan;
	std::vector<Profile>profiles;
	std::vector<Anchor> anchors; 

public:
	//Constructors
	FileParser(void);
	FileParser(std::string f);

	//Getters
	FloorPlan getFloorPlan();
	std::vector<Profile> getProfiles();
	std::vector<Anchor> getAnchors(); 

	//Key Methods
	void parseFile();
	bool testOrder(std::vector<PlanEdge> currentPlan);

	//Destructors
	~FileParser(void);
};

