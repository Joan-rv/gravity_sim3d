#include <vector>

#include "mesh.hpp"
#include "pi.hpp"
#include "sphere.hpp"

struct SphereVertex {
    glm::vec3 pos;
    glm::vec2 tex_coords;
};

const std::vector<VertexAttribute> sphere_attributes = {
    {0, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex),
     offsetof(SphereVertex, pos)},
    {1, 2, GL_FLOAT, GL_FALSE, sizeof(SphereVertex),
     offsetof(SphereVertex, tex_coords)}};

Mesh gen_sphere_mesh(unsigned int stacks, unsigned int sectors) {
    // Based on https://www.songho.ca/opengl/gl_sphere.html
    assert(stacks >= 2 && sectors >= 3);
    std::vector<SphereVertex> vertices;
    vertices.reserve((stacks + 1) * (sectors + 1));
    for (unsigned int stack = 0; stack <= stacks; stack++) {
        double psi = PI * stack / stacks - PI_2;
        double y = sin(psi);
        for (unsigned int sector = 0; sector <= sectors; sector++) {
            double theta = 2.0 * PI * sector / sectors;
            double x = sin(theta) * cos(psi);
            double z = cos(theta) * cos(psi);
            glm::vec3 pos = {x, y, z};
            glm::vec2 tex_coords = {
                static_cast<float>(sector) / sectors,
                static_cast<float>(stack) / stacks,
            };
            vertices.push_back({pos, tex_coords});
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

    return Mesh(&vertices[0], vertices.size() * sizeof(SphereVertex),
                sphere_attributes, indices);
}
