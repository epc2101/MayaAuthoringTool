//
// Copyright (C) Protrusion
// 
// File: AuthoringToolPluginNode.cpp
//
// Dependency Graph Node: AuthoringToolPlugin
//
// Author: Maya Plug-in Wizard 2.0.1
// Revised by Isaac Peral
//

#include "AuthoringToolPluginNode.h"
#include "SweepPlane.h"
#include "Edge.h"
#include "FloorPlan.h"
#include "Profile.h"
#include <maya/MFnMeshData.h>
#include <maya\MFnDoubleArrayData.h>
#include "FileParser.h"
#include <vector>

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MGlobal.h>

// You MUST change this to a unique value!!!  The id is a 32bit value used
// to identify this type of node in the binary file format.  
//
	//This is for the floorplan
	 MObject AuthoringToolPlugin::fileName;
	 MObject AuthoringToolPlugin::numberOfPoints;
	 MObject AuthoringToolPlugin::anchorPositions; 
	 MObject AuthoringToolPlugin::anchorRotations;

	//Output mesh
	 MObject AuthoringToolPlugin::outputMesh;


MTypeId     AuthoringToolPlugin::id( 0x01317 );

   
AuthoringToolPlugin::AuthoringToolPlugin() {}
AuthoringToolPlugin::~AuthoringToolPlugin() {}

