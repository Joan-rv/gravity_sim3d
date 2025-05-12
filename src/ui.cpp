#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "camera.hpp"
#include "pi.hpp"
#include "planet.hpp"
#include "ui.hpp"

Ui::Ui(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

bool Ui::draw(const Camera &camera) {
    bool ret;

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
        ImGui::InputScalarN("position", ImGuiDataType_Float,
                            &new_planet_.position, 3);
        ImGui::InputScalarN("initial velocity", ImGuiDataType_Float,
                            &new_planet_.velocity, 3);
        ImGui::InputScalar("radius", ImGuiDataType_Float, &new_planet_.radius);
        static float density = 1.0f;
        ImGui::InputScalar("density", ImGuiDataType_Float, &density);
        float radius_cube =
            new_planet_.radius * new_planet_.radius * new_planet_.radius;
        new_planet_.mass = density * (4.0f / 3.0f) * PI * radius_cube;
        ImGui::Text("Mass: %f", new_planet_.mass);

        ret = ImGui::Button("Add planet");
        ImGui::Separator();
        ImGui::Checkbox("Show velocity vectors", &show_vectors_);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return ret;
}

Planet &Ui::new_planet() { return new_planet_; }
bool Ui::show_vectors() const { return show_vectors_; }

Ui::~Ui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
