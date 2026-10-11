# Orbiting Cubes around a Planet (OpenGL)




An interactive 3D scene written in C++ and OpenGL 3.3 (Core Profile). A planet, loaded from an `.obj` file, moves along a circular path and acts as a light source. Six textured cubes orbit around it, and each cube is lit only on the side that faces the planet.

*Coursework for the Computer Graphics course, National and Kapodistrian University of Athens (2025-2026).*

## Features
- **Lambert (diffuse) lighting** in the fragment shader, with the light source placed at the planet's position. Faces turned away from the planet stay dark.
- **Hierarchical motion:** each cube starts from the planet's transformation matrix, so it follows the planet along its path while orbiting it. Every cube also spins around its own axis, with a different axis and speed per cube.
- **`.obj` model loading** and texture loading (stb_image, with mipmaps). The cube vertex data is defined directly in the code.
- **Normal matrix** (`transpose(inverse(model))`) to keep lighting correct while objects rotate.

## Controls
| Key | Action |
|---|---|
| Arrow keys (←, →, ↑, ↓) | Rotate the camera around the X and Y axes |
| Space | Pause / resume the animation |
| Esc | Quit |

## Build and Run
Tested on Windows 11 with Visual Studio 2026 (C++20).

1. Open `Graphics_2026.slnx`.
2. Select the **x64** platform (Debug or Release).
3. Press **F5**. The GLEW, GLFW and GLM libraries are restored automatically through NuGet.

## Licenses and Third-Party Materials
The source code in this repository is licensed under the MIT License (see `LICENSE`).
The following files are not my work and are not covered by that license:
- `planet/planet.obj`: 3D model provided by the course.
- `texture/container.jpg`: texture provided by the course.
- `include/stb_image.h`: [stb_image](https://github.com/nothings/stb) by Sean Barrett, public domain / MIT (see the license notice inside the file).
- GLEW, GLFW and GLM are downloaded through NuGet and keep their own licenses.
