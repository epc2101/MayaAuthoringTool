// AuthoringToolAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileParser.h"
#include "SweepPlane.h"
#include <iostream>

using namespace std;


int main(int argc, char** argv)
{
	cout<<"Name of file: "<<argv[1]<<endl<<endl;;
	FileParser *parser = new FileParser(argv[1]);
	parser->parseFile();
	SweepPlane *sweep = new SweepPlane(parser->getFloorPlan(),parser->getProfiles());
	sweep->validateData();
	system("pause");

	return 0;
}

