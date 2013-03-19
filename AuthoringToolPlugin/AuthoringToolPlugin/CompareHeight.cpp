#include "CompareHeight.h"


CompareHeight::CompareHeight(void)
{
}


CompareHeight::~CompareHeight(void)
{
}

bool CompareHeight::operator()(Event& a, Event&b)
{
	if (a.getHeight() > b.getHeight()){
		return true;
	}
	return false;
}