/**
 * \file enums.h
 * Central collection of all the enumerations required for the project. Enums
 * are encouraged, and should be kept here for reference.  
 */
#ifndef _ENUMS_H_
#define _ENUMS_H_


#include <iostream>
#include <map>
#include <string>

#include "basetypes.h"

/**
 * Config file names.
 */
#define  MESHES_DIR           "res/meshes/"
#define  TEXUTRES_DIR         "res/textures/"
#define  SHADERS_DIR          "res/shaders/"
#define  CONFIG_DIR           "config/"
#define  SCRIPTS_DIR          "config/scripts/"

/**
 * Enumeration defining the types of errors which may be encountered during
 * excecution. It should be noted that error codes returned by functions are
 * prefered to exception handling, and should be used where ever possible to
 * reduce the overhead. 
 */
enum class ErrorCode {

	Success, // Good Excecution.
	Failure, // Failed Execution.
	NotYetImplemented, // Function is not yet implemented.
	RunnableAlreadyRunning, // A Runnable cannot be run on two separate threads.
	ConfigFileNotFound, // No such config file exists.
	
};

/**
 * Enumeration specifying valid dimensions for function calls.
 */
enum class Dimension {

	Width,
	Height,
	Depth,
	
};

/**
 * @brief Enumeration describing an orientation.
 * @details Describes the posible directions that can be referenced in the 
 * program. 
 */
enum class Direction {

	Left,
	Right,
	Forward,
	Backward,
	Up,
	Down,

};


enum class ShaderType {

	Mesh,
	Splat,

};


enum class Uniform {

	ModelToWorld,
	ModelToProjection,
	Texture,
	LightSource,
	AmbientLight,

};

enum class MeshAttribute {

	Position,
	Color,
	Normal,
	TexCoord,

};

extern std::map<std::string, ShaderType> stringShaders;
extern std::map<Uniform, std::string> meshUniformStrings;
extern std::map<MeshAttribute, std::string> meshAttributeStrings;

#endif // _ENUMS_H_