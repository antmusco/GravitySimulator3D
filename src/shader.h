#ifndef _SHADER_H_
#define _SHADER_H_

//==============================================================================
// Header Files 
//==============================================================================

#include <string>
#include <map>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "enums.h"
#include "interfaces/configurable.h"

//==============================================================================
// Constants.
//==============================================================================

#define NUM_SHADERS 2
#define VERT 0
#define FRAG 1

//==============================================================================
// Typedefs.
//==============================================================================

class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

//==============================================================================
// Shader Class.
//==============================================================================

/**
 * Class which manages the compilation and linking of shader programs.
 */
class Shader : public Configurable {

private:

	/**
	 * Name of the vertex shader file to be used by this Shader.
	 */
	std::string vertexShaderFile;

	/**
	 * Name of the fragment shader file to be used by this Shader.
	 */
	std::string fragmentShaderFile;

	/**
	 * The type of Shader being used.
	 */
	ShaderType type;

	/**
	 * Handle to the compiled program.
	 */
	GLuint program;

	/**
	 * Shader handles (Vertex, Fragment).
	 */
	GLuint shaders[NUM_SHADERS];

	/**
	 * Handles to the locations of the uniform variables in the shader program.
	 * The index of each location should correspond exactly to the name given
	 * in /c uniformNames.
	 */
	std::map<Uniform, GLuint> uniformLocations;

	// Create the shader program.
	void createShaderProgram(std::string vertexShaderFile, std::string 
		fragmentShaderFile);

	// Check for compilation error.
	bool checkShaderError(GLuint shaderID);

	// Check for linking error.
	bool checkProgramError(GLuint program);

	// Grab the uniform locations from the shader program.
	virtual void initializeUniformLocations();

	// Bind the attributes to be drawn.
	virtual void bindAttributes() const;

protected:

	// Configurable interface.
	virtual ErrorCode configureVariables();

public:
	
	//Constructor.
	Shader(const char* _configFile);

	// Tell OpenGL to use this program.
	void use() const;

	// Setters
	void setUniformMat4(Uniform type, const glm::mat4& mat);
	void setUniformInt1(Uniform type, GLint i);
};

#endif
