#include "TestingFramework.h"
using namespace std;
float EPSILONTEST = 0.001;

TestingFramework::TestingFramework(void)
{
}


TestingFramework::~TestingFramework(void)
{
}


bool TestingFramework::assertEquals(float a, float b)
{
	if (abs(a-b)<EPSILONTEST){
		return true;
	}
	else {
		return false;
	}
	
}
bool TestingFramework::assertEquals(int a, int b)
{
	if (a == b){
		return true;
	}
	else {
		return false;
	}
}
bool TestingFramework::assertVectorEquals(glm::vec3 a, glm::vec3 b)
{
	cout<<"The first vector is: "<<a.x<<" "<<a.y<<" "<<a.z<<endl;
	cout<<"The second vector is: "<<b.x<<" "<<b.y<<" "<<b.z<<endl;

	if (abs(a.x-b.x)<EPSILONTEST && abs(a.y-b.y)<EPSILONTEST && abs(a.z-b.z)<EPSILONTEST){
		return true;
	}
	else {
		return false;
	}
}

void TestingFramework::runTests(){
	cout<<"***************Begin run of tests*********************"<<endl<<endl;
	cout<<"Testing Parse Directions"<<endl;
	cout<<"Testing Clockwise Direction: "<<endl;
	testParseCC();
	cout<<endl;
	cout<<"Testing CounterClockwise Direction: "<<endl;
	testParseCCW();
	cout<<endl;

	//TODO - this test breaks for me...not sure why...
	//cout<<"Testing Getting Edges From Heights"<<endl;
	//testSingleProfileEdge();
	//cout<<endl;


	//testVectorRotation();
	//cout<<"Testing arbitrary corner intersection vectors"<<endl;
	//testArbitraryCornerVector();

	//cout<<"Testing Intersection Vectors"<<endl;
	//testVectorsSquareOneLevel();

	//cout<<"Testing Intersection Vectors"<<endl;
	//testVectorsSquareOneLevel();
	//cout<<endl;

	cout<<"Testing intersections test for two vectors"<<endl;
	testIntesections();
	cout<<endl;

	//cout<<"Testing anchor rotations"<<endl;
	//testAnchorRotateSimple();
	//cout<<endl;
	//testAnchorRotateComplex();

	cout<<endl;

	cout<<"\n**************Ending run of tests*********************"<<endl<<endl;

}

