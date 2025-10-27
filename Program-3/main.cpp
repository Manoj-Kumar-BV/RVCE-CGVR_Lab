#include <GL/glut.h>

float angle = 0.0f;
int win1, win2;

void drawRect() {
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0); glVertex2f(-100, -50);
    glColor3f(0, 1, 0); glVertex2f(100, -50);
    glColor3f(0, 0, 1); glVertex2f(100, 50);
    glColor3f(1, 1, 0); glVertex2f(-100, 50);
    glEnd();
}

void display1() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawRect();
    glutSwapBuffers();
}

void display2() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(angle, 0, 0, 1);
    drawRect();
    glutSwapBuffers();
}

void spin() {
    angle += 0.5f;
    if (angle > 360) angle -= 360;
    glutSetWindow(win2);
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250, 250, -250, 250);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    win1 = glutCreateWindow("Static Rectangle");
    glClearColor(0.1, 0.1, 0.1, 1);
    glutDisplayFunc(display1);
    glutReshapeFunc(reshape);

    glutInitWindowPosition(550, 100);
    win2 = glutCreateWindow("Spinning Rectangle");
    glClearColor(0.2, 0.2, 0.2, 1);
    glutDisplayFunc(display2);
    glutReshapeFunc(reshape);

    glutIdleFunc(spin);
    glutMainLoop();
    return 0;
}
