#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "arrow.hpp"
#include "mesh.hpp"
#include "pi.hpp"
#include "planet.hpp"
#include "shader.hpp"

struct ArrowVertex {
    glm::vec3 position;
    glm::vec3 normal;
};

static const std::vector<VertexAttribute> arrow_attribs = {
    {0, 3, GL_FLOAT, false, sizeof(ArrowVertex),
     offsetof(ArrowVertex, position)},
    {
        1,
        3,
        GL_FLOAT,
        false,
        sizeof(ArrowVertex),
        offsetof(ArrowVertex, normal),
    }};

static Mesh gen_stem(float width, size_t resolution) {
    std::vector<ArrowVertex> vertices;
    vertices.reserve(2 + 2 * (resolution + 1));
    vertices.push_back({{-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}});
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * PI * i / resolution;
        vertices.push_back({{-1.0f, width * sin(angle), width * cos(angle)},
                            {-1.0f, 0.0f, 0.0f}});
    }
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * PI * i / resolution;
        vertices.push_back({{-1.0f, width * sin(angle), width * cos(angle)},
                            {0.0f, sin(angle), cos(angle)}});
    }
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * PI * i / resolution;
        vertices.push_back({{1.0f, width * sin(angle), width * cos(angle)},
                            {0.0f, sin(angle), cos(angle)}});
    }
    vertices.push_back({{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * PI * i / resolution;
        vertices.push_back({{1.0f, width * sin(angle), width * cos(angle)},
                            {1.0f, 0.0f, 0.0f}});
    }
    assert(2 + 4 * (resolution + 1) == vertices.size());

    std::vector<unsigned int> indices;
    indices.reserve(4 * 3 * resolution);
    const size_t bot_center = 0;
    const size_t bot_outer = 1;
    const size_t bot_cilinder = 1 + (resolution + 1);
    const size_t top_cilinder = 1 + 2 * (resolution + 1);
    const size_t top_center = 1 + 3 * (resolution + 1);
    const size_t top_outer = 2 + 3 * (resolution + 1);
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(bot_center);
        indices.push_back(bot_outer + i);
        indices.push_back(bot_outer + i + 1);
    }
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(bot_cilinder + i);
        indices.push_back(top_cilinder + i);
        indices.push_back(top_cilinder + i + 1);

        indices.push_back(top_cilinder + i + 1);
        indices.push_back(bot_cilinder + i + 1);
        indices.push_back(bot_cilinder + i);
    }
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(top_center);
        indices.push_back(top_outer + i + 1);
        indices.push_back(top_outer + i);
    }
    assert(4 * 3 * resolution == indices.size());

    return Mesh(&vertices[0], vertices.size() * sizeof(ArrowVertex),
                arrow_attribs, indices);
}

static Mesh gen_tip(float tip_length, float width, size_t resolution) {
    std::vector<ArrowVertex> vertices;
    vertices.reserve(2 + resolution + 1);
    vertices.push_back({{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}});
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * PI * i / resolution;
        vertices.push_back({{0.0f, width * sin(angle), width * cos(angle)},
                            {-1.0f, 0.0f, 0.0f}});
    }
    for (size_t i = 0; i <= resolution; i++) {
        float angle = 2.0f * PI * i / resolution;
        vertices.push_back({{0.0f, width * sin(angle), width * cos(angle)},
                            {0.0f, sin(angle), cos(angle)}});
    }
    vertices.push_back({{tip_length, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    assert(2 + 2 * (resolution + 1) == vertices.size());

    std::vector<unsigned int> indices;
    indices.reserve(2 * 3 * resolution);
    const size_t bot_center = 0;
    const size_t bot_inner = 1;
    const size_t bot_outer = 1 + (resolution + 1);
    const size_t top_center = 1 + 2 * (resolution + 1);
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(bot_center);
        indices.push_back(bot_inner + i);
        indices.push_back(bot_inner + i + 1);
    }
    for (size_t i = 0; i < resolution; i++) {
        indices.push_back(bot_outer + i);
        indices.push_back(top_center);
        indices.push_back(bot_outer + i + 1);
    }
    assert(2 * 3 * resolution == indices.size());

    return Mesh(&vertices[0], vertices.size() * sizeof(ArrowVertex),
                arrow_attribs, indices);
}

Arrow::Arrow(float tip_length, float tip_width, float stem_width,
             size_t resolution)
    : stem_(gen_stem(stem_width, resolution)),
      tip_(gen_tip(tip_length, tip_width, resolution)),
      tip_length_(tip_length) {}

void Arrow::draw(glm::vec3 origin, glm::vec3 end, float scale,
                 Shader &shader) const {
    assert(shader.is_used());

    glm::vec3 dir = end - origin;
    float length = glm::length(dir);
    length = fmaxf(length - tip_length_, 0.0f);
    dir = glm::normalize(dir);
    constexpr glm::vec3 unitx(1.0f, 0.0f, 0.0f);
    glm::vec3 axis = glm::cross(unitx, dir);
    if (glm::dot(axis, axis) < 1e-9f) {
        axis = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    float angle = acos(glm::dot(unitx, dir));

    glm::mat4 stem_model = glm::mat4(1.0f);
    stem_model = glm::translate(stem_model, origin + 0.5f * length * dir);
    stem_model = glm::rotate(stem_model, angle, axis);
    stem_model = glm::scale(stem_model, glm::vec3(0.5f * length, scale, scale));
    shader.set_mat4("model", stem_model);
    stem_.draw();

    glm::mat4 tip_model = glm::mat4(1.0f);
    tip_model = glm::translate(tip_model, origin + 1.0f * length * dir);
    tip_model = glm::rotate(tip_model, angle, axis);
    tip_model = glm::scale(tip_model, glm::vec3(scale));
    shader.set_mat4("model", tip_model);
    tip_.draw();
}

void Arrow::draw(const Planet &planet, Shader &shader) const {
    glm::vec3 origin =
        planet.position + planet.radius * glm::normalize(planet.velocity);
    glm::vec3 end = origin + planet.velocity;
    draw(origin, end, planet.radius, shader);
}
