#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>

struct Point { int x, y; };
struct Line { Point p1, p2; };

std::vector<Line> lines;
Point startPoint;
bool firstClick = true;

// Function to draw a pixel
void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham’s Line Drawing Algorithm (handles all slopes)
void bresenhamLine(Point p1, Point p2) {
    int x = p1.x, y = p1.y;
    int dx = std::abs(p2.x - p1.x);
    int dy = std::abs(p2.y - p1.y);
    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        drawPixel(x, y);
        if (x == p2.x && y == p2.y) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    for (auto& line : lines)
        bresenhamLine(line.p1, line.p2);
    glFlush();
}

// Mouse input handler
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = 500 - y; // Convert from top-left to bottom-left origin
        if (firstClick) {
            startPoint = { x, y };
            firstClick = false;
            std::cout << "Start point: (" << x << ", " << y << ")\n";
        }
        else {
            lines.push_back({ startPoint, {x, y} });
            firstClick = true;
            std::cout << "End point: (" << x << ", " << y << ")\n";
            glutPostRedisplay();
        }
    }
}

// Keyboard input to clear all lines
void keyboard(unsigned char key, int, int) {
    if (key == 'c' || key == 'C') {
        lines.clear();
        std::cout << "Screen cleared.\n";
        glutPostRedisplay();
    }
}

// Initialization
void init() {
    glClearColor(0.0, 0.0, 0.2, 1.0);
    gluOrtho2D(0, 500, 0, 500);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Bresenham Line Drawing Algorithm");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
