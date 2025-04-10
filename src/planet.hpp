#ifndef PLANET_HPP
#define PLANET_HPP

#include <glm/glm.hpp>

struct Planet {
    glm::mat4 model() const;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 forces;
    float mass;
    float restitution;
    float radius;
};

#endif
