#include <stdexcept>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "debug.hpp"
#include "window.hpp"

Window::Window(int width, int height) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize glfw");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    window_ = glfwCreateWindow(width, height, "gravity_sim3d", NULL, NULL);
    if (window_ == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to create glfw window");
    }
    glfwMakeContextCurrent(window_);

    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to load glad");
    }

    opengl_debug_setup();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

Window::~Window() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}

bool Window::should_close() const { return glfwWindowShouldClose(window_); }
void Window::swap_buffers() { glfwSwapBuffers(window_); }

GLFWwindow *Window::ptr() { return window_; }
