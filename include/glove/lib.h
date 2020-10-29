/**
 * Main include of library glove.
 *
 * Glove is a bad opengl wrapper library (wrapper, glove, get it? Wow I suck at
 * naming things) meant to make me write more modular code. It should not be
 * used by anyone else due to the fact that it's terrible.
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
#include <glove/AnimatedSpriteSheet.h>
#include <glove/Components.h>
#include <glove/GameState.h>
#include <glove/Model.h>
#include <glove/ResourceStore.h>
#include <glove/ShaderProgram.h>
#include <glove/Texture.h>
#include <glove/VertexBuffer.h>
#include <glove/VertexFormats.h>
#include <glove/Window.h>
#include <glove/utility.h>
