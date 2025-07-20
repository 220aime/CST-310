/*Aime Serge Tuyishime
JULY 20,2025
CST-310 |PROJECT 7: PUTTING IT ALL TOGETHER| PART2
ColorCubeFlyby.cpp
Enhanced RGB Color Cubes with FIXED keyboard controls and user-friendly interface
*/
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>
#include <iostream>

// ---- Global state variables ----
bool isAnimating = true;        // Camera flyby animation
bool cubesBouncing = true;      // Cube bouncing physics
float manualRotationY = 0.0f;   // Manual scene rotation (R key)
float yOffset = 0.0f;           // Vertical scene offset (U/D keys)
float zoomFactor = 1.0f;        // Scene zoom (+/- keys)

static float cameraU = 0.0f;    // Camera orbit parameter

// Three cubes with positions and velocities
float cubePositions[3][3] = {
    { 0.5f,  0.5f,  0.5f },     // Center cube
    { -1.0f, 0.0f,  1.0f },     // Left cube  
    { 2.0f,  1.0f, -0.5f }      // Right cube
};

float cubeVelocities[3][3] = {
    { 0.03f,  0.025f, 0.02f },  // Slower, more visible
    { -0.025f, 0.03f, -0.02f },
    { 0.02f, -0.025f,  0.03f }
};

// Boundary walls
const float LEFT_WALL = -3.0f;
const float RIGHT_WALL = 4.0f;
const float TOP_WALL = 2.5f;
const float BOTTOM_WALL = -2.5f;
const float FRONT_WALL = 2.5f;
const float BACK_WALL = -2.5f;

// ---- Original RGB Cube ----
namespace Cube {
    const int V = 8, F = 6;
    GLint vertices[V][3] = {
        {0,0,0}, {0,0,1}, {0,1,0}, {0,1,1},
        {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}
    };
    GLint faces[F][4] = {
        {1,5,7,3}, {5,4,6,7}, {4,0,2,6},
        {3,7,6,2}, {0,1,3,2}, {0,4,5,1}
    };
    GLfloat colors[V][3] = {
        {0.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f}, {0.0f,1.0f,0.0f}, {0.0f,1.0f,1.0f},
        {1.0f,0.0f,0.0f}, {1.0f,0.0f,1.0f}, {1.0f,1.0f,0.0f}, {1.0f,1.0f,1.0f}
    };

    void draw() {
        glBegin(GL_QUADS);
        for (int i = 0; i < F; i++) {
            for (int j = 0; j < 4; j++) {
                glColor3fv(colors[faces[i][j]]);
                glVertex3iv(vertices[faces[i][j]]);
            }
        }
        glEnd();
    }
}

// ---- Red-tinted Cube ----
namespace RedCube {
    const int V = 8, F = 6;
    GLint vertices[V][3] = {
        {0,0,0}, {0,0,1}, {0,1,0}, {0,1,1},
        {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}
    };
    GLint faces[F][4] = {
        {1,5,7,3}, {5,4,6,7}, {4,0,2,6},
        {3,7,6,2}, {0,1,3,2}, {0,4,5,1}
    };
    GLfloat colors[V][3] = {
        {0.3f,0.0f,0.0f}, {0.3f,0.0f,0.8f}, {0.3f,0.8f,0.0f}, {0.3f,0.8f,0.8f},
        {1.0f,0.2f,0.2f}, {1.0f,0.2f,0.8f}, {1.0f,0.8f,0.2f}, {1.0f,0.8f,0.8f}
    };

    void draw() {
        glBegin(GL_QUADS);
        for (int i = 0; i < F; i++) {
            for (int j = 0; j < 4; j++) {
                glColor3fv(colors[faces[i][j]]);
                glVertex3iv(vertices[faces[i][j]]);
            }
        }
        glEnd();
    }
}

// ---- Blue-tinted Cube ----
namespace BlueCube {
    const int V = 8, F = 6;
    GLint vertices[V][3] = {
        {0,0,0}, {0,0,1}, {0,1,0}, {0,1,1},
        {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}
    };
    GLint faces[F][4] = {
        {1,5,7,3}, {5,4,6,7}, {4,0,2,6},
        {3,7,6,2}, {0,1,3,2}, {0,4,5,1}
    };
    GLfloat colors[V][3] = {
        {0.0f,0.0f,0.3f}, {0.2f,0.2f,1.0f}, {0.0f,0.8f,0.3f}, {0.2f,0.8f,1.0f},
        {0.8f,0.0f,0.3f}, {0.8f,0.2f,1.0f}, {0.8f,0.8f,0.3f}, {0.8f,0.8f,1.0f}
    };

