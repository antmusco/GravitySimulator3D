#ifndef _EVENT_MANAGER_H_
#define _EVENT_MANAGER_H_

//==============================================================================
// Header Files 
//==============================================================================

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "camera.h"

//==============================================================================
// Constants 
//==============================================================================

#define WARP_SCALE 1.25f
#define MAX_WARP 4.0f
#define MIN_WARP 0.25f

//==============================================================================
// EventManager Class. 
//==============================================================================

/**
 * Class which acts as an event manager for the application. All input and
 * output will be handled by an event manager object.
 */
class EventManager {

public:

	// Constructor
	EventManager(Camera& _camera, GLfloat& _warpFactor, bool& _quit);

	// Handle an SDL Event.
	void handleSDLEvent(SDL_Event* event);

	// Handle a mouse motion event.
	void handleMouseMotion(GLint newX, GLint newY);

	// Handle a key press event.
	void handleKeyPress(SDL_Scancode key);

	// Getters.
	const Camera& getCamera();

	// Setters.
	void setCamera(Camera& _camera);

private:

	/**
	 * Struct conatining the current state of the event manager.
	 */
	struct {

		GLint mouseX;
		GLint mouseY;
		bool  leftMouseButtonDown;
		bool  rightMouseButtonDown;

	} state;
	
	/**
	 * Reference to the camera used in the program.
	 */
	Camera& camera;
	
	/**
	 * Reference to the warp factor used in the program
	 */
	GLfloat& warpFactor;

	/**
	 * Reference to the boolean quit flag used in the program.
	 */
	bool& quit;

};

#endif
