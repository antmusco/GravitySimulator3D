//==============================================================================
// Header Files 
//==============================================================================

#include "shadersingleton.h"

//==============================================================================
// ShaderSingleton class.
//==============================================================================

// Initialize static memeber.
ShaderSingleton* ShaderSingleton::instance = NULL;

/**
 * @brief Private constructor for the ShaderSingleton class.
 * @details Configures the ShaderSingleton by loading all Shader objects into
 * its /c shaders member vector.
 * 
 * @param _configFile The string name of the config file.
 */
ShaderSingleton::ShaderSingleton(const char* _configFile) :

	Configurable(_configFile) {

 	// Create the shaders.
	configureVariables();

}


/**
 * @brief Virtual Destructor.
 * @details Virtual Destructor.
 */
ShaderSingleton::~ShaderSingleton() {

	// Empty.

}



/**
 * @brief Creates all of the Shader objects described in the config file.
 * @details Uses the \c variablesNames and \c variablesValues vectors set 
 * 		during the Configurable constructor.
 * @return ErrorCode indicating success or cause of failure.
 */
ErrorCode ShaderSingleton::configureVariables() {

	// Initialize all of the shaders.
	for(auto it = initialValuesMap.begin(); it != initialValuesMap.end();
		++it) {

		if(it->first == "Mesh") {

			shaders[ShaderType::Mesh] = ShaderPtr(
				new Shader(it->second.c_str())
			);

		}

	}

	return ErrorCode::Success;

}



/**
 * @brief Static method for accessing a specific Shader object.
 * @details Uses a the ShaderType enumeration to access the specified Shader
 * object from the ShaderSingleton's instance.
 * 
 * @param type ShaderType indicating which Shader to access.
 * @return A pointer to the Shader object requested.
 */
ShaderPtr ShaderSingleton::getShader(ShaderType type) {

	return instance->shaders[type];

}



/**
 * @brief Static initialization function which creates the ShaderSingelton 
 * instance.
 * @details Only creates the ShaderSingleton instance if the static \c instance
 * variable is NULL.
 * 
 * @param _configFile The string name of the config file.
 */
void ShaderSingleton::createShaderSingleton(const char* _configFile) {

	// Only create singleton once.
	if (instance == NULL) {

		instance = new ShaderSingleton(_configFile);

	} else {

		std::cout << "Can only create ShaderSingleton once!" << std::endl;

	}

}


/**
 * @brief Destroys the ShaderSingleton instance.
 * @details What more do you want from me?
 */
void ShaderSingleton::destroyShaderSingleton() {

	// Destroy the instance.
	delete instance;

}
