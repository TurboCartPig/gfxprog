#pragma once

#include <glm/glm.hpp>

struct Vertex2D {
	glm::vec2 pos; ///< Position
};

struct Vertex2DRgb {
	glm::vec2 pos;   ///< Position
	glm::vec3 color; ///< RGB Color
};

struct Vertex2DTex {
	glm::vec2 pos; ///< Position
	glm::vec2 uv;  ///< Normalized texture coordinates
};

struct Vertex2DTexRgbav {
	glm::vec2 pos;     ///< Position
	glm::vec2 uv;      ///< Normalized texture coordinates
	glm::vec4 rgba;    ///< rgba OR xyzw
	float     variant; ///< Is rgba or xyzw
};

struct Vertex3DNormTex {
	glm::vec3 pos;    ///< Position
	glm::vec3 normal; ///< Normal
	glm::vec2 uv;     ///< Normalized texture coordinates
};