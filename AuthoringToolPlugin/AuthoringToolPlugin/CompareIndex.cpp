#include "CompareIndex.h"


CompareIndex::CompareIndex(void)
{
}


CompareIndex::~CompareIndex(void)
{
}

bool CompareIndex::operator()(Anchor& a, Anchor&b)
{
	if (a.getIndex() > b.getIndex()){
		return true;
	}
	return false;
}