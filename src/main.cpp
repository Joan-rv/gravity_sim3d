#include <glm/ext/matrix_clip_space.hpp>

#include "arrow.hpp"
#include "camera.hpp"
#include "controller.hpp"
#include "mesh.hpp"
#include "planet.hpp"
#include "shader.hpp"
#include "simulation.hpp"
#include "skysphere.hpp"
#include "sphere.hpp"
#include "texture.hpp"
#include "ui.hpp"
#include "util.hpp"
#include "window.hpp"

int main() {
    const int width = 600;
    const int height = 600;

    GLFWwindow *window = window_init(width, height);
    if (!window) {
        return -1;
    }

    Camera camera({0.0f, 0.0f, 5.0f}, 0.0f, -M_PI_2);

    // must be constructed before ImGui to properly register callbacks
    Controller controller(window, camera);
    imgui_init(window);

    Mesh sphere_mesh = gen_sphere_mesh(20, 20);
    Shader sphere_shader(DATAPATH("shaders/sphere.vert"),
                         DATAPATH("shaders/sphere.frag"));

    Mesh skysphere_mesh(&skysphere_vertices, sizeof(skysphere_vertices),
                        skysphere_attribs, skysphere_indices);
    Shader skysphere_shader(DATAPATH("shaders/skysphere.vert"),
                            DATAPATH("shaders/skysphere.frag"));
    Texture skysphere_texture(DATAPATH("textures/subdued_blue_nebulae_1.png"));

    Arrow arrow(0.5f, 0.3f, 0.1f, 20);
    Shader arrow_shader(DATAPATH("shaders/arrow.vert"),
                        DATAPATH("shaders/arrow.frag"));
    arrow_shader.use();
    arrow_shader.set_vec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    std::vector<Planet> planets = {{{0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f},
                                    (4.0f / 3.0f) * static_cast<float>(M_PI),
                                    0.8f,
                                    1.0f}};

    bool show_vel_vectors = false;

    double last_time = glfwGetTime();
    double accumulator = 0.0;
    const double fixed_dt = 0.01;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        double curr_time = glfwGetTime();
        double dt = curr_time - last_time;
        last_time = curr_time;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        controller.handle_frame(dt);

        if (!controller.paused()) {
            accumulator += dt;
            while (accumulator >= fixed_dt) {
                sim_update(fixed_dt, planets);
                accumulator -= fixed_dt;
            }
        }

        glm::mat4 projection =
            glm::perspective(static_cast<float>(M_PI_4),
                             controller.aspect_ratio(), 0.1f, 100.0f);

        sphere_shader.use();
        sphere_shader.set_mat4("projection", projection);
        sphere_shader.set_mat4("view", camera.view());
        for (const Planet &planet : planets) {
            sphere_shader.set_mat4("model", planet.model());
            sphere_mesh.draw();
        }

        if (show_vel_vectors) {
            arrow_shader.use();
            arrow_shader.set_mat4("projection", projection);
            arrow_shader.set_mat4("view", camera.view());
            for (const Planet &planet : planets) {
                glm::vec3 origin =
                    planet.position +
                    planet.radius * glm::normalize(planet.velocity);
                glm::vec3 end = origin + planet.velocity;
                arrow.draw(origin, end, arrow_shader);
            }
        }

        glDepthFunc(GL_LEQUAL);
        skysphere_shader.use();
        skysphere_shader.set_mat4("projection", projection);
        skysphere_shader.set_mat4("view", glm::mat4(glm::mat3(camera.view())));
        skysphere_texture.use(0);
        skysphere_shader.set_int("skysphere", 0);
        skysphere_mesh.draw();
        glDepthFunc(GL_LESS);

        imgui_draw(camera, planets, show_vel_vectors);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    imgui_end();

    window_end(window);

    return 0;
}
