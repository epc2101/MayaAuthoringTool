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
	if (a.getSource().size() > 0)
		indexA = a.getSource().at(a.getSource().size()-1).getIndex();
	else 
		indexA = 0; 
	if (b.getSource().size() > 0) 
		indexB = b.getSource().at(b.getSource().size()-1).getIndex();
	else 
		indexB = 0; 

	if (indexA > indexB){
		return true;
	}
	

	return false;
}