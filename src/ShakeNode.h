#ifndef _ShakeNode
#define _ShakeNode
//
// File: ShakeNode.h
//
// Dependency Graph Node: shake
//
// Author: Benjamin H. Singleton
//

#include "Noise.h"

#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MTime.h>
#include <maya/MDistance.h>
#include <maya/MAngle.h>
#include <maya/MVector.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MAnimControl.h>
#include <maya/MTypeId.h> 
#include <maya/MGlobal.h>


struct ShakeSettings
{

	float time = 0.0;
	int seed = 0;
	float frequency = 0.5;
	bool fractal = true;
	float roughness = 0.0;
	float rampIn = 0.0;
	float rampOut = 0.0;

};


class Shake : public MPxNode
{

public:

						Shake();
	virtual				~Shake();

	virtual MStatus		compute(const MPlug& plug, MDataBlock& data);
	static	float		noiseAtTime(const int index, const ShakeSettings& settings, const bool positive);

	static  void*		creator();
	static  MStatus		initialize();

public:

	static	MObject		envelope;
	static	MObject		time;
	static	MObject		seed;
	static	MObject		frequency;
	static	MObject		fractal;
	static	MObject		roughness;
	static	MObject		rampIn;
	static	MObject		rampOut;
	static	MObject		strength;
	static	MObject		strengthX;
	static	MObject		strengthY;
	static	MObject		strengthZ;
	static	MObject		positive;
	static	MObject		positiveX;
	static	MObject		positiveY;
	static	MObject		positiveZ;

	static	MObject		outputTranslate;
	static	MObject		outputTranslateX;
	static	MObject		outputTranslateY;
	static	MObject		outputTranslateZ;
	static	MObject		outputRotate;
	static	MObject		outputRotateX;
	static	MObject		outputRotateY;
	static	MObject		outputRotateZ;
	static	MObject		outputScale;
	static	MObject		outputScaleX;
	static	MObject		outputScaleY;
	static	MObject		outputScaleZ;

	static	MString		inputCategory;
	static	MString		outputCategory;

	static	MTypeId		id;

};

#endif