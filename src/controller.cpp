#include "controller.hpp"
#include "imgui.h"
#include "util.hpp"

Camera *Controller::camera_ = nullptr;
float Controller::aspect_ratio_ = 1.0f;
bool Controller::cursor_shown_ = false;

void Controller::init(GLFWwindow *window, Camera *camera) {
    camera_ = camera;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    aspect_ratio_ = static_cast<float>(width) / height;
    glfwSetCursorPosCallback(window, cursor_pos_callback_);
    glfwSetKeyCallback(window, key_callback_);
    glfwSetMouseButtonCallback(window, mouse_button_callback_);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

float Controller::aspect_ratio() { return aspect_ratio_; }

void Controller::framebuffer_size_callback_(GLFWwindow *window, int width,
                                            int height) {
    UNUSED(window);
    glViewport(0, 0, width, height);
    aspect_ratio_ = static_cast<float>(width) / height;
}

void Controller::key_callback_(GLFWwindow *window, int key, int scancode,
                               int action, int mods) {
    UNUSED(window);
    UNUSED(scancode);
    UNUSED(mods);
    ImGuiIO &io = ImGui::GetIO();
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        cursor_shown_ = true;
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (io.WantCaptureKeyboard)
        return;
}

void Controller::mouse_button_callback_(GLFWwindow *window, int button,
                                        int action, int mods) {
    UNUSED(window);
    UNUSED(mods);
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cursor_shown_ = false;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void Controller::cursor_pos_callback_(GLFWwindow *window, double xpos,
                                      double ypos) {
    UNUSED(window);
    assert(camera_);
    constexpr float turn_speed = 0.01f;
    static bool first = true;
    static double last_xpos;
    static double last_ypos;
    if (!first && !cursor_shown_) {
        float dy = -(ypos - last_ypos);
        float dx = xpos - last_xpos;
        camera_->pitch(camera_->pitch() + dy * turn_speed);
        camera_->yaw(camera_->yaw() + dx * turn_speed);
    } else if (first) {
        first = false;
    }
    last_xpos = xpos;
    last_ypos = ypos;
}
