#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

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
	explicit ShaderProgram(const std::vector<std::string> &paths);

	~ShaderProgram();

	void use();

	void setUniform(const std::string &name, const float x);

	void setUniform(const std::string &name, const glm::vec2);

	void setUniform(const std::string &name, const glm::vec3);

	void setUniform(const std::string &name, const glm::vec4);

	GLuint getProgram() const { return m_program; }

  private:
	GLuint m_program;
};
