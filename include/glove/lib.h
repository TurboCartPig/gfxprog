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

// Common imports from libraries
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Reexport internal headers
#include <glove/ShaderProgram.h>
#include <glove/Texture.h>
#include <glove/VertexBufferObject.h>
#include <glove/VertexFormats.h>
#include <glove/Window.h>
#include <glove/utility.h>
