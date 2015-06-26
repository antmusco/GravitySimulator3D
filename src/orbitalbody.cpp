//==============================================================================
// Header Files 
//==============================================================================

#include "enums.h"
#include "shadersingleton.h"
#include "orbitalbody.h"

//==============================================================================
// OrbitalBody Class 
//==============================================================================

OrbitalBody::OrbitalBody(const char* _name, GLfloat _mass, GLfloat _radius, 
	const char* _objFile, const char* _textFile, const glm::vec3& 
	_linearPosition, const glm::vec3& _linearVelocity, GLfloat _tilt, GLfloat
	_angularVelocity) :

	Mesh(_objFile, _textFile), // Load obj constructor.
	Drawable(ShaderSingleton::getShader(ShaderType::Mesh)),
	name(std::string(_name)),
	radius(_radius),
	scale(glm::vec3(1.0f) * _radius),
	mass(_mass),
	gravityVector(0),
	linearPosition(_linearPosition),
	linearVelocity(_linearVelocity),
	linearAccel(0),
	linearThrust(0),
	rotationalAxis(DEFAULT_ROT_AXIS),
	rotationalAngle(0),
	angularPosition(0),
	angularVelocity(0),
	angularAccel(0),
	angularThrust(0),
	modelToWorld() {

	setRotationalAxis(_tilt);
	setAngularVelocity(_angularVelocity);

}



OrbitalBody::~OrbitalBody() {

	// Empty.

}



void OrbitalBody::draw(const glm::mat4& worldToProjection) {

	// Calculate the snapshot matrix.
	updateModelToWorld();

	shader->use();

	// Calculate the modelToProjection matrix.
	glm::mat4 modelToProjection = worldToProjection * modelToWorld;

	// Bind the vertex array object.
	glBindVertexArray(vertexArrayID);

	// Bind the element array buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[Mesh::INDEX]);

	// Bind the texture (if necessary).
	if(textureID != -1) {

		glBindTexture(GL_TEXTURE_2D, textureID);
		glActiveTexture(GL_TEXTURE0);

	}

	// Set the uniform variables.
	shader->setUniformMat4(Uniform::ModelToWorld, modelToWorld);
	shader->setUniformMat4(Uniform::ModelToProjection, modelToProjection);
	shader->setUniformInt1(Uniform::Texture, 0);

	// Draw the elements.
	glDrawElements(drawMode, vertices.size(), GL_UNSIGNED_SHORT, 0);

}



/**
 * @brief Update the Model-To-World Matrix.
 * @details Updates the matrix representing the Model-To-World Matrix
 * transformation. This function applies the scale, the rotation, and the
 * translation, in that order.
 */
void OrbitalBody::updateModelToWorld() {

	// Scale the body.
	glm::mat4 scaleM = glm::scale(glm::mat4(), scale);

	// Rotate the body.
	glm::mat4 rotM;
	
	glm::vec3 cross = glm::cross(DEFAULT_ROT_AXIS, rotationalAxis);

	// Rotate to angle of inclination.
	if(rotationalAxis != DEFAULT_ROT_AXIS)
		rotM = glm::rotate(rotationalAngle, cross);
	
	rotM = glm::rotate(rotM, angularPosition, DEFAULT_ROT_AXIS);

	// Translate the body.
	glm::mat4 tranM = glm::translate(glm::mat4(), linearPosition);

	// Update the transformation matrix.
	modelToWorld   = tranM * rotM * scaleM ;

}



/** 	
 * @brief Incremet the motion of the OrbitalBody.
 * @detail Step forward in time by dt seconds and calculate the updated values
 * for the position, velocity, and acceleration of the body.
 */
void OrbitalBody::increment(GLfloat dt) {

	// If the mass of the body is 0, do nothing.
	if (mass == 0) return;

	// Translational parameters.
	linearAccel     += dt * (linearThrust / mass);
	linearVelocity  += dt * linearAccel + (gravityVector / mass);
	linearPosition  += dt * linearVelocity;

	// Rotational parameters.
	angularAccel    += dt * (angularThrust / mass);
	angularVelocity += dt * angularAccel;
	angularPosition += dt * angularVelocity;

	// Account for full revolution.
	if(angularPosition > DEGREES_PER_REV) 
		angularPosition -= DEGREES_PER_REV;

}

//==============================================================================
// Getters 
//==============================================================================


const std::string& OrbitalBody::getName() const {

	return name;

}



GLfloat OrbitalBody::getRadius() const {

	return radius;

}



const glm::vec3& OrbitalBody::getScale() const {

	return scale;

}



GLfloat OrbitalBody::getMass() const {

	return mass;

}



const glm::vec3& OrbitalBody::getGravityVector() const {

	return gravityVector;

}



const glm::vec3& OrbitalBody::getLinearPosition() const {

	return linearPosition;

}



const glm::vec3& OrbitalBody::getLinearVelocity() const {

	return linearVelocity;

}



const glm::vec3& OrbitalBody::getLinearAccel() const {

	return linearAccel;

}



const glm::vec3& OrbitalBody::getLinearThrust() const {

	return linearThrust;

}



const glm::vec3& OrbitalBody::getRotationalAxis() const {

	return rotationalAxis;

}



GLfloat OrbitalBody::getAngularPosition() const {

	return angularPosition;

}



GLfloat OrbitalBody::getAngularVelocity() const {

	return angularVelocity;

}



GLfloat OrbitalBody::getAngularAccel() const {

	return angularAccel;

}



GLfloat OrbitalBody::getAngularThrust() const {

	return angularThrust;

}



const glm::mat4& OrbitalBody::getModelToWorld() const {

	return modelToWorld;

}

//==============================================================================
// Setters 
//==============================================================================

void OrbitalBody::setName(const std::string& _name) {

	name = _name;

}



void OrbitalBody::setRadius(GLfloat _radius) {

	radius = _radius;

}



void OrbitalBody::setScale(const glm::vec3& _scale) {

	scale = _scale;

}



void OrbitalBody::setMass(GLfloat _mass) {

	mass = _mass;

}



void OrbitalBody::setGravityVector(const glm::vec3& _gravityVector) {

	gravityVector = _gravityVector;

}



void OrbitalBody::setLinearPosition(const glm::vec3& _linearPosition) {

	linearPosition = _linearPosition;

}



void OrbitalBody::setLinearVelocity(const glm::vec3& _linearVelocity) {

	linearVelocity = _linearVelocity;

}



void OrbitalBody::setLinearAccel(const glm::vec3& _linearAccel) {

	linearAccel = _linearAccel;

}



void OrbitalBody::setLinearThrust(const glm::vec3& _linearThrust) {

	linearThrust = _linearThrust;

}



void OrbitalBody::setRotationalAxis(GLfloat _tilt) {

	rotationalAngle = _tilt * DEG_TO_RAD;
	rotationalAxis  = glm::rotateX(DEFAULT_ROT_AXIS, rotationalAngle);

}



void OrbitalBody::setAngularPosition(GLfloat _angularPosition) {

	if(_angularPosition < 2 * M_PI)
		angularPosition = _angularPosition; 
	else
		angularPosition = _angularPosition - ((GLfloat) (2 * M_PI));

}



void OrbitalBody::setAngularVelocity(GLfloat _angularVelocity) {

	angularVelocity = _angularVelocity;

}



void OrbitalBody::setAngularAccel(GLfloat _angularAccel) {

	angularAccel = _angularAccel;

}



void OrbitalBody::setAngularThrust(GLfloat _angularThrust) {

	angularThrust = _angularThrust;

}
