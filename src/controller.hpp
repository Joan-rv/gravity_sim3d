#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <GLFW/glfw3.h>

class Camera;

class Controller {
public:
    static void init(GLFWwindow *window, Camera *camera);
    static float aspect_ratio();
    static bool paused();

private:
    static Camera *camera_;
    static float aspect_ratio_;
    static bool paused_;
    static void framebuffer_size_callback_(GLFWwindow *window, int width,
                                           int height);
    static void key_callback_(GLFWwindow *window, int key, int scancode,
                              int action, int mods);
    static void mouse_button_callback_(GLFWwindow *window, int button,
                                       int action, int mods);
    static void cursor_pos_callback_(GLFWwindow *window, double xpos,
                                     double ypos);
};

#endif
