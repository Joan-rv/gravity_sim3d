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
    Mesh(void *data, size_t size, std::vector<VertexAttribute> attributes,
         std::vector<unsigned int> indices);
    void draw();

private:
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
    size_t ebo_size_;
};

#endif
