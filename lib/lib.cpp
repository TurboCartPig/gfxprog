#include "glove/lib.h"

#include <fstream>
#include <iostream>

/**
 * Construct a shader program from multiple shader stages.
 *
 * The shaders stage is determent based on the file extension.
 * Eg. .vert for vertex shader and .geom for geometry shader.
 *
 * @note Does not handle being passed multiple shaders for the same stage.
 * @note Does not handle tessellation or compute shaders
 *
 * @param paths An array of paths to the shader source code.
 */
ShaderProgram::ShaderProgram(std::vector<std::string> &paths) {
	// Load all shaders from disk and compile them
	std::vector<GLuint> shaders;
	shaders.reserve(paths.size());
	for (const auto &path : paths) {
		// Open the file for reading
		std::ifstream file(path, std::ifstream::in);
		if (file.bad() || !file.is_open()) {
			throw std::runtime_error("Error: Failed to open file.");
		}

		// Read in the file line by line
		std::string source;
		std::string line;
		while (getline(file, line)) {
			source.append(line + "\n");
		}
		// Why can I not do this?
		// file >> source;
		file.close();

		// Find the shader type
		GLenum type;
		if (path.find(".vert") != std::string::npos)
			type = GL_VERTEX_SHADER;
		else if (path.find(".geom") != std::string::npos)
			type = GL_GEOMETRY_SHADER;
		else if (path.find(".frag") != std::string::npos)
			type = GL_FRAGMENT_SHADER;
		else {
			std::cout << "Error: Shader with unknown file extension: " << path
			          << std::endl;
			throw std::runtime_error("GL Error: Unknown file extension.");
		}

		// Compile the shader
		const char *const cstring = source.c_str();
		GLuint            shader  = glCreateShader(type);
		glShaderSource(shader, 1, &cstring, nullptr);
		glCompileShader(shader);

		// Get the compile status
		GLint success;
		char  log[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		glGetShaderInfoLog(shader, 512, nullptr, log);

		std::cout << "Compiling shader: " << path << std::endl;
		if (success == GL_FALSE) {
			std::cout << "\tShader type: " << (int)type << std::endl;
			std::cout << "\tSource code:\n" << source << std::endl;
			std::cout << "\tCompilation status: " << (bool)success << std::endl;
			std::cout << "\tCompilation log:\n" << log << std::endl;

			throw std::runtime_error("GL Error: Shader failed to compile");
		}

		shaders.push_back(shader);
	}

	// Build and link a shader program
	program = glCreateProgram();
	for (const auto shader : shaders)
		glAttachShader(program, shader);

	glLinkProgram(program);

	// Get the link status
	GLint success;
	char  log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(program, 512, nullptr, log);

	std::cout << "Linking Program: " << std::endl;
	if (success == GL_FALSE) {
		std::cout << "\tLinking status: " << (bool)success << std::endl;
		std::cout << "\tLinking log:\n" << log << std::endl;

		throw std::runtime_error("GL Error: Failed to link program");
	}

	for (const auto shader : shaders)
		glDeleteShader(shader);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(program); }

/**
 * Use this shader program for future graphics processing.
 */
void ShaderProgram::use() { glUseProgram(program); }

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const char *message, const void *userParam) {
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
	std::cout << std::endl;
}
