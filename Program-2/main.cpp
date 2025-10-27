#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>

struct Point { int x, y; };
struct Circle { Point center; int radius; };

std::vector<Circle> circles;
Point centerPoint;
bool firstClick = true;

// Draw a pixel
void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Draw 8 symmetric points
void drawCirclePoints(int xc, int yc, int x, int y) {
    drawPixel(xc + x, yc + y);
    drawPixel(xc - x, yc + y);
    drawPixel(xc + x, yc - y);
    drawPixel(xc - x, yc - y);
    drawPixel(xc + y, yc + x);
    drawPixel(xc - y, yc + x);
    drawPixel(xc + y, yc - x);
    drawPixel(xc - y, yc - x);
}

// Bresenham’s Circle Algorithm
void bresenhamCircle(Circle c) {
    int xc = c.center.x, yc = c.center.y, r = c.radius;
    int x = 0, y = r, p = 3 - 2 * r;

    while (x <= y) {
        drawCirclePoints(xc, yc, x, y);
        if (p < 0)
            p += 4 * x + 6;
        else {
            p += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 1, 1);
    for (auto& c : circles)
        bresenhamCircle(c);
    glFlush();
}

// Mouse input
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = 500 - y; // convert coordinates
        if (firstClick) {
            centerPoint = { x, y };
            firstClick = false;
            std::cout << "Center: (" << x << ", " << y << ")\n";
        }
        else {
            int dx = x - centerPoint.x;
            int dy = y - centerPoint.y;
            int r = static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy)));
            circles.push_back({ centerPoint, r });
            firstClick = true;
            std::cout << "Radius: " << r << "\n";
            glutPostRedisplay();
        }
    }
}

// Keyboard input
void keyboard(unsigned char key, int, int) {
    if (key == 'c' || key == 'C') {
        circles.clear();
        std::cout << "Screen cleared.\n";
        glutPostRedisplay();
    }
}

// Initialize OpenGL
void init() {
    glClearColor(0.2, 0.0, 0.2, 1.0);
    gluOrtho2D(0, 500, 0, 500);
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Bresenham Circle Drawing");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
