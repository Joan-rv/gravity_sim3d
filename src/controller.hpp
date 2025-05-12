#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <GLFW/glfw3.h>

class Camera;
class Window;

class Controller {
public:
    Controller(Window &window, Camera &camera);
    Controller(const Controller &) = delete;
    Controller &operator=(const Controller &) = delete;
    float aspect_ratio() const;
    bool paused() const;
    void handle_frame(double dt);

private:
    GLFWwindow *window_;
    Camera &camera_;
    float aspect_ratio_;
    bool paused_;
    void framebuffer_size_callback_(int width, int height);
    static void glfw_framebuffer_size_callback_(GLFWwindow *window, int width,
                                                int height);
    void key_callback_(GLFWwindow *window, int key, int scancode, int action,
                       int mods);
    static void glfw_key_callback_(GLFWwindow *window, int key, int scancode,
                                   int action, int mods);
    void mouse_button_callback_(GLFWwindow *window, int button, int action,
                                int mods);
    static void glfw_mouse_button_callback_(GLFWwindow *window, int button,
                                            int action, int mods);
    void cursor_pos_callback_(double xpos, double ypos);
    static void glfw_cursor_pos_callback_(GLFWwindow *window, double xpos,
                                          double ypos);
};

#endif
