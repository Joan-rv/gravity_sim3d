#include <glm/glm.hpp>
#include <vector>

#include "mesh.hpp"

struct SphereVertex {
    glm::vec3 pos;
    glm::vec2 tex_coords;
};

const std::vector<VertexAttribute> sphere_attributes = {
    {0, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex),
     offsetof(SphereVertex, pos)},
    {1, 2, GL_FLOAT, GL_FALSE, sizeof(SphereVertex),
     offsetof(SphereVertex, tex_coords)}};

std::pair<std::vector<SphereVertex>, std::vector<unsigned int>>
sphere_vertices(unsigned int stacks, unsigned int sectors);
