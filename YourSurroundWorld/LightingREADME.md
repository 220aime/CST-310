# OpenGL Specular Lighting Demo

A comprehensive demonstration of the **Phong Lighting Model** in OpenGL, showcasing how different shininess values affect specular highlights on 3D objects.

## Overview

This project implements the complete **Phong lighting model** as described in the [LearnOpenGL](https://learnopengl.com/Lighting/Basic-Lighting) tutorial, 
demonstrating the visual impact of varying shininess values on specular highlights. The demo renders 8 cubes in a 2×4 grid, each with different shininess 
values (2, 4, 8, 16, 32, 64, 128, 256) to show the progression from broad, soft highlights to small, concentrated, intense highlights.

## Features

- **Complete Phong Lighting Implementation**:
  - **Ambient lighting**: Base illumination simulation
  - **Diffuse lighting**: Directional surface shading
  - **Specular lighting**: Reflective highlights with variable shininess

- **Visual Shininess Progression**:
  - 8 cubes demonstrating shininess values from 2 to 256
  - Clear visualization of how shininess affects highlight size and intensity
  - Real-time rendering with proper lighting calculations

- **Standards Compliant**:
  - Follows LearnOpenGL tutorial specifications exactly
  - Uses standard camera and light positions
  - Implements proper normal matrix calculations

## Expected Output

The demo displays 8 orange cubes arranged in a 2×4 grid:

```
[2]    [4]    [8]    [16]
[32]   [64]   [128]  [256]
```

**Visual Progression**:
- **Cube 2**: Broad, soft specular highlight covering large surface area
- **Cubes 4-16**: Gradually focusing highlights with moderate brightness
- **Cubes 32-128**: Smaller, brighter highlights
- **Cube 256**: Tiny, intensely bright concentrated spot (smallest but brightest)

The lighting demonstrates the principle: *"The higher the shininess value of an object, the more it properly reflects the light instead of scattering it all around and thus the smaller the highlight becomes."*

## Prerequisites

### Required Libraries
- **GLFW** (3.3+): Window management and input handling
- **GLEW**: OpenGL extension loading
- **GLM**: OpenGL mathematics library
- **OpenGL** (3.3+): Graphics rendering

### Development Environment
- C++ compiler with C++11 support (GCC, Clang, MSVC)
- OpenGL 3.3+ compatible graphics driver

##Installation

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install libglfw3-dev libglew-dev libglm-dev build-essential
```

### macOS (with Homebrew)
```bash
brew install glfw glew glm
```

### Windows (with vcpkg)
```bash
vcpkg install glfw3 glew glm
```

### Arch Linux
```bash
sudo pacman -S glfw-x11 glew glm gcc
```

## Compilation

### Linux/macOS
```bash
g++ -o specular_demo main.cpp -lglfw -lGLEW -lGL -std=c++11
```

### macOS (Alternative)
```bash
g++ -o specular_demo main.cpp -lglfw -lGLEW -framework OpenGL -std=c++11
```

### Windows (MinGW)
```bash
g++ -o specular_demo.exe main.cpp -lglfw3 -lglew32 -lopengl32 -std=c++11
```

### CMake (Cross-platform)
Create `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.10)
project(SpecularLightingDemo)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(specular_demo main.cpp)

target_link_libraries(specular_demo 
    OpenGL::GL 
    glfw 
    GLEW::GLEW
)
```

Then build:
```bash
mkdir build && cd build
cmake ..
make
```

## Running the Demo

```bash
./specular_demo
```

**Controls**:
- The demo runs automatically with no user input required
- Close the window or press `ESC` to exit

## Code Structure

### Shader Components

**Vertex Shader**:
- Transforms vertices to screen space
- Calculates world space positions for lighting
- Applies proper normal matrix for correct normal transformation

**Fragment Shader**:
- Implements complete Phong lighting model
- Calculates ambient, diffuse, and specular components
- Combines lighting with material color

### Key Implementation Details

1. **Normal Matrix Calculation**:
   ```cpp
   Normal = mat3(transpose(inverse(model))) * aNormal;
   ```
   Ensures normals remain perpendicular to surfaces after transformations.

2. **Specular Calculation** (LearnOpenGL compliant):
   ```glsl
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
   ```

3. **Final Lighting Combination**:
   ```glsl
   vec3 result = (ambient + diffuse + specular) * objectColor;
   ```

### Configuration Values

| Parameter | Value | Purpose |
|-----------|-------|---------|
| Camera Position | `(0, 0, 3)` | Standard LearnOpenGL camera setup |
| Light Position | `(1.2, 1.0, 2.0)` | Standard LearnOpenGL light setup |
| Object Color | `(1.0, 0.5, 0.31)` | LearnOpenGL coral/orange color |
| Ambient Strength | `0.1` | Base illumination level |
| Specular Strength | `0.5` | Highlight intensity |
| Shininess Values | `2, 4, 8, 16, 32, 64, 128, 256` | Demonstration range |

## Learning Objectives

This demo teaches:

1. **Phong Lighting Model**: Understanding the three components of realistic lighting
2. **Specular Highlights**: How surface properties affect light reflection
3. **Shininess Parameter**: Mathematical relationship between exponent and highlight appearance
4. **OpenGL Shaders**: Practical implementation of lighting calculations
5. **3D Graphics Principles**: World space transformations and normal calculations

## Technical Concepts

### Phong Lighting Components

1. **Ambient Lighting**:
   - Simulates indirect light bouncing
   - Prevents completely black areas
   - Formula: `ambient = ambientStrength * lightColor`

2. **Diffuse Lighting**:
   - Simulates direct light hitting surfaces
   - Depends on surface normal and light direction
   - Formula: `diffuse = max(dot(normal, lightDirection), 0.0) * lightColor`

3. **Specular Lighting**:
   - Simulates mirror-like reflections
   - Depends on view angle and reflection direction
   - Formula: `specular = pow(max(dot(viewDirection, reflectionDirection), 0.0), shininess)`

### Shininess Mathematics

The shininess parameter controls highlight appearance through the power function:
- **Low values (2-8)**: `pow(x, 2-8)` creates gentle falloff → broad highlights
- **High values (64-256)**: `pow(x, 64-256)` creates sharp falloff → concentrated highlights

## Troubleshooting

### Common Issues

1. **Black Screen**:
   - Check OpenGL driver support (3.3+ required)
   - Verify GLEW initialization
   - Ensure shaders compile correctly

2. **Compilation Errors**:
   - Verify all libraries are installed
   - Check library linking order
   - Ensure C++11 support is enabled

3. **Missing Libraries**:
   ```bash
   # Check if libraries are installed (Linux)
   pkg-config --libs glfw3 glew
   ```

4. **Performance Issues**:
   - Update graphics drivers
   - Enable GPU acceleration
   - Check system requirements

## References

- [LearnOpenGL - Basic Lighting](https://learnopengl.com/Lighting/Basic-Lighting)
- [OpenGL Documentation](https://www.opengl.org/documentation/)
- [Phong Reflection Model](https://en.wikipedia.org/wiki/Phong_reflection_model)
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [GLEW Documentation](http://glew.sourceforge.net/)
