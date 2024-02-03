//
// File: ShakeNode.cpp
//
// Dependency Graph Node: shake
//
// Author: Benjamin H. Singleton
//

#include "ShakeNode.h"

MObject		Shake::envelope;
MObject		Shake::time;
MObject		Shake::seed;
MObject		Shake::frequency;
MObject		Shake::fractal;
MObject		Shake::roughness;
MObject		Shake::rampIn;
MObject		Shake::rampOut;
MObject		Shake::strength;
MObject		Shake::strengthX;
MObject		Shake::strengthY;
MObject		Shake::strengthZ;
MObject		Shake::positive;
MObject		Shake::positiveX;
MObject		Shake::positiveY;
MObject		Shake::positiveZ;

MObject		Shake::outputTranslate;
MObject		Shake::outputTranslateX;
MObject		Shake::outputTranslateY;
MObject		Shake::outputTranslateZ;
MObject		Shake::outputRotate;
MObject		Shake::outputRotateX;
MObject		Shake::outputRotateY;
MObject		Shake::outputRotateZ;
MObject		Shake::outputScale;
MObject		Shake::outputScaleX;
MObject		Shake::outputScaleY;
MObject		Shake::outputScaleZ;

MString		Shake::inputCategory("Input");
MString		Shake::outputCategory("Output");

MTypeId		Shake::id(0x0013b1ce);


Shake::Shake() {};
Shake::~Shake() {};


