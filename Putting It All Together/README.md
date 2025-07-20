# OpenGL Graphics Demos

This folder contains two OpenGL programs in one unified script:

- **CheckeredTriangles**: Renders a full-window checkerboard of alternating colored triangles.
- **ColorCubeFlyby**: An interactive 3D demo featuring bouncing RGB cubes between white planes, with camera fly-around and lighting.

---

## Prerequisites

- C++11-capable compiler  
- OpenGL development headers and libraries (GL, GLU)  
- GLUT or FreeGLUT  
- Terminal or command-prompt environment  

---

## Building

### Linux / macOS

    # Part 1
    g++ -std=c++11 CheckeredTriangles.cpp -lGL -lGLU -lglut -o CheckeredTriangles

    # Part 2
    g++ -std=c++11 ColorCubeFlyby.cpp   -lGL -lGLU -lglut -o ColorCubeFlyby

### Windows (MinGW)

    REM Part 1
    g++ -std=c++11 CheckeredTriangles.cpp -lfreeglut -lopengl32 -o CheckeredTriangles.exe

    REM Part 2
    g++ -std=c++11 ColorCubeFlyby.cpp   -lfreeglut -lopengl32 -o ColorCubeFlyby.exe

---

## Part 1: CheckeredTriangles.cpp

Renders a full-window checkerboard of alternating colored triangles.  
Good for learning:
- Immediate-mode vertex setup  
- Color assignment  
- Viewport resizing  

### Run

    ./CheckeredTriangles

### Controls

- **ESC** – exit  

---

## Part 2: ColorCubeFlyby.cpp

An interactive 3D demo featuring multiple RGB cubes bouncing between two white planes, with basic lighting and camera fly-around.

### Features

- Camera orbits the cube cluster  
- Optional continuous scene rotation  
- Multiple colored cubes with bounce physics  
- White vertical boundary planes  
- Zoom and vertical pan  
- Ambient/diffuse/specular lighting  

### Run

    ./ColorCubeFlyby

### Controls

    r     Toggle continuous scene rotation  
    s     Stop all animation (camera & bouncing)  
    c     Resume animation  
    u     Move scene up  
    d     Move scene down  
    +     Zoom in  
    -     Zoom out  
    ESC   Exit  

---

## Repository Layout

    .
    └── Putting It All Together
      ├── CheckeredTriangles.cpp   # Part 1: static checkerboard
      ├── ColorCubeFlyby.cpp       # Part 2: interactive cube demo
      └── README.md                # this file
