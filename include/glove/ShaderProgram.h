#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>

struct DirectionalLight {
	glm::vec3 color;
	glm::vec3 direction;
	float     specularity;
};

struct UniformSpec {
	GLuint index;    ///< Index of the uniform
	GLenum type;     ///< Type of the uniform
	GLint  size;     ///< Size of the uniform
	GLuint location; ///< Location of the uniform
};

/**
 * A shader program abstraction.
 *
 * Handles loading, compiling and linking shaders into a program.
 */
class ShaderProgram {
  public:
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
	ShaderProgram(const std::initializer_list<std::string> paths);

	/**
	 * Deleting the copy constructor to avoid double frees.
	 * @param other
	 */
	ShaderProgram(const ShaderProgram &other) = delete;

	ShaderProgram(const ShaderProgram &&other) = delete;

	ShaderProgram operator=(const ShaderProgram &other) = delete;

	ShaderProgram operator=(const ShaderProgram &&other) = delete;

	~ShaderProgram();

	/**
	 * Use / Bind the shader program.
	 */
	void use() const;

	void setUniform(const std::string &name, const unsigned int x);

	void setUniform(const std::string &name, const float x);

	void setUniform(const std::string &name, const glm::vec2 v);

	void setUniform(const std::string &name, const glm::vec3 v);

	void setUniform(const std::string &name, const glm::vec4 v);

	void setUniform(const std::string &name, const glm::ivec2 v);

	void setUniform(const std::string &name, const glm::ivec3 v);

	void setUniform(const std::string &name, const glm::ivec4 v);

	void setUniform(const std::string &name, const glm::mat4 v);

	void setUniform(const std::string &name, const DirectionalLight &v);

  private:
	GLuint                                       m_program;
	std::unordered_map<std::string, UniformSpec> m_uniforms;
};
