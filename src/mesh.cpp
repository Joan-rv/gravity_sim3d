#include "mesh.hpp"

Mesh::Mesh() : vao_(0), vbo_(0), ebo_(0), ebo_size_(0) {}
Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}

Mesh::Mesh(const void *data, size_t size,
           const std::vector<VertexAttribute> &attributes,
           const std::vector<unsigned int> &indices)
    : ebo_size_(indices.size()) {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    for (const VertexAttribute &attribute : attributes) {
        glVertexAttribPointer(attribute.index, attribute.size, attribute.type,
                              attribute.normalized, attribute.stride,
                              (void *)attribute.offset);
        glEnableVertexAttribArray(attribute.index);
    }
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]),
                 &indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Mesh::draw() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, ebo_size_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
