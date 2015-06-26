#ifndef _MESH_H_
#define _MESH_H_

//==============================================================================
// Header Files 
//==============================================================================

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <glm/glm.hpp>

//==============================================================================
// Constants.
//==============================================================================

#define DEFAULT_NUM_BUFFERS	    3
#define DEFAULT_DRAW_MODE       GL_TRIANGLES
#define DEFAULT_VERTEX_COLOR    glm::vec3(+1.0f, +1.0f, +1.0f)
#define ATTRIBUTE_0_OFFSET      (sizeof(GLfloat) * 0)
#define ATTRIBUTE_1_OFFSET      (sizeof(GLfloat) * 3)
#define ATTRIBUTE_2_OFFSET      (sizeof(GLfloat) * 6)
#define ATTRIBUTE_3_OFFSET      (sizeof(GLfloat) * 9)

//==============================================================================
// Typedefs.
//==============================================================================

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef GLushort Index;

//==============================================================================
// Vertex Struct.
//==============================================================================

/**
 * Struct representing a simple vertex in 3-D space. The vertex consists of 11
 * sequential float values: the x, y, and z coordinates of the position,
 * the r, g, and b values of the color, the x_n, y_n, z_n coordinates of the 
 * vertex normal, and the s, t values for the texture coordinate.
 */
struct Vertex {

	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 textureCoordinate;

};

//==============================================================================
// Mesh Class.
//==============================================================================

/**
 * Class representing a collection of vertices in 3-D space representing an
 * object. All vertices are only recorded once, with the indexes indicating
 * the draw order for each vertex.
 */
class Mesh {

protected:

	/**
	 * Collection of vertices contained within this mesh.
	 */
	std::vector<Vertex> vertices;

	/**
	 * Collection of numbers inidcating the written order in which the 
	 * Vertex objects are to be drawn.
	 */
	std::vector<Index> indices;

	/**
	 * IDs of the buffers which has been generated for the Mesh.
	 */
	std::vector<GLuint> bufferIDs;

	/**
	 * Vertex Array Object ID for the Mesh.
	 */
	GLuint vertexArrayID;

	/**
	 * ID of the texture buffer in which the texture is located.
	 */
	GLint textureID;
	
	/**
	 * GLenum for the draw mode of this Mesh. Can be GL_TRIANGLES, GL_LINES, 
	 * GL_QUADS, etc.
	 */
	GLenum drawMode;

	// Convenience method to get the number of bytes for the vertex buffer.
	inline GLsizeiptr getVertexBufferSize() const;

	// Convenience method to get the number of bytes for the index buffer.
	inline GLsizeiptr getIndexBufferSize() const;

public:

	/**
	 * @brief Enumeration indicating the types of buffers used.
	 * @details Enumerates the types of buffers used by this Mesh. The types 
	 * are Vertex and Index. 
	 */
	enum BufferType {

		VERTEX,
		INDEX,
		BUFFER_TYPE_COUNT
		
	};

	// Load from OBJ file.
	static MeshPtr loadObj(const char* objFile, const char* textureFile = NULL);

	// Constructors.
	Mesh();
	Mesh(const Mesh& rhs);
	Mesh(const char* objFile, const char* textureFile = NULL);

	// Destructor. 
	virtual ~Mesh();

	// Generate the graphics buffers and IDs for the mesh. 
	void genBufferArrayID();

	// Generate the vertex array object and ID for the mesh.
	void genVertexArrayID();

	// Generate the texture buffer and ID for the mesh. 
	void genTextureID(const char* filename);


	// Getters
	const std::vector<Vertex>& getVertices() const;
	const std::vector<Index>& getIndices() const;
	GLint getTextureID() const;
	const std::vector<GLuint>& getBufferIDs() const;
	GLuint getBufferID(GLuint i)  const; 
	GLuint getVertexArrayID() const;
	GLenum getDrawMode() const;
											    						 
	// Setters
	void setVertices(const std::vector<Vertex>& v);
	void setIndices(const std::vector<Index>& v);
	void setDrawMode(GLenum d);

};

#endif
