#include "simulation.hpp"

#define G 1.0f

void sim_update(float dt, std::vector<Planet> &planets) {
    // compute gravitational force
    for (size_t i = 0; i < planets.size(); i++) {
        Planet &p1 = planets[i];
        for (size_t j = i + 1; j < planets.size(); j++) {
            Planet &p2 = planets[j];
            glm::vec3 vec = p2.position - p1.position;
            float dist_sq = glm::dot(vec, vec);
            if (dist_sq < 1e-8)
                continue;

            glm::vec3 force =
                G * p1.mass * p2.mass / dist_sq * glm::normalize(vec);
            p1.forces += force;
            p2.forces -= force;
        }
    }

    // apply forces
    for (Planet &p : planets) {
        p.velocity += dt * p.forces / p.mass;
        p.position += dt * p.velocity;
        p.forces = glm::vec3(0.0f);
    }

    // resolve collisions
    bool done = false;
    for (size_t k = 0; k < 5 && !done; k++) {
        done = true;
        for (size_t i = 0; i < planets.size(); i++) {
            Planet &p1 = planets[i];
            for (size_t j = i + 1; j < planets.size(); j++) {
                Planet &p2 = planets[j];

                float dist = glm::distance(p1.position, p2.position) -
                             p1.radius - p2.radius;
                if (dist < 0.0f) {
                    done = false;
                    glm::vec3 normal = p2.position - p1.position;
                    normal = glm::normalize(normal);
                    glm::vec3 correction = 0.2f * -dist * normal;
                    p1.position -= correction;
                    p2.position += correction;

                    glm::vec3 relative_vel = p2.velocity - p1.velocity;
                    float normal_vel = glm::dot(relative_vel, normal);
                    if (normal_vel > 0.0f)
                        continue;

                    float inv_mass1 = 1.0f / p1.mass;
                    float inv_mass2 = 1.0f / p2.mass;
                    float restitution =
                        std::min(p1.restitution, p2.restitution);
                    float j = -(1.0f + restitution) * normal_vel;
                    j /= inv_mass1 + inv_mass2;

                    glm::vec3 impulse = j * normal;
                    p1.velocity -= impulse * inv_mass1;
                    p2.velocity += impulse * inv_mass2;
                }
            }
        }
    }
}