MStatus Shake::compute(const MPlug& plug, MDataBlock& data)
/**
This method should be overridden in user defined nodes.
Recompute the given output based on the nodes inputs.
The plug represents the data value that needs to be recomputed, and the data block holds the storage for all of the node's attributes.
The MDataBlock will provide smart handles for reading and writing this node's attribute values.
Only these values should be used when performing computations!

@param plug: Plug representing the attribute that needs to be recomputed.
@param data: Data block containing storage for the node's attributes.
@return: Return status.
*/
{

	MStatus status;

	// Check requested attribute
	//
	MObject attribute = plug.attribute(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MFnAttribute fnAttribute(attribute, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (fnAttribute.hasCategory(Shake::outputCategory))
	{

		// Get input handles
		//
		MDataHandle envelopeHandle = data.inputValue(Shake::envelope, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle timeHandle = data.inputValue(Shake::time, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle seedHandle = data.inputValue(Shake::seed, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle frequencyHandle = data.inputValue(Shake::frequency, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle fractalHandle = data.inputValue(Shake::fractal, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle roughnessHandle = data.inputValue(Shake::roughness, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle rampInHandle = data.inputValue(Shake::rampIn, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle rampOutHandle = data.inputValue(Shake::rampOut, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle strengthHandle = data.inputValue(Shake::strength, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle strengthXHandle = strengthHandle.child(Shake::strengthX);
		MDataHandle strengthYHandle = strengthHandle.child(Shake::strengthY);
		MDataHandle strengthZHandle = strengthHandle.child(Shake::strengthZ);

		MDataHandle positiveHandle = data.inputValue(Shake::positive, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle positiveXHandle = positiveHandle.child(Shake::positiveX);
		MDataHandle positiveYHandle = positiveHandle.child(Shake::positiveY);
		MDataHandle positiveZHandle = positiveHandle.child(Shake::positiveZ);

		// Get input values
		//
		float envelope = envelopeHandle.asFloat();
		float time = timeHandle.asTime().asUnits(MTime::uiUnit());
		int seed = seedHandle.asInt();
		float frequency = frequencyHandle.asFloat();
		bool fractal = fractalHandle.asBool();
		float roughness = roughnessHandle.asFloat();
		float rampIn = rampInHandle.asTime().asUnits(MTime::uiUnit());
		float rampOut = rampOutHandle.asTime().asUnits(MTime::uiUnit());
		double strengthX = strengthXHandle.asDouble();
		double strengthY = strengthYHandle.asDouble();
		double strengthZ = strengthZHandle.asDouble();
		bool positiveX = positiveXHandle.asBool();
		bool positiveY = positiveYHandle.asBool();
		bool positiveZ = positiveZHandle.asBool();

		ShakeSettings settings { time, seed, frequency, fractal, roughness, rampIn, rampOut };

		// Calculate noise
		//
		double noiseX = lerp(envelope, 0.0f, Shake::noiseAtTime(0, settings, positiveX) * strengthX);
		double noiseY = lerp(envelope, 0.0f, Shake::noiseAtTime(1, settings, positiveY) * strengthY);
		double noiseZ = lerp(envelope, 0.0f, Shake::noiseAtTime(2, settings, positiveZ) * strengthZ);

		MDistance translateX = MDistance(noiseX, MDistance::Unit::kCentimeters);
		MDistance translateY = MDistance(noiseY, MDistance::Unit::kCentimeters);
		MDistance translateZ = MDistance(noiseZ, MDistance::Unit::kCentimeters);

		MAngle rotateX = MAngle(noiseX, MAngle::Unit::kDegrees);
		MAngle rotateY = MAngle(noiseY, MAngle::Unit::kDegrees);
		MAngle rotateZ = MAngle(noiseZ, MAngle::Unit::kDegrees);

		double scaleX = 1.0 + noiseX;
		double scaleY = 1.0 + noiseY;
		double scaleZ = 1.0 + noiseZ;

		// Get output handles
		//
		MDataHandle outputTranslateXHandle = data.outputValue(Shake::outputTranslateX, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle outputTranslateYHandle = data.outputValue(Shake::outputTranslateY, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle outputTranslateZHandle = data.outputValue(Shake::outputTranslateZ, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle outputRotateXHandle = data.outputValue(Shake::outputRotateX, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle outputRotateYHandle = data.outputValue(Shake::outputRotateY, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle outputRotateZHandle = data.outputValue(Shake::outputRotateZ, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle outputScaleXHandle = data.outputValue(Shake::outputScaleX, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle outputScaleYHandle = data.outputValue(Shake::outputScaleY, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle outputScaleZHandle = data.outputValue(Shake::outputScaleZ, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Update output values
		//
		outputTranslateXHandle.setMDistance(translateX);
		outputTranslateXHandle.setClean();

		outputTranslateYHandle.setMDistance(translateY);
		outputTranslateYHandle.setClean();

		outputTranslateZHandle.setMDistance(translateZ);
		outputTranslateZHandle.setClean();

		outputRotateXHandle.setMAngle(rotateX);
		outputRotateXHandle.setClean();

		outputRotateYHandle.setMAngle(rotateY);
		outputRotateYHandle.setClean();

		outputRotateZHandle.setMAngle(rotateZ);
		outputRotateZHandle.setClean();

		outputScaleXHandle.setDouble(scaleX);
		outputScaleXHandle.setClean();

		outputScaleYHandle.setDouble(scaleY);
		outputScaleYHandle.setClean();

		outputScaleZHandle.setDouble(scaleZ);
		outputScaleZHandle.setClean();

		// Mark plug as clean
		//
		status = data.setClean(plug);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		return MS::kSuccess;

	}
	else
	{

		return MS::kUnknownParameter;

	}

};


float Shake::noiseAtTime(const int index, const ShakeSettings& settings, const bool positive)
/**
Computes a noise value for the supplied settings.

@param index: The array index to compute for.
@param settings: The user settings to compute from.
@return: Noise value.
*/
{

	// Compute ramping
	//
	float ramp = 1.0;

	float startTime = MAnimControl::animationStartTime().asUnits(MTime::uiUnit());
	float endTime = MAnimControl::animationEndTime().asUnits(MTime::uiUnit());

	if (settings.time < startTime + settings.rampIn)
	{

		float u = (settings.time - startTime) / settings.rampIn;
		ramp *= u * u * (3.0f - 2.0f * u);

	}

	if (settings.time > endTime - settings.rampOut)
	{

		float u = (endTime - settings.time) / settings.rampOut;
		ramp *= u * u * (3.0f - 2.0f * u);

	}

	// Compute noise
	// With certain frame rates, calculating whole frame random values always returned zero!
	// Adding the 0.0005 just edges it off of a whole number and we get a desirable result!
	//
	float res = 0.0;
	float v = settings.time * 0.0055 * settings.frequency + Noise::permutate(settings.seed + index);

	if (settings.fractal)
	{

		res = Noise::fractionalBrownianMotion(v, 1.0 - settings.roughness, 2.0, 6);

	}
	else
	{

		res = Noise::noise1(v);

	}

	if (positive)
	{

		res = fabs(res);

	}
	
	return res;

};


void* Shake::creator()
/**
This function is called by Maya when a new instance is requested.
See pluginMain.cpp for details.

@return: SoftIK
*/
{

	return new Shake();

};


MStatus Shake::initialize()
/**
This function is called by Maya after a plugin has been loaded.
Use this function to define any static attributes.

@return: MStatus
*/
{
	
	MStatus status;

	// Initialize function sets
	//
	MFnNumericAttribute fnNumericAttr;
	MFnUnitAttribute fnUnitAttr;
	MFnMatrixAttribute fnMatrixAttr;
	MFnCompoundAttribute fnCompoundAttr;

	// Input attributes:
	// ".envelope" attribute
	//
	Shake::envelope = fnNumericAttr.create("envelope", "env", MFnNumericData::kFloat, 1.0f, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(0.0));
	CHECK_MSTATUS(fnNumericAttr.setMax(1.0));
	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".time" attribute
	//
	Shake::time = fnUnitAttr.create("time", "t", MFnUnitAttribute::kTime, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.addToCategory(Shake::inputCategory));

	// ".seed" attribute
	//
	Shake::seed = fnNumericAttr.create("seed", "s", MFnNumericData::kInt, 0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(0));
	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".frequency" attribute
	//
	Shake::frequency = fnNumericAttr.create("frequency", "freq", MFnNumericData::kFloat, 0.5f, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	
	CHECK_MSTATUS(fnNumericAttr.setMin(DBL_MIN));
	CHECK_MSTATUS(fnNumericAttr.setMin(10.0));
	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".fractal" attribute
	//
	Shake::fractal = fnNumericAttr.create("fractal", "f", MFnNumericData::kBoolean, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".roughness" attribute
	//
	Shake::roughness = fnNumericAttr.create("roughness", "r", MFnNumericData::kFloat, 0.0f, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setMin(0.0));
	CHECK_MSTATUS(fnNumericAttr.setMin(1.0));
	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".rampIn" attribute
	//
	Shake::rampIn = fnUnitAttr.create("rampIn", "ri", MFnUnitAttribute::kTime, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(Shake::inputCategory));

	// ".rampOut" attribute
	//
	Shake::rampOut = fnUnitAttr.create("rampOut", "ro", MFnUnitAttribute::kTime, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setChannelBox(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(Shake::inputCategory));

	// ".strengthX" attribute
	//
	Shake::strengthX = fnNumericAttr.create("strengthX", "stx", MFnNumericData::kDouble, 5.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setKeyable(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".strengthY" attribute
	//
	Shake::strengthY = fnNumericAttr.create("strengthY", "sty", MFnNumericData::kDouble, 5.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setKeyable(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".strengthZ" attribute
	//
	Shake::strengthZ = fnNumericAttr.create("strengthZ", "stz", MFnNumericData::kDouble, 5.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setKeyable(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".strength" attribute
	//
	Shake::strength = fnNumericAttr.create("strength", "st", Shake::strengthX, Shake::strengthY, Shake::strengthZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setKeyable(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".positiveX" attribute
	//
	Shake::positiveX = fnNumericAttr.create("positiveX", "px", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".positiveY" attribute
	//
	Shake::positiveY = fnNumericAttr.create("positiveY", "py", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".positiveZ" attribute
	//
	Shake::positiveZ = fnNumericAttr.create("positiveZ", "pz", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// ".positive" attribute
	//
	Shake::positive = fnNumericAttr.create("positive", "p", Shake::positiveX, Shake::positiveY, Shake::positiveZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::inputCategory));

	// Output attributes:
	// ".outputTranslateX" attribute
	//
	Shake::outputTranslateX = fnUnitAttr.create("outputTranslateX", "otx", MFnUnitAttribute::kDistance, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setWritable(false));
	CHECK_MSTATUS(fnUnitAttr.setStorable(false));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(Shake::outputCategory));

	// ".outputTranslateY" attribute
	//
	Shake::outputTranslateY = fnUnitAttr.create("outputTranslateY", "oty", MFnUnitAttribute::kDistance, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setWritable(false));
	CHECK_MSTATUS(fnUnitAttr.setStorable(false));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(Shake::outputCategory));

	// ".outputTranslateZ" attribute
	//
	Shake::outputTranslateZ = fnUnitAttr.create("outputTranslateZ", "otz", MFnUnitAttribute::kDistance, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setWritable(false));
	CHECK_MSTATUS(fnUnitAttr.setStorable(false));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(Shake::outputCategory));

	// ".outputTranslate" attribute
	//
	Shake::outputTranslate = fnNumericAttr.create("outputTranslate", "ot", Shake::outputTranslateX, Shake::outputTranslateY, Shake::outputTranslateZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::outputCategory));

	// ".outputRotateX" attribute
	//
	Shake::outputRotateX = fnUnitAttr.create("outputRotateX", "orx", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setWritable(false));
	CHECK_MSTATUS(fnUnitAttr.setStorable(false));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(Shake::outputCategory));

	// ".outputRotateY" attribute
	//
	Shake::outputRotateY = fnUnitAttr.create("outputRotateY", "ory", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setWritable(false));
	CHECK_MSTATUS(fnUnitAttr.setStorable(false));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(Shake::outputCategory));

	// ".outputRotateZ" attribute
	//
	Shake::outputRotateZ = fnUnitAttr.create("outputRotateZ", "orz", MFnUnitAttribute::kAngle, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setWritable(false));
	CHECK_MSTATUS(fnUnitAttr.setStorable(false));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(Shake::outputCategory));

	// ".outputRotate" attribute
	//
	Shake::outputRotate = fnNumericAttr.create("outputRotate", "or", Shake::outputRotateX, Shake::outputRotateY, Shake::outputRotateZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::outputCategory));

	// ".outputScaleX" attribute
	//
	Shake::outputScaleX = fnNumericAttr.create("outputScaleX", "osx", MFnNumericData::kDouble, 1.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::outputCategory));

	// ".outputScaleY" attribute
	//
	Shake::outputScaleY = fnNumericAttr.create("outputScaleY", "osy", MFnNumericData::kDouble, 1.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::outputCategory));

	// ".outputScaleZ" attribute
	//
	Shake::outputScaleZ = fnNumericAttr.create("outputScaleZ", "osz", MFnNumericData::kDouble, 1.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::outputCategory));

	// ".outputScale" attribute
	//
	Shake::outputScale = fnNumericAttr.create("outputScale", "os", Shake::outputScaleX, Shake::outputScaleY, Shake::outputScaleZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(Shake::outputCategory));

	// Add attributes to node
	//
	CHECK_MSTATUS(Shake::addAttribute(Shake::envelope));
	CHECK_MSTATUS(Shake::addAttribute(Shake::time));
	CHECK_MSTATUS(Shake::addAttribute(Shake::seed));
	CHECK_MSTATUS(Shake::addAttribute(Shake::frequency));
	CHECK_MSTATUS(Shake::addAttribute(Shake::fractal));
	CHECK_MSTATUS(Shake::addAttribute(Shake::roughness));
	CHECK_MSTATUS(Shake::addAttribute(Shake::rampIn));
	CHECK_MSTATUS(Shake::addAttribute(Shake::rampOut));
	CHECK_MSTATUS(Shake::addAttribute(Shake::strength));
	CHECK_MSTATUS(Shake::addAttribute(Shake::positive));

	CHECK_MSTATUS(Shake::addAttribute(Shake::outputTranslate));
	CHECK_MSTATUS(Shake::addAttribute(Shake::outputRotate));
	CHECK_MSTATUS(Shake::addAttribute(Shake::outputScale));

	// Define attribute relationships
	//
	CHECK_MSTATUS(Shake::attributeAffects(Shake::envelope, Shake::outputTranslate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::time, Shake::outputTranslate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::seed, Shake::outputTranslate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::frequency, Shake::outputTranslate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::fractal, Shake::outputTranslate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::roughness, Shake::outputTranslate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::rampIn, Shake::outputTranslate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::rampOut, Shake::outputTranslate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::strength, Shake::outputTranslate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::positive, Shake::outputTranslate));

	CHECK_MSTATUS(Shake::attributeAffects(Shake::envelope, Shake::outputRotate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::time, Shake::outputRotate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::seed, Shake::outputRotate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::frequency, Shake::outputRotate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::fractal, Shake::outputRotate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::roughness, Shake::outputRotate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::rampIn, Shake::outputRotate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::rampOut, Shake::outputRotate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::strength, Shake::outputRotate));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::positive, Shake::outputRotate));

	CHECK_MSTATUS(Shake::attributeAffects(Shake::envelope, Shake::outputScale));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::time, Shake::outputScale));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::seed, Shake::outputScale));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::frequency, Shake::outputScale));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::fractal, Shake::outputScale));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::roughness, Shake::outputScale));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::rampIn, Shake::outputScale));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::rampOut, Shake::outputScale));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::strength, Shake::outputScale));
	CHECK_MSTATUS(Shake::attributeAffects(Shake::positive, Shake::outputScale));

	return status;

};