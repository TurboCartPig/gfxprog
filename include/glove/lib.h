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

// Reexport internal headers
#include <glove/ShaderProgram.h>
#include <glove/VertexBufferObject.h>
#include <glove/VertexFormats.h>
#include <glove/Window.h>

void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const char *message, const void *userParam);
