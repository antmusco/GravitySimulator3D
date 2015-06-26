
//==============================================================================
// Header Files 
//==============================================================================

#include <iostream>
#include <string>
#include <ctime>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "oscompat.h"
#include "window.h"
#include "shader.h"
#include "shadersingleton.h"
#include "mesh.h"
#include "camera.h"
#include "eventmanager.h"
#include "orbitalbody.h"
#include "orbitalsystem.h"

//==============================================================================
// Constants.
//==============================================================================

#define  FRAMES_PER_SECOND    40
#define  PROJECT_TITLE        "GravitySimulator3D"
#define  PRINT(a)             std::cout << a << std::endl;

//==============================================================================
// Global variables.
//==============================================================================


/**
 * Beginning point of the program.
 *
 * \param argc The number of command line strings.
 * \param argv The array of command line stirngs.
 * 
 * \returns 0 on success, any non-zero value on failure.
 */
int main(int argc, char* argv[]) {

	// Speed of the simulation. 
	GLfloat warpFactor = + 8.000e2f;
	bool quit = false;

	// Initialize SDL with all subsystems. 
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create the window.
	Window window(std::string(PROJECT_TITLE), "window.config");

	// create the camera.
	Camera camera("camera.config");
	
	// Create the EventManager.
	EventManager eventManager(camera, warpFactor, quit);
	
	// Create the ShaderSingleton.
	ShaderSingleton::createShaderSingleton("shaders.config");

	// Create the orbital system. 
	OrbitalSystem system = OrbitalSystem::loadFile("res/data/system.xml");

	// Get the list of Drawables.
	std::vector<Drawable*> drawables = system.getDrawables();

	// Instantiate the event reference. 
	SDL_Event event;
	SDL_PollEvent(&event);	

	// Begin the milliseconds counter. 
	GLuint microsPerFrame = (GLuint) ((1.0 / FRAMES_PER_SECOND) *
		MICROS_PER_SECOND);

	uint64 micros= 0;
	uint64 frameStart = getMicros();

	// Main loop. 
	while (event.type != SDL_QUIT) {

		// Handle the new event. 
		eventManager.handleSDLEvent(&event);
		//GLfloat interpolateSeconds = (warpFactor * (getMicros() - micros)) / 
		//	MICROS_PER_SECOND;


		// Interpolate the system over the interval. 
		//system.interpolate(interpolateSeconds);

		// If a new frame is to be drawn, update the window. 
		if ((micros - frameStart) >= microsPerFrame) {

			window.repaint(camera, drawables);
			frameStart = getMicros();
		
		}

		// Update the temporary millisecond counter. 
		micros = getMicros();
		
		// Get the next event. 
		SDL_PollEvent(&event);
		
	}

	// Free the shapes. 
	system.cleanUp();

	ShaderSingleton::destroyShaderSingleton();

	// Quit using SDL. 
	SDL_Quit();

	// Exit Success. 
	return 0;
}
