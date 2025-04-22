#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>

// Returns NULL on error
GLFWwindow *window_init(int width, int height);

void window_end(GLFWwindow *window);

#endif
