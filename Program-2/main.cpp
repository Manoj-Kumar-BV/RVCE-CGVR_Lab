#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

// Struct to hold a 2D integer point
struct Point {
    int x, y;
};

// Struct to hold a circle's center and radius
struct Circle {
    Point center;
    int radius;
};

// Global variables
std::vector<Circle> circles;   // Stores all circles to be drawn
Point temp_center;             // Temporarily stores the center click
bool first_click = true;       // Flag to check if it's the first or second click

// Function to draw a single pixel on the screen
void draw_pixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Helper function to draw the 8 symmetric points for a given offset (x, y)
void draw_circle_pixels(int xc, int yc, int x, int y) {
    draw_pixel(xc + x, yc + y);
    draw_pixel(xc - x, yc + y);
    draw_pixel(xc + x, yc - y);
    draw_pixel(xc - x, yc - y);
    draw_pixel(xc + y, yc + x);
    draw_pixel(xc - y, yc + x);
    draw_pixel(xc + y, yc - x);
    draw_pixel(xc - y, yc - x);
}

// Bresenham's circle drawing algorithm
void bresenham_draw_circle(Circle c) {
    int xc = c.center.x;
    int yc = c.center.y;
    int r = c.radius;

    int x = 0;
    int y = r;

    // Initial decision parameter
    // Derived from p = 5/4 - r, but using only integers
    int p = 3 - 2 * r;

    // The first set of 8 points
    draw_circle_pixels(xc, yc, x, y);

    // Loop to calculate and plot points in the first octant
    // and then mirror them to the other 7 octants
    while (x <= y) {
        x++; // Always increment x

        if (p < 0) {
            // The next point is (x, y), midpoint is inside the circle
            p = p + 4 * x + 6;
        }
        else {
            // The next point is (x, y-1), midpoint is outside or on the circle
            y--;
            p = p + 4 * (x - y) + 10;
        }
        draw_circle_pixels(xc, yc, x, y);
    }
}

// The main display function, called whenever the window needs to be redrawn
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen to the background color
    glColor3f(0.0, 1.0, 1.0);     // Set drawing color to cyan

    // Redraw all the circles that have been created
    for (const auto& circle : circles) {
        bresenham_draw_circle(circle);
    }

    glFlush(); // Force execution of GL commands
}

// Mouse callback function, triggered on mouse clicks
void mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convert GLUT's top-left origin to OpenGL's bottom-left origin
        y = 500 - y; // Assuming window height is 500

        if (first_click) {
            // First click: Store the center point
            temp_center = { x, y };
            first_click = false;
            std::cout << "Center point set at (" << x << ", " << y << ")" << std::endl;
        }
        else {
            // Second click: Define radius and create the circle
            Point p2 = { x, y };

            // Calculate radius using the distance formula
            int dx = p2.x - temp_center.x;
            int dy = p2.y - temp_center.y;
            int radius = static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy)));

            // Add the new circle to our list
            circles.push_back({ temp_center, radius });
            first_click = true; // Reset for the next circle
            std::cout << "Radius point set at (" << x << ", " << y << "). Drawing circle with radius " << radius << "." << std::endl;

            // Request a redraw to show the new circle
            glutPostRedisplay();
        }
    }
}

// Initializes OpenGL settings
void init() {
    glClearColor(0.2, 0.0, 0.2, 1.0); // Set background color to a dark purple
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500); // Set up a 2D orthographic viewing area
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Bresenham Circle Drawing");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse_click);
    glutMainLoop();

    return 0;
}