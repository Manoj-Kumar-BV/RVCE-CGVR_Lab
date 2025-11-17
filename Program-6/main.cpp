#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

// Use the M_PI constant from math.h for better precision
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void sphere()
{
    double c = M_PI / 180.0, phi, phir, phir20, theta, thetar, x, y, z;

    // Main body of the sphere using QUAD_STRIP
    // We loop from phi = -80 degrees to phi = 60 degrees.
    // The last strip will be from 60 to 80 degrees.
    for (phi = -80.0; phi <= 60.0; phi += 20.0)
    {
        phir = c * phi;
        phir20 = c * (phi + 20);

        glBegin(GL_QUAD_STRIP);
        // Loop for longitude (theta)
        for (theta = -180.0; theta <= 180.0; theta += 20.0)
        {
            thetar = c * theta;

            // First vertex (at latitude phi)
            x = sin(thetar) * cos(phir);
            y = cos(thetar) * cos(phir);
            z = sin(phir);
            glVertex3d(x, y, z);

            // Second vertex (at latitude phi + 20)
            x = sin(thetar) * cos(phir20);
            y = cos(thetar) * cos(phir20);
            z = sin(phir20);
            glVertex3d(x, y, z);
        }
        glEnd();
    }

    // Top pole (North Pole) - Use TRIANGLE_FAN
    // Connects the pole (0,0,1) to the 80-degree latitude circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(0.0, 0.0, 1.0); // Top pole
    double phir80 = c * 80.0;
    z = sin(phir80);
    for (theta = -180.0; theta <= 180.0; theta += 20.0)
    {
        thetar = c * theta;
        x = sin(thetar) * cos(phir80);
        y = cos(thetar) * cos(phir80);
        glVertex3d(x, y, z);
    }
    glEnd();

    // Bottom pole (South Pole) - Use TRIANGLE_FAN
    // Connects the pole (0,0,-1) to the -80-degree latitude circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(0.0, 0.0, -1.0); // Bottom pole
    z = -sin(phir80); // z for -80 degrees
    for (theta = -180.0; theta <= 180.0; theta += 20.0)
    {
        thetar = c * theta;
        x = sin(thetar) * cos(phir80);
        y = cos(thetar) * cos(phir80);
        glVertex3d(x, y, z);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Use wireframe to see the approximation
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move the camera back -3 units to see the sphere (which is at z=0)
    // Using gluLookAt is also a good alternative.
    glTranslatef(0.0, 0.0, -3.0);

    // Rotate the sphere so we aren't looking straight at a pole
    glRotatef(60.0, 1.0, 1.0, 0.0);

    sphere();

    glFlush();
}

void myinit()
{
    glClearColor(1.0, 1.0, 1.0, 0.0); // White background
    glColor3f(0.0, 0.0, 1.0);       // Blue drawing color

    // --- THIS IS THE MAIN FIX ---
    // We must use a 3D projection matrix.
    // 1. Set the matrix mode to Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 2. Use gluPerspective for a 3D view
    // (45 degree field of view, 1.0 aspect ratio, near clip 1.0, far clip 10.0)
    gluPerspective(45.0, 1.0, 1.0, 10.0);

    // As an alternative, you could use a 3D orthographic projection:
    // glOrtho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}

// FIXED: Use int main() and return 0
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // Added GLUT_DEPTH
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sphere Display (Fixed)");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
    return 0;
}

// FIXED: Removed the extra '}' that was here