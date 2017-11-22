# ifdef __APPLE__
# include< OpenGL/gl.h>
# include< GLUT/glut.h>
# else
# include<GL/gl.h>
# include<GL/glut.h>
# endif

#include <stdlib.h>

void setup(void){
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glOrtho (0, 500, 0, 500, -1 ,1);
}

void drawScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

 
  glColor3f(0.0, 0.0, 0.0);
  glLineWidth(15.0);
  glBegin(GL_LINES);
  glVertex2i(0,0);  glVertex2i(190,200);
  glVertex2i(500,0); glVertex2i(300,200);
  glEnd();

  glBegin(GL_LINES);
  glVertex2i(250,0);  glVertex2i(250,50);
  glVertex2i(250,100);  glVertex2i(250,150);
  glEnd();

  
  glFlush();
  glutSwapBuffers();
 }

void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 27:
	exit(0);
	break;
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100); 
  glutCreateWindow("TOPGEAR2017.cpp");
  setup();
  glutDisplayFunc(drawScene);
  glutKeyboardFunc(keyboard);
  
  glutMainLoop();
  
}