void TestingFramework::testParseCC()
{
	//FileParser parser = FileParser("C:\\Users\\Greg\\Documents\\GitHub\\MayaAuthoringTool\\testCC.txt");
	FileParser parser = FileParser("C:\\Users\\Beth\\MayaAuthoringTool\\MayaAuthoringTool\\testCC.txt");
	parser.parseFile();
	cout<<"From Clockwise File: ";
	if (!parser.testOrder(parser.getFloorPlan().getEdgeList())){
		cout<<"PASSED"<<endl;
	}
	else{
		cout<<"FAILED"<<endl;
	}
	cout<<"From Edge List At Origin: ";

	std::vector<PlanEdge> CCPlan;
	PlanEdge edge1 = PlanEdge(glm::vec3(0,0,0), glm::vec3(0,0,4), 1);
	PlanEdge edge2 = PlanEdge(glm::vec3(0,0,4), glm::vec3(2,0,6), 1);
	PlanEdge edge3 = PlanEdge(glm::vec3(2,0,6), glm::vec3(4,0,4), 1);
	PlanEdge edge4 = PlanEdge(glm::vec3(4,0,4), glm::vec3(4,0,0), 1);
	PlanEdge edge5 = PlanEdge(glm::vec3(4,0,0), glm::vec3(0,0,0), 1);
	CCPlan.push_back(edge1);
	CCPlan.push_back(edge2);
	CCPlan.push_back(edge3);
	CCPlan.push_back(edge4);
	CCPlan.push_back(edge5);

	if (parser.testOrder(CCPlan)){
		cout<<"PASSED"<<endl;
	}
	else{
		cout<<"FAILED"<<endl;
	}

	cout<<"From Edge List At Translated Over 2 to Straddle Origin: ";

	std::vector<PlanEdge> CCPlan2;
	edge1 = PlanEdge(glm::vec3(-2,0,0), glm::vec3(-2,0,4), 1);
	edge2 = PlanEdge(glm::vec3(-2,0,4), glm::vec3(0,0,6), 1);
	edge3 = PlanEdge(glm::vec3(0,0,6), glm::vec3(2,0,4), 1);
	edge4 = PlanEdge(glm::vec3(2,0,4), glm::vec3(2,0,0), 1);
	edge5 = PlanEdge(glm::vec3(2,0,0), glm::vec3(-2,0,0), 1);
	CCPlan2.push_back(edge1);
	CCPlan2.push_back(edge2);
	CCPlan2.push_back(edge3);
	CCPlan2.push_back(edge4);
	CCPlan2.push_back(edge5);

	if (parser.testOrder(CCPlan)){
		cout<<"PASSED"<<endl;
	}
	else{
		cout<<"FAILED"<<endl;
	}



}
void TestingFramework::testParseCCW()
{
	//FileParser parser = FileParser("C:\\Users\\Greg\\Documents\\GitHub\\MayaAuthoringTool\\testCCW.txt");
	FileParser parser = FileParser("C:\\Users\\Beth\\MayaAuthoringTool\\MayaAuthoringTool\\testCCW.txt");
	parser.parseFile();

	//Testing whether CCW is converted to clockwise
	cout<<"Testing CCW File That Should be Converted to CW: ";
	if (!parser.testOrder(parser.getFloorPlan().getEdgeList())){
		cout<<"PASSED"<<endl;
	}
	else {
		cout<<"FAILED"<<endl;
	}

	//Test General Edges for Counter Clockwiseness
	cout<<"Testing Group of Edges for CCW\'ness: ";
	std::vector<PlanEdge> CCPlan;
	PlanEdge edge1 = PlanEdge(glm::vec3(0,0,0), glm::vec3(1,0,0), 1);
	PlanEdge edge2 = PlanEdge(glm::vec3(1,0,0), glm::vec3(1,0,1), 1);
	PlanEdge edge3 = PlanEdge(glm::vec3(1,0,1), glm::vec3(0,0,1), 1);
	PlanEdge edge4 = PlanEdge(glm::vec3(0,0,1), glm::vec3(0,0,0), 1);
	
	CCPlan.push_back(edge1);
	CCPlan.push_back(edge2);
	CCPlan.push_back(edge3);
	CCPlan.push_back(edge4);
	

	if (parser.testOrder(CCPlan) == false){
		cout<<"PASSED"<<endl;
	}
	else{
		cout<<"FAILED"<<endl;
	}
}

void TestingFramework::testVectorsSquareOneLevel()
{
	//FileParser parser = FileParser("C:\\Users\\Greg\\Documents\\GitHub\\MayaAuthoringTool\\testSingleLevelVectors.txt");
	FileParser parser = FileParser("C:\\Users\\Beth\\MayaAuthoringTool\\testSingleLevelVectors.txt");
	parser.parseFile();
	SweepPlane plane = SweepPlane(parser.getFloorPlan(), parser.getProfiles(), parser.getAnchors());
	plane.setThePlan(parser.getFloorPlan());
	cout<<endl;
	cout<<plane.getThePlan().getEdgeList().size()<<" is the number of edges in the active plan"<<endl;
	plane.updateIntersectionVectors(0);
	//std::vector<glm::vec3> intVectors = plane.getThePlan().getIntersectionVectors();
	glm::vec3 testVec(0,2,0);
	testVec = glm::normalize(testVec);
	cout<<"Size of intVec "<<plane.getThePlan().getIntersectionVectors().size()<<endl;
	cout<<"Testing Single First Vector (VERTICAL): ";
	if (assertVectorEquals(plane.getThePlan().getIntersectionVectors().at(0),testVec)){
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
	}

	cout<<"Testing Second Level Vectors (ANGLED): ";
	plane.updateIntersectionVectors(2.0);
	
}

//Test rotating vectors
void TestingFramework::testVectorRotation()
{
	cout<<endl<<"Testing vector rotations"<<endl<<endl;
	SweepPlane plane = SweepPlane();
	glm::vec3 testVec1(1,0,0);
	glm::vec3 correctVec1(0,0,1);
	glm::vec3 newVec1 = plane.rotateVector(testVec1);
	
	cout<<"Testing CCW Rotation of 2D Vector 1,0: "<<endl;
	if (assertVectorEquals(newVec1,correctVec1)){
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
	}

	cout<<"Testing CCW Rotation of 2D Vector 1,1: "<<endl;

	glm::vec3 testVec2(1,0,1);
	glm::vec3 correctVec2(-.707,0,.707);
	glm::vec3 newVec2 = plane.rotateVector(testVec2);

	if (assertVectorEquals(newVec2,correctVec2)){
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
	}

	cout<<endl;

}

