#ifndef UI_HPP
#define UI_HPP

#include <vector>

#include <GLFW/glfw3.h>

class Camera;
struct Planet;

void imgui_init(GLFWwindow *window);
void imgui_draw(const Camera &camera, std::vector<Planet> &planets,
                bool &show_vel_vectors);
void imgui_end();

#endif
