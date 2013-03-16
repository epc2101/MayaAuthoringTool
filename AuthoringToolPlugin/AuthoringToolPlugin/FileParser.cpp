#include "FileParser.h"

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

	//Variables for Creating Floor Plan
	std::vector<Edge> FloorPlanEdges;
	std::vector<int> FloorPlanProfile;

	cout<<"Name "<<fileName<<endl;

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
				Edge tempEdge(glm::vec3(startPoint.x,0,startPoint.y), glm::vec3(endPoint.x, 0, endPoint.y), anchor);
				FloorPlanEdges.push_back(tempEdge);
				FloorPlanProfile.push_back(profile);

				cout<<"lineHeader: "<<lineHeader<<endl;
				//Check the beginning of next line to see if should end;
				configFile >> lineHeader;
			}

			plan = FloorPlan(FloorPlanEdges, FloorPlanEdges.size(), FloorPlanProfile);
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
				std::vector<Edge> profileEdgeList;
				
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
						Edge tempEdge(glm::vec3(startPoint.x,0,startPoint.y), glm::vec3(endPoint.x, 0, endPoint.y), anchor);

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
	}
	configFile.close();
}

FloorPlan FileParser::getFloorPlan(){
	return plan;
}

std::vector<Profile> FileParser::getProfiles(){
	return profiles;
}


FileParser::~FileParser(void)
{
}
