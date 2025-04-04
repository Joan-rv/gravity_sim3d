#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "camera.hpp"
#include "debug.hpp"
#include "mesh.hpp"
#include "planet.hpp"
#include "shader.hpp"
#include "sphere.hpp"
#include "util.hpp"

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height);

Camera camera({0.0f, 0.0f, -5.0f}, 0.0f, M_PI_2);
void glfw_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action,
                       int mods);
void glfw_mouse_button_callback(GLFWwindow *window, int button, int action,
                                int mods);
bool cursor_shown = false;

const int width = 600;
const int height = 600;
float aspect_ratio = static_cast<float>(width) / height;

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow *window =
        glfwCreateWindow(width, height, "gravity_sim3d", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create glfw window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to load glad\n";
        glfwTerminate();
        return -1;
    }

    opengl_debug_setup();

    auto [vertices, indices] = sphere_vertices(10, 10);
    Mesh sphere_mesh(&vertices[0], vertices.size() * sizeof(SphereVertex),
                     sphere_attributes, indices);

    std::vector<Planet> planets = {{{0.0f, 0.0f, 0.0f}, 1.0f}};
    float new_sphere_pos[3] = {0};

    Shader shader("../src/sphere.vert", "../src/sphere.frag");

    shader.use();
    double last_time = glfwGetTime();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        double curr_time = glfwGetTime();
        double dt = curr_time - last_time;
        last_time = curr_time;
        glClear(GL_COLOR_BUFFER_BIT);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.move_forward(dt);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.move_backward(dt);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.move_left(dt);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.move_right(dt);
        }

        glm::mat4 projection =
            glm::perspective(static_cast<float>(M_PI_4), 1.0f, 0.1f, 100.0f);
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", camera.view());
        for (Planet planet : planets) {
            shader.set_mat4("model", planet.model());
            sphere_mesh.draw();
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Simulation control window");
            ImGui::Text("Position");
            ImGui::InputScalarN("##position", ImGuiDataType_Float,
                                &new_sphere_pos, 3);
            if (ImGui::Button("Add planet")) {
                planets.push_back(
                    {{new_sphere_pos[0], new_sphere_pos[1], new_sphere_pos[2]},
                     1.0f});
            }
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    UNUSED(window);
    glViewport(0, 0, width, height);
    aspect_ratio = static_cast<float>(width) / height;
}

void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action,
                       int mods) {
    UNUSED(window);
    UNUSED(scancode);
    UNUSED(mods);
    ImGuiIO &io = ImGui::GetIO();
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        cursor_shown = true;
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (io.WantCaptureKeyboard)
        return;
}

void glfw_mouse_button_callback(GLFWwindow *window, int button, int action,
                                int mods) {
    UNUSED(window);
    UNUSED(mods);
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cursor_shown = false;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void glfw_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    UNUSED(window);
    constexpr float turn_speed = 0.01f;
    static bool first = true;
    static double last_xpos;
    static double last_ypos;
    if (!first && !cursor_shown) {
        float dy = -(ypos - last_ypos);
        float dx = xpos - last_xpos;
        camera.pitch(camera.pitch() + dy * turn_speed);
        camera.yaw(camera.yaw() + dx * turn_speed);
    } else if (first) {
        first = false;
    }
    last_xpos = xpos;
    last_ypos = ypos;
}
