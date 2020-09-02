#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

/**
 * A shader program abstraction.
 *
 * Handles loading, compiling and linking shaders into a program.
 */
class ShaderProgram {
  public:
	explicit ShaderProgram(const std::vector<std::string> &paths);
	~ShaderProgram();
	void   use();
	GLuint getProgram() const { return program; }

  private:
	GLuint program;
};
