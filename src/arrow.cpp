#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "arrow.hpp"
#include "mesh.hpp"
#include "shader.hpp"

static Mesh gen_stem(float width, size_t resolution) {
    std::vector<glm::vec3> vertices;
    vertices.reserve(2 + 2 * (resolution + 1));
    vertices.push_back({-1.0f, 0.0f, 0.0f});
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * M_PI * i / resolution;
        vertices.push_back({-1.0f, width * sin(angle), width * cos(angle)});
    }
    vertices.push_back({1.0f, 0.0f, 0.0f});
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * M_PI * i / resolution;
        vertices.push_back({1.0f, width * sin(angle), width * cos(angle)});
    }
    assert(2 + 2 * (resolution + 1) == vertices.size());

    std::vector<unsigned int> indices;
    indices.reserve(4 * 3 * resolution);
    const size_t stem_bot_center = 0;
    const size_t stem_bot = 1;
    const size_t stem_top_center = 1 + resolution + 1;
    const size_t stem_top = 1 + resolution + 2;
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(stem_bot_center);
        indices.push_back(stem_bot + i);
        indices.push_back(stem_bot + i + 1);
    }
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(stem_bot + i);
        indices.push_back(stem_top + i);
        indices.push_back(stem_top + i + 1);

        indices.push_back(stem_top + i + 1);
        indices.push_back(stem_bot + i + 1);
        indices.push_back(stem_bot + i);
    }
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(stem_top_center);
        indices.push_back(stem_top + i + 1);
        indices.push_back(stem_top + i);
    }
    assert(4 * 3 * resolution == indices.size());

    const std::vector<VertexAttribute> attribs = {
        {0, 3, GL_FLOAT, false, sizeof(glm::vec3), 0}};
    return Mesh(&vertices[0], vertices.size() * sizeof(glm::vec3), attribs,
                indices);
}

static Mesh gen_tip(float width, size_t resolution) {
    std::vector<glm::vec3> vertices;
    vertices.reserve(2 + resolution + 1);
    vertices.push_back({0.0f, 0.0f, 0.0f});
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * M_PI * i / resolution;
        vertices.push_back({0.0f, width * sin(angle), width * cos(angle)});
    }
    vertices.push_back({1.0f, 0.0f, 0.0f});
    assert(2 + resolution + 1 == vertices.size());

    std::vector<unsigned int> indices;
    indices.reserve(2 * 3 * resolution);
    const size_t tip_bot_center = 0;
    const size_t tip_bot = 1;
    const size_t tip_top_center = 1 + resolution + 1;
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(tip_bot_center);
        indices.push_back(tip_bot + i);
        indices.push_back(tip_bot + i + 1);
    }
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(tip_bot + i);
        indices.push_back(tip_top_center);
        indices.push_back(tip_bot + i + 1);
    }
    assert(2 * 3 * resolution == indices.size());

    const std::vector<VertexAttribute> attribs = {
        {0, 3, GL_FLOAT, false, sizeof(glm::vec3), 0}};
    return Mesh(&vertices[0], vertices.size() * sizeof(glm::vec3), attribs,
                indices);
}

Arrow::Arrow(float tip_width, float stem_width, size_t resolution)
    : stem_(gen_stem(stem_width, resolution)),
      tip_(gen_tip(tip_width, resolution)) {}

void Arrow::draw(glm::vec3 origin, glm::vec3 end, Shader &shader) const {
    glm::vec3 dir = end - origin;
    float length = glm::length(dir);
    dir = glm::normalize(dir);
    glm::vec3 unitx(1.0f, 0.0f, 0.0f);
    glm::vec3 axis = glm::cross(unitx, dir);
    float angle = acos(glm::dot(unitx, dir));

    glm::mat4 stem_model = glm::mat4(1.0f);
    stem_model = glm::rotate(stem_model, angle, axis);
    stem_model = glm::scale(stem_model, glm::vec3(length, 1.0f, 1.0f));
    shader.set_mat4("model", stem_model);
    stem_.draw();

    glm::mat4 tip_model = glm::mat4(1.0f);
    tip_model = glm::rotate(tip_model, angle, axis);
    tip_model = glm::translate(tip_model, glm::vec3(length, 0.0f, 0.0f));
    shader.set_mat4("model", tip_model);
    tip_.draw();
}
