#pragma clang diagnostic push
#pragma ide diagnostic   ignored "cppcoreguidelines-pro-type-reinterpret-cast"

#include <glove/VertexBuffer.h>
#include <glove/VertexFormats.h>

/**
 * Sets up and enables the vertex attribute pointers for the given vertex
 * format.
 *
 * @tparam VertexFormat A struct containing one field per vertex attribute.
 */
template <typename VertexFormat>
void setVertexAttribs();

template <>
void setVertexAttribs<Vertex2D>() {
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), nullptr);
	glEnableVertexAttribArray(0);
}

template <>
void setVertexAttribs<Vertex2DRgb>() {
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DRgb),
	                      nullptr);
	glVertexAttribPointer(
	    1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DRgb),
	    reinterpret_cast<const void *>(offsetof(Vertex2DRgb, color)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

template <>
void setVertexAttribs<Vertex2DTex>() {
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DTex),
	                      nullptr);
	glVertexAttribPointer(
	    1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DTex),
	    reinterpret_cast<const void *>(offsetof(Vertex2DTex, uv)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

template <>
void setVertexAttribs<Vertex2DTexRgbav>() {
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DTexRgbav),
	                      nullptr);
	glVertexAttribPointer(
	    1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DTexRgbav),
	    reinterpret_cast<const void *>(offsetof(Vertex2DTexRgbav, uv)));
	glVertexAttribPointer(
	    2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2DTexRgbav),
	    reinterpret_cast<const void *>(offsetof(Vertex2DTexRgbav, rgba)));
	glVertexAttribPointer(
	    3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex2DTexRgbav),
	    reinterpret_cast<const void *>(offsetof(Vertex2DTexRgbav, variant)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}

template <>
void setVertexAttribs<Vertex3DNormTex>() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3DNormTex),
	                      nullptr);
	glVertexAttribPointer(
	    1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3DNormTex),
	    reinterpret_cast<const void *>(offsetof(Vertex3DNormTex, normal)));
	glVertexAttribPointer(
	    2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3DNormTex),
	    reinterpret_cast<const void *>(offsetof(Vertex3DNormTex, uv)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

template <typename VertexFormat>
VertexBuffer<VertexFormat>::VertexBuffer(size_t size, bool indexed,
                                         GLenum usage) {
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
	m_indexed         = false;
	m_primitive_count = vertices.size(); // TODO: Is this correct?

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

	glDeleteVertexArrays(1, &m_vao);
}

template <typename VertexFormat>
void VertexBuffer<VertexFormat>::draw() {
	if (m_indexed) { // Draw indexed
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_primitive_count, GL_UNSIGNED_INT,
		               nullptr);
	} else { // Draw non indexed
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_primitive_count);
	}
}

template <typename VertexFormat>
void VertexBuffer<VertexFormat>::uploadWhole(
    const std::vector<VertexFormat> &vertices,
    const std::vector<GLuint> &      indices) {
	m_primitive_count = indices.size();

	glBindVertexArray(m_vao);

	// Orphan the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat),
	             nullptr, m_usage);
	// Upload the new data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(VertexFormat),
	                vertices.data());

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
	             nullptr, m_usage);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint),
	                indices.data());
}

// Explicitly template the class for the provided vertex formats
template class VertexBuffer<Vertex2D>;
template class VertexBuffer<Vertex2DRgb>;
template class VertexBuffer<Vertex2DTex>;
template class VertexBuffer<Vertex2DTexRgbav>;
template class VertexBuffer<Vertex3DNormTex>;

#pragma clang diagnostic pop