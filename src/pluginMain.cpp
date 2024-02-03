//
// File: pluginMain.cpp
//
// Author: Benjamin H. Singleton
//

#include "ShakeNode.h"
#include "ShakeCmd.h"
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

	status = plugin.registerCommand(COMMAND_NAME, ShakeCmd::creator, ShakeCmd::newSyntax);

	if (!status)
	{

		status.perror("registerCommand");
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

	status = plugin.deregisterCommand(COMMAND_NAME);

	if (!status)
	{

		status.perror("deregisterCommand");
		return status;

	}

	return status;

};