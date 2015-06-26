//==============================================================================
// Header Files 
//==============================================================================

#include <iostream>

#include <tinyxml2.h>
#include <rapidxml/rapidxml.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "orbitalsystem.h"

//==============================================================================
// OrbitalSystem Class 
//==============================================================================

/**
 * @brief Default constructor for the OrbitalSystem class.
 * @details Initializes the member variables to their default values.
 */
OrbitalSystem::OrbitalSystem() :

	G(0.0f), 
	clock(0.0f), 
	scale(1.0f),
	bodies(),
	celestialSphere(nullptr) {

	// Empty.

}

OrbitalSystem::OrbitalSystem(const char* objFile, const char* textureFile,
	GLfloat starsScale) :

	G(DEFAULT_G),
	clock(0),
	scale(1),
	bodies(),
	celestialSphere(new OrbitalBody("Celestial Sphere", 0.0f, starsScale, 
		objFile, textureFile, glm::vec3(0), glm::vec3(0), 0.0f, 0.0f)) {

	// Empty.

}



/**
 * Add an OrbitalBody to the OrbitalSystem.
 *
 * \param body A pointer to the OrbitalBody to add to the system.
 */
void OrbitalSystem::addBody(OrbitalBody* body) {

	// Add the pointer, mesh, and transformation.
	bodies.push_back(body);

}



/**
 * Remove an OrbitalBody from the system.
 *
 * \param i The index of the OrbitalBody to be removed.
 */
void OrbitalSystem::removeBody(const GLuint i) {

	bodies.erase(bodies.begin() + i);

}

/**
 * Calculates the vector for gravity felt by an OrbitalBody given all of the
 * OrbitalBody objects in the system. 
 *
 * \param subject The body for which the gravity vector is being calculated.
 * \param position The position of the subject in world coordinates.
 *
 * \returns A vector representing the direction and magnitude of gravity felt 
 * 		by the subject body.
 */
glm::vec3 OrbitalSystem::gravityVector(OrbitalBody* subject, glm::vec3 position) {

	// Initialize netGravity and direction to 0-vectors.
	glm::vec3 netGravity(0);
	glm::vec3 direction(0);

	// Calculate attraction to all bodies.
	for(OrbitalBody* body : bodies)
	{
		// Do not compare subject with itself.
		if(body != subject) {

			// Get the displacement vector.
			direction = body->getLinearPosition() - position;

			// Get the magnitude of the displacement vector.
			GLfloat radius = glm::length(direction);
			direction /= radius;
			
			// Get the magnitude of the force of gravity
			//  -> magnitude = G * m / r^2
			GLfloat magnitude = (G * body->getMass()) / (radius * radius);
			
			// Calculate gravity and apply to body.
			netGravity += magnitude * direction;

		}

	}

	// Return gravity vector.
	return netGravity;

}


glm::vec3 OrbitalSystem::A(OrbitalBody* subject, const glm::vec3 position, float dt) {

	// Calculate the force of gravity the the body's new position.
	glm::vec3 netAcceleration = gravityVector(subject, position);

	// Account for thrust and return net Acceleration.

	return netAcceleration;
	//return netAcceleration += dt * subject->getLinearThrust();

}

void OrbitalSystem::rungeKuttaApprx(OrbitalBody* subject, const GLfloat dt) {

	const GLuint order = 4;
	const GLfloat c = 1.0f / 6.0f;

	glm::vec3 k[order];
	glm::vec3 l[order];
	glm::vec3 r = subject->getLinearPosition();
	glm::vec3 v = subject->getLinearVelocity();
			     	  
	k[0]  = dt * v;
	l[0]  = dt * A(subject, r, 0);
	k[1]  = dt * (v + (0.5f * l[0]));
	l[1]  = dt * A(subject, r + (0.5f * k[0]), (0.5f * dt));
	k[2]  = dt * (v + (0.5f * l[1]));
	l[2]  = dt * A(subject, r + (0.5f * k[1]), (0.5f * dt));
	k[3]  = dt * (v + l[2]);
	l[3]  = dt * A(subject, r + k[2], dt);

	r += c * (k[0] + k[1] + k[2] + k[3]);
	v += c * (l[0] + l[1] + l[2] + l[3]);

	subject->setLinearPosition(r);
	subject->setLinearVelocity(v);
	subject->setGravityVector(gravityVector(subject, r));
	subject->setAngularPosition(subject->getAngularPosition() + subject->getAngularVelocity() * dt);

}

