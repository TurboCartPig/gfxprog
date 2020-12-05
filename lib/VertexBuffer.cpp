#include <glove/VertexBuffer.h>
#include <glove/VertexFormats.h>

template <typename VertexFormat>
VertexBuffer<VertexFormat>::VertexBuffer(size_t size, bool indexed,
                                         GLenum usage) {
	m_instanced       = false;
	m_indexed         = indexed;
	m_primitive_count = indexed ? size * 6 : size;
	m_usage           = usage;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * 4 * size, nullptr,
	             usage);

	if (indexed) {
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		             sizeof(GLuint) * m_primitive_count, nullptr, usage);
	}

	setVertexAttribs<VertexFormat>();
}

template <typename VertexFormat>
VertexBuffer<VertexFormat>::VertexBuffer(
    const std::vector<VertexFormat> &vertices) {
	m_instanced       = false;
	m_indexed         = false;
	m_primitive_count = vertices.size(); // FIXME: Is this correct?

	// Generate a vao
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Generate a vbo
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat),
	             vertices.data(), GL_STATIC_DRAW);

	setVertexAttribs<VertexFormat>();
}

template <typename VertexFormat>
VertexBuffer<VertexFormat>::VertexBuffer(
    const std::vector<VertexFormat> &vertices,
    const std::vector<GLuint> &      indices) {
	m_instanced       = false;
	m_indexed         = true;
	m_primitive_count = indices.size();

	// Generate a vertex array
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Generate a vertex buffer
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat),
	             vertices.data(), GL_STATIC_DRAW);

	// Generate element buffer
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
	             indices.data(), GL_STATIC_DRAW);

	setVertexAttribs<VertexFormat>();
}

template <typename VertexFormat>
VertexBuffer<VertexFormat>::~VertexBuffer() {
	glDeleteBuffers(1, &m_vbo);

	if (m_indexed)
		glDeleteBuffers(1, &m_ebo);

	if (m_instanced)
		glDeleteBuffers(1, &m_instance_vbo);

	glDeleteVertexArrays(1, &m_vao);
}

template <typename VertexFormat>
void VertexBuffer<VertexFormat>::draw() const {
	glBindVertexArray(m_vao);

	// Draw the VBO using the appropriate gl command
	if (m_instanced) {
		if (m_indexed) {
			glDrawElementsInstanced(GL_TRIANGLES, m_primitive_count,
			                        GL_UNSIGNED_INT, nullptr, m_instance_count);
		} else {
			glDrawArraysInstanced(GL_TRIANGLES, 0, m_primitive_count,
			                      m_instance_count);
		}
	} else {
		if (m_indexed) {
			glDrawElements(GL_TRIANGLES, m_primitive_count, GL_UNSIGNED_INT,
			               nullptr);
		} else {
			glDrawArrays(GL_TRIANGLES, 0, m_primitive_count);
		}
	}
}

template <typename VertexFormat>
void VertexBuffer<VertexFormat>::uploadWhole(
    const std::vector<VertexFormat> &vertices,
    const std::vector<GLuint> &      indices) {
	m_primitive_count = indices.size();

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	// Orphan the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat),
	             nullptr, m_usage);
	// Upload the new data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(VertexFormat),
	                vertices.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	// Same as above, but for indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
	             nullptr, m_usage);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint),
	                indices.data());
}

template <typename VertexFormat>
template <typename InstanceFormat>
void VertexBuffer<VertexFormat>::enableInstancing() {
	m_instanced = true;

	// Create a new VBO for per-instance data
	glGenBuffers(1, &m_instance_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);

	// Associate the new VBO with the preexisting VAO
	glBindVertexArray(m_vao);
	setVertexAttribs<InstanceFormat>();
}

template <typename VertexFormat>
template <typename InstanceFormat>
void VertexBuffer<VertexFormat>::uploadInstanceData(
    const std::vector<InstanceFormat> &instance_data) {
	assert(m_instanced);

	m_instance_count = instance_data.size();

	glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
	glBufferData(GL_ARRAY_BUFFER, instance_data.size() * sizeof(InstanceFormat),
	             nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
	                sizeof(InstanceFormat) * instance_data.size(),
	                instance_data.data());
}

// Explicit template specialization
// FIXME: Is there a better way to do this? Other than putting everything in the
// header?

template class VertexBuffer<Vertex2D>;
template class VertexBuffer<Vertex2DRgb>;
template class VertexBuffer<Vertex2DTex>;
template class VertexBuffer<Vertex2DTexRgbav>;
template class VertexBuffer<Vertex3DNormTex>;

template void VertexBuffer<Vertex3DNormTex>::enableInstancing<glm::mat4>();
template void VertexBuffer<Vertex3DNormTex>::uploadInstanceData<glm::mat4>(
    const std::vector<glm::mat4> &);
