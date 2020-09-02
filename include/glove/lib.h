/**
 * Main include of library glove.
 *
 * Glove is a bad opengl library meant to make me write more modular code. It
 * should not be used by anyone else due to the fact that it's terrible.
 *
 * @author Dennis Kristiansen, NTNU
 * @file lib.h
 */

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

void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const char *message, const void *userParam);
