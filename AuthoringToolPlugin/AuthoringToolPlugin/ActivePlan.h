#pragma once
#include "Corner.h"
#include "Edge.h"
#include "FloorPlan.h"
#include <deque>

class ActivePlan
{
	std::deque<Corner> activePlan;

public:
	std::deque<Corner> getActivePlan();
	void setIndexNums(); 
	ActivePlan(FloorPlan thePlan);
	ActivePlan(std::deque<Corner> newPlan); 
	ActivePlan(void);
	~ActivePlan(void);
};