void TestingFramework::testArbitraryCornerVector()
{
	//Start with a simpler case - the 1x1 square with 2 sided vector
	SweepPlane plane;
	glm::vec3 edge1(1,0,0), edge2(0,0,-1), prof1(1,5,0), prof2(2,3,0);

	cout<<endl<<"Testing 1 corner of  1x1 square with same profile: ";
	glm::vec3 testVec1 = plane.generateIntersection(edge1, edge2, prof1, prof1);
	glm::vec3 compareVec1(0.19245,0.96225,0.19245);
	if (assertVectorEquals(testVec1,compareVec1)){
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
	}
	cout<<endl;

	cout<<"Testing 1 corner of 1x1 square with 2 different profiles: ";
	glm::vec3 testVec2 = plane.generateIntersection(edge1, edge2, prof1, prof2);
	glm::vec3 compareVec2(0.547126,0.820763,0.164153);
	if (assertVectorEquals(testVec2,compareVec2)){
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
	}
	cout<<endl;
}


//Test getting profile edges
void TestingFramework::testSingleProfileEdge()
{

	

	//FileParser parser = FileParser("C:\\Users\\Greg\\Documents\\GitHub\\MayaAuthoringTool\\testGetSingleProfile.txt");
	FileParser parser = FileParser("C:\\Users\\Beth\\MayaAuthoringTool\\MayaAuthoringTool\\testGetSingleProfile.txt");
	parser.parseFile();
	SweepPlane plane = SweepPlane(parser.getFloorPlan(), parser.getProfiles(), parser.getAnchors());

	std::vector<ProfileEdge> profileTest;
	profileTest = plane.getProfileEdgesAtHeight(0);
	
	cout<<"Testing SingleProfile First Segment: ";
	if (assertVectorEquals(profileTest.at(0).getStartPoint(),glm::vec3(0,0,0)) && assertVectorEquals(profileTest.at(0).getEndPoint(),glm::vec3(0,2,0))){
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
	}

	cout<<"Testing SingleProfile Second Segment at Start: ";
	profileTest.pop_back();
	profileTest = plane.getProfileEdgesAtHeight(3.0);
	cout<<"Size of vector is "<<profileTest.size()<<endl;
	if (assertVectorEquals(profileTest.at(0).getStartPoint(),glm::vec3(0,2,0)) && assertVectorEquals(profileTest.at(0).getEndPoint(),glm::vec3(1,5,0))){
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
	}
}

//Testing intersection points found

