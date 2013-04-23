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

	cout<<"Testing Getting Edges From Heights"<<endl;
	testSingleProfileEdge();
	cout<<endl;

	cout<<"Testing Intersection Vectors"<<endl;
	testVectorsSquareOneLevel();
	cout<<endl;

	cout<<"\n**************Ending run of tests*********************"<<endl<<endl;

}

void TestingFramework::testParseCC()
{
	//FileParser parser = FileParser("C:\\Users\\Greg\\Documents\\GitHub\\MayaAuthoringTool\\testCC.txt");
	FileParser parser = FileParser("C:\\Users\\Beth\\MayaAuthoringTool\\MayaAuthoringTool\\testCC.txt");
	parser.parseFile();
	cout<<"From Clockwise File: ";
	if (parser.testOrder(parser.getFloorPlan().getEdgeList())){
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
	if (parser.testOrder(parser.getFloorPlan().getEdgeList())){
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
	FileParser parser = FileParser("C:\\Users\\Greg\\Documents\\GitHub\\MayaAuthoringTool\\testSingleLevelVectors.txt");
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


//Test anchor rotations
void TestingFramework::testAnchorRotate()
{


}