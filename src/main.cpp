#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "camera.hpp"
#include "controller.hpp"
#include "debug.hpp"
#include "mesh.hpp"
#include "planet.hpp"
#include "shader.hpp"
#include "simulation.hpp"
#include "sphere.hpp"

const int width = 600;
const int height = 600;

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
    Camera camera({0.0f, 0.0f, 5.0f}, 0.0f, -M_PI_2);
    Controller::init(window, &camera);

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

    std::vector<Planet> planets = {{{0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f},
                                    static_cast<float>(M_PI),
                                    1.0f,
                                    1.0f}};

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

        if (!Controller::paused())
            sim_update(dt, planets);

        glm::mat4 projection =
            glm::perspective(static_cast<float>(M_PI_4),
                             Controller::aspect_ratio(), 0.1f, 100.0f);
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", camera.view());
        for (const Planet &planet : planets) {
            shader.set_mat4("model", planet.model());
            sphere_mesh.draw();
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Simulation control window");
            ImGui::Text("Add new planet");
            static float pos[3] = {0};
            ImGui::InputScalarN("position", ImGuiDataType_Float, &pos, 3);
            static float radius = 1.0f;
            ImGui::InputScalar("radius", ImGuiDataType_Float, &radius);
            if (ImGui::Button("Add planet")) {
                planets.push_back({{pos[0], pos[1], pos[2]},
                                   {0.0f, 0.0f, 0.0f},
                                   {0.0f, 0.0f, 0.0f},
                                   static_cast<float>(M_PI) * radius * radius,
                                   1.0f,
                                   radius});
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