    void draw() {
        glBegin(GL_QUADS);
        for (int i = 0; i < F; i++) {
            for (int j = 0; j < 4; j++) {
                glColor3fv(colors[faces[i][j]]);
                glVertex3iv(vertices[faces[i][j]]);
            }
        }
        glEnd();
    }
}

// ---- Draw boundary walls ----
void drawWalls() {
    // Disable lighting for walls to ensure visibility
    glDisable(GL_LIGHTING);

    // Semi-transparent white walls
    glColor4f(0.9f, 0.9f, 0.9f, 0.3f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Left wall
    glBegin(GL_QUADS);
    glVertex3f(LEFT_WALL, BOTTOM_WALL, BACK_WALL);
    glVertex3f(LEFT_WALL, TOP_WALL, BACK_WALL);
    glVertex3f(LEFT_WALL, TOP_WALL, FRONT_WALL);
    glVertex3f(LEFT_WALL, BOTTOM_WALL, FRONT_WALL);
    glEnd();

    // Right wall
    glBegin(GL_QUADS);
    glVertex3f(RIGHT_WALL, BOTTOM_WALL, BACK_WALL);
    glVertex3f(RIGHT_WALL, BOTTOM_WALL, FRONT_WALL);
    glVertex3f(RIGHT_WALL, TOP_WALL, FRONT_WALL);
    glVertex3f(RIGHT_WALL, TOP_WALL, BACK_WALL);
    glEnd();

    glDisable(GL_BLEND);

    // Draw wall outlines for better visibility
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);

    // Left wall outline
    glBegin(GL_LINE_LOOP);
    glVertex3f(LEFT_WALL, BOTTOM_WALL, BACK_WALL);
    glVertex3f(LEFT_WALL, TOP_WALL, BACK_WALL);
    glVertex3f(LEFT_WALL, TOP_WALL, FRONT_WALL);
    glVertex3f(LEFT_WALL, BOTTOM_WALL, FRONT_WALL);
    glEnd();

    // Right wall outline
    glBegin(GL_LINE_LOOP);
    glVertex3f(RIGHT_WALL, BOTTOM_WALL, BACK_WALL);
    glVertex3f(RIGHT_WALL, BOTTOM_WALL, FRONT_WALL);
    glVertex3f(RIGHT_WALL, TOP_WALL, FRONT_WALL);
    glVertex3f(RIGHT_WALL, TOP_WALL, BACK_WALL);
    glEnd();

    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

// ---- Update cube physics ----
void updateCubePositions() {
    if (!cubesBouncing) return;

    for (int i = 0; i < 3; i++) {
        // Update positions
        for (int k = 0; k < 3; k++) {
            cubePositions[i][k] += cubeVelocities[i][k];
        }

        // Bounce off walls (with cube size consideration)
        // X-axis (left/right walls)
        if (cubePositions[i][0] <= LEFT_WALL + 0.5f) {
            cubePositions[i][0] = LEFT_WALL + 0.5f;
            cubeVelocities[i][0] = -cubeVelocities[i][0] * 0.9f; // Energy loss
        }
        if (cubePositions[i][0] >= RIGHT_WALL - 0.5f) {
            cubePositions[i][0] = RIGHT_WALL - 0.5f;
            cubeVelocities[i][0] = -cubeVelocities[i][0] * 0.9f;
        }

        // Y-axis (top/bottom)
        if (cubePositions[i][1] <= BOTTOM_WALL + 0.5f) {
            cubePositions[i][1] = BOTTOM_WALL + 0.5f;
            cubeVelocities[i][1] = -cubeVelocities[i][1] * 0.9f;
        }
        if (cubePositions[i][1] >= TOP_WALL - 0.5f) {
            cubePositions[i][1] = TOP_WALL - 0.5f;
            cubeVelocities[i][1] = -cubeVelocities[i][1] * 0.9f;
        }

        // Z-axis (front/back)
        if (cubePositions[i][2] <= BACK_WALL + 0.5f) {
            cubePositions[i][2] = BACK_WALL + 0.5f;
            cubeVelocities[i][2] = -cubeVelocities[i][2] * 0.9f;
        }
        if (cubePositions[i][2] >= FRONT_WALL - 0.5f) {
            cubePositions[i][2] = FRONT_WALL - 0.5f;
            cubeVelocities[i][2] = -cubeVelocities[i][2] * 0.9f;
        }
    }
}

// ---- Display function ----
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set camera position (flyby animation)
    gluLookAt(
        10.0f * cos(cameraU),
        7.0f * cos(cameraU) - 1.0f,
        4.0f * cos(cameraU / 3.0f) + 2.0f,
        0.5f, 0.5f, 0.5f,  // Look at center
        cos(cameraU), 1.0f, 0.0f   // Up vector
    );

    // Apply user transformations
    glPushMatrix();

    // Apply zoom
    glScalef(zoomFactor, zoomFactor, zoomFactor);

    // Apply vertical offset
    glTranslatef(0.0f, yOffset, 0.0f);

    // Apply manual rotation
    glRotatef(manualRotationY, 0.0f, 1.0f, 0.0f);

    // Draw walls first
    drawWalls();

    // Draw cubes at their positions
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(
            cubePositions[i][0] - 0.5f,  // Center the cube
            cubePositions[i][1] - 0.5f,
            cubePositions[i][2] - 0.5f
        );

        if (i == 0)      Cube::draw();
        else if (i == 1) RedCube::draw();
        else            BlueCube::draw();

        glPopMatrix();
    }

    glPopMatrix();

    glutSwapBuffers();
}

