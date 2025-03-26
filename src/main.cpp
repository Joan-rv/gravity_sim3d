#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "sphere.hpp"

void glfw_error_callback(int error, const char *description);
void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id,
                              GLenum severity, GLsizei length,
                              const char *message, const void *userParam);

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

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

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);
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
    std::vector<char> svert_shader(
        (std::istreambuf_iterator<char>(fvert_shader)),
        std::istreambuf_iterator<char>());
    fvert_shader.close();
    const char *cvert_shader = &svert_shader[0];
    const int cvert_shader_size = svert_shader.size();
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &cvert_shader, &cvert_shader_size);
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
    std::vector<char> sfrag_shader(
        (std::istreambuf_iterator<char>(ffrag_shader)),
        std::istreambuf_iterator<char>());
    ffrag_shader.close();
    const char *cfrag_shader = &sfrag_shader[0];
    const int cfrag_shader_size = sfrag_shader.size();
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &cfrag_shader, &cfrag_shader_size);
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

void glfw_error_callback(int error, const char *description) {
    std::cerr << "Error " << error << ": " << description;
    exit(EXIT_FAILURE);
}

void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id,
                              GLenum severity, GLsizei length,
                              const char *message, const void *userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