void TestingFramework::testIntesections()
{
	cout<<"1.  Testing no intersection"<<endl;
	SweepPlane plane;
	glm::vec3 start1(0, 0, 0); 
	glm::vec3 end1(0, 1, 0); 
	glm::vec3 start2(0, 0, 1); 
	glm::vec3 end2(0, 1, 1); 
	glm::vec3 intersect; 
	if (plane.intersectionTest(start1, end1, start2, end2, intersect) == false) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected no intersection, but found intersection "<<intersect.x<<" "<<intersect.y<<" "<<intersect.z<<endl;
	}

	cout<<"2. Testing intersection"<<endl;
	start1 = glm::vec3(1, 0, 0); 
	start2 = glm::vec3(0, 5, 5); 
	end1 = glm::vec3(5, 6, 2); 
	end2 = glm::vec3(5, 6, 2); 
	if (plane.intersectionTest(start1, end1, start2, end2, intersect) == false) {
		cout<<"FAILED"<<endl;
		cout<<"Expected an intersection, but found none!"<<endl;
	} else {
		if (intersect.x == 5 && intersect.y == 6 && intersect.z == 2) {
			cout<<"PASSED!!"<<endl; 
		} else {
			cout<<"FAILED!!"<<endl;
			cout<<"Found intersection at: "<<intersect.x<<" "<<intersect.y<<" "<<intersect.z<<endl;
		}
	}

	cout<<"3. Testing intersection"<<endl;
	start1 = glm::vec3(6, 8, 4); 
	start2 = glm::vec3(6, 8, 2); 
	end1 = glm::vec3(12, 15, 4); 
	end2 = glm::vec3(12, 15, 6); 
	if (plane.intersectionTest(start1, end1, start2, end2, intersect) == false) {
		cout<<"FAILED"<<endl;
		cout<<"Expected an intersection, but found none!"<<endl;
	} else {
		if (intersect.x == 9 && intersect.y == 23.0/2.0 && intersect.z == 4) {
			cout<<"PASSED!!"<<endl; 
		} else {
			cout<<"FAILED!!"<<endl;
			cout<<"Found intersection at: "<<intersect.x<<" "<<intersect.y<<" "<<intersect.z<<endl;
		}
	}

	cout<<"4. Testing no intersection"<<endl;
	start1 = glm::vec3(0, 0, 0); 
	start2 = glm::vec3(1, 0, 0); 
	end1 = glm::vec3(-2, 2, 0); 
	end2 = glm::vec3(3, 2, 0); 
	if (plane.intersectionTest(start1, end1, start2, end2, intersect) == false) {
		cout<<"PASSED"<<endl;
	} else {
			cout<<"FAILED!!"<<endl;
			cout<<"Did it find neg intersection? Found intersection at: "<<intersect.x<<" "<<intersect.y<<" "<<intersect.z<<endl;
	}	

	cout<<"5. Testing expected intersection. Testing real example that fails"<<endl;
	start1 = glm::vec3(-1.96793, 0, -7.434402 ); 
	start2 = glm::vec3(3.104956, 0, -7.87172); 
	end1 = glm::vec3(2.836363, 5.779027, -1.280395); 
	end2 = glm::vec3(-1.354797, 5.779027, -0.91909); 
	if (plane.intersectionTest(start1, end1, start2, end2, intersect) == false) {
		cout<<"FAILED"<<endl;
	} else {
			cout<<"PASSED!!"<<endl;
			cout<<"Did it find neg intersection? Found intersection at: "<<intersect.x<<" "<<intersect.y<<" "<<intersect.z<<endl;
	}	
}

//Test anchor rotations
void TestingFramework::testAnchorRotateSimple()
{
	cout<<"#1:  4 anchors on cube.  CounterClockwise."<<endl;
	//FileParser parser = FileParser("C:\\Users\\Greg\\Documents\\GitHub\\MayaAuthoringTool\\testAnchorsSquareRotations.txt");
	FileParser parser = FileParser("C:\\Users\\Beth\\MayaAuthoringTool\\testAnchorsSquareRotations.txt");

	parser.parseFile();
	SweepPlane plane = SweepPlane(parser.getFloorPlan(), parser.getProfiles(), parser.getAnchors());
	plane.setThePlan(parser.getFloorPlan());
	plane.calcAnchorTransforms(parser.getAnchors().at(0)); 
	//*********************************
	//EDGE 1
	cout<<"\t----Testing edge index 0---- "<<endl;
	Anchor a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors();  
	if (a.getFloorPlanIndex() == 0) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 0, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == -90) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected -90, but got "<<a.getRotY()<<endl;
	}
	//*********************************
	//EDGE 2
	plane.calcAnchorTransforms(parser.getAnchors().at(1)); 
	cout<<"\t----Testing edge index 1---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 1) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 1, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == 180) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 180, but got "<<a.getRotY()<<endl;
	}

	//*********************************
	//EDGE 3
	plane.calcAnchorTransforms(parser.getAnchors().at(2)); 
	cout<<"\t----Testing edge index 2---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 2) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 2, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == 90) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 90, but got "<<a.getRotY()<<endl;
	}

	//*********************************
	//EDGE 4
	plane.calcAnchorTransforms(parser.getAnchors().at(3)); 
	cout<<"\t----Testing edge index 3---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors();  
	if (a.getFloorPlanIndex() == 3) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 3, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == 0) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 0, but got "<<a.getRotY()<<endl;
	}

	//***************************************************************************************
	cout<<"#2:  4 anchors on cube.  Clockwise."<<endl;
	//FileParser parser = FileParser("C:\\Users\\Greg\\Documents\\GitHub\\MayaAuthoringTool\\testAnchorsSquareRotations.txt");
	parser = FileParser("C:\\Users\\Beth\\MayaAuthoringTool\\testAnchorsSquareRotationsClockwise.txt");
	parser.parseFile();
	plane = SweepPlane(parser.getFloorPlan(), parser.getProfiles(), parser.getAnchors());
	plane.setThePlan(parser.getFloorPlan());
	plane.calcAnchorTransforms(parser.getAnchors().at(0)); 

	//*********************************
	//EDGE 1
	cout<<"\t----Testing edge index 0---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors();  
	if (a.getFloorPlanIndex() == 0) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 0, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == 0) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 0, but got "<<a.getRotY()<<endl;
	}
	//*********************************
	//EDGE 2
	plane.calcAnchorTransforms(parser.getAnchors().at(1)); 
	cout<<"\t----Testing edge index 1---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 3) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 3, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == 90) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 90, but got "<<a.getRotY()<<endl;
	}

	//*********************************
	//EDGE 3
	plane.calcAnchorTransforms(parser.getAnchors().at(2)); 
	cout<<"\t----Testing edge index 2---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 2) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 2, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == 180) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 180, but got "<<a.getRotY()<<endl;
	}

	//*********************************
	//EDGE 4
	plane.calcAnchorTransforms(parser.getAnchors().at(3)); 
	cout<<"\t----Testing edge index 3---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors();  
	if (a.getFloorPlanIndex() == 1) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected 1, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == -90) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected -90, but got "<<a.getRotY()<<endl;
	}
}

