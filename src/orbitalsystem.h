#ifndef _ORBITAL_SYSTEM_H_
#define	_ORBITAL_SYSTEM_H_

//==============================================================================
// Header Files 
//==============================================================================

#include <string>
#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "drawable.h"
#include "orbitalbody.h"
#include "mesh.h"

//==============================================================================
// Constants 
//==============================================================================

#define   SIM_SECONDS_PER_REAL_SECOND                            1.0f
#define   SECONDS_PER_HOUR                                    3600.0f
#define   MAX_DELTA_T                                          100.0f                
#define   DEFAULT_G                                      6.67384e-20f
#define   DEFAULT_TILT_AXIS            glm::vec3{+1.0f, +0.0f, +0.0f}


//==============================================================================
// OrbitalSystem Class 
//==============================================================================

/**
 * Class representing an Orbital System, which is a collection of orbital
 * bodies. This system manages the interactions between bodies via gravity
 * and collisions. This class deines several orbital system constants used
 * by the orbital bodies to simulate physics.
 */
class OrbitalSystem {

private:
	
	// Private default constructor (used for loading xml file).
	OrbitalSystem();

protected:

	/**
	 * Gravitational constant G.
	 */
	GLfloat G;

	/**
	 * Global time of the system.
	 */
	GLfloat clock;

	/**
	 * Scale of the system.
	 */
	GLfloat scale;

	/**
	 * Vector of massive bodies.
	 */
	std::vector<OrbitalBody*> bodies;

	/**
	 * Mesh representing the surrounding celestial sphere (stars).
	 */
	OrbitalBody* celestialSphere;

public:

	// Constructor.
	OrbitalSystem(const char* objFile, const char* textureFile, GLfloat
		starsScale);

	// Load an orbital system from a file.
	static OrbitalSystem loadFile(const char* xmlFile);

	// Add a body to the system.
	void addBody(OrbitalBody* body);
	
	// Remove a body from the system given its name.
	void removeBody(const GLuint i);
	
	// Adjust the gravity vector for each body in the system.
	void compute();
	// Update the system by incrementing the time until seconds have passed.
	void interpolate(const GLfloat seconds);
	
	// Calculate the gravitational forces felt by each body.
	glm::vec3 gravityVector(OrbitalBody* subject, glm::vec3 position);

	// Calculate the second derivative at the specified point.
	glm::vec3 A(OrbitalBody* subject, const glm::vec3 position,const GLfloat dt);
	
	// Approximation of the change in variables using Runge-Katta method.
	void rungeKuttaApprx(OrbitalBody* subject, const GLfloat t);

	// Remove all of the allocated space.
	void cleanUp();

	// Getters.
	GLfloat getG() const;
	
	GLfloat t() const;

	OrbitalBody* getBody(GLuint i);
	
	Mesh* getCelestialSphere() const;

	std::vector<Drawable*> getDrawables();

};

#endif
