#include "sphere.hpp"

std::pair<std::vector<SphereVertex>, std::vector<unsigned int>>
sphere_vertices(unsigned int stacks, unsigned int sectors) {
    std::vector<SphereVertex> vertices;
    for (unsigned int stack = 0; stack <= stacks; stack++) {
        double psi = M_PI * stack / stacks - M_PI_2;
        double y = sin(psi);
        for (unsigned int sector = 0; sector <= sectors; sector++) {
            double theta = 2.0 * M_PI * sector / sectors;
            double x = sin(theta) * cos(psi);
            double z = cos(theta) * cos(psi);
            glm::vec3 pos = {x, y, z};
            glm::vec2 tex_coords = {
                static_cast<float>(sector) / sectors,
                static_cast<float>(stack) / stacks,
            };
            vertices.push_back({.pos = pos, .tex_coords = tex_coords});
        }
    }

    std::vector<unsigned int> indices;
    for (unsigned int stack = 0; stack < stacks; stack++) {
        for (unsigned int sector = 0; sector < sectors; sector++) {
            indices.push_back(stack * (stacks + 1) + sector);
            indices.push_back((stack + 1) * (stacks + 1) + sector + 1);
            indices.push_back((stack + 1) * (stacks + 1) + sector);
            if (stack != 0 && stack + 1 != stacks) {
                indices.push_back(stack * (stacks + 1) + sector);
                indices.push_back(stack * (stacks + 1) + sector + 1);
                indices.push_back((stack + 1) * (stacks + 1) + sector + 1);
            }
        }
    }

    return {vertices, indices};
}
