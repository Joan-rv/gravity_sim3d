#ifndef UI_HPP
#define UI_HPP

#include <GLFW/glfw3.h>

#include "planet.hpp"

class Camera;
class Window;

class Ui {
public:
    Ui(Window &window);
    Ui(const Ui &) = delete;
    Ui &operator=(const Ui &) = delete;
    ~Ui();

    bool draw(const Camera &camera);
    Planet &new_planet();
    bool show_vectors() const;

private:
    Planet new_planet_ = {
        glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.8f, 1.0f};
    bool show_vectors_;
};

#endif
