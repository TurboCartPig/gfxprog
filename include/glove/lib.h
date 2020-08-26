/**
 * Main include of library "lib".
 *
 * @author Dennis Kristiansen, NTNU
 * @file lib.h
 */

#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>

/**
 * A shader program abstraction.
 *
 * Handles loading, compiling and linking shaders into a program.
 */
class ShaderProgram {
  public:
	explicit ShaderProgram(std::vector<std::string> &paths);
	~ShaderProgram();
	void   use();
	GLuint getProgram() const { return program; }

  private:
	GLuint program;
};

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const char *message, const void *userParam);
