#pragma once

#include <GL/glew.h>
#include <vector>

/**
 * VertexBufferObject is a class for representing a vertex buffer and freeing
 * the resources on deconstruction.
 *
 * @tparam VertexFormat
 */
template <typename VertexFormat>
class VertexBufferObject {
  public:
	/**
	 * Construct a VBO containing a single vertex buffer of triangles.
	 * @param vertices
	 */
	explicit VertexBufferObject(const std::vector<VertexFormat> &vertices);

	/**
	 * Construct a VBO containing both a vertex buffer and an index buffer
	 * representing triangles.
	 * @param vertices
	 * @param indices
	 */
	VertexBufferObject(const std::vector<VertexFormat> &vertices,
	                   const std::vector<GLuint> &      indices);

	/**
	 * Deleting the copy constructor to avoid double frees.
	 * @param other
	 */
	VertexBufferObject(const VertexBufferObject &other) = delete;

	~VertexBufferObject();

	/**
	 * Draw the entire VBO.
	 */
	void draw();

  private:
	bool   m_indexed;
	GLuint m_primitive_count;
	GLuint m_vbo;
	GLuint m_ebo;
	GLuint m_vao;
};
