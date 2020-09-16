#include <filesystem>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glove/ShaderProgram.h>
#include <iostream>

ShaderProgram::ShaderProgram(
    const std::initializer_list<const std::string> paths) {
	// Load all shaders from disk and compile them
	std::vector<GLuint> shaders;

	for (const auto &path : paths) {
		// Open the file for reading
		std::ifstream file(path, std::ifstream::in | std::ifstream::binary);
		if (file.bad() || !file.is_open()) {
			throw std::runtime_error("Error: Failed to open file.");
		}

		// Get the file size in bytes
		const auto file_size = std::filesystem::file_size(path);
		// Preallocate a zeroed string
		std::string source(file_size, '\0');
		// Read in the whole file to the preallocated string
		file.read(source.data(), file_size);

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
		GLint       success;
		std::string log(512, '\0');
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		glGetShaderInfoLog(shader, log.capacity(), nullptr, log.data());

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
	m_program = glCreateProgram();
	for (const auto shader : shaders)
		glAttachShader(m_program, shader);

	glLinkProgram(m_program);

	// Get the link status
	GLint       success;
	std::string log(512, '\0');
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(m_program, log.capacity(), nullptr, log.data());

	std::cout << "Linking Program: (id: " << m_program << ")" << std::endl;
	if (success == GL_FALSE) {
		std::cout << "\tLinking status: " << (bool)success << std::endl;
		std::cout << "\tLinking log:\n" << log << std::endl;

		throw std::runtime_error("GL Error: Failed to link program");
	}

	for (const auto shader : shaders) {
		glDetachShader(m_program, shader);
		glDeleteShader(shader);
	}

	// TODO: Find a way to obtain the vertex specification from the compiled
	// shaders

	GLint               num_active_uniforms;
	std::vector<GLenum> props = {GL_NAME_LENGTH, GL_TYPE, GL_ARRAY_SIZE,
	                             GL_LOCATION};
	std::vector<GLint>  values(props.size());
	// Query the number of active uniforms in the current program
	glGetProgramInterfaceiv(m_program, GL_UNIFORM, GL_ACTIVE_RESOURCES,
	                        &num_active_uniforms);

	// Iterate over the uniforms in the current program
	for (GLuint i = 0; i < (GLuint)num_active_uniforms; i++) {
		// Query the program for properties specified by 'props'
		glGetProgramResourceiv(m_program, GL_UNIFORM, i, props.size(),
		                       props.data(), values.size(), nullptr,
		                       values.data());
		// Preallocate string
		std::string uniform_name(values[0], '\0');

		// Query for the name of the current uniform
		glGetProgramResourceName(m_program, GL_UNIFORM, i, uniform_name.size(),
		                         nullptr, uniform_name.data());
		// Remove '\0'
		uniform_name.pop_back();

		// Store the uniform specification for later use
		m_uniforms.insert(
		    {uniform_name,
		     UniformSpec{i, static_cast<GLuint>(values[1]), values[2],
		                 static_cast<GLuint>(values[3])}});
	}
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(m_program); }

void ShaderProgram::use() const { glUseProgram(m_program); }

void ShaderProgram::setUniform(const std::string &name, const float x) {
	glUniform1f(m_uniforms.at(name).location, x);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec2 v) {
	glUniform2f(m_uniforms.at(name).location, v.x, v.y);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec3 v) {
	glUniform3f(m_uniforms.at(name).location, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec4 v) {
	glUniform4f(m_uniforms.at(name).location, v.x, v.y, v.z, v.w);
}

void ShaderProgram::setUniform(const std::string &name, const glm::mat4 v) {
	glUniformMatrix4fv(m_uniforms.at(name).location, 1, GL_FALSE,
	                   glm::value_ptr(v));
}
