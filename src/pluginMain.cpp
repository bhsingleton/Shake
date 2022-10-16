//
// File: pluginMain.cpp
//
// Author: Benjamin H. Singleton
//

#include "ShakeNode.h"
#include <maya/MFnPlugin.h>


MStatus initializePlugin(MObject obj)
{

	MStatus status;

	MFnPlugin plugin(obj, "Ben Singleton", "2017", "Any");
	status = plugin.registerNode("shake", Shake::id, Shake::creator, Shake::initialize);

	if (!status)
	{

		status.perror("registerNode");
		return status;

	}

	return status;

};


MStatus uninitializePlugin(MObject obj)
{

	MStatus status;

	MFnPlugin plugin(obj);
	status = plugin.deregisterNode(Shake::id);

	if (!status)
	{

		status.perror("deregisterNode");
		return status;

	}

	return status;

};