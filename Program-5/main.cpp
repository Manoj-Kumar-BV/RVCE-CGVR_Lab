#include <GL/glut.h>
#include <stdio.h>

int m;
typedef float point[3];

// Defined the initial tetrahedron vertices
point tetra[4] = { {0,100,-100}, {0,0,100}, {100,-100,-100}, {-100,-100,-100} };

void draw_triangle(point p1, point p2, point p3)
{
    glBegin(GL_TRIANGLES);
    glVertex3fv(p1);
    glVertex3fv(p2);
    glVertex3fv(p3);
    glEnd();
}

void divide_triangle(point a, point b, point c, int m)
{
    point v1, v2, v3;
    int j;
    if (m > 0) {
        // Calculate midpoints for all 3 sides
        for (j = 0; j < 3; j++) v1[j] = (a[j] + b[j]) / 2;
        for (j = 0; j < 3; j++) v2[j] = (a[j] + c[j]) / 2;
        for (j = 0; j < 3; j++) v3[j] = (b[j] + c[j]) / 2;

        // Recursively subdivide the three corner triangles
        // (This effectively removes the middle triangle)
        divide_triangle(a, v1, v2, m - 1);
        divide_triangle(c, v2, v3, m - 1);
        divide_triangle(b, v3, v1, m - 1);
    }
    else {
        draw_triangle(a, b, c);
    }
}

void tetrahedron(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Face 1: Red
    glColor3f(1.0, 0.0, 0.0);
    divide_triangle(tetra[0], tetra[1], tetra[2], m);

    // Face 2: Green
    glColor3f(0.0, 1.0, 0.0);
    divide_triangle(tetra[3], tetra[2], tetra[1], m);

    // Face 3: Blue
    glColor3f(0.0, 0.0, 1.0);
    divide_triangle(tetra[0], tetra[3], tetra[1], m);

    // Face 4: Black (Visible on white background, but improved logical consistency)
    glColor3f(0.0, 0.0, 0.0);
    divide_triangle(tetra[0], tetra[2], tetra[3], m);

    glFlush();
}

void myinit()
{
    glClearColor(1, 1, 1, 1); // White Background
    glOrtho(-500.0, 500.0, -500.0, 500.0, -500.0, 500.0);
}

// FIXED: Use correct argument naming (argc, argv)
int main(int argc, char** argv)
{
    printf("Enter the number of iterations: ");
    // Use scanf for broader compatibility, or scanf_s if strictly on Visual Studio
#ifdef _MSC_VER
    scanf_s("%d", &m);
#else
    scanf("%d", &m);
#endif

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 200);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Sierpinski Gasket"); // Fixed typo in title "Seirpinski"
    glutDisplayFunc(tetrahedron);
    glEnable(GL_DEPTH_TEST);
    myinit();
    glutMainLoop();
    return 0;
}