#include <GLFW/glfw3.h>
#include <imgui.h>

#include "camera.hpp"
#include "controller.hpp"
#include "util.hpp"

Controller::Controller(GLFWwindow *window, Camera &camera)
    : window_(window), camera_(camera), paused_(false) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    aspect_ratio_ = static_cast<float>(width) / height;
    glfwSetCursorPosCallback(window, glfw_cursor_pos_callback_);
    glfwSetKeyCallback(window, glfw_key_callback_);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback_);
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback_);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, this);
}

float Controller::aspect_ratio() const { return aspect_ratio_; }

bool Controller::paused() const { return paused_; }

void Controller::handle_frame(double dt) {
    const float movement_speed = 2.0f;
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        camera_.move_forward(movement_speed * dt);
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        camera_.move_backward(movement_speed * dt);
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        camera_.move_left(movement_speed * dt);
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        camera_.move_right(movement_speed * dt);
    }
}

void Controller::framebuffer_size_callback_(int width, int height) {
    glViewport(0, 0, width, height);
    aspect_ratio_ = static_cast<float>(width) / height;
}
void Controller::glfw_framebuffer_size_callback_(GLFWwindow *window, int width,
                                                 int height) {
    Controller *self =
        static_cast<Controller *>(glfwGetWindowUserPointer(window));
    self->framebuffer_size_callback_(width, height);
}

void Controller::key_callback_(GLFWwindow *window, int key, int scancode,
                               int action, int mods) {
    UNUSED(scancode);
    UNUSED(mods);
    ImGuiIO &io = ImGui::GetIO();
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        paused_ = true;
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (io.WantCaptureKeyboard)
        return;
}
void Controller::glfw_key_callback_(GLFWwindow *window, int key, int scancode,
                                    int action, int mods) {
    Controller *self =
        static_cast<Controller *>(glfwGetWindowUserPointer(window));
    self->key_callback_(window, key, scancode, action, mods);
}

void Controller::mouse_button_callback_(GLFWwindow *window, int button,
                                        int action, int mods) {
    UNUSED(window);
    UNUSED(mods);
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        paused_ = false;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}
void Controller::glfw_mouse_button_callback_(GLFWwindow *window, int button,
                                             int action, int mods) {
    Controller *self =
        static_cast<Controller *>(glfwGetWindowUserPointer(window));
    self->mouse_button_callback_(window, button, action, mods);
}

void Controller::cursor_pos_callback_(double xpos, double ypos) {
    constexpr float turn_speed = 0.01f;
    static bool first = true;
    static double last_xpos;
    static double last_ypos;
    if (!first && !paused_) {
        float dy = -(ypos - last_ypos);
        float dx = xpos - last_xpos;
        camera_.pitch(camera_.pitch() + dy * turn_speed);
        camera_.yaw(camera_.yaw() + dx * turn_speed);
    } else if (first) {
        first = false;
    }
    last_xpos = xpos;
    last_ypos = ypos;
}
void Controller::glfw_cursor_pos_callback_(GLFWwindow *window, double xpos,
                                           double ypos) {
    Controller *self =
        static_cast<Controller *>(glfwGetWindowUserPointer(window));
    self->cursor_pos_callback_(xpos, ypos);
}
