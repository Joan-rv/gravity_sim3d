cmake_minimum_required(VERSION 3.24)
include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(gravity_sim3d LANGUAGES C CXX)

set(sources src/main.cpp src/sphere.cpp src/shader.cpp src/mesh.cpp src/camera.cpp)
add_executable(gravity_sim3d ${sources})

FetchContent_Declare(
    glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    FIND_PACKAGE_ARGS 3.4
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    FIND_PACKAGE_ARGS 1.0.1
)
FetchContent_MakeAvailable(glfw3 glm)

find_package(OpenGL REQUIRED)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

target_link_libraries(gravity_sim3d PRIVATE OpenGL::GL glad glfw glm)

if(MSVC)
    target_compile_options(gravity_sim3d PRIVATE /W4)
else()
    target_compile_options(gravity_sim3d PRIVATE -Wall -Wextra -pedantic)
endif()