MStatus AuthoringToolPlugin::compute( const MPlug& plug, MDataBlock& data )
//
//	Description:
//		This method computes the value of the given output plug based
//		on the values of the input attributes.
//
//	Arguments:
//		plug - the plug to compute
//		data - object that provides access to the attributes for this node
//
{
	MStatus returnStatus;
 
	// Check which output attribute we have been asked to compute.  If this 
	// node doesn't know how to compute it, we must return 
	// MS::kUnknownParameter.
	// 
	if( plug == outputMesh || plug == anchorPositions || plug == anchorRotations)
	{
		// Get a handle to the input attribute that we will need for the
		// computation.  If the value is being supplied via a connection 
		// in the dependency graph, then this call will cause all upstream  
		// connections to be evaluated so that the correct value is supplied.
		// 

		MDataHandle numberOfPointsHandle = data.inputValue(numberOfPoints, &returnStatus); //Done
		MDataHandle fileNameHandle = data.inputValue(fileName, &returnStatus);

		if( returnStatus != MS::kSuccess )
			MGlobal::displayError( "Node AuthoringToolPlugin cannot get value\n" );
		
		//This section is a lot longer than I would like.  I'll look at refactoring this and moving it to other methods.
		else
		{
			//These values will be used to construct the FloorPlan object
			int numberOfPoints = numberOfPointsHandle.asInt();
			MString thefileName = fileNameHandle.asString();
			
			std::string myFile = thefileName.asChar();
			MGlobal::displayInfo(thefileName);

			//MString thefile = fileNameHandle.asString();
			
			//std::string myFile = thefile.asChar();
			//std::cout<<"The name of the file is: "<<myFile<<std::endl;
			std::cout<<"In Node..."<<std::endl;

			FileParser parser = FileParser(myFile);
			parser.parseFile();

			std::cout<<"***********************************************************"<<std::endl;
			std::cout<<"The size of the anchors in node is: "<<parser.getAnchors().size()<<std::endl;
			SweepPlane *sweep = new SweepPlane(parser.getFloorPlan(), parser.getProfiles(), parser.getAnchors());

			//Validate that the data is structured correctly -> WILL NEED TO SET THE OUTPUT VALUES CORRECTLY!
			sweep->validateData();
			sweep->buildIt();
			

			//WE will now call the sweep plan methods and produce the mesh
		
	
			// Get a handle to the output attribute.  This is similar to the
			// "inputValue" call above except that no dependency graph 
			// computation will be done as a result of this call.
			// 
			MDataHandle outputMeshHandle = data.outputValue(AuthoringToolPlugin::outputMesh, &returnStatus );
			//TODO - add anchor info here
			MDataHandle outputAnchorPosHandle = data.outputValue(AuthoringToolPlugin::anchorPositions, &returnStatus); 
			MDataHandle outputAnchorRotHandle = data.outputValue(AuthoringToolPlugin::anchorRotations, &returnStatus); 

			MFnMeshData dataCreator;
			MObject newOutputData = dataCreator.create(&returnStatus);

			MGlobal::displayInfo("Before create mesh..addd mesh output");

			MFnDoubleArrayData dataCreator2;
			MObject newAnchorPosData = dataCreator2.create(&returnStatus);

		    MFnDoubleArrayData dataCreator3;
			MObject newAnchorRotData = dataCreator3.create(&returnStatus);

			sweep->createMesh(newOutputData, returnStatus);
			// This just copies the input value through to the output.  
			cout<<"Created the mesh successfully"<<endl;

			sweep->createAnchors(newAnchorPosData, newAnchorRotData, returnStatus); 
			if (!returnStatus)
				cout<<"UHOH! Our anchors didn't get created"<<endl;
			MFnDoubleArrayData test = newAnchorPosData;
			MDoubleArray test2 = test.array(&returnStatus); 
		    cout<<"Passed the data in OK?! "<<test2[0]<<endl;
			outputAnchorPosHandle.set( newAnchorPosData );
			outputMeshHandle.set( newOutputData );
			outputAnchorRotHandle.set( newAnchorRotData );

			// Mark the destination plug as being clean.  This will prevent the
			// dependency graph from repeating this calculation until an input 
			// of this node changes.
			// 
			data.setClean(plug);
		}
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* AuthoringToolPlugin::creator()
//
//	Description:
//		this method exists to give Maya a way to create new objects
//      of this type. 
//
//	Return Value:
//		a new object of this type
//
{
	return new AuthoringToolPlugin();
}

MStatus AuthoringToolPlugin::initialize()
//
//	Description:
//		This method is called to create and initialize all of the attributes
//      and attribute dependencies for this node type.  This is only called 
//		once when the node type is registered with Maya.
//
//	Return Values:
//		MS::kSuccess
//		MS::kFailure
//		
{
	// This sample creates a single input float attribute and a single
	// output float attribute.
	//
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	MStatus				stat;


	MGlobal::displayInfo("In initialize 1..\n");

	AuthoringToolPlugin::numberOfPoints = nAttr.create("numPoints","np",MFnNumericData::kInt, 1.0, &stat);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);

	AuthoringToolPlugin::fileName = tAttr.create("fileName","fileN",MFnData::kString, &stat);
	tAttr.setStorable(true);
	tAttr.setKeyable(true);

	AuthoringToolPlugin::anchorPositions = tAttr.create("anchorPosition", "ap", MFnData::kDoubleArray, &stat); 
	tAttr.setStorable(false);
	tAttr.setKeyable(true);

	AuthoringToolPlugin::anchorRotations = tAttr.create("anchorRotations", "ar", MFnData::kDoubleArray, &stat); 
	tAttr.setStorable(false);
	tAttr.setKeyable(true);

	AuthoringToolPlugin::outputMesh = tAttr.create("outputMesh", "outMesh", MFnData::kMesh, &stat);

	tAttr.setStorable(false);
	tAttr.setWritable(true);
	

	//Need to figure out what traits to a

	CHECK_MSTATUS(addAttribute(AuthoringToolPlugin::fileName));
	CHECK_MSTATUS(addAttribute(AuthoringToolPlugin::numberOfPoints));
	CHECK_MSTATUS(addAttribute(AuthoringToolPlugin::anchorPositions));
	CHECK_MSTATUS(addAttribute(AuthoringToolPlugin::anchorRotations));
	CHECK_MSTATUS(addAttribute(AuthoringToolPlugin::outputMesh));

	CHECK_MSTATUS(attributeAffects(AuthoringToolPlugin::fileName,AuthoringToolPlugin::outputMesh));
	CHECK_MSTATUS(attributeAffects(AuthoringToolPlugin::numberOfPoints,AuthoringToolPlugin::outputMesh));

	//// Add the attributes we have created to the node
	////
	//stat = addAttribute( input );
	//	if (!stat) { stat.perror("addAttribute"); return stat;}
	//stat = addAttribute( output );
	//	if (!stat) { stat.perror("addAttribute"); return stat;}

	//// Set up a dependency between the input and the output.  This will cause
	//// the output to be marked dirty when the input changes.  The output will
	//// then be recomputed the next time the value of the output is requested.
	////
	//stat = attributeAffects( input, output );
	//	if (!stat) { stat.perror("attributeAffects"); return stat;}
    MGlobal::displayInfo("In initialize 2..\n");
	return MS::kSuccess;

}

