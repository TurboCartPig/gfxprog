#pragma once

#include <GL/glew.h>
#include <vector>

/**
 * VertexBuffer is a class for representing a vertex buffer and freeing
 * the resources on deconstruction.
 *
 * @tparam VertexFormat
 */
template <typename VertexFormat>
class VertexBuffer {
  public:
	/**
	 * Construct empty VBO.
	 * @note Must be filled before reading.
	 * @see "uploadWhole"
	 * @param size Size of VBO in primitives.
	 * @param indexed Should the VBO use an index buffer.
	 * @param usage How is the VBO going to be used? Default is STATIC DRAW
	 */
	VertexBuffer(size_t size, bool indexed,
	                      GLenum usage = GL_STATIC_DRAW);

	/**
	 * Construct a VBO containing a single vertex buffer of triangles.
	 * @param vertices Vertices with VertexFormat.
	 */
	explicit VertexBuffer(const std::vector<VertexFormat> &vertices);

	/**
	 * Construct a VBO containing both a vertex buffer and an index buffer
	 * representing triangles.
	 * @param vertices Vertices with VertexFormat.
	 * @param indices Indices in GLuints.
	 */
	VertexBuffer(const std::vector<VertexFormat> &vertices,
	             const std::vector<GLuint> &      indices);

	/**
	 * Deleting the copy constructor to avoid double frees.
	 * @param other
	 */
	VertexBuffer(const VertexBuffer &other) = delete;

	VertexBuffer(const VertexBuffer &&other) = delete;

	VertexBuffer operator=(const VertexBuffer &other) = delete;

	VertexBuffer operator=(const VertexBuffer &&other) = delete;

	~VertexBuffer();

	/**
	 * Draw the entire VBO.
	 */
	void draw();

	/**
	 * Upload new content to the whole buffer.
	 * @param vertices Vertices to upload.
	 * @param indices Indices to upload.
	 */
	void uploadWhole(const std::vector<VertexFormat> &vertices,
	                 const std::vector<GLuint> &      indices);

  private:
	bool   m_indexed;
	GLenum m_usage;
	GLuint m_primitive_count;
	GLuint m_vbo;
	GLuint m_ebo;
	GLuint m_vao;
};
