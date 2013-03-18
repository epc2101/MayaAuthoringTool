#include "CompareParent.h"


CompareParent::CompareParent(void)
{
}


CompareParent::~CompareParent(void)
{
}

bool CompareParent::operator()(Corner &a, Corner &b)
{
	int indexA, indexB;
	indexA = a.getSource().at(a.getSource().size()-1).getIndex();
	indexB = b.getSource().at(b.getSource().size()-1).getIndex();
	if (indexA < indexB){
		return true;
	}

	return false;
}