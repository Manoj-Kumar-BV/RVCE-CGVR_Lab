#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

// Struct to hold a 2D integer point
struct Point {
    int x, y;
};

// Struct to hold a line defined by two points
struct Line {
    Point p1, p2;
};

// Global variables
std::vector<Line> lines; // Stores all lines to be drawn
Point temp_point;        // Temporarily stores the first click of a new line
bool first_click = true; // Flag to check if it's the first or second click

// Function to draw a single pixel on the screen
void draw_pixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham's line drawing algorithm for all slopes
void bresenham_draw_line(Point p1, Point p2) {
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    // Determine the direction of increment for x and y
    int inc_x = (x1 < x2) ? 1 : -1;
    int inc_y = (y1 < y2) ? 1 : -1;

    // Current point starts at the first point
    int x = x1;
    int y = y1;

    // Case 1: Gentle slope (|m| < 1), iterate along x-axis
    if (dx > dy) {
        int p = 2 * dy - dx; // Initial decision parameter
        for (int i = 0; i <= dx; ++i) {
            draw_pixel(x, y);
            if (p < 0) {
                p = p + 2 * dy;
            }
            else {
                y += inc_y;
                p = p + 2 * dy - 2 * dx;
            }
            x += inc_x;
        }
    }
    // Case 2: Steep slope (|m| >= 1), iterate along y-axis
    else {
        int p = 2 * dx - dy; // Initial decision parameter
        for (int i = 0; i <= dy; ++i) {
            draw_pixel(x, y);
            if (p < 0) {
                p = p + 2 * dx;
            }
            else {
                x += inc_x;
                p = p + 2 * dx - 2 * dy;
            }
            y += inc_y;
        }
    }
}

// The main display function, called whenever the window needs to be redrawn
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen to the background color
    glColor3f(1.0, 1.0, 1.0);     // Set drawing color to white

    // Redraw all the lines that have been created
    for (const auto& line : lines) {
        bresenham_draw_line(line.p1, line.p2);
    }

    glFlush(); // Force execution of GL commands
}

// Mouse callback function, triggered on mouse clicks
void mouse_click(int button, int state, int x, int y) {
    // We only care about the left mouse button being pressed down
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // GLUT's coordinate system has (0,0) at the top-left,
        // but OpenGL's is at the bottom-left. We need to convert.
        y = 500 - y; // Assuming window height is 500

        if (first_click) {
            // This is the first click, so we store the starting point
            temp_point = { x, y };
            first_click = false;
            std::cout << "Starting point set at (" << x << ", " << y << ")" << std::endl;
        }
        else {
            // This is the second click, so we create the line and add it to our list
            Point p2 = { x, y };
            lines.push_back({ temp_point, p2 });
            first_click = true; // Reset for the next line
            std::cout << "Ending point set at (" << x << ", " << y << "). Drawing line." << std::endl;
            glutPostRedisplay(); // Request a redraw to show the new line
        }
    }
}

// Initializes OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.2, 1.0); // Set background color to a dark blue
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500); // Set up a 2D orthographic viewing area
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);                     // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode
    glutInitWindowSize(500, 500);              // Set window size
    glutInitWindowPosition(100, 100);          // Set window position
    glutCreateWindow("Bresenham Line Drawing"); // Create the window with a title

    init(); // Initialize OpenGL settings

    glutDisplayFunc(display);       // Register the display callback function
    glutMouseFunc(mouse_click);     // Register the mouse callback function
    glutMainLoop();                 // Enter the main event-processing loop

    return 0;
}