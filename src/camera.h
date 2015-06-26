#ifndef _CAMERA_H_
#define _CAMERA_H_

//==============================================================================
// Header Files 
//==============================================================================

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL/SDL.h>

#include "configurable.h"
#include "enums.h"

//==============================================================================
// Constants 
//==============================================================================

#define MIN_NEAR_PLANE 0.0f
#define MAX_FAR_PLANE 10000.0f

//==============================================================================
// Camera Class
//==============================================================================

/**
 * @brief Class representing the camera observing a 3-D space.
 * @detail The camera consists of a position, a view direction, an up direction,
 * and several movement parameters. The camera abstracts the world to view
 * projection process and allows the user to easily move aroun the 3-D space.
 */
class Camera : public Configurable {

private:

	/**
	 * The close limit for the Camera view.
	 */
	GLfloat nearPlane;

	/**
	 * The depth limit for the Camera view.
	 */
	GLfloat farPlane;
	
	/**
	 * The angle in radians describing the Camera's field of view.
	 */
	GLfloat fieldOfView;

	/**
	 * Position vector of the Camera.
	 */
	glm::vec3 position;

	/**
	 * Unit vector indicating the direction the Camera is pointed.
	 */
	glm::vec3 viewDirection;

	/**
	 * Unit vector pointing in the up direction.
	 */
	glm::vec3 upDirection;

	/**
	 * Unit vector pointing perpendicular to the view and the up direction.
	 */
	glm::vec3 sideDirection;

	/**
	 * Scaling factor for Camera rotation.
	 */
	GLfloat rotateSpeed;

	/**
	 * Scaling factor for Camera translation.
	 */
	GLfloat translateSpeed;

	/**
	 * Maximum amount of movement for the Camera in one update.
	 */
	GLfloat maxMovement;

	/**
	 * Matrix representing the projection of the view space onto the projection
	 * space (Camera lens).
	 */
	glm::mat4 viewToProjection;

	// Inline function which updates the sideDirection member variable.
	inline void updateSideDirection();

protected:

	// Configurable interface.
	ErrorCode configureVariables();

public:

	// Constructor.
	Camera(const char* _configFile);


	// Respond to moouse input.
	void rotateViewDirection(GLfloat deltaX, GLfloat deltaY);

	// Move the camera.
	void step(Direction dir);

	// Getters.
	GLfloat getNearPlane() const;
	GLfloat getFarPlane() const;
	GLfloat getFieldOfView() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getViewDirection() const;
	const glm::vec3& getUpDirection() const;
	glm::mat4 getWorldToViewMatrix() const;
	glm::mat4 getViewToProjectionMatrix(GLfloat width, GLfloat height) const;
	glm::mat4 getWorldToProjectionMatrix(GLfloat width, GLfloat height) const;
	// Setters.
	void setNearPlane(GLfloat _nearPlane);
	void setFarPlane(GLfloat _farPlane);
	void setFieldOfView(GLfloat _fieldOfView);
    void setPosition(const glm::vec3& _position);
	void setViewDirection(const glm::vec3& _viewDirection);
	void setUpDirection(const glm::vec3& _upDirection);

};

#endif
