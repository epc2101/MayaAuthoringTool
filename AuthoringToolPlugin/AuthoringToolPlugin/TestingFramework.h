#pragma once
#include "SweepPlane.h"
#include "FileParser.h"

class TestingFramework
{
public:
	TestingFramework(void);
	~TestingFramework(void);
	void runTests();
	bool assertEquals(float a, float b);
	bool assertEquals(int a, int b);
	bool assertVectorEquals(glm::vec3 a, glm::vec3 b);

	//The various test headers go here
	void testParseCC();
	void testParseCCW();

	//Test the "intersection vectors generated at each corner
	void testVectorsSquareOneLevel();


	//Test getting profile edges
	void testSingleProfileEdge();


	//Test that a horizontal edge was grabbed from a profile at a certain height

	//Test that anchors are correctly rotates
	
};

