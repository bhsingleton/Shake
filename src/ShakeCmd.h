#ifndef _SHAKE_COMMAND
#define _SHAKE_COMMAND
//
// File: ShakeCmd.h
//
// MEL Command: shake
//
// Author: Benjamin H. Singleton
//

#include <maya/MPxCommand.h>

#include <maya/MAnimControl.h>
#include <maya/MTime.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MSyntax.h>
#include <maya/MStatus.h>
#include <maya/MGlobal.h>

#define COMMAND_NAME "shake"


class ShakeCmd : public MPxCommand
{

public:

						ShakeCmd();
	virtual				~ShakeCmd();

	virtual MStatus		doIt(const MArgList& args);
	virtual	MStatus		redoIt();

	virtual	bool		isUndoable() const;

	static  void*		creator();

	static	MSyntax		newSyntax();
	virtual	bool		hasSyntax();

protected:

	int				seed;
	float			frequency;
	float			roughness;
	bool			fractal;
	float			rampIn;
	float			rampOut;
	MTime			startTime;
	MTime			endTime;
	unsigned int	size;
	unsigned int	step; 
	int				timeScale;

};
#endif