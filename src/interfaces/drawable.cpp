//==============================================================================
// Header Files 
//==============================================================================

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "drawable.h"

//==============================================================================
// Drawable Interface 
//==============================================================================


/**
 * @brief Public constructor for a Drawable.
 * @details Constructor which initalizes the Shader for this Drawable.
 * 
 * @param _shader Pointer to a shader to be used for this Drawable.
 */
Drawable::Drawable(ShaderPtr _shader) :
	shader(_shader) {

	// Empty.

}