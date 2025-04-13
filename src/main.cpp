#include "arrow.hpp"
#include "util.hpp"
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
#include "skysphere.hpp"
#include "sphere.hpp"
#include "texture.hpp"

const int width = 600;
const int height = 600;

const float movement_speed = 2.0f;

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
    Controller controller(window, camera);

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Mesh sphere_mesh = gen_sphere_mesh(20, 20);
    Shader sphere_shader(DATAPATH("shaders/sphere.vert"),
                         DATAPATH("shaders/sphere.frag"));

    Mesh skysphere_mesh(&skysphere_vertices, sizeof(skysphere_vertices),
                        skysphere_attribs, skysphere_indices);
    Shader skysphere_shader(DATAPATH("shaders/skysphere.vert"),
                            DATAPATH("shaders/skysphere.frag"));
    Texture skysphere_texture(DATAPATH("textures/subdued_blue_nebulae_1.png"));

    Mesh arrow_mesh = gen_arrow(0.2, 0.2, 20);

    /*
    std::vector<Planet> planets = {{{0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f},
                                    static_cast<float>(M_PI),
                                    0.8f,
                                    1.0f}};
    */
    std::vector<Planet> planets;

    double last_time = glfwGetTime();
    double accumulator = 0.0;
    const double fixed_dt = 0.01;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        double curr_time = glfwGetTime();
        double dt = curr_time - last_time;
        accumulator += dt;
        last_time = curr_time;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.move_forward(movement_speed * dt);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.move_backward(movement_speed * dt);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.move_left(movement_speed * dt);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.move_right(movement_speed * dt);
        }

        while (!controller.paused() && accumulator >= fixed_dt) {
            sim_update(fixed_dt, planets);
            accumulator -= fixed_dt;
        }

        sphere_shader.use();
        glm::mat4 projection =
            glm::perspective(static_cast<float>(M_PI_4),
                             controller.aspect_ratio(), 0.1f, 100.0f);
        sphere_shader.set_mat4("projection", projection);
        sphere_shader.set_mat4("view", camera.view());
        for (const Planet &planet : planets) {
            sphere_shader.set_mat4("model", planet.model());
            sphere_mesh.draw();
        }

        sphere_shader.set_mat4("model", glm::mat4(1.0f));
        arrow_mesh.draw();

        glDepthFunc(GL_LEQUAL);
        skysphere_shader.use();
        skysphere_shader.set_mat4("projection", projection);
        skysphere_shader.set_mat4("view", glm::mat4(glm::mat3(camera.view())));
        skysphere_texture.use(0);
        skysphere_shader.set_int("skysphere", 0);
        skysphere_mesh.draw();
        glDepthFunc(GL_LESS);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Simulation control window");
            ImGui::Text("Add new planet");
            static float pos[3] = {0};
            ImGui::InputScalarN("position", ImGuiDataType_Float, &pos, 3);
            static float v_ini[3] = {0};
            ImGui::InputScalarN("initial velocity", ImGuiDataType_Float, &v_ini,
                                3);
            static float radius = 1.0f;
            ImGui::InputScalar("radius", ImGuiDataType_Float, &radius);
            static float density = 1.0f;
            ImGui::InputScalar("density", ImGuiDataType_Float, &density);
            ImGui::Text("Mass: %f",
                        density * static_cast<float>(M_PI) * radius * radius);

            if (ImGui::Button("Add planet")) {
                planets.push_back(
                    {{pos[0], pos[1], pos[2]},
                     {v_ini[0], v_ini[1], v_ini[2]},
                     {0.0f, 0.0f, 0.0f},
                     density * static_cast<float>(M_PI) * radius * radius,
                     0.8f,
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
