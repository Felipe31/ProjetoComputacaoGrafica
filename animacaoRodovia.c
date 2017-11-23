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
/*void drawScene(void)
{                     
  glClear(GL_COLOR_BUFFER_BIT);
  //Céu
  glColor3f(0.68, 0.85, 0.9);
  glBegin(GL_QUADS);
  glVertex2i(0,200); glVertex2i(0,500);
  glVertex2i(500,500); glVertex2i(500,500);
  glVertex2i(500,200);glVertex2i(500,500);
  glVertex2i(0,200); glVertex2i(500,200);
  glEnd();
 
  //pista
  glColor3f(0.66, 0.66, 0.66); 
  glBegin(GL_QUADS);
  glVertex2i(0,0);  glVertex2i(190,200);
  glVertex2i(300,200); glVertex2i(300,200);
  glVertex2i(500,0); glVertex2i(300,200);
  glVertex2i(0,0); glVertex2i(500,0);
  glEnd();
  
  //Passarela
  glColor3f(0.5,0.5,0.5);
  glBegin(GL_POLYGON);
  glVertex2i(100,210); glVertex2i (100,220); glVertex2i(400,210); glVertex2i(400,220);
  glEnd();
  glColor3f(0.66,0.66,0.66);
  glBegin(GL_QUADS);
  glVertex2i(100,200); glVertex2i (100,220); glVertex2i(120,220); glVertex2i(120,200);
  glVertex2i(380,200); glVertex2i (380,220); glVertex2i(400,220); glVertex2i(400,200);
  glEnd();
  
  //placa
  glColor3f(1.0,1.0,0.0);
  glBegin(GL_QUADS);
  glVertex2i(420, 120); glVertex2i(440,140); glVertex2i(460,120); glVertex2i(440,100);
  glEnd();

  glColor3f(0.72, 0.45, 0.2);
  glBegin(GL_LINES);
  glVertex2i(440,100);glVertex2i(440,80);
  glEnd();
  
  glColor3f(0.0,0.0,0.0);
  glBegin(GL_LINES);
  glVertex2i(432,115);glVertex2i(437,125);
  glVertex2i(438,115);glVertex2i(443,125);
  glVertex2i(444,115);glVertex2i(449,125);
  glEnd();
  
  //faixa pista
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex2i(250,0);  glVertex2i(250,50);
  glVertex2i(250,100);  glVertex2i(250,150);
  glVertex2i(250,180); glVertex2i(250, 200);
  glEnd();

  
  glFlush();
  glutSwapBuffers();
 }*/

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
