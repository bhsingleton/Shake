cmake_minimum_required(VERSION 3.21)
project(Shake)
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

set(
	SOURCE_FILES
	"pluginMain.cpp"
	"Noise.h"
	"Noise.cpp"
	"ShakeNode.h"
	"ShakeNode.cpp"
)

set(
	LIBRARIES
	OpenMaya
	OpenMayaAnim
	Foundation
)

build_plugin()