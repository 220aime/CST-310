# CST-310: Computer Graphics Projects
Computer Graphics Lecture &amp; Lab: This course covers 2D and 3D concepts, algorithms, and implementation methods using shader-based programming. Main topics covered include coordinate systems, transformations, material simulation, and animation. 

**Author**: Aime Serge Tuyishime  
**Course**: CST-310  
**GitHub Repository**: [https://github.com/220aime/CST-310](https://github.com/220aime/CST-310)

---

## Project 1: Your Surrounding World (HTML + Three.js)

### Description
This project presents an interactive 3D corridor scene built with HTML and Three.js. It demonstrates camera controls, WebGL rendering, and dynamic scene interaction using standard web technologies.

### File
- `YourSurroundWorld.HTML`

### Software Requirements

#### Minimum System Requirements
- **Operating System**: Windows 10/11, macOS 10.14+, or Ubuntu 18.04+
- **RAM**: 4GB (8GB recommended)
- **Graphics**: Integrated or dedicated GPU with OpenGL 3.0+ support
- **Storage**: At least 50MB free space
- **Internet**: Required for loading Three.js via CDN

#### Required Software
- Modern Web Browser with WebGL Support:
  - Google Chrome 56+ (recommended)
  - Mozilla Firefox 51+
  - Safari 12+ (macOS only)
  - Microsoft Edge 79+
  - Opera 43+

#### Browser Configuration
- WebGL 1.0+ support
- JavaScript enabled
- Hardware acceleration enabled
- Canvas 2D support
- Local file access permission

#### Dependencies
- Three.js (r128) via CDN
- No additional libraries or extensions required

### Installation Instructions

**Step 1: File Setup**
1. Ensure you have the file: `YourSurroundWorld.HTML`
2. Create a project folder (e.g., `3D_GCU_Corridor`)
3. Place the file inside
4. Verify it's not corrupted (size should be ~25–30KB)

**Step 2: Browser Setup**
1. Open your browser
2. Enable JavaScript in settings
3. Enable local file access if needed

**Step 3: WebGL Check**
1. Visit [https://get.webgl.org](https://get.webgl.org)
2. Confirm the cube appears
3. If not, update your browser or graphics drivers

### Running Instructions

**Method 1: Direct File Opening**
1. Double-click `YourSurroundWorld.HTML`
2. Wait 3–5 seconds for it to load in your browser

**Method 2: Open via Browser Menu**
1. Open browser → Press `Ctrl + O` (or `Cmd + O` on macOS)
2. Select and open the HTML file

**Method 3: Local Server (if needed)**
1. Open terminal in the project folder
2. Run:
python -m http.server 8000
3. Open `http://localhost:8000` in browser and click the file

### Operation Instructions

**Camera Controls**
- Click + drag: Rotate camera
- Scroll wheel: Zoom
- View buttons: Change camera angle

**Performance Tips**
- Close unused tabs
- Use Chrome for best results
- Reduce browser zoom

### Troubleshooting

| Issue | Solution |
|-------|----------|
| Black screen | Refresh, check WebGL, try another browser |
| Controls not working | Ensure JavaScript is enabled, click scene first |
| Laggy performance | Close other tabs, update drivers |
| File won’t open | Use right-click → Open with browser |
| Textures missing | Check internet, clear cache, try private mode |

---

## Project 2: Sierpinski Gasket (C++ / OpenGL)

### Description
This program renders different versions of the Sierpinski Gasket fractal using OpenGL in both 2D and 3D modes. It includes recursive triangle subdivision, chaos game simulation, and 3D tetrahedron rendering.

### File
- `sierpinski.cpp`

### Software Requirements

#### Essential Components
- C++ Compiler: GCC 4.8+, Clang 3.4+, or Visual Studio 2015+
- OpenGL 2.1 or higher
- GLUT or FreeGLUT for window/input handling
- GLU (OpenGL Utility Library)

#### Windows Setup (Visual Studio)
1. Create an Empty C++ Project
2. Add `sierpinski.cpp` to source files
3. Configure project:
- Linker → Input → Add:
  - `opengl32.lib`
  - `glu32.lib`
  - `glut32.lib` or `freeglut.lib`
4. Ensure FreeGLUT headers/libraries are included
5. Build and run

#### Windows Setup (Code::Blocks)
1. Create Console Application
2. Add `sierpinski.cpp` to project
3. Link libraries:
- `opengl32`
- `glu32`
- `glut32`
4. Build and run

### Running the Program

- Use:
- 
#### Keyboard Controls

| Key | Action |
|-----|--------|
| 1 | 2D recursive triangle subdivision |
| 2 | 2D chaos game method |
| 3 | 3D tetrahedral gasket |
| + | Increase recursion depth |
| - | Decrease recursion depth |
| R | Rotate 3D view |
| Q or ESC | Quit program |

### Expected Output
- Mode 1: Blue triangular recursive fractal
- Mode 2: Red point-cloud fractal (chaos game)
- Mode 3: Green 3D rotating tetrahedron fractal

---

### Troubleshooting

**"GL/glut.h not found"**  
- Install FreeGLUT and configure include/library paths

**"Undefined reference to glut functions"**  
- Link with `glut32.lib` or `freeglut.lib`

**"OpenGL functions not working"**  
- Update graphics drivers and verify OpenGL 2.1+ support

**"Program crashes on startup"**  
- Ensure all required DLLs are present and correct driver is installed

---

*Last updated: July 2025*

