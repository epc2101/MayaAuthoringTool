#pragma once
#include "Event.h"
class CompareHeight
{
public:
	CompareHeight(void);
	~CompareHeight(void);
	bool operator()(Event& a, Event&b);
};
