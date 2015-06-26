//==============================================================================
// Header Files 
//==============================================================================

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "enums.h"
#include "eventmanager.h"

//==============================================================================
// EventManager Class. 
//==============================================================================

/**
 * @brief Public constructor for the event manager.
 * @details Initializes the camera object and warp factor for the EventManager.
 * 
 * @param camera Reference to the Camera used in the program.
 * @param warpFactor Reference to the warp factor used in the program.
 */
EventManager::EventManager(Camera& _camera, GLfloat& _warpFactor, bool& _quit) :
	state{0, 0, false, false},
	camera(_camera),
	warpFactor(_warpFactor),
	quit(_quit) {

	// Empty.

}



/**
 * @brief Handle an SDL Event.
 * @details Primary event handling function for the application. All SDL events
 * are sent here, which then routes the events to specific subroutines to
 * handle the specific action required. 
 * 
 * @param event The SDL event to be handled.
 */
void EventManager::handleSDLEvent(SDL_Event* event) {

	if (event->type == SDL_MOUSEMOTION) {

		handleMouseMotion(event->motion.x, event->motion.y);
	
	} else if (event->type == SDL_KEYDOWN) {

		handleKeyPress(event->key.keysym.scancode);
	
	}

}



void EventManager::handleMouseMotion(GLint newX, GLint newY) {

	GLint deltaX = newX - state.mouseX;
	GLint deltaY = newY - state.mouseY;

	camera.rotateViewDirection(deltaX, deltaY);

	state.mouseX = newX;
	state.mouseY = newY;

}



/**
 * @brief Handle a key press event.
 * @details Handles a key press event, typically indicating some type of camera
 * interaction.
 * 
 * @param key The SDL_Scancode indicating the key which was pressed.
 */
void EventManager::handleKeyPress(SDL_Scancode key) {

	switch (key) {

		// Speed Up.
		case SDL_SCANCODE_T:

			if(warpFactor < MAX_WARP) {

				warpFactor *= WARP_SCALE;
			
			} else {

				std::cout << "Max warp achieved: " << MAX_WARP << "x" 
						  << std::endl;

			}

			break;
		
		// Slow Down.
		case SDL_SCANCODE_R:

			if(warpFactor > MIN_WARP) {

				warpFactor /= WARP_SCALE;
			
			} else {

				std::cout << "Min warp achieved: " << MIN_WARP << "x" 
						  << std::endl;

			}
			break;

		// Strafe Right.
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:

			camera.step(Direction::Right);
			break;

		// Strafe Left.
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:

			camera.step(Direction::Left);
			break;

		// Step Forward.
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:

			camera.step(Direction::Forward);
			break;

		// Step Backward.
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:

			camera.step(Direction::Backward);
			break;

		// Step Down.
		case SDL_SCANCODE_Z:

			camera.step(Direction::Down);
			break;

		// Step Up.
		case SDL_SCANCODE_X:

			camera.step(Direction::Up);
			break;

		// Quit the program.
		case  SDL_SCANCODE_ESCAPE:
			
			quit = true;
			break;

		default:
		
			// Silently ignore all other keys.
			break;

	}

}

//==============================================================================
// Getters. 
//==============================================================================

/**
 * @brief Public getter method for the EventManager's camera.
 * @details Returns a constant reference to the camera used by this EventManager.
 * @return A constant reference to the camera used by this EventManager.
 */
const Camera& EventManager::getCamera() {

	return camera;

}

//==============================================================================
// Setters. 
//==============================================================================

/**
 * @brief Public setter method for the EventManager's camera.
 * @details Sets the camera member to the indicated reference.
 * 
 * @param _camera Reference to a camera object to be used by this EventManager.
 */
void EventManager::setCamera(Camera& _camera) {

	camera = _camera;

}