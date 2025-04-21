#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <vector>

struct VertexAttribute {
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    size_t offset;
};

class Mesh {
public:
    Mesh();
    ~Mesh();
    Mesh(const void *data, size_t size,
         const std::vector<VertexAttribute> &attributes,
         const std::vector<unsigned int> &indices);
    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;
    void draw() const;

private:
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
    size_t ebo_size_;
};

#endif
