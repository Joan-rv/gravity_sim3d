cmake_minimum_required(VERSION 3.24)
include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(gravity_sim3d LANGUAGES C CXX)

set(sources
    src/main.cpp
    src/sphere.cpp
    src/shader.cpp
    src/mesh.cpp
    src/camera.cpp
    src/debug.cpp
    src/planet.cpp
    src/controller.cpp
    src/simulation.cpp
    src/texture.cpp
    src/arrow.cpp
    src/window.cpp
    src/ui.cpp
    src/timer.cpp)
add_executable(gravity_sim3d ${sources})
target_compile_features(gravity_sim3d PRIVATE cxx_std_17)

FetchContent_Declare(
  glfw3
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
  FIND_PACKAGE_ARGS 3.4)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
  FIND_PACKAGE_ARGS 1.0.1)
FetchContent_MakeAvailable(glfw3 glm)

find_package(OpenGL REQUIRED)

add_library(glad external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(stb external/stb/stb_image.c)
target_include_directories(stb PUBLIC external/stb)

file(GLOB imgui_src external/imgui/*.cpp external/imgui/*.h
     external/imgui/backends/imgui_impl_glfw.cpp
     external/imgui/backends/imgui_impl_opengl3.cpp)
target_sources(gravity_sim3d PRIVATE ${imgui_src})
target_include_directories(gravity_sim3d PRIVATE external/imgui)

target_link_libraries(gravity_sim3d PRIVATE OpenGL::GL glad glfw glm stb)

if(MSVC)
  target_compile_options(gravity_sim3d PRIVATE /W4)
else()
  target_compile_options(gravity_sim3d PRIVATE -Wall -Wextra -pedantic)
endif()
