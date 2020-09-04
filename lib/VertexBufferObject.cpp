#include <glove/VertexBufferObject.h>
#include <glove/VertexFormats.h>

/**
 * Sets up and enables the vertex attribute pointers for the given vertex
 * format.
 *
 * @tparam VertexFormat
 */
template <typename VertexFormat>
void setVertexAttribs();

template <>
void setVertexAttribs<Vertex2D>() {
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
	                      (GLvoid *)0);
	glEnableVertexAttribArray(0);
}

template <>
void setVertexAttribs<Vertex3D>() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
	                      (GLvoid *)0);
	glEnableVertexAttribArray(0);
}

template <>
void setVertexAttribs<Vertex2DRgb>() {
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DRgb), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DRgb),
	                      (GLvoid *)(sizeof(float) * 2));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

template <>
void setVertexAttribs<Vertex3DRgb>() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3DRgb), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3DRgb),
	                      (GLvoid *)(sizeof(float) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

template <typename VertexFormat>
VertexBufferObject<VertexFormat>::VertexBufferObject(
    const std::vector<VertexFormat> &vertices) {
	m_indexed         = false;
	m_primitive_count = vertices.size() / 3;

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
VertexBufferObject<VertexFormat>::VertexBufferObject(
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
VertexBufferObject<VertexFormat>::~VertexBufferObject() {
	glDeleteBuffers(1, &m_vbo);

	if (m_indexed)
		glDeleteBuffers(1, &m_ebo);

	glDeleteVertexArrays(1, &m_vao);
}

template <typename VertexFormat>
void VertexBufferObject<VertexFormat>::draw() {
	if (m_indexed) { // Draw indexed
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_primitive_count, GL_UNSIGNED_INT,
		               (GLvoid *)0);
	} else { // Draw non indexed
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_primitive_count);
	}
}

// Explicitly template the class for the provided vertex formats
template class VertexBufferObject<Vertex2D>;
template class VertexBufferObject<Vertex2DRgb>;
template class VertexBufferObject<Vertex3D>;
template class VertexBufferObject<Vertex3DRgb>;