void TestingFramework::testAnchorRotateComplex() 
{
	cout<<"#3:  Testing the hut with anchor on each edge. Clockwise"<<endl;
	//FileParser parser = FileParser("C:\\Users\\Greg\\Documents\\GitHub\\MayaAuthoringTool\\testAnchorsComplex.txt");
	FileParser parser = FileParser("C:\\Users\\Beth\\MayaAuthoringTool\\testAnchorsComplex.txt");
	parser.parseFile();
	SweepPlane plane = SweepPlane(parser.getFloorPlan(), parser.getProfiles(), parser.getAnchors());
	plane.setThePlan(parser.getFloorPlan());

	//*********************************	
	plane.calcAnchorTransforms(parser.getAnchors().at(0)); 
	cout<<"\t----Testing index 0---- "<<endl;
	Anchor a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 0) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected index 0, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == -45) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected angle -45, but got "<<a.getRotY()<<endl;
	}
		//*********************************	
	plane.calcAnchorTransforms(parser.getAnchors().at(1)); 
	cout<<"\t----Testing index 1---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 5) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected index 5, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == 0) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected angle 0, but got "<<a.getRotY()<<endl;
	}
		//*********************************	
	plane.calcAnchorTransforms(parser.getAnchors().at(2)); 
	cout<<"\t----Testing index 2---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 4) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected index 4, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() >= 63.4349 && a.getRotY() < 63.4351) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected angle 63.435, but got "<<a.getRotY()<<endl;
	}
		//*********************************	
	plane.calcAnchorTransforms(parser.getAnchors().at(3)); 
	cout<<"\t----Testing index 3---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 3) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected index 3, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() < 116.5651 && a.getRotY() > 116.5649) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected angle 116.565, but got "<<a.getRotY()<<endl;
	}
		//*********************************	
	plane.calcAnchorTransforms(parser.getAnchors().at(4)); 
	cout<<"\t----Testing index 4---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 2) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected index 2, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == 180) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected angle 180, but got "<<a.getRotY()<<endl;
	}
		//*********************************	
	plane.calcAnchorTransforms(parser.getAnchors().at(5)); 
	cout<<"\t----Testing index 5---- "<<endl;
	a = plane.getOutputAnchors().front(); 
	plane.popOutputAnchors(); 
	if (a.getFloorPlanIndex() == 1) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected index 1, but got "<<a.getFloorPlanIndex()<<endl;
	}
	cout<<"\tTesting rotation"<<endl;
	if (a.getRotY() == -135) {
		cout<<"PASSED"<<endl;
	} else {
		cout<<"FAILED"<<endl;
		cout<<"Expected angle -135, but got "<<a.getRotY()<<endl;
	}
}
