#ifndef _AuthoringToolPluginNode
#define _AuthoringToolPluginNode
//
// Copyright (C) Protrusion
// 
// File: AuthoringToolPluginNode.h
//
// Dependency Graph Node: AuthoringToolPlugin
//
// Author: Maya Plug-in Wizard 2.0.1
// Revised by Isaac Peral
//
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

 
class AuthoringToolPlugin : public MPxNode
{
public:
						AuthoringToolPlugin();
	virtual				~AuthoringToolPlugin(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:

	// There needs to be a MObject handle declared for each attribute that
	// the node will have.  These handles are needed for getting and setting
	// the values later.
	//
	static  MObject		input;		// Example input attribute
	static  MObject		output;		// Example output attribute

	//These are associated with the floorplan
	 static MObject edgeArray;
	 static MObject numberOfPoints;
	 static MObject profileEdgeArray;
	 static MObject anchorArray;

	//These are general objects coming in
	 static MObject numberOfProfiles;
	 static MObject profileLengthArray;

	//These are for the profiles - note that the profiles will be aggregated together
	 static MObject profileEdges;
	 static MObject profileAnchorArray;

	//Output mesh
	 static MObject outputMesh;

	// The typeid is a unique 32bit indentifier that describes this node.
	// It is used to save and retrieve nodes of this type from the binary
	// file format.  If it is not unique, it will cause file IO problems.
	//
	static	MTypeId		id;
};

#endif
