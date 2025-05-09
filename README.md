# gravity_sim3d

A 3D gravity simulation. Built using OpenGL.

## Build and run

Build and run using CMake:

```sh
cmake -B build
cmake --build build
cd build
./gravity_sim3d
```

If the required dependencies are not present, CMake will try to automatically fetch and build them.\
The code assumes the shaders and textures are located one directory up from the current working directory (e.g. if you run from inside `./build`, the shaders and textures are in `./shaders` and `./textures`). You can change this behaviour by modifying the `DATAPATH` macro in `src/util.hpp`.

## Controls

Use WASD and the mouse to move the camera around the 3D world. To interact with the UI, pause the simulation by pressing ESC. To restart the simulation click anywhere outside of the control menu. All coordinates are in a right-handed system (i.e. the positive Z axis is pointing outside the screen).

## Techinal specifications

The graphics are built directly in OpenGL (a low-level graphics API), with the GLAD and GLFW libraries. The UI is built using ImGui. The physics engine uses [Newton's law of universal gravitation](https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation), [Euler integration](https://en.wikipedia.org/wiki/Euler_method) with a fixed time-step, and an impulse-based constraint based collision solver.

## Acknowledgements

`./textures/subdued_blue_nebulae_1.png` texture from <https://www.spacespheremaps.com/blue-nebulae-spheremaps/>.
