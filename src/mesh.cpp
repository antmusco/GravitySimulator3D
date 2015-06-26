//==============================================================================
// Header Files 
//==============================================================================

#include <string>
#include <cstring>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL/SDL_image.h>
#include <tiny_obj_loader.h>

#include "mesh.h"

//==============================================================================
// Mesh Class.
//=============================================================================

/**
 * @brief Loads an OBJ file and generate a Mesh object.
 * @details Loads an OBJ file and generates a Mesh object based on the Vertex 
 * and Index data. This function also allows the option for the caller to apply 
 * a texture to the newly loaded Mesh via the textureFile parameter.
 * 
 * @param objFile The path to the OBJ file that is to be loaded. Must point to 
 * a valid OBJ file.
 * @param textureFile  The path to the texture file to be loaded. Can point to
 * any valid image format. By default this parameter is NULL.
 * 
 * @returns A shared pointer to the newly created Mesh object.
 */
MeshPtr Mesh::loadObj(const char* objFile, const char* textureFile) {

	return MeshPtr(new Mesh(objFile, textureFile));

}



/**
 * Default constructor which initializes all members of Mesh to 0 (NULL)
 * except for members with default values (numBuffers and drawMode).
 */
Mesh::Mesh() :
    vertices(),
    indices(),
    bufferIDs(BUFFER_TYPE_COUNT),
    textureID(-1),
    drawMode(DEFAULT_DRAW_MODE) {

	// Empty.

}



/**
 * @brief Copy Constructor.
 * @details Copies a Mesh object to have independent vertex, index, and buffer
 * data.
 * 
 * @param rhs The mesh to be copied.
 */
Mesh::Mesh(const Mesh& other) :
	vertices(other.getVertices()),
	indices(other.getIndices()),
	bufferIDs(BUFFER_TYPE_COUNT),
	textureID(other.getTextureID()),
	drawMode(other.getDrawMode()) {

	// Generate the separate buffers.
	genBufferArrayID();
	genVertexArrayID();

}



/**
 * @brief Loads an OBJ file to construct the Mesh object.
 * @details Loads an OBJ file and generates a Mesh object based on the Vertex 
 * and Index data. This function also allows the option for the caller to apply 
 * a texture to the newly loaded Mesh via the textureFile parameter.
 * 
 * @param objFile The path to the OBJ file that is to be loaded. Must point to 
 * a valid OBJ file.
 * @param textureFile  The path to the texture file to be loaded. Can point to
 * any valid image format. By default this parameter is NULL.
 */
Mesh::Mesh(const char* objFile, const char* textureFile) {

	// Declare the Vectors for shape and material data.
	std::vector<tinyobj::shape_t>    shapes;
	std::vector<tinyobj::material_t> materials;

	// Load the indicated OBJ file and get the error message.
	std::string errMsg = tinyobj::LoadObj(shapes, materials, objFile);

	// If there was an error message, prompt the user and exit function.
	if (!errMsg.empty()) {

		std::cerr << "Error loading obj: " << errMsg << std::endl;
		return;
	
	}

	// Get the shape from the file.
	tinyobj::shape_t s = shapes.at(0);

	// Copy over the Vertex data.
	for(GLuint i = 0; i < (s.mesh.positions.size() / 3); i++) {

		vertices.push_back({

			// Vertex Position.
			{s.mesh.positions.at((3 * i) + 0), 
			 s.mesh.positions.at((3 * i) + 1), 
			 s.mesh.positions.at((3 * i) + 2)},

			// Vertex Color.
			DEFAULT_VERTEX_COLOR,

			// Vertex Normal.
			{s.mesh.normals.at((3 * i) + 0), 
			 s.mesh.normals.at((3 * i) + 1), 
			 s.mesh.normals.at((3 * i) + 2)},

			// U, V Coordinate.
			{s.mesh.texcoords.at((2 * i) + 0), 
			 1 - s.mesh.texcoords.at((2 * i) + 1) }

		});

	}

	// Copy the Index data.
	for(GLuint i = 0; i < s.mesh.indices.size(); i++) {

		indices.push_back(	

			(Index) s.mesh.indices.at(i) 

		);

	}
	
	// Generate buffer and vertex arrays.
	genBufferArrayID();
	genVertexArrayID();

	// If the texture file was provided, generate the texture.
	genTextureID(textureFile);

}



/**
 * Public destructor. Ensures that any allocated space is freed before the Mesh
 * is discarded.
 */
Mesh::~Mesh() {

	// Delete the buffers on the graphics hardware.
	glDeleteBuffers(BUFFER_TYPE_COUNT, &bufferIDs[0]);

	// Delete the vertex array object.
	glDeleteVertexArrays(1, (GLuint*) &textureID);

	// Delete the texture if necessary.
	if(textureID != -1) {

		glDeleteTextures(1, (GLuint*) &textureID);

	}

}



/**
 * @brief Generates the vertex and array buffers.
 * @detail Generates the graphics hardware buffers for data regarding this Mesh.
 * The two specific buffers for this class are the vertex buffer and the index
 * buffer. The IDs of these buffers are stored in the bufferIDs array.
 */
