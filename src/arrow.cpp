#include <glm/glm.hpp>

#include "arrow.hpp"
#include "mesh.hpp"

Mesh gen_arrow(float tip_size_ratio, float stem_size_ratio, size_t resolution) {
    /*
     *                     top_outer
     *  bottom_outer       v
     *  v                  |\
     *  -------------------| \
     *  |                  ^  \
     *  |           top_inner  \
     *  | < bottom_center       | < tip,   pointing to x+
     *  |                      /
     *  |                     /
     *  -------------------| /
     *                     |/
     *
     *  vertices[] = {bottom_center, bottom_outer, top_inner, top_outer, tip}
     */

    std::vector<glm::vec3> vertices;
    vertices.reserve(2 + 3 * (resolution + 1));
    vertices.push_back({-1.0f, 0.0f, 0.0f}); // bottom_center
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * M_PI * i / resolution;
        vertices.push_back({-1.0f, stem_size_ratio * sin(angle),
                            stem_size_ratio * cos(angle)});
    }
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * M_PI * i / resolution;
        vertices.push_back({1.0f - 2 * tip_size_ratio,
                            stem_size_ratio * sin(angle),
                            stem_size_ratio * cos(angle)});
    }
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * M_PI * i / resolution;
        vertices.push_back({1.0f - 2 * tip_size_ratio, sin(angle), cos(angle)});
    }
    vertices.push_back({1.0f, 0.0f, 0.0f});

    assert(2 + 3 * (resolution + 1) == vertices.size());

    std::vector<unsigned int> indices;
    indices.reserve(6 * 3 * resolution);
    const int bot_center_idx = 0;
    const int bot_out_base = 1;
    const int top_in_base = 1 + resolution + 1;
    const int top_out_base = 1 + 2 * (resolution + 1);
    const int tip_idx = 1 + 3 * (resolution + 1);

    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(bot_center_idx);
        indices.push_back(bot_out_base + i);
        indices.push_back(bot_out_base + i + 1);
    }
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(bot_out_base + i);
        indices.push_back(top_in_base + i);
        indices.push_back(bot_out_base + i + 1);

        indices.push_back(top_in_base + i);
        indices.push_back(top_in_base + i + 1);
        indices.push_back(bot_out_base + i + 1);
    }
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(top_in_base + i);
        indices.push_back(top_out_base + i);
        indices.push_back(top_in_base + i + 1);

        indices.push_back(top_out_base + i);
        indices.push_back(top_out_base + i + 1);
        indices.push_back(top_in_base + i + 1);
    }
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(top_out_base + i);
        indices.push_back(tip_idx);
        indices.push_back(top_out_base + i + 1);
    }

    assert(6 * 3 * resolution == indices.size());

    const std::vector<VertexAttribute> attribs = {
        {0, 3, GL_FLOAT, false, sizeof(glm::vec3), 0}};
    return Mesh(&vertices[0], vertices.size() * sizeof(glm::vec3), attribs,
                indices);
}
