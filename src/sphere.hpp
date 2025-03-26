#include <glm/glm.hpp>
#include <vector>

struct SphereVertex {
    glm::vec3 pos;
    glm::vec2 tex_coords;
};

std::pair<std::vector<SphereVertex>, std::vector<unsigned int>>
sphere_vertices(unsigned int stacks, unsigned int sectors);
