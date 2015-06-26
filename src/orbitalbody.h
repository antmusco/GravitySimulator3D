#ifndef _ORBITAL_BODY_H_
#define	_ORBITAL_BODY_H_

//==============================================================================
// Header Files 
//==============================================================================

#include  <iostream>
#include  <string>
#include  <math.h>

#include  <glm/glm.hpp>
#include  <glm/gtc/matrix_transform.hpp>
#include  <glm/gtx/vector_angle.hpp>
#include  <glm/gtx/rotate_vector.hpp>

#include "drawable.h"
#include  "mesh.h"

//==============================================================================
// Constants 
//==============================================================================

#define   DEGREES_PER_REV    360
#define   DEFAULT_ROT_AXIS   glm::vec3(+0.0f, +1.0f, +0.0f)
#define   RAD_TO_DEG         (180.0f / M_PI)
#define   DEG_TO_RAD         (M_PI / 180.0f)

//==============================================================================
// OrbitalBody Class 
//==============================================================================

/**
 * @brief Class representing an OrbitalBody
 * @details Class representing an OrbitalBody, which is any object having mass
 * and volume. Orbital bodies are part of an orbital system, which is a 
 * collection of bodies that influence each other via gravity. Orbital bodies 
 * can have linear and rotational postitions, velocities, accelerations, and
 * thrusts, which may be altered by outside forces.
 */
class OrbitalBody : public Mesh, public Drawable {

protected:

	/**
	 * Name of the body. 
	 */
	std::string name;

	/**
	 * Bounding radius of the orbital body. 
	 */
	GLfloat radius;

	/**
	 * Scale of x, y, and z dimensions of the body. 
	 */
	glm::vec3 scale;

	/**
	 * Mass of the body. 
	 */
	GLfloat mass;

	/**
	 * Force of gravity felt by the body. 
	 */
	glm::vec3 gravityVector;

	/**
	 * Position of the body in 3-D space.
	 */
	glm::vec3 linearPosition;

	/**
	 * Velocity vector of the body in METERS PER SECOND. 
	 */
	glm::vec3 linearVelocity;

	/**
	 * Linear acceleration vector on the body. 
	 */
	glm::vec3 linearAccel;

	/**
	 * Linear thrust vector on the body. 
	 */
	glm::vec3 linearThrust;

	/**
	 * Axis of rotation of the body. 
	 */
	glm::vec3 rotationalAxis;

	/**
	 * Angle offset from the default axis of rotation. 
	 */
	GLfloat rotationalAngle;

	/**
	 * Angular offset of the body. 
	 */
	GLfloat angularPosition;

	/**
	 * Angular velocity of the body in RADIANS PER SECOND. 
	 */
	GLfloat angularVelocity;

	/**
	 * Angular acceleration vector on the body. 
	 */
	GLfloat angularAccel;

	/**
	 * Angular thrust vector on the body. 
	 */
	GLfloat angularThrust;

	/**
	 * Mesh transformation data. 
	 */
	glm::mat4 modelToWorld;

public:

	// Default Constructor.
	OrbitalBody(const char* _name, GLfloat _mass, GLfloat _radius, const char* 
		_objFile, const char* _textFile, const glm::vec3& _linearPosition, 
		const glm::vec3& _linearVelocity, GLfloat _tilt, GLfloat 
		_angularVelocity);

	// Destructor.
	virtual ~OrbitalBody();

	// Drawable Interface.
	virtual void draw(const glm::mat4& worldToProjection);

	// Update the model to world matrix.
	void updateModelToWorld();

	/** 
	 * Step forward in time by dt seconds and calculate the updated values
	 * for the position, velocity, and acceleration of the body.
	 */
	void increment(GLfloat dt);
												  
	// Getters.		
	const std::string& getName() const;
	GLfloat getRadius() const;
	const glm::vec3& getScale() const;
	GLfloat getMass() const;
	const glm::vec3& getGravityVector() const;
	const glm::vec3& getLinearPosition() const;
	const glm::vec3& getLinearVelocity() const;
	const glm::vec3& getLinearAccel() const;
	const glm::vec3& getLinearThrust() const;
	const glm::vec3& getRotationalAxis() const;
	GLfloat getAngularPosition() const;
	GLfloat getAngularVelocity() const;
	GLfloat getAngularAccel() const;
	GLfloat getAngularThrust() const;
	const glm::mat4& getModelToWorld() const;
												  
	// Setters.			
	void setName(const std::string& _name);
	void setRadius(GLfloat _radius);
	void setScale(const glm::vec3& _scale);
	void setMass(GLfloat _mass);
	void setGravityVector(const glm::vec3& _gravity);
	void setLinearPosition(const glm::vec3& _position);
	void setLinearVelocity(const glm::vec3& _velocity);
	void setLinearAccel(const glm::vec3& _acceleration);
	void setLinearThrust(const glm::vec3& _thrust);
	void setRotationalAxis(GLfloat _tilt);
	void setAngularPosition(GLfloat _angularPosition);
	void setAngularVelocity(GLfloat _angularVelocity);
	void setAngularAccel(GLfloat _angularAccel);
	void setAngularThrust(GLfloat _angularThrust);

};

#endif
