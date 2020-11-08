#include <glove/utility.h>
#include <iostream>
#include <ostream>

void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
                              GLenum severity, [[maybe_unused]] GLsizei length,
                              const char *                 message,
                              [[maybe_unused]] const void *userParam) {
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
		case GL_DEBUG_SOURCE_API: std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			std::cout << "Source: Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			std::cout << "Source: Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			std::cout << "Source: Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			std::cout << "Source: Application";
			break;
		case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other"; break;
	}
	std::cout << std::endl;

	switch (type) {
		case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			std::cout << "Type: Deprecated Behaviour";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			std::cout << "Type: Undefined Behaviour";
			break;
		case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other"; break;
	}
	std::cout << std::endl;

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			std::cout << "Severity: notification";
			break;
	}
	std::cout << std::endl;
}

std::ostream &operator<<(std::ostream &os, const glm::vec2 &vector) {
	os << "vec2(" << vector.x << ", " << vector.y << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::vec3 &vector) {
	os << "vec3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::vec4 &vector) {
	os << "vec4(" << vector.x << ", " << vector.y << ", " << vector.z << ", "
	   << vector.w << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::ivec2 &vector) {
	os << "ivec2(" << vector.x << ", " << vector.y << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::ivec3 &vector) {
	os << "ivec3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::ivec4 &vector) {
	os << "ivec4(" << vector.x << ", " << vector.y << ", " << vector.z << ", "
	   << vector.w << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::uvec2 &vector) {
	os << "uvec2(" << vector.x << ", " << vector.y << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::uvec3 &vector) {
	os << "uvec3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::uvec4 &vector) {
	os << "uvec4(" << vector.x << ", " << vector.y << ", " << vector.z << ", "
	   << vector.w << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::mat2 &matrix) {
	os << "mat2("
	   << "(" << matrix[0][0] << ", " << matrix[0][1] << "), "
	   << "(" << matrix[1][0] << ", " << matrix[1][1] << ")"
	   << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::mat3 &matrix) {
	os << "mat3("
	   << "(" << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2]
	   << "), "
	   << "(" << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2]
	   << "), "
	   << "(" << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2]
	   << ")"
	   << ")";

	return os;
}

std::ostream &operator<<(std::ostream &os, const glm::mat4 &matrix) {
	os << "mat4("
	   << "(" << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2]
	   << ", " << matrix[0][3] << "), "
	   << "(" << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2]
	   << ", " << matrix[1][3] << "), "
	   << "(" << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2]
	   << ", " << matrix[2][3] << "), "
	   << "(" << matrix[3][0] << ", " << matrix[3][1] << ", " << matrix[3][2]
	   << ", " << matrix[3][3] << ")"
	   << ")";

	return os;
}