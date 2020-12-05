#include <GL/glew.h>
#include <glove/VertexFormats.h>

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

/*
 * This one is a little different. This sets up per instance data for instanced
 * rendering.
 */
template <>
void setVertexAttribs<glm::mat4>() {
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), nullptr);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
	                      reinterpret_cast<const void *>(sizeof(glm::vec4)));
	glVertexAttribPointer(
	    6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
	    reinterpret_cast<const void *>(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(
	    7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
	    reinterpret_cast<const void *>(sizeof(glm::vec4) * 3));

	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);

	// Sets attribute feed rate to once per instance.
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
}
