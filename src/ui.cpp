#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "camera.hpp"
#include "planet.hpp"
#include "ui.hpp"

void imgui_init(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void imgui_draw(const Camera &camera, std::vector<Planet> &planets,
                bool &show_vel_vectors) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("Simulation control window");
        ImGui::SeparatorText("Stats");
        glm::vec3 cam_pos = camera.position();
        ImGui::Text("Camera position: %.2f %.2f %.2f", cam_pos[0], cam_pos[1],
                    cam_pos[2]);
        ImGui::SeparatorText("Add new planet");
        static float pos[3] = {0};
        ImGui::InputScalarN("position", ImGuiDataType_Float, &pos, 3);
        static float v_ini[3] = {0};
        ImGui::InputScalarN("initial velocity", ImGuiDataType_Float, &v_ini, 3);
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
        ImGui::Separator();
        ImGui::Checkbox("Show velocity vectors", &show_vel_vectors);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_end() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
