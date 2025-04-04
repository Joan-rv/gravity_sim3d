#include "planet.hpp"
#include <glm/ext/matrix_transform.hpp>

glm::mat4 Planet::model() const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius));
    return model;
}
