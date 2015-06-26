#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

//==============================================================================
// Header Files 
//==============================================================================

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.h"

//==============================================================================
// Drawable Interface 
//==============================================================================

class Drawable {

protected:

	/**
	 * Shader pointer to the Shader object used to draw this Drawable object.
	 */
	ShaderPtr shader;

public:

	// Public constructor.
	Drawable(ShaderPtr _shader);

	/**
	 * @brief Interface of drawing a drawable with the current OpenGL context.
	 * @details Pure virtual function which must be overridden. 
	 * 
	 * @param  worldToProjection Transformation matrix representing the 
	 * projection of the world onto the view.
	 */
	virtual void draw(const glm::mat4& worldToProjection) = 0;

};

#endif