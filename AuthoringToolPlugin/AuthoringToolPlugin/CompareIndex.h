#pragma once
#include "Anchor.h"

class CompareIndex
{
public:
	CompareIndex(void);
	~CompareIndex(void);
	bool operator()(Anchor& a, Anchor&b);
};

