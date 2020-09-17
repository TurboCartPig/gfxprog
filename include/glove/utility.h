#pragma once

#include <GL/glew.h>

void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
                              GLenum severity, [[maybe_unused]] GLsizei length,
                              const char *                 message,
                              [[maybe_unused]] const void *userParam);