void Mesh::genBufferArrayID() {

	// Generate the buffer space.
	bufferIDs.resize(BUFFER_TYPE_COUNT);
	glGenBuffers(BUFFER_TYPE_COUNT, bufferIDs.data());

	// Create vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[static_cast<GLint>(VERTEX)]);
	glBufferData(GL_ARRAY_BUFFER, getVertexBufferSize(), (GLvoid*) &vertices[0], 
		GL_STATIC_DRAW);

	// Create index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[static_cast<GLint>(INDEX)]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexBufferSize(), (GLvoid*) &indices[0], 
		GL_STATIC_DRAW);

}



/**
 * @brief Generates the Vertex Array Object buffer.
 * @detail Generates the vertex array object buffer for this mesh. The vertex 
 * array object keeps track of the vertex attribute locations for this specific
 * Mesh. To draw this Mesh, the vertex array object must be bound before 
 * telling OpenGL to draw its elements. The vertex array ID is stored in the 
 * vertexArrayID value.
 */
void Mesh::genVertexArrayID() {

	// Generate Vertex Array Object.
	glGenVertexArrays(1, &vertexArrayID);

	// Bind this vertex array ID.
	glBindVertexArray(vertexArrayID);

	// Bind the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[VERTEX]);

	// Enable the vertex attributes.
	glEnableVertexAttribArray(0); // Position.
	glEnableVertexAttribArray(1); // Color.
	glEnableVertexAttribArray(2); // Normal.
	glEnableVertexAttribArray(3); // Texture Coordinates.

	// Vertex position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(void*) ATTRIBUTE_0_OFFSET);

	// Vertex color attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,	sizeof(Vertex), 
		(void*) ATTRIBUTE_1_OFFSET);

	// Vertex normal attribute.
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*) ATTRIBUTE_2_OFFSET);

	// Vertex texture coordinate attribute.
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*) ATTRIBUTE_3_OFFSET);

}



/**
 * @brief Generates the texture buffer ID for the Mesh.
 * @details Generates the texture buffer and sends the data from the indicated 
 * file down to the graphics hardware. The texture ID is saved to the indicated
 * parameter for this Mesh.
 * 
 * @param filename The path to the texture that is to be loaded. Can be of any 
 * valid image format.
 */
void Mesh::genTextureID(const char* filename) {

	// Enable Texture 2D.
	glEnable(GL_TEXTURE_2D);

	// If the filename is null, silently ignore.
	if(filename != NULL) {

		// Load the SDL_Surface from the file.
		SDL_Surface* textureSurface = IMG_Load(filename);

		// If the image was not loaded correctly, do nothing.
		if(textureSurface != NULL) {

			// The default color scheme is RGB.
			GLenum colorScheme = GL_RGB;

			// If the file is a bitmap, change the color scheme to BGR.
			std::string file(filename);
			std::string ext = file.substr(file.find('.'), file.length() - 1);
			if(ext == ".bmp") 
				colorScheme = GL_BGR;

			// Generate the texture buffer.
			glGenTextures(1, (GLuint*)&textureID);

			// Bind the texture ID to the appropriate binding point.
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Send the image data down to the graphics card.
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSurface->w, 
				textureSurface->h, 0, colorScheme, GL_UNSIGNED_BYTE, 
				textureSurface->pixels);

			// Set the desred texture parameters.
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		}

	}

}

//==============================================================================
// Getters.
//==============================================================================

/**
 * @brief Get the size of the vertex buffer.
 * @details Calculates the number of bytes required for this Mesh's 
 * vertex buffer.
 * 
 * @return The size of the vertex buffer.
 */
inline GLsizeiptr Mesh::getVertexBufferSize() const {

	return vertices.size() * sizeof(Vertex);

}



/**
 * @brief Get the size of the index buffer.
 * @details Calculates the number of bytes required for this Mesh's 
 * index buffer.
 * 
 * @return The size of the index buffer.
 */
inline GLsizeiptr Mesh::getIndexBufferSize() const {

	return indices.size() * sizeof(Index);

}



const std::vector<Vertex>& Mesh::getVertices() const {

	return vertices;

}



const std::vector<Index>& Mesh::getIndices() const { 

	return indices;

}



GLint Mesh::getTextureID() const {

	return textureID;

}



const std::vector<GLuint>& Mesh::getBufferIDs() const {

	return bufferIDs;

}



GLuint Mesh::getBufferID(GLuint i) const {

	return bufferIDs[i];

}



GLuint Mesh::getVertexArrayID() const {

	return vertexArrayID;

}



GLenum Mesh::getDrawMode() const {

	return drawMode;

}

//==============================================================================
// Setters.
//==============================================================================

/**
 * @brief Set the vertices of this mesh given a vector.
 * @details Copies over the vertices in the vector to the Mesh's vertices 
 * member.
 * 
 * @param v Reference to a vector object containing the Vertex objects to copy.
 */
void Mesh::setVertices(const std::vector<Vertex>& v) {

	// Copy the vertex data.
	vertices = v;

}



/**
 * @brief Set the indices of this mesh given a vector.
 * @details Copies over the indices in the vector to the Mesh's indices 
 * member.
 * 
 * @param v Reference to a vector object containing the indices to copy.
 */
void Mesh::setIndices(const std::vector<Index>& i) {

	// Copy the index data.
	indices = i;

}



void Mesh::setDrawMode(GLenum d) {

	drawMode = d;

}
