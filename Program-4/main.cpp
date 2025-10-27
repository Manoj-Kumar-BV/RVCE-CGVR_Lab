#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// Define M_PI if not already defined in math.h
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//----------------------------------------------------------------------
// Global Variables
//----------------------------------------------------------------------

int window1_id;
int window2_id;
float rotation_angle = 0.0f; // Shared rotation angle for both shapes

//----------------------------------------------------------------------
// Shape Drawing Function: Cylinder
// Extrudes a circle along the Y-axis
//----------------------------------------------------------------------
void drawCylinder(float radius, float height, int slices) {
    int i;
    float angle_step = 2.0f * M_PI / (float)slices;
    float h_half = height / 2.0f;

    // --- 1. Draw the sides (the "extrusion") ---
    // Use a Quad Strip to connect the top and bottom circles
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= slices; i++) {
        float angle = (float)i * angle_step;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Normal vector for the side (points outwards)
        glNormal3f(x / radius, 0.0f, z / radius);

        glVertex3f(x, h_half, z);  // Top vertex
        glVertex3f(x, -h_half, z); // Bottom vertex
    }
    glEnd();

    // --- 2. Draw the Top Cap (a polygon) ---
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f); // Normal points straight up
    for (i = 0; i < slices; i++) {
        float angle = (float)i * angle_step;
        glVertex3f(radius * cos(angle), h_half, radius * sin(angle));
    }
    glEnd();

    // --- 3. Draw the Bottom Cap (a polygon) ---
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f); // Normal points straight down
    for (i = slices - 1; i >= 0; i--) { // Reverse order for correct winding
        float angle = (float)i * angle_step;
        glVertex3f(radius * cos(angle), -h_half, radius * sin(angle));
    }
    glEnd();
}

//----------------------------------------------------------------------
// Shape Drawing Function: Parallelepiped
// Extrudes a quadrilateral (a rectangle)
//----------------------------------------------------------------------
void drawParallelepiped(float width, float height, float depth) {
    float w2 = width / 2.0f;
    float h2 = height / 2.0f;
    float d2 = depth / 2.0f;

    // Define the 8 vertices of the box
    float vertices[8][3] = {
        {-w2, -h2, d2}, { w2, -h2, d2}, { w2,  h2, d2}, {-w2,  h2, d2},
        {-w2, -h2, -d2}, { w2, -h2, -d2}, { w2,  h2, -d2}, {-w2,  h2, -d2}
    };

    // Draw the 6 quadrilateral faces
    glBegin(GL_QUADS);

    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);

    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);

    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[7]);
    glVertex3fv(vertices[3]);

    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[2]);

    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);

    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[4]);

    glEnd();
}

//----------------------------------------------------------------------
// Common Initialization for 3D
//----------------------------------------------------------------------
void initGL() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Enable smooth shading
    glShadeModel(GL_SMOOTH);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Set up a light
    GLfloat light_pos[] = { 1.0f, 1.0f, 5.0f, 0.0f }; // Directional light
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

//----------------------------------------------------------------------
// Common Reshape Function (sets 3D perspective)
//----------------------------------------------------------------------
void reshapeCommon(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------------------------
// Window 1: Cylinder
//----------------------------------------------------------------------
void displayWindow1() {
    glutSetWindow(window1_id);

    // Clear buffer
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Dark blue background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset matrix
    glLoadIdentity();

    // Set camera position
    gluLookAt(0.0, 3.0, 5.0,  // Camera (eye) position
        0.0, 0.0, 0.0,  // Look-at position
        0.0, 1.0, 0.0); // Up vector

    // Apply rotation
    glRotatef(rotation_angle, 1.0f, 1.0f, 0.0f);

    // Set color and draw
    glColor3f(0.0f, 0.7f, 1.0f); // Cyan
    drawCylinder(1.0, 2.0, 30);  // radius=1, height=2, slices=30

    // Swap buffers
    glutSwapBuffers();
}

//----------------------------------------------------------------------
// Window 2: Parallelepiped
//----------------------------------------------------------------------
void displayWindow2() {
    glutSetWindow(window2_id);

    // Clear buffer
    glClearColor(0.2f, 0.1f, 0.1f, 1.0f); // Dark red background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset matrix
    glLoadIdentity();

    // Set camera position
    gluLookAt(0.0, 3.0, 5.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // Apply rotation
    glRotatef(rotation_angle, 0.0f, 1.0f, 1.0f);

    // Set color and draw
    glColor3f(1.0f, 0.5f, 0.0f); // Orange
    drawParallelepiped(2.0, 1.5, 1.0); // width=2, height=1.5, depth=1

    // Swap buffers
    glutSwapBuffers();
}

//----------------------------------------------------------------------
// Idle Function (for animation)
//----------------------------------------------------------------------
void idle() {
    // Increment angle
    rotation_angle += 0.5f;
    if (rotation_angle > 360.0f) {
        rotation_angle -= 360.0f;
    }

    // Post redisplay for *both* windows
    glutSetWindow(window1_id);
    glutPostRedisplay();

    glutSetWindow(window2_id);
    glutPostRedisplay();
}

//----------------------------------------------------------------------
// Main Function
//----------------------------------------------------------------------
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set display mode: Double Buffered, RGB color, Depth testing
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // --- Create Window 1 ---
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    window1_id = glutCreateWindow("Window 1: Extruded Cylinder");

    // Set common 3D init states for this window's context
    initGL();

    // Register callbacks for window 1
    glutDisplayFunc(displayWindow1);
    glutReshapeFunc(reshapeCommon);

    // --- Create Window 2 ---
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(550, 100);
    window2_id = glutCreateWindow("Window 2: Extruded Parallelepiped");

    // Set common 3D init states for this window's context
    initGL();

    // Register callbacks for window 2
    glutDisplayFunc(displayWindow2);
    glutReshapeFunc(reshapeCommon);

    // --- Set Idle Function and Start Loop ---
    glutIdleFunc(idle); // Register animation function
    glutMainLoop();     // Enter the main event loop

    return 0;
}