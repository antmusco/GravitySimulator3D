//==============================================================================
// Header Files 
//==============================================================================

#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "camera.h"

//==============================================================================
// Camera Class
//==============================================================================

/**
 * @brief Inline function which updates the sideDirection member variable.
 * @details Convenience function which re-calculates the sideDirection member
 * variable by crossing the view direction with the up direction.
 */
inline void Camera::updateSideDirection() {

	sideDirection = glm::normalize(glm::cross(viewDirection, upDirection));

}



/**
 * @brief Sets the initial values of all member variables for the Camera.
 * @details Uses the \c initialValuesMap set during the Configurable 
 * constructor.
 * @return ErrorCode indicating success or cause of failure.
 */
ErrorCode Camera::configureVariables() {

	// Initialize the member variable values.
	for(auto it = initialValuesMap.begin(); it != initialValuesMap.end(); 
		++it) {

		if(it->first == "nearPlane") {

			nearPlane = atof(it->second.c_str());

		} else if (it->first == "farPlane") {

			farPlane = atof(it->second.c_str());

		} else if (it->first == "fieldOfView") {

			fieldOfView = atof(it->second.c_str());

		} else if (it->first == "position.x") {

			position.x = atof(it->second.c_str());

		} else if (it->first == "position.y") {

			position.y = atof(it->second.c_str());

		} else if (it->first == "position.z") {

			position.z = atof(it->second.c_str());

		} else if (it->first == "viewDirection.x") {

			viewDirection.x = atof(it->second.c_str());

		} else if (it->first == "viewDirection.y") {

			viewDirection.y = atof(it->second.c_str());

		} else if (it->first == "viewDirection.z") {

			viewDirection.z = atof(it->second.c_str());

		} else if (it->first == "upDirection.x") {

			upDirection.x = atof(it->second.c_str());

		} else if (it->first == "upDirection.y") {

			upDirection.y = atof(it->second.c_str());

		} else if (it->first == "upDirection.z") {

			upDirection.z = atof(it->second.c_str());

		} else if (it->first == "rotateSpeed") {

			rotateSpeed = atof(it->second.c_str());

		} else if (it->first == "translateSpeed") {

			translateSpeed = atof(it->second.c_str());

		} else if (it->first == "maxMovement") {

			maxMovement = atof(it->second.c_str());

		} 

	}

	// Initialize the vectors.
	viewDirection = glm::normalize(viewDirection);
	upDirection = glm::normalize(upDirection);
	sideDirection = glm::normalize(glm::cross(viewDirection, upDirection));

	return ErrorCode::Success;

}



/**
 * @brief Public constructor for the Camera class.
 * @details Initializes all member variables to their default values.
 */
Camera::Camera(const char* _configFile) :
	Configurable(_configFile) {

	// Configure the variables.
	configureVariables();

}



/**
 * @brief Rotates the camera by the indicated amount.
 * @details Given a cange in pixel coordinates, rotate the camera and update 
 * the view.
 * 
 * @param deltaX The change in pixels horizontally.
 * @param deltaY The change in pixels vertically.
 */
void Camera::rotateViewDirection(GLfloat deltaX, GLfloat deltaY) {

	// If the mouse moved greater than maxMovement pixels, don't move 
	if (sqrt(deltaX * deltaX + deltaY * deltaY) < maxMovement) {

		// Rotate vertically, then rotate horizontally.
		glm::mat4 rotator = glm::rotate(deltaX * rotateSpeed, -upDirection) *
			glm::rotate(deltaY * rotateSpeed, -sideDirection);

		// Set the new view direction.
		viewDirection = glm::normalize(glm::mat3(rotator) * viewDirection);

		// Determine the new side direction.
		updateSideDirection();

	}

}



/**
 * @brief Translates the camera incrementally in the indicated Direction.
 * @details Uses the /c translateSpeed member variable to determine how large
 * of a step the camera will take in the inidcated Direction.
 * 
 * @param dir Direction enum indicating where the camera will be translated.
 */
void Camera::step(Direction dir) {

	// Determine which direction the camera is moving.
	switch(dir) {

		case Direction::Forward:

			position += translateSpeed * viewDirection;
			break;

		case Direction::Backward:

			position -= translateSpeed * viewDirection;
			break;

		case Direction::Left:

			position -= translateSpeed * sideDirection;
			break;

		case Direction::Right:

			position += translateSpeed * sideDirection;
			break;

		case Direction::Up:

			position += translateSpeed * upDirection;
			break;

		case Direction::Down:

			position -= translateSpeed * upDirection;
			break;

	}

}

//==============================================================================
// Getters. 
//==============================================================================

/**
 * @brief Public getter method for the nearPlane member variable
 * @details Returns a constant reference to the nearPlane member variable.
 * @return The value of the nearPlane member variable.
 */
GLfloat Camera::getNearPlane() const {

	return nearPlane;

}



/**
 * @brief Public getter method for the farPlane member variable
 * @details Returns a constant reference to the farPlane member variable.
 * @return The value of the farPlane member variable.
 */
GLfloat Camera::getFarPlane() const {

	return farPlane;

}



/**
 * @brief Public getter method for the fieldOfView member variable
 * @details Returns the value of the fieldOfView member variable.
 * @return The value of the fieldOfView member variable.
 */
GLfloat Camera::getFieldOfView() const {

	return fieldOfView;

}



/**
 * @brief Public getter method for the position member variable
 * @details Returns a constant reference to the position member variable.
 * @return A constant reference to the position member variable.
 */
const glm::vec3& Camera::getPosition() const {

	return position;

}



/**
 * @brief Public getter method for the viewDirection member variable
 * @details Returns a constant reference to the viewDirection member variable.
 * @return A constant reference to the viewDirection member variable.
 */
const glm::vec3& Camera::getViewDirection() const {

	return viewDirection;

}



/**
 * @brief Public getter method for the upDirection member variable
 * @details Returns a constant reference to the upDirection member variable.
 * @return A constant reference to the upDirection member variable.
 */
const glm::vec3& Camera::getUpDirection() const {

	return upDirection;

}



/**
 * @brief Generate the World-to-View Matrix.
 * @details Generates the matrix representing the World-to-View matrix, which 
 * is dependent on the Camera position, view direction, and up direction.
 * 
 * @return A 4 x 4 matrix representing the World-to-View matrix.
 */
glm::mat4 Camera::getWorldToViewMatrix() const {

	// Look in the current direction.
	return glm::lookAt(position, position + viewDirection, upDirection);

}



/**
 * @brief Generate the View-to-Projection Matrix.
 * @details Generates the matrix representing the View-to-Projection Matrix
 * transformation. This is dependent on the Camnera's fieldOfView, nearPlane,
 * and farPlane, as well as the indicated width and height.
 * 
 * @param width The width of the Projection.
 * @param height The height of the Projection.
 * 
 * @return A 4 x 4 matrix representing the View-to-Projection matrix.
 */
glm::mat4 Camera::getViewToProjectionMatrix(GLfloat width, GLfloat height) 
	const {

	// Calculate the View-To-Projection matrix.
	return glm::perspectiveFov(fieldOfView, width, height, nearPlane, farPlane);


}




glm::mat4 Camera::getWorldToProjectionMatrix(GLfloat width, GLfloat height) 
	const {

	return getViewToProjectionMatrix(width, height) * getWorldToViewMatrix();

}



//==============================================================================
// Setters. 
//==============================================================================

/**
 * @brief Public setter method for the nearPlane member variable.
 * @details Sets the nearPlane member variable to the indicated float value.
 * 
 * @param _nearPlane The new nearPlane.
 */
void Camera::setNearPlane(GLfloat _nearPlane) {

	// If _nearPlane is invalid, silently ignore.
	if (_nearPlane >= MIN_NEAR_PLANE && _nearPlane < farPlane) {

		nearPlane = _nearPlane;

	}

}



/**
 * @brief Public setter method for the farPlane member variable.
 * @details Sets the farPlane member variable to the indicated float value.
 * 
 * @param _farPlane The new farPlane.
 */
void Camera::setFarPlane(GLfloat _farPlane) {

	// If _farPlane is invalid, silently ignore.
	if (_farPlane > nearPlane && _farPlane <= MAX_FAR_PLANE) {

		farPlane = _farPlane;

	}

}



/**
 * @brief Public setter method for the fieldOfView member variable.
 * @details Sets the fieldOfView member variable to the indicated float value.
 * 
 * @param _fieldOfView The new fieldOfView.
 */
void Camera::setFieldOfView(GLfloat _fieldOfView) {

	// If _fieldOfView is invalid, silently ignore.
	if (_fieldOfView > 0.0f && _fieldOfView < 180.0f) {
		
		fieldOfView = _fieldOfView;

	}

}



/**
 * @brief Public setter method for the position member variable.
 * @details Sets the position member variable to the indicated vector. Allows
 * for 'jumps' throughout the 3-D space.
 * 
 * @param _position The new position for the camera.
 */
void Camera::setPosition(const glm::vec3& _position) {

	position = _position;

}



/**
 * @brief Public setter method for the viewDirection member variable.
 * @details Sets the viewDirection member variable to the indicated vector. If
 * \c _viewDirection is not a unit vector, it is normalized.
 * 
 * @param _viewDirection The new view direction (may be non-normalized).
 */
void Camera::setViewDirection(const glm::vec3& _viewDirection) {

	viewDirection = glm::normalize(_viewDirection);
	updateSideDirection();

}



/**
 * @brief Public setter method for the upDirection member variable.
 * @details Sets the upDirection member variable to the indicated vector. If
 * \c _upDirection is not a unit vector, it is normalized.
 * 
 * @param _upDirection The new up direction (may be non-normalized).
 */
void Camera::setUpDirection(const glm::vec3& _upDirection) {

	upDirection = glm::normalize(_upDirection);
	updateSideDirection();

}
