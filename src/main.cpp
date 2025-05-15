#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

#include "arrow.hpp"
#include "camera.hpp"
#include "controller.hpp"
#include "mesh.hpp"
#include "pi.hpp"
#include "planet.hpp"
#include "shader.hpp"
#include "simulation.hpp"
#include "skysphere.hpp"
#include "sphere.hpp"
#include "texture.hpp"
#include "timer.hpp"
#include "ui.hpp"
#include "util.hpp"
#include "window.hpp"

void run();

int main() {
    try {
        run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Critical error. Unknown exception\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void run() {
    const int width = 600;
    const int height = 600;

    Window window(width, height);
    Camera camera({0.0f, 0.0f, 5.0f}, 0.0f, -PI_2);

    Controller controller(window, camera);
    Ui ui(window);

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

    Shader normals_shader(DATAPATH("shaders/normals.vert"),
                          DATAPATH("shaders/normals.geom"),
                          DATAPATH("shaders/normals.frag"));

    std::vector<Planet> planets = {{{0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f},
                                    (4.0f / 3.0f) * PI,
                                    0.8f,
                                    1.0f}};

    Timer timer;
    double accumulator = 0.0;
    const double fixed_dt = 0.01;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!window.should_close()) {
        double dt = timer.tick();
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
            glm::perspective(PI_4, controller.aspect_ratio(), 0.1f, 100.0f);

        sphere_shader.use();
        sphere_shader.set_mat4("projection", projection);
        sphere_shader.set_mat4("view", camera.view());
        sphere_shader.set_vec4("color", glm::vec4(1.0f));
        for (const Planet &planet : planets) {
            sphere_shader.set_mat4("model", planet.model());
            sphere_mesh.draw();
        }

        if (ui.show_vectors()) {
            arrow_shader.use();
            arrow_shader.set_mat4("projection", projection);
            arrow_shader.set_mat4("view", camera.view());
            for (const Planet &planet : planets) {
                arrow.draw(planet, arrow_shader);
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

        if (controller.paused()) {
            sphere_shader.use();
            sphere_shader.set_vec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            sphere_shader.set_mat4("model", ui.new_planet().model());
            sphere_mesh.draw();
            if (ui.show_vectors()) {
                arrow_shader.use();
                arrow.draw(ui.new_planet(), arrow_shader);
                normals_shader.use();
                normals_shader.set_mat4("projection", projection);
                normals_shader.set_mat4("view", camera.view());
                arrow.draw(ui.new_planet(), normals_shader);
            }
            ui.draw(camera);
            if (ui.create_new_planet()) {
                planets.push_back(ui.new_planet());
            }
        }

        window.swap_buffers();
        glfwPollEvents();
    }
}