/* Delta t is in real-time seconds. */
void OrbitalSystem::interpolate(GLfloat realSeconds) {

	// Convert from real time to game time.
	GLfloat dt = (GLfloat) (realSeconds * SIM_SECONDS_PER_REAL_SECOND);

	// Add the time to the global clock.
	clock += dt;

	// Use Runge-Katta approximation to update the state vectors.
	for(OrbitalBody* subject : bodies) 
		rungeKuttaApprx(subject, dt);
	
}



OrbitalSystem OrbitalSystem::loadFile(const char* xmlFile) {

	OrbitalSystem newSystem;

	tinyxml2::XMLDocument doc;
	if(!doc.LoadFile(xmlFile)) {

		// Get the root, background, and bodies elements of the document. */
		tinyxml2::XMLElement* root = doc.RootElement();
		tinyxml2::XMLElement* background = root->FirstChildElement("background");
		tinyxml2::XMLElement* bodies = root->FirstChildElement("bodies");

		// Parse the root parameters of the system. */
		const char* g_str          = root->FirstChildElement("g")->GetText();
		GLfloat     g_float        = (GLfloat) atof(g_str);
		const char* scale_str      = root->FirstChildElement("scale")->GetText();
		GLfloat     scale_float    = (GLfloat) atof(scale_str);

		// Set the root parameters of the system. */
		newSystem.scale            = scale_float;
		newSystem.G                = g_float / scale_float;

		// Parse the background parameters of the system. */
		const char* bgMeshFile_str = background->FirstChildElement("meshFile")->GetText();
		const char* bgTextFile_str = background->FirstChildElement("textureFile")->GetText();
		const char* bgRadius_str   = background->FirstChildElement("radius")->GetText();
		GLfloat     bgRadius_float = (GLfloat) atof(bgRadius_str);
		const char* bgTilt_str     = background->FirstChildElement("tilt")->GetText();
		GLfloat     bgTilt_float   = (GLfloat) atof(bgTilt_str);

		// Set the background parameters of the system. */
		newSystem.celestialSphere = new OrbitalBody("Celestial Sphere", 0.0f, 
			bgRadius_float, bgMeshFile_str, bgTextFile_str,	glm::vec3(0), 
			glm::vec3(0), bgTilt_float, 0.0f);
	
		// Parse each body of the system. */
		for(tinyxml2::XMLElement* body = bodies->FirstChildElement("body"); body != NULL; body = body->NextSiblingElement("body"))
		{
			// Parse the body parameters for each body. */
			const char* bodyName_str       = body->FirstChildElement("name")->GetText();
			const char* bodyMass_str       = body->FirstChildElement("mass")->GetText();
			GLfloat     bodyMass_float     = (GLfloat) atof(bodyMass_str);
			const char* bodyRadius_str     = body->FirstChildElement("radius")->GetText();
			GLfloat     bodyRadius_float   = (GLfloat) atof(bodyRadius_str);
			const char* bodyMeshFile_str   = body->FirstChildElement("meshFile")->GetText();
			const char* bodyTextFile_str   = body->FirstChildElement("textureFile")->GetText();
			const char* bodyPosX_str       = body->FirstChildElement("position")->FirstChildElement("x")->GetText();
			GLfloat     bodyPosX_float     = (GLfloat) atof(bodyPosX_str);  
			const char* bodyPosY_str       = body->FirstChildElement("position")->FirstChildElement("y")->GetText();
			GLfloat     bodyPosY_float     = (GLfloat) atof(bodyPosY_str);  
			const char* bodyPosZ_str       = body->FirstChildElement("position")->FirstChildElement("z")->GetText();
			GLfloat     bodyPosZ_float     = (GLfloat) atof(bodyPosZ_str);  
			const char* bodyVelX_str       = body->FirstChildElement("velocity")->FirstChildElement("x")->GetText();
			GLfloat     bodyVelX_float     = (GLfloat) atof(bodyVelX_str);  
			const char* bodyVelY_str       = body->FirstChildElement("velocity")->FirstChildElement("y")->GetText();
			GLfloat     bodyVelY_float     = (GLfloat) atof(bodyVelY_str);  
			const char* bodyVelZ_str       = body->FirstChildElement("velocity")->FirstChildElement("z")->GetText();
			GLfloat     bodyVelZ_float     = (GLfloat) atof(bodyVelZ_str);  
			const char* bodyTilt_str       = body->FirstChildElement("tilt")->GetText();
			GLfloat     bodyTilt_float     = (GLfloat) atof(bodyTilt_str);  
			const char* bodyRotSpeed_str   = body->FirstChildElement("rotationalSpeed")->GetText();
			GLfloat     bodyRotSpeed_float = (GLfloat) atof(bodyRotSpeed_str);

			// Set the body parameters for each body, and add the body to the system. */
			glm::vec3   bodyPos_vec{bodyPosX_float, bodyPosY_float, bodyPosZ_float};
			glm::vec3   bodyVel_vec{bodyVelX_float, bodyVelY_float, bodyVelZ_float};
			OrbitalBody* newBody = new OrbitalBody(bodyName_str,
			                                 bodyMass_float / scale_float, 
			                                 bodyRadius_float / scale_float,
			                                 bodyMeshFile_str, 
			                                 bodyTextFile_str, 
			                                 bodyPos_vec / scale_float, 
			                                 bodyVel_vec / (GLfloat) sqrt(scale_float),
			                                 bodyTilt_float,
			                                 bodyRotSpeed_float);

			newSystem.addBody(newBody);

		}

	}
	
	// Return the system. */
	return newSystem;

}

