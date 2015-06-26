#ifndef _DISPLAY_H_
#define _DISPLAY_H_

//==============================================================================
// Header Files 
//==============================================================================

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "drawable.h"
#include "configurable.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"

//==============================================================================
// Window Class
//==============================================================================

/**
 * @brief Class representing the window in which the OpenGL context may render.
 * @detail Creates an SDL Window and an OpenGL context. The Window class 
 * manages all drawing, and is interfaced via the repaint() method.
 */
class Window : public Configurable {

private:

	/**
	 * The width of the window in pixels.
	 */
	GLint width;

	/**
	 * The height of the window in pixels.
	 */
	GLint height;

	/**
	 * Current aspect ratio of the window.
	 */
	GLfloat aspectRatio;

	/**
	 * Pointer to the SDL Window.
	 */
	SDL_Window* sdlWindow;
	
	/**
	 * Pointer to the GL Context.
	 */
	SDL_GLContext context;

protected:

	/**
	 * @brief Sets the initial values of variables.
	 * @details After being initialized, this function allows the initial values
	 * of variables to be set.
	 * @return Error code indicating success or cause of failure.
	 */
	ErrorCode configureVariables();

public:

	// Constructor.
	Window(const std::string& title, const char* configFile);

	// Destructor.
	~Window();

	// Update the size of the viewport on window resize.
	void updateViewport();

	// Maximnize the display on the current screen.
	void maximize();

	// Repaint the graphics.
	void repaint(const Camera& camera, const std::vector<Drawable*>& drawables);
	
	// Getters.
	GLushort getDisplayDimension(Dimension d);
	GLushort getWidth() const;
	GLushort getHeight() const;

	// Setters.
	void setClearColor(GLclampf r, GLclampf b, GLclampf g, GLclampf a);


};

#endif
