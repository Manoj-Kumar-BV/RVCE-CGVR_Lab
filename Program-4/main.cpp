#include <GL/glut.h>

void draw_pixel(GLint cx, GLint cy)
{
	glBegin(GL_POINTS);
	glVertex2i(cx, cy);
	glEnd();
}

void plotpixels(GLint h, GLint k, GLint x, GLint y)
{
	draw_pixel(x + h, y + k);
	draw_pixel(-x + h, y + k);
	draw_pixel(x + h, -y + k);
	draw_pixel(-x + h, -y + k);
	draw_pixel(y + h, x + k);
	draw_pixel(-y + h, x + k);
	draw_pixel(y + h, -x + k);
	draw_pixel(-y + h, -x + k);
}

void Circle_draw(GLint h, GLint k, GLint r)
{
	GLint d = 1 - r, x = 0, y = r;
	while (y > x)
	{
		plotpixels(h, k, x, y);
		if (d < 0)
			d += 2 * x + 3;
		else
		{
			d += 2 * (x - y) + 5;
			--y;
		}
		++x;
	}
	plotpixels(h, k, x, y);
}

// Draw cylinder by extruding circle
void displayCylinder()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	GLint xc = 200, yc = 150, r = 80, i, n = 100;
	for (i = 0; i < n; i += 3)
		Circle_draw(xc, yc + i, r);

	glFlush();
}

void parallelepiped(int x1, int x2, int y1, int y2)
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);
	glVertex2i(x1, y2);
	glEnd();
}

// Draw parallelepiped by extruding quadrilateral
void displayParallelepiped()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);

	int x1 = 150, x2 = 300, y1 = 150, y2 = 250, i, n = 50;
	for (i = 0; i < n; i += 2)
		parallelepiped(x1 + i, x2 + i, y1 + i, y2 + i);

	glFlush();
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// Create first window for Cylinder
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	int window1 = glutCreateWindow("Cylinder - Extruded Circle");
	init();
	glutDisplayFunc(displayCylinder);

	// Create second window for Parallelepiped
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(550, 100);
	int window2 = glutCreateWindow("Parallelepiped - Extruded Quadrilateral");
	init();
	glutDisplayFunc(displayParallelepiped);

	glutMainLoop();
	return 0;
}
