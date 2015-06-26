#ifndef _SHADER_SINGLETON_H_
#define _SHADER_SINGLETON_H_

//==============================================================================
// Header Files 
//==============================================================================

#include <string>

#include "enums.h"
#include "configurable.h"
#include "shader.h"

//==============================================================================
// ShaderSingleton class.
//==============================================================================

class ShaderSingleton : Configurable {

private:

	/**
	 * Single instance of the ShaderSingleton class.
	 */
	static ShaderSingleton* instance;

	/**
	 * Map of Shaders.
	 */
	std::map<ShaderType, ShaderPtr> shaders;

	// Private constructor.
	ShaderSingleton(const char* _configFile);

	// Destructor.
	virtual ~ShaderSingleton();

protected:

	ErrorCode configureVariables();

public:

	// Get the Mesh Shader.
	static ShaderPtr getShader(ShaderType type);

	// Create and configure the ShaderSingleton.
	static void createShaderSingleton(const char* _configFile);

	// Destroy the ShaderSingleton instance.
	static void destroyShaderSingleton();
	
};



#endif