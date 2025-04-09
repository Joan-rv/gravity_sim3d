#include "simulation.hpp"

#define G 1.0f

void sim_update(float dt, std::vector<Planet> &planets) {
    for (size_t i = 0; i < planets.size(); i++) {
        for (size_t j = i + 1; j < planets.size(); j++) {
            Planet &p1 = planets[i];
            Planet &p2 = planets[j];
            glm::vec3 vec = p2.position - p1.position;

            glm::vec3 force = G * p1.mass * p2.mass / glm::dot(vec, vec) *
                              glm::normalize(vec);
            p1.forces += force;
            p2.forces -= force;
        }
    }

    for (Planet &p : planets) {
        p.velocity += dt * p.forces / p.mass;
        p.position += dt * p.velocity;
        p.forces = glm::vec3(0.0f);
    }
}