/**
 * Cleans up the orbital system by deleting all bodies and meshes from the
 * graphics hardware and main memory.
 */
void OrbitalSystem::cleanUp() {

	// Clean up the celestial sphere
	delete celestialSphere;
	celestialSphere = nullptr;


	for(GLuint i = 0; i < bodies.size(); ++i) {

		delete bodies[i];
		bodies[i] = nullptr;
	}

}

//==============================================================================
// Getter Functions 
//==============================================================================

/**
 * Public getter method for the gravitational constant. 
 *
 * \returns The value of the gravitational constant.
 */
GLfloat OrbitalSystem::getG() const {  

	return G;

}

/**
 * Public getter method for the simulation time. 
 *
 * \returns The value of the simulation time.
 */
GLfloat OrbitalSystem::t() const {  

	return clock;

}

OrbitalBody* OrbitalSystem::getBody(GLuint i) {

	return bodies.at(i);

}

Mesh* OrbitalSystem::getCelestialSphere() const {

	return celestialSphere;

}



/**
 * @brief Get all of the drawable objects in this OrbitalSystem.
 * @details Adds all of the Drawable objects in this OrbitalSystem to a
 * vector and returns it to the caller.
 * @return A vector of all the Drawable objects in this OrbitalSystem.
 */
std::vector<Drawable*> OrbitalSystem::getDrawables() {

	// Create the vector to return.
	std::vector<Drawable*> drawables;

	// Add the celestial sphere.
	drawables.push_back(celestialSphere);

	// Add all of the orbital bodies.
	for(auto it = bodies.begin(); it != bodies.end(); ++it) {

		drawables.push_back(*it);

	}

	return drawables;

}

