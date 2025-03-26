#include <fstream>
#include <iostream>
#include <sstream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "sphere.hpp"

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(600, 600, "gravity_sim3d", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create glfw window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to load glad\n";
        glfwTerminate();
        return -1;
    }

    auto pair = sphere_vertices(10, 10);
    std::vector<SphereVertex> &vertices = pair.first;
    std::vector<unsigned int> &indices = pair.second;

    unsigned int sphere_vao, sphere_vbo, sphere_ebo;
    glGenVertexArrays(1, &sphere_vao);
    glBindVertexArray(sphere_vao);
    glGenBuffers(1, &sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SphereVertex),
                 &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex),
                          (void *)offsetof(SphereVertex, pos));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SphereVertex),
                          (void *)offsetof(SphereVertex, tex_coords));
    glEnableVertexAttribArray(1);
    glGenBuffers(1, &sphere_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);

    std::ifstream fvert_shader("../src/sphere.vert");
    std::stringstream ssvert_shader;
    ssvert_shader << fvert_shader.rdbuf();
    fvert_shader.close();
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    std::string svert_shader = ssvert_shader.str();
    const char *cvert_shader = svert_shader.c_str();
    glShaderSource(vert_shader, 1, &cvert_shader, NULL);
    glCompileShader(vert_shader);
    int status;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
        std::cerr << "Failed to compile vertex shader\n" << info_log << '\n';
        return -1;
    }
    std::ifstream ffrag_shader("../src/sphere.frag");
    std::stringstream ssfrag_shader;
    ssfrag_shader << ffrag_shader.rdbuf();
    ffrag_shader.close();
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string sfrag_shader = ssfrag_shader.str();
    const char *cfrag_shader = sfrag_shader.c_str();
    glShaderSource(frag_shader, 1, &cfrag_shader, NULL);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
        std::cerr << "Failed to compile fragex shader\n" << info_log << '\n';
        return -1;
    }

    unsigned int shader = glCreateProgram();
    glAttachShader(shader, vert_shader);
    glAttachShader(shader, frag_shader);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetProgramInfoLog(shader, 512, NULL, info_log);
        std::cerr << "Failed to link shader\n" << info_log << '\n';
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glUseProgram(shader);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
