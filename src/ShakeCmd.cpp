//
// File: ShakeCmd.cpp
//
// MEL Command: shake
//
// Author: Benjamin H. Singleton
//

#include "ShakeCmd.h"
#include "Noise.h"


ShakeCmd::ShakeCmd() 
{

	this->seed = 0;
	this->frequency = 0.5f;
	this->roughness = 0.0f;
	this->fractal = true;
	this->rampIn = 0.0f;
	this->rampOut = 0.0f;
	this->step = 1;
	this->size = 1;
	this->timeScale = 1;

};


ShakeCmd::~ShakeCmd() {};


MStatus ShakeCmd::doIt(const MArgList& args)
/**
This method should perform a command by setting up internal class data and then calling the redoIt method.
The actual action performed by the command should be done in the redoIt method. This is a pure virtual method, and must be overridden in derived classes.

@param args: List of arguments passed to the command.
@return Status code.
*/
{

	MStatus status;
	
	// Initialize argument parser
	//
	MSyntax syntax = ShakeCmd::syntax();

	MArgDatabase argData(syntax, args, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Check for `seed` flag
	//
	bool isFlagSet = argData.isFlagSet("seed", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (isFlagSet)
	{

		this->seed = argData.flagArgumentInt("seed", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Check for `frequency` flag
	//
	isFlagSet = argData.isFlagSet("frequency", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (isFlagSet)
	{

		this->frequency = (float)argData.flagArgumentDouble("frequency", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Check for `roughness` flag
	//
	isFlagSet = argData.isFlagSet("roughness", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (isFlagSet)
	{

		this->roughness = (float)argData.flagArgumentDouble("roughness", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Check for `fractal` flag
	//
	isFlagSet = argData.isFlagSet("fractal", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (isFlagSet)
	{

		this->fractal = argData.flagArgumentBool("fractal", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Check for `rampIn` flag
	//
	isFlagSet = argData.isFlagSet("rampIn", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (isFlagSet)
	{

		this->rampIn = (float)argData.flagArgumentDouble("rampIn", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Check for `rampOut` flag
	//
	isFlagSet = argData.isFlagSet("rampOut", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (isFlagSet)
	{

		this->rampOut = (float)argData.flagArgumentDouble("rampOut", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Check for `size` flag
	//
	isFlagSet = argData.isFlagSet("size", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (isFlagSet)
	{

		this->size = argData.flagArgumentInt("size", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Check for `step` flag
	//
	isFlagSet = argData.isFlagSet("step", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (isFlagSet)
	{

		this->step = argData.flagArgumentInt("step", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Check for `timeScale` flag
	//
	isFlagSet = argData.isFlagSet("timeScale", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (isFlagSet)
	{

		this->timeScale = argData.flagArgumentInt("timeScale", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	// Get current animation range
	//
	this->startTime = MAnimControl::animationStartTime();
	this->endTime = MAnimControl::animationEndTime();

	return this->redoIt();

};


MStatus ShakeCmd::redoIt()
/**
This method should do the actual work of the command based on the internal class data only.
Internal class data should be set in the doIt method.

@return: Status code.
*/
{
	
	// Iterate through size range
	//
	unsigned int end = this->size * this->timeScale;

	float rampScale = float(end) / float(this->endTime.value() - this->startTime.value());
	float rampIn = this->rampIn * rampScale;
	float rampOut = this->rampOut * rampScale;

	float ramp, noise, u, v;
	int time;

	for (int x = 0; x < (this->size + this->step); x += this->step)
	{

		time = x * this->timeScale;
		ramp = 1.0f;

		// mjm 9.28.98
		// add a check for div by zero on cont->rampin
		// since x>=0, then t>=0, so t should never be < cont->rampin when cont->rampin is 0
		// but fix is added for symmetry with fix below
		//
		if ((rampIn != 0.0f) && (time < rampIn))
		{

			u = float(time) / float(rampIn);
			ramp *= u * u * (3.0f - 2.0f * u);

		}

		// mjm 9.28.98
		// bugfix -- add a check for div by zero on cont->rampout
		// because of STEPSIZE and GRAPH_TIMESCALE, t jumps from being < end to being > end.
		// when cont->rampout equals 0, (its default setting), then (t>end-cont->rampout) is TRUE,
		// leading to the div by zero
		//
		if ((rampOut != 0.0f) && (time > end - rampOut))
		{

			u = float(end - time) / float(rampOut);
			ramp *= u * u * (3.0f - 2.0f * u);

		}

		// Compute noise
		// KZ 25/11/2015, changed 0.005 to 0.0055. The number was too perfect.
		// With certain frame rates, calculating whole frame random values always returned zero.
		// Adding the 0.0005 just edges it off of a whole number and we get a desirable result.
		//
		v = float(time) * float(0.0055) * this->frequency + Noise::permutate(this->seed);

		if (this->fractal) 
		{

			noise = Noise::fractionalBrownianMotion(v, 1.0f - this->roughness, 2.0f, 6 /*octaves*/);

		}
		else 
		{

			noise = Noise::noise1(v);

		}

		noise *= ramp;
		this->appendToResult(noise);

	}
	
	return MS::kSuccess;

};


bool ShakeCmd::isUndoable() const
/**
Evaluates if this command is undoable.
This command was original developed for Vertex Blender so we want the tool to handle disposing of the color set.

@return: Boolean.
*/
{

	return false;

};


MSyntax ShakeCmd::newSyntax()
/**
Returns a syntax object that can parse this command.

@return: The syntax.
*/
{

	MStatus status;

	// Define optional flags
	//
	MSyntax syntax;
	
	status = syntax.addFlag("-s", "-seed", MSyntax::kLong);
	CHECK_MSTATUS(status);

	status = syntax.addFlag("-frq", "-frequency", MSyntax::kDouble);
	CHECK_MSTATUS(status);

	status = syntax.addFlag("-r", "-roughness", MSyntax::kDouble);
	CHECK_MSTATUS(status);

	status = syntax.addFlag("-f", "-fractal", MSyntax::kBoolean);
	CHECK_MSTATUS(status);

	status = syntax.addFlag("-ri", "-rampIn", MSyntax::kDouble);
	CHECK_MSTATUS(status);

	status = syntax.addFlag("-ro", "-rampOut", MSyntax::kDouble);
	CHECK_MSTATUS(status);

	status = syntax.addFlag("-sz", "-size", MSyntax::kLong);
	CHECK_MSTATUS(status);

	status = syntax.addFlag("-stp", "-step", MSyntax::kLong);
	CHECK_MSTATUS(status);

	status = syntax.addFlag("-ts", "-timeScale", MSyntax::kLong);
	CHECK_MSTATUS(status);

	// Define extended behavior
	//
	syntax.useSelectionAsDefault(false);
	syntax.enableEdit(false);
	syntax.enableQuery(false);

	return syntax;

};


bool ShakeCmd::hasSyntax()
/**
Evaluates if this command uses syntax.

@return: Boolean.
*/
{

	return true;

};


void* ShakeCmd::creator()
/**
Returns a new instance of this command.

@return: A new instance.
*/
{

	return new ShakeCmd();

};