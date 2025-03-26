#include "sphere.hpp"

std::pair<std::vector<SphereVertex>, std::vector<unsigned int>>
sphere_vertices(unsigned int stacks, unsigned int sectors) {
    // Based on https://www.songho.ca/opengl/gl_sphere.html
    assert(stacks >= 2 && sectors >= 3);
    std::vector<SphereVertex> vertices;
    vertices.reserve((stacks + 1) * (sectors + 1));
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

    // Note that the index for the vertex at some stack and sector is:
    // `stack * (sectors + 1) + sector` (ie sequential matrix access)
    std::vector<unsigned int> indices;
    indices.reserve(3 * (2 * stacks * sectors - 2 * sectors));
    for (unsigned int stack = 0; stack < stacks; stack++) {
        for (unsigned int sector = 0; sector < sectors; sector++) {
            if (stack + 1 != stacks) {
                indices.push_back(stack * (sectors + 1) + sector);
                indices.push_back((stack + 1) * (sectors + 1) + sector + 1);
                indices.push_back((stack + 1) * (sectors + 1) + sector);
            }
            if (stack != 0) {
                indices.push_back(stack * (sectors + 1) + sector);
                indices.push_back(stack * (sectors + 1) + sector + 1);
                indices.push_back((stack + 1) * (sectors + 1) + sector + 1);
            }
        }
    }

    return {vertices, indices};
}
