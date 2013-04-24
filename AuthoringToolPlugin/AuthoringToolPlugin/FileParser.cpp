#include "FileParser.h"
#include "maya/MGlobal.h"


using namespace std;

FileParser::FileParser(void)
{
}

FileParser::FileParser(string f){
	fileName = f;
}

//Reads in a text file containing necessary data for the algorithm and populate the field variables
void FileParser::parseFile(){

	//Variables for temporary storage during parse
	string lineHeader, secondaryLineHeader;
	glm::vec2 startPoint, endPoint;
	int profile, anchor;

	//MGlobal::executeCommand((MString)"print This was correctly reached",true, false);

	//Variables for Creating Floor Plan
	std::vector<PlanEdge> FloorPlanEdges;
	std::vector<int> FloorPlanProfile;

	cout<<"Name "<<fileName<<endl;
	bool orderIsCW = false; 
	ifstream configFile (fileName);
	if (configFile.is_open())
	{
		configFile >> lineHeader;
		
		//Setup the floor plan data
		if(lineHeader == "#FloorPlanStart"){
			configFile >> lineHeader;
			cout<<"In floorplan start..."<<endl;
			while(lineHeader != "#FloorPlanEnd"){
				
				//Parse first point, second point, profile and anchor
				configFile >> startPoint.x >> startPoint.y;
				configFile >> secondaryLineHeader >> endPoint.x >> endPoint. y;
				configFile >> secondaryLineHeader >> profile;
				configFile >> secondaryLineHeader >> anchor;

				//Construct the data structures for the floorplan
				PlanEdge tempEdge(glm::vec3(startPoint.x,0,startPoint.y), glm::vec3(endPoint.x, 0, endPoint.y), profile, anchor);
				FloorPlanEdges.push_back(tempEdge);
				FloorPlanProfile.push_back(profile);

				cout<<"The profile index at parsing is"<<FloorPlanProfile.back()<<endl;

				cout<<"lineHeader: "<<lineHeader<<endl;
				//Check the beginning of next line to see if should end;
				configFile >> lineHeader;
			}

			//In this section we determine if the floorplan has been arranged clockwise or counter.  Is counter we reverse the edges and floorPlan profileVector

			//Test the ordering of the plan edges - if CW we are good, otherweise we need to flip
			orderIsCW = testOrder(FloorPlanEdges);
			cout<<"IS counter clockwise: "<<orderIsCW<<endl;
			if (!orderIsCW){
				plan = FloorPlan(FloorPlanEdges, FloorPlanEdges.size(), FloorPlanProfile);
			}
			else {
				std::vector<PlanEdge> tempPlanEdge;
				std::vector<int> tempProfileList;

				while(!FloorPlanEdges.empty()){
					PlanEdge temp = FloorPlanEdges.back();
					FloorPlanEdges.pop_back();
					int tempNum = FloorPlanProfile.back();
					FloorPlanProfile.pop_back();

					glm::vec3 newStart, newEnd;
					newStart = temp.getEndPoint();
					newEnd = temp.getStartPoint();
					int anchor = temp.getAnchorType();
					int profile = temp.getProfileType();

					PlanEdge newEdge = PlanEdge(newStart,newEnd,profile,anchor);
					tempPlanEdge.push_back(newEdge);	
					tempProfileList.push_back(tempNum);
				}

				plan = FloorPlan(tempPlanEdge, tempPlanEdge.size(), tempProfileList);
			}
		}
		else {
			cout<<"File is incorrectly formatted"<<endl;
			return;
		}

		//Parse the profiles
		int numProfiles;
		configFile >> lineHeader;
		if (lineHeader == "#Profiles"){
			cout<<"In profiles"<<endl;
			configFile >> numProfiles;
			for (int i = 0; i<numProfiles; i++){
				//Set up comparison
				string comparisonString = "#ProfileStart";
				string numString = std::to_string((long long)i);
				comparisonString.append(numString);

				//Create temporary storagefor points
				std::vector<ProfileEdge> profileEdgeList;
				
				configFile >> lineHeader;
				if (lineHeader.compare(comparisonString)==0){
					
					
					//Set up comparison for while loop
					configFile >>lineHeader;
					string comparisonStringEnd = "#ProfileEnd";
					comparisonStringEnd.append(numString);
					
					//We can now parse the individual profiles
					while(lineHeader.compare(comparisonStringEnd)!=0){
						configFile >> startPoint.x >> startPoint.y;
						configFile >> secondaryLineHeader >> endPoint.x >> endPoint. y;
						configFile >> secondaryLineHeader >> anchor;

						//We will determine the topmost point and horizontalness at normalization
						bool isHorizontal = false, isTop = false;
						ProfileEdge tempEdge(glm::vec3(startPoint.x,startPoint.y,0), glm::vec3(endPoint.x, endPoint.y,0),isTop,isHorizontal, anchor);

						profileEdgeList.push_back(tempEdge);

						//Check beginning of next line to see if it should end
						configFile >> lineHeader;
					}
				}
				else {
					cout<<"Profile headers are incorrectly formatted"<<endl;
					return;
				}

				//Because the last point isn't share, we add an additional 1 to the total point size
				Profile tempProf = Profile(profileEdgeList, profileEdgeList.size()+1);

				profiles.push_back(tempProf);
			}
		}
		else {
			cout<<"Number of profiles formatted incorrectly"<<endl;
			return;
		}

		cout<<"GOT HERE??!!"<<endl;
		//******Beths code start...I'm on a boat....
		configFile >> lineHeader;
		int numAnchors = 0; 
		float floorPlanEdge, profileEdge; 
		int profile; 
		if (lineHeader == "#NumAnchors") {
			cout<<lineHeader<<"-----------------------"<<endl;
		    configFile >> numAnchors;
			cout<<"In anchors.  #: "<<numAnchors<<endl;
			for (int i = 0; i < numAnchors; i++) {
				configFile >> lineHeader; /* #AnchorStarti*/ cout<<" "<<lineHeader<<" ";
				configFile >> lineHeader; /*#FloorPlanEdge*/ cout<<" "<<lineHeader<<" ";
				configFile >> floorPlanEdge;  
				configFile >> lineHeader; //#Profile
				configFile >> profile; 
				configFile >> lineHeader; //ProfileEdge
				configFile >> profileEdge; 
				configFile >> lineHeader; //#AnchorEndi
				Anchor a = Anchor(i, floorPlanEdge, profile, profileEdge); 
				Profile p = profiles.at(i); 
				ProfileEdge e = p.getEdgeList().at((int) profileEdge);
				glm::vec3 dir = e.getEndPoint() - e.getStartPoint();
				float profilePercent = profileEdge - (int)profileEdge; 
				a.setHeight((e.getStartPoint() + dir * profilePercent).y); 
				//Add the anchor to the floorPlanEdge Anchor list
				a.setID(i); 
				anchors.push_back(a); 

				//TODO - THIS ISN"T WORKING
				//plan.getEdgeList().at((int)floorPlanEdge).addAnchor(a);

			}
			if(orderIsCW) {
				for (int i = 0; i < anchors.size(); i++)
				{
					cout<<"REVERSIZING ANCHOR PERCENTS!!"<<endl;
					anchors.at(i).setFloorPlanPercent(1.f - anchors.at(i).getFloorPlanPercent());
				}

			}
		}

	}
	configFile.close();
}

bool FileParser::testOrder(std::vector<PlanEdge> currentPlan)
{
	int testNum = 0;
	for(int i = 0; i<currentPlan.size(); i++){
		PlanEdge tempEdge = currentPlan.at(i);
		int xNum = tempEdge.getEndPoint().x - tempEdge.getStartPoint().x;
		int zNum = tempEdge.getEndPoint().z + tempEdge.getStartPoint().z;
		testNum += (xNum*zNum);
	}
	if(testNum < 0){
		cout<<"IS COUNTER CLOCKWISE"<<endl;
		return false;
	}
	else {
		if (testNum == 0) {
			//Try running around the edges and seeing whether the cross product is pos/neg
			cout<<"Uhoh! We got 0"<<endl;
			return false;
		}
		cout<<"IS CLOCKWISE"<<endl;
		return true;
	}


}

FloorPlan FileParser::getFloorPlan(){
	return plan;
}

std::vector<Profile> FileParser::getProfiles(){
	return profiles;
}

std::vector<Anchor> FileParser::getAnchors()
{
	cout<<"The size of the anchors in file parse is: "<<anchors.size()<<endl; 
	return anchors; 
}

FileParser::~FileParser(void)
{
}