// ---- Timer function ----
void timer(int value) {
    // Update camera animation
    if (isAnimating) {
        cameraU += 0.008f;  // Slower camera for better viewing
    }

    // Update cube physics
    updateCubePositions();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// ---- Window reshape ----
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 0.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

// ---- FIXED Keyboard controls with user feedback ----
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'r':
    case 'R':
        manualRotationY += 15.0f;  // Rotate by 15 degrees each press
        if (manualRotationY >= 360.0f) manualRotationY -= 360.0f;
        std::cout << "Scene rotated to: " << manualRotationY << " degrees" << std::endl;
        break;

    case 's':
    case 'S':
        isAnimating = false;
        cubesBouncing = false;
        std::cout << "Animation STOPPED" << std::endl;
        break;

    case 'c':
    case 'C':
        isAnimating = true;
        cubesBouncing = true;
        std::cout << "Animation CONTINUED" << std::endl;
        break;

    case 'u':
    case 'U':
        yOffset += 0.3f;
        std::cout << "Moved UP, offset: " << yOffset << std::endl;
        break;

    case 'd':
    case 'D':
        yOffset -= 0.3f;
        std::cout << "Moved DOWN, offset: " << yOffset << std::endl;
        break;

    case '+':
    case '=':
        zoomFactor *= 1.15f;
        if (zoomFactor > 5.0f) zoomFactor = 5.0f;  // Max zoom
        std::cout << "Zoomed IN, factor: " << zoomFactor << std::endl;
        break;

    case '-':
    case '_':
        zoomFactor /= 1.15f;
        if (zoomFactor < 0.2f) zoomFactor = 0.2f;  // Min zoom
        std::cout << "Zoomed OUT, factor: " << zoomFactor << std::endl;
        break;

    case 'h':
    case 'H':
        // Print help
        std::cout << "\n=== CONTROLS HELP ===" << std::endl;
        std::cout << "R - Rotate scene by 15°" << std::endl;
        std::cout << "S - Stop all animation" << std::endl;
        std::cout << "C - Continue animation" << std::endl;
        std::cout << "U - Move scene up" << std::endl;
        std::cout << "D - Move scene down" << std::endl;
        std::cout << "+ - Zoom in" << std::endl;
        std::cout << "- - Zoom out" << std::endl;
        std::cout << "H - Show this help" << std::endl;
        std::cout << "ESC - Exit program" << std::endl;
        std::cout << "===================" << std::endl;
        break;

    case 27: // ESC key
        std::cout << "Goodbye!" << std::endl;
        exit(0);
        break;

    default:
        std::cout << "Unknown key '" << key << "'. Press 'H' for help." << std::endl;
        break;
    }

    glutPostRedisplay();
}

// ---- OpenGL initialization ----
void init() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Simple lighting setup
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = { 2.0f, 2.0f, 2.0f, 0.0f };
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

// ---- Main function ----
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 800);
    glutCreateWindow("Enhanced RGB Color Cubes - FIXED VERSION");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    init();

    // Welcome message
    std::cout << "\n=== ENHANCED RGB COLOR CUBES ===" << std::endl;
    std::cout << "Fixed keyboard controls!" << std::endl;
    std::cout << "\nCONTROLS:" << std::endl;
    std::cout << "  R - Rotate scene manually" << std::endl;
    std::cout << "  S - Stop animation" << std::endl;
    std::cout << "  C - Continue animation" << std::endl;
    std::cout << "  U - Move up" << std::endl;
    std::cout << "  D - Move down" << std::endl;
    std::cout << "  + - Zoom in" << std::endl;
    std::cout << "  - - Zoom out" << std::endl;
    std::cout << "  H - Show help" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "\nProgram started! Try the controls..." << std::endl;
    std::cout << "====================================\n" << std::endl;

    glutMainLoop();
    return 0;
}
