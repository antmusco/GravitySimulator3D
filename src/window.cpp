//==============================================================================
// Header Files 
//==============================================================================

#include <cstdlib>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL/SDL_video.h>

#include "utilities.h"
#include "enums.h"
#include "mesh.h"

#include "window.h"

//==============================================================================
// Window Class
//==============================================================================

/**
 * @brief Sets the initial values of all member variables for the Window.
 * @details Uses the \c variablesNames and \c variablesValues vectors set 
 * 		during the Configurable constructor.
 * @return ErrorCode indicating success or cause of failure.
 */
ErrorCode Window::configureVariables() {

	// Initialize the member variable values.
	for(auto it = initialValuesMap.begin(); it != initialValuesMap.end(); 
		++it) {

		if(it->first == "width") {

			width = atoi(it->second.c_str());

		} else if (it->first == "height") {

			height = atoi(it->second.c_str());

		}

	}

	return ErrorCode::Success;

}



/**
 * @brief Constructor for the Window class.
 * @details creates the window and the GL context using SDL (Simple Directmedia
 * Layer) and initializes GLEW. GLEW (GL Extension Wrangler Library) "binds" 
 * all of OpenGL's functions to the hardware-specific implementation (OpenGL 
 * acts as an Adapter Class) 
 * 
 * @param title The title of the window to be displayed in the toolbar.
 */
Window::Window(const std::string& title, const char* configFile) :
	Configurable(configFile),
	width(100),
	height(100),
	aspectRatio(1),
	sdlWindow(NULL),
	context(0) {

 	// Configure variables before proceeding.
	configureVariables();

	// Create the SDL window.
	uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	// Create the SDL window.
	sdlWindow = SDL_CreateWindow(title.c_str(), 0, 0, width, height, flags);

	// Create the SDL GL context.
	context = SDL_GL_CreateContext(sdlWindow);


	// Initialize GLEW (binds all of OpenGL's functions to the hardware).
	GLenum status = glewInit();

	// Validate GLEW initialized correctly.
	if (status != GLEW_OK) {

		//If the functions were not bound, output error.
		std::cerr << "Glew failed to initialize!" << std::endl;
		std::cerr << "Status: " << glewGetErrorString(status) << std::endl;

	}

	// Show the version of GLEW currently being used.
	std::cout << "Stats: Using GLEW " << glewGetString(GLEW_VERSION) 
			  << std::endl;
	
	// Set the clear color and clear the window.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Update the viewport.
	updateViewport();

}



/**
 * @brief Destructor for the Display class.
 * @details Deletes the OpenGL context and destroys the SDL Window.
 */
Window::~Window() {

	// Delete the GL context.
	SDL_GL_DeleteContext(context);

	// Destroy the window.
	SDL_DestroyWindow(sdlWindow);

}



/**
 * Function which gets the width and height of the window and updates the
 * aspect ratio, the glViewwport, and the viewToPorojectionMatrix for the
 * display.
 */
void Window::updateViewport() {

	//Get the width and height of the window and calculate aspect ratio.
	SDL_GetWindowSize(sdlWindow, &width, &height);
	aspectRatio = (GLfloat)width / height;

	//Update the GLviewport.
	glViewport(0, 0, width, height);

}



/**
 * @brief Maximizes the Window.
 * @details Matches the height and width of the Window to the width and height
 * of the display.
 */
void Window::maximize() {

	//Maximize the window.
	SDL_MaximizeWindow(sdlWindow);

}



/**
 * @brief Clears the display and re-paints the desired components.
 * @details Uses several GL draw calls to render to the Window.
 */
void Window::repaint(const Camera& camera, const std::vector<Drawable*>& 
	drawables) {

	//Tell OpenGL to clear the color buffer and depth buffer.
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	

	//Get the window dimensions and update the viewport.
	updateViewport();

	// Enable depth culling.
	glEnable(GL_DEPTH_TEST);

	glm::mat4 worldToProjection = camera.getWorldToProjectionMatrix(width, 
		height);

	//printMat4(worldToProjection);

	//Draw 3-D space.
	for (auto it = drawables.begin(); it != drawables.end(); ++it) {

		(*it)->draw(worldToProjection);

	}

	//Swap the double buffer.
	SDL_GL_SwapWindow(sdlWindow);

}



/**
 * @brief Convenience method to get the indicated dimension of the display.
 * @details Gets the indicated Dimension of the display and returns it to the 
 * caller.
 * 
 * @param d Dimension indicating what the user is looking for.
 * @return The dimension of the display.
 */
GLushort Window::getDisplayDimension(Dimension d) {

	//Place the dimensions of the screen in an SDL_Rect struct.
	SDL_Rect display;
	SDL_GetDisplayBounds(0, &display);
	
	//Return the indicated Dimension.
	switch (d) {

		case Dimension::Height:
			return display.h;

		case Dimension::Width:
			return display.w;

		default:
			return 0;

	}

}



/**
 * @brief Public getter method for the width of the Window
 * @details Returns the width of the Window.
 * @return The width of the Window.
 */
GLushort Window::getWidth() const {

	return width;

}



/**
 * @brief Public getter method for the height of the Window.
 * @details Returns the height of the Window.
 * @return The height of the Window.
 */
GLushort Window::getHeight() const {

	return height;

}



/**
 * @brief Sets the clear color for the Display.
 * @details Takes the red, blue, green, and alpha values for the default clear
 * color and sets it for the Display.
 * 
 * @param r The red component of the clear color [0.0 - 1.0].
 * @param b The blue component of the clear color [0.0 - 1.0].
 * @param g The green component of the clear color [0.0 - 1.0].
 * @param a The alpha component of the clear color [0.0 - 1.0].
 */
void Window::setClearColor(GLclampf r, GLclampf b, GLclampf g, GLclampf a) {

	glClearColor(r, b, g, a);

}
