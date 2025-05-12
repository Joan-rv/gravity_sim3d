#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height);
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    ~Window();

    bool should_close() const;
    void swap_buffers();

    GLFWwindow *ptr();

private:
    GLFWwindow *window_;
};

#endif
