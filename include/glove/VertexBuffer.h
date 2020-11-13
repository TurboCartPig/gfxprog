#pragma once

#include <GL/glew.h>
#include <vector>

/**
 * @brief VertexBuffer is a class for representing a vertex buffer and freeing
 * the resources on deconstruction.
 */
template <typename VertexFormat>
class VertexBuffer {
  public:
	/**
	 * @brief Construct empty VBO.
	 * @note Must be filled before reading.
	 * @see "uploadWhole"
	 * @param size Size of VBO in primitives.
	 * @param indexed Should the VBO use an index buffer.
	 * @param usage How is the VBO going to be used? Default is STATIC DRAW
	 */
	VertexBuffer(size_t size, bool indexed, GLenum usage = GL_STATIC_DRAW);

	/**
	 * @brief Construct a VBO containing a single vertex buffer of triangles.
	 * @param vertices Vertices with VertexFormat.
	 */
	explicit VertexBuffer(const std::vector<VertexFormat> &vertices);

	/**
	 * @brief Construct a VBO containing both a vertex buffer and an index
	 * buffer representing triangles.
	 * @param vertices Vertices with VertexFormat.
	 * @param indices Indices in GLuints.
	 */
	VertexBuffer(const std::vector<VertexFormat> &vertices,
	             const std::vector<GLuint> &      indices);

	VertexBuffer(const VertexBuffer &other) = delete;

	VertexBuffer(const VertexBuffer &&other) = delete;

	auto operator=(const VertexBuffer &other) = delete;

	auto operator=(const VertexBuffer &&other) = delete;

	/**
	 * @brief Destroy the Vertex Buffer object and deletes all associated opengl
	 * objects.
	 */
	~VertexBuffer();

	/**
	 * @brief Draw the entire VBO.
	 */
	void draw() const;

	/**
	 * @brief Upload new content to the whole buffer.
	 * @param vertices Vertices to upload.
	 * @param indices Indices to upload.
	 */
	void uploadWhole(const std::vector<VertexFormat> &vertices,
	                 const std::vector<GLuint> &      indices);

	/**
	 * @brief Sets up and enables instancing.
	 * @note MUST also upload instance data after enabling, and before next
	 * draw.
	 * @see "uploadInstanceData(...)".
	 * @tparam InstanceFormat Format of the per instance data.
	 */
	template <typename InstanceFormat>
	void enableInstancing();

	/**
	 * @brief Upload instance data in "InstanceFormat".
	 * Each element in the instance_data vector is passed to the vertex shader
	 * on a per instance basis.
	 * @note MUST first enable instancing via "enableInstancing()".
	 * @see "enableInstancing()".
	 * @tparam InstanceFormat Format of the per instance data.
	 * @param instance_data Per instance data.
	 */
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
