//==============================================================================
// Header Files 
//==============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "utilities.h"
#include "shader.h"

//==============================================================================
// Code 
//==============================================================================

/**
 * @brief Creates the program from the vertex and fragment shader files.
 * @details Extracts the source code from both the vertex shader and the 
 * fragment shader, compiles each pogram separately, and links them together
 * to create the program.
 * 
 * @param vertexShaderFile Name of the vertex shader file.
 * @param fragmentShaderFile Name of the fragment shader file.
 */
void Shader::createShaderProgram(std::string vertexShaderFile, std::string 
	fragmentShaderFile) {

	// Load the source code (GLSL) into the indicated strings.
	std::string vertexSource = fileToString(std::string(SHADERS_DIR) + 
		vertexShaderFile);
	std::string fragmentSource = fileToString(std::string(SHADERS_DIR) + 
		fragmentShaderFile);

	// Create the shader program.
	program = glCreateProgram();

	// Create the vertex shader and the fragment shader
	shaders[VERT] = glCreateShader(GL_VERTEX_SHADER);
	shaders[FRAG] = glCreateShader(GL_FRAGMENT_SHADER);


	const char* inputs[1];

	// Add the source code to the shaders.
	inputs[0] = vertexSource.c_str();
	glShaderSource(shaders[VERT], 1, inputs, 0);
	inputs[0] = fragmentSource.c_str();
	glShaderSource(shaders[FRAG], 1, inputs, 0);

	// Compile the added source code.
	glCompileShader(shaders[VERT]);
	glCompileShader(shaders[FRAG]);

	// If either of the shaders did not compile correctly, return.
	if (checkShaderError(shaders[VERT]) || 
		checkShaderError(shaders[FRAG])) {

		return;

	}

	// Attach the shaders to the program.
	glAttachShader(program, shaders[VERT]);
	glAttachShader(program, shaders[FRAG]);

	// Link the shader objects.
	glLinkProgram(program);

	// If linking failed, return.
	if (checkProgramError(program)) {

		return;
	
	}

}



/**
 * Checks to see if the shader was compiled correctly.
 * 
 * @param The handle of the shader to check for compile error.
 * 
 * @returns  A boolean value indicating whether there was an error or not.     *
 */
bool Shader::checkShaderError(GLuint shaderID) {

	// Get the compile status and store it in the GLint.
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

	// If the compile status is not 1, output the error information.
	if (compileStatus != GL_TRUE) {

		// Get the handle of the info log.
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		// Get the last entry in the info log.
		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);

		// Output the error log.
		std::cout << buffer << std::endl;

		// Delete dynamic memory and return that there was an error.
		delete [] buffer;
		return true;
		
	}

	// Return there was not an error.
	return false;

}



/**
 * This function tests a program to see whether there was an error linking.
 * 
 * @param programID The handle of the program to check for linking error.
 * @returns A boolean value indicating whether there was an error or not.
 */
bool Shader::checkProgramError(GLuint programID) {

	// Get the link status and store it in the GLint.
	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);

	// If the link status is not 1, output the error information.
	if (linkStatus != GL_TRUE) {

		// Get the handle of the info log.
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		// Get the last entry in the info log.
		GLsizei bufferSize;
		glGetProgramInfoLog(programID, infoLogLength, &bufferSize, buffer);

		// Output the error log.
		std::cout << buffer << std::endl;

		// Delete dynamic memory and return that there was an error.
		delete[] buffer;
		return true;

	}

	// Return there was not an error.
	return false;

}


/**
 * @brief Initializes the Uniform locations for the current Shader program.
 * @details Depending on which type of Shader is being used (e.g. Mesh, Splat,
 * etc.) different uniforms are located within the shader program.
 */
void Shader::initializeUniformLocations() {

	glUseProgram(program);

	switch(type) {

		case ShaderType::Mesh:

			for(auto it = meshUniformStrings.begin(); it != 
				meshUniformStrings.end(); ++it) {
				
				uniformLocations[it->first] = glGetUniformLocation(program,
					 it->second.c_str());

			}	
			break;

		case ShaderType::Splat:

			break;

		default:

			break;
 
	}

}



/**
 * @brief Binds the attributes of this Shader for drawing.
 * @details Takes the attributes and binds their locations to the corresponding
 * 		attribute location.
 */
void Shader::bindAttributes() const {

	glUseProgram(program);
	int i = 0;

	switch(type) {

		case ShaderType::Mesh:

			// Bind each attribute with the given name.
			for(auto it = meshAttributeStrings.begin(); it != 
				meshAttributeStrings.end(); ++it) {

				glBindAttribLocation(program, i, it->second.c_str());
				++i;

			}
			break;

		case ShaderType::Splat:

			break;

		default:

			break;
 
	}

}



/**
 * @brief Sets the initial values of all member variables for the Camera.
 * @details Uses the \c initialValuesMap set during the Configurable 
 * constructor.
 * @return ErrorCode indicating success or cause of failure.
 */
ErrorCode Shader::configureVariables() {

	// Grab the vertex shader file and the fragment shader file.
	for(auto it = initialValuesMap.begin(); it != initialValuesMap.end(); ++it) {

		if(it->first == "type") {

			type = stringShaders[it->second];

		} if(it->first == "vertexShaderFile") {

			vertexShaderFile = it->second;

		} else if (it->first == "fragmentShaderFile") {

			fragmentShaderFile = it->second;

		}

	}

	return ErrorCode::Success;

}



/**
 * @brief Constructor for the Shader class.
 * @details Loads the text from the indicated files into strings and compiles 
 * them into executable shader objects. The function then links them into an 
 * executable shader program, and tells OpenGL to use this program for shading.
 * 
 * @param _configFile File in which the configuration information is located.
 */
Shader::Shader(const char* _configFile) :
	
	Configurable(_configFile) {

 	// Configure variables before proceeding.
	configureVariables();

	// Create the program.
	createShaderProgram(vertexShaderFile, fragmentShaderFile);

	// Get the locations of the Uniform variables.
	initializeUniformLocations();

}



/**
 * @brief Tells OpenGL to use this Shader
 * @details Tells OpenGL to use this shder program and updates all of the 
 * uniform variables.
 */
void Shader::use() const {

	glUseProgram(program);
	bindAttributes();

}



/**
 * @brief Copies a matrix Uniform down to the graphics hard
 * @details Sends the specified uniform matrix down to the graphics hardware.
 * 
 * @param type The type of matrix uniform to copy.
 * @param mat The matrix to copy.
 */
void Shader::setUniformMat4(Uniform type, const glm::mat4& mat) {

	glUniformMatrix4fv(uniformLocations[type], 1, 0, &mat[0][0]);

}



/**
 * @brief Copies an integer Uniform down to the graphics hard
 * @details Sends the specified uniform integer down to the graphics hardware.
 * 
 * @param type The type of integer uniform to copy.
 * @param mat The integer to copy.
 */
void Shader::setUniformInt1(Uniform type, GLint i) {

	glUniform1i(uniformLocations[type], i);

}