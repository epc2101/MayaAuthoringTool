#pragma once
#include "Corner.h"
class CompareParent
{
public:
	CompareParent(void);
	bool operator()(Corner &a, Corner &b);
	~CompareParent(void);
};

