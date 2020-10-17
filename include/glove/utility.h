#pragma once

#include <GL/glew.h>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
                              GLenum severity, [[maybe_unused]] GLsizei length,
                              const char *                 message,
                              [[maybe_unused]] const void *userParam);

// Overload ostream to support printing glm primitives.

std::ostream &operator<<(std::ostream &os, const glm::vec2 &vector);

std::ostream &operator<<(std::ostream &os, const glm::vec3 &vector);

std::ostream &operator<<(std::ostream &os, const glm::vec4 &vector);

std::ostream &operator<<(std::ostream &os, const glm::ivec2 &vector);

std::ostream &operator<<(std::ostream &os, const glm::ivec3 &vector);

std::ostream &operator<<(std::ostream &os, const glm::ivec4 &vector);

std::ostream &operator<<(std::ostream &os, const glm::uvec2 &vector);

std::ostream &operator<<(std::ostream &os, const glm::uvec3 &vector);

std::ostream &operator<<(std::ostream &os, const glm::uvec4 &vector);

std::ostream &operator<<(std::ostream &os, const glm::mat2 &matrix);

std::ostream &operator<<(std::ostream &os, const glm::mat3 &matrix);

std::ostream &operator<<(std::ostream &os, const glm::mat4 &matrix);
