#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <glut.h>
#endif

/*
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#  include <GL/GLUT.h>
*/
void drawScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE);
	
	glVertex2d(0, 0);
	glVertex2d(230, 200);
	glEnd();


	glBegin(GL_LINE);

	glVertex2d(500, 0);
	glVertex2d(270, 200);
	glEnd();
	glFlush();
}


void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
}

void keyInput(unsigned char key, int x, int y)
{
	
}

void specialKeyInput(int key, int x, int y)
{
	/*
	if (key == GLUT_KEY_LEFT) animationPeriod += 5;
	if (key == GLUT_KEY_RIGHT) if (animationPeriod > 5) animationPeriod -= 5;
	glutPostRedisplay();
	*/
}

void printInteraction(void)
{
	printf("Interaction:");
	printf("Press space to toggle between animation on and off.");
	printf("Press the up/down arrow keys to speed up/slow down animation.");
	printf("Press the x, X, y, Y, z, Z keys to rotate the scene.");
}

int main(int argc, char **argv)
{
	printInteraction();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TOPGEAR2017.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	//glutKeyboardFunc(keyInput);
	//glutSpecialFunc(specialKeyInput);

	setup();

	glutMainLoop();
}
