/*Aime Serge Tuyishime
JULY 20, 2025
CST-310 |PROJECT 7: PURTING IT ALL TOGETHER |PART I
CheckeredTriangles.cpp
Trivial illustration of texture mapping, with interactive controls.
*/
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cstdlib>

// 2×2 checkered pattern (red / yellow)
#define red     {0xff, 0x00, 0x00}
#define yellow  {0xff, 0xff, 0x00}
GLubyte texture[][3] = {
    red, yellow,
    yellow, red,
};

// --- global state ----------------------
static float angle = 0.0f;
static bool  spinning = true;
static float xtrans = 0.0f;
static float ytrans = 0.0f;
static float zoom = 1.0f;

// Called on window reshape: sets up projection, view, and texture
void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, GLfloat(width) / height, 1.0, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2, -1, 5,   // eye
        0, 0, 0,   // center
        0, 1, 0);  // up

    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D,
        0,         // mipmap level
        3,         // RGB
        2, 2,      // 2×2 texels
        0,         // no border
        GL_RGB,
        GL_UNSIGNED_BYTE,
        texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Draws three textured triangles, wrapped in our transform stack
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    // apply translation / zoom / rotation
    glTranslatef(xtrans, ytrans, 0.0f);
    glScalef(zoom, zoom, 1.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);
    // first triangle
    glTexCoord2f(0.5f, 1.0f); glVertex2f(-3, 3);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-3, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0, 0);

    // second triangle
    glTexCoord2f(4.0f, 8.0f); glVertex2f(3, 3);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
    glTexCoord2f(8.0f, 0.0f); glVertex2f(3, 0);

    // third triangle
    glTexCoord2f(5.0f, 5.0f); glVertex2f(0, -0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.5f, -3);
    glTexCoord2f(4.0f, 0.0f); glVertex2f(1.5f, -3);
    glEnd();

    glPopMatrix();

    glFlush();
}

// Idle callback: advance the spin if enabled
void idle() {
    if (spinning) {
        angle += 0.2f;              // tweak speed here
        if (angle >= 360.0f) angle -= 360.0f;
        glutPostRedisplay();
    }
}

// Keyboard callback: controls for pause/continue, move, zoom
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'p':  spinning = false;          break;
    case 'c':  spinning = true;           break;
    case 'u':  ytrans += 0.1f;          break;
    case 'd':  ytrans -= 0.1f;          break;
    case 'L':  xtrans -= 0.1f;          break;
    case 'R':  xtrans += 0.1f;          break;
    case '+':  zoom *= 1.1f;          break;
    case '-':  zoom /= 1.1f;          break;
    case 27:   std::exit(0);              // ESC to quit
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(520, 390);
    glutCreateWindow("Interactive Textured Triangles");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
