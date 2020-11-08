#pragma once

#include <GL/glew.h>
#include <vector>

/**
 * VertexBuffer is a class for representing a vertex buffer and freeing
 * the resources on deconstruction.
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
	VertexBuffer(size_t size, bool indexed, GLenum usage = GL_STATIC_DRAW);

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

	/**
	 * @brief Destroy the Vertex Buffer object and deletes all associated opengl
	 * objects.
	 */
	~VertexBuffer();

	/**
	 * Draw the entire VBO.
	 */
	void draw() const;

	/**
	 * Upload new content to the whole buffer.
	 * @param vertices Vertices to upload.
	 * @param indices Indices to upload.
	 */
	void uploadWhole(const std::vector<VertexFormat> &vertices,
	                 const std::vector<GLuint> &      indices);

	/**
	 * Creates and associates a instance array with the vertex buffer to enable
	 * instanced rendering. Also enables instanced rendering autmatically from
	 * now on.
	 *
	 * @tparam InstanceFormat Format of the per instance data.
	 * @param instance_data Per instance data.
	 */
	template <typename InstanceFormat>
	void enableInstancing();

	template <typename InstanceFormat>
	void uploadInstanceData(const std::vector<InstanceFormat> &instance_data);

  private:
	bool m_indexed;   ///< Indicates whether the VBO has an associated index
	                  ///< buffer and whether the VBO should be drawn using
	                  ///< indexed rendering.
	bool m_instanced; ///< Indicates whether the VBO has a associated per
	                  ///< instance data used in instanced rendering.

	GLenum m_usage;           ///< How is this VBO used?
	GLuint m_primitive_count; ///< How many primitives are in the VBO.
	GLuint m_instance_count;  ///< How many instances are in the VBO.

	GLuint m_vao;          ///< Vertex Array Object.
	GLuint m_vbo;          ///< Internal Vertex Buffer Object.
	GLuint m_ebo;          ///< Element Buffer Object.
	GLuint m_instance_vbo; ///< VBO with per instance data.
};
