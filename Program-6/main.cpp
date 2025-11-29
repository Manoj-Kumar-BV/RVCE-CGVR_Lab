#include<stdio.h>
#include<gl/glut.h>
#include<math.h>

#define PI 3.14159265

void sphere()
{
double c=PI/180, phi, phir, phir20, theta, thetar, x, y, z, c80;
// to get longitudes
for(phi=-80.0; phi<=60.0; phi+=20.0)
{
	phir=c*phi;
	phir20=c*(phi+20);
	glBegin(GL_QUAD_STRIP);
	// to get latitudes
	for (theta=-180.0; theta<=180.0; theta+=20.0)
	{
		thetar=c*theta;  // =-180*c = -3.132 – 1st time
		
		x=sin(thetar)*cos(phir); 
		y=cos(thetar)*cos(phir);
		z=sin(phir);
		glVertex3d(x,y,z); // 1st point

		phir20=c*(phi+20);
		x=sin(thetar)*cos(phir20); 
		y=cos(thetar)*cos(phir20);
		z=sin(phir20);
		glVertex3d(x,y,z); // 2nd point
		
		

	}
	glEnd();
}

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0.0,0.0,1.0);
	c80=c*80.0;
	z=sin(c80);
	for(theta=-180.0; theta<=180.0; theta+=20.0)
	{
		thetar=c*theta;
		x=sin(thetar)*cos(c80);
		y=cos(thetar)*cos(c80);
		glVertex3d(x,y,z);	
	}
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0.0,0.0,-1.0);
	z=-sin(c80);
	for(theta=-180.0; theta<=180.0; theta+=20.0)
	{
		thetar=c*theta;
		x=sin(thetar)*cos(c80);
		y=cos(thetar)*cos(c80);
		glVertex3d(x,y,z);	
	}
	glEnd();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.0);
	glRotatef(60.0, 1.0, 1.0, 0.0);
	
	sphere();
	glFlush();
}
void myinit()
{
	glClearColor(1.0,1.0,1.0,0);
	glColor3f(0,0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1.0, 10.0);
	glEnable(GL_DEPTH_TEST);
}
int main(int argc,char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Sphere Display");
	glutDisplayFunc(display);
	myinit();
	glutMainLoop();
	return 0;
}