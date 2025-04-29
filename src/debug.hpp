#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <glad/gl.h>

void opengl_debug_setup();
void glfw_error_callback(int error, const char *description);
void GLAPIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id,
                                GLenum severity, GLsizei length,
                                const char *message, const void *userParam);

#endif
