#include <stdio.h>

# ifdef __APPLE__
// # include< OpenGL/gl.h>
#include <GLUT/glut.h>
# else
// # include<GL/gl.h>
# include<GL/glut.h>
# endif


#define PI 3.14159265358979324


// Globals.
static float t = 0.0; // Animation parameter.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.
static unsigned int base; // Display lists base index.

// Initialization routine.
void setup(void)
{  
    glClearColor(0.2, 0.75, 0.9, 1.0);
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    
    base = glGenLists(3);
    glListBase(base);
    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void desenhaPassarela(){
    glNewList(base+2, GL_COMPILE);
    glColor4f(0.5,0.5,0.5,1);
    glBegin(GL_QUADS);

    //RAMPA DIREITA
    glVertex3f(300,200, 100); 
    glVertex3f(200,200, 100); 
    glVertex3f(200,50, 1); 
    glVertex3f(300,50, 1); 
    // Plano ZY 1
    glColor4f(0.5,0.5,0.5,0.5);
    glVertex3f(300,200, 100); 
    glVertex3f(300,200, 125); 
    glVertex3f(300,50, 25); 
    glVertex3f(300,50, 1); 
    // Plano ZY 2
    glVertex3f(200,200, 100); 
    glVertex3f(200,200, 125); 
    glVertex3f(200,50, 25); 
    glVertex3f(200,50, 1); 

    //RAMPA ESQUERDA
    glColor4f(0.5,0.5,0.5,1);
    glVertex3f(-300,250, 100); 
    glVertex3f(-200,250, 100); 
    glVertex3f(-200,400, 1); 
    glVertex3f(-300,400, 1); 
    // Plano ZY 1
    glColor4f(0.5,0.5,0.5,0.5);
    glVertex3f(-300,250, 100); 
    glVertex3f(-300,250, 125); 
    glVertex3f(-300,400, 25); 
    glVertex3f(-300,400, 1); 
    // Plano ZY 2
    glVertex3f(-200,250, 100); 
    glVertex3f(-200,250, 125); 
    glVertex3f(-200,400, 25); 
    glVertex3f(-200,400, 1); 

    // TOPO DA PASSARELA
    // Plano XY chão
    glColor4f(0.5,0.5,0.5,1);
    glVertex3f(300,200, 100); 
    glVertex3f(300,250, 100); 
    glVertex3f(-300,250, 100); 
    glVertex3f(-300,200, 100); 
    // Plano ZY 1
    glColor4f(0.5,0.5,0.5,0.5);
    glVertex3f(300,200, 100); 
    glVertex3f(300,250, 100); 
    glVertex3f(300,250, 125); 
    glVertex3f(300,200, 125); 
    // Plano ZY 2
    glVertex3f(-300,200, 100); 
    glVertex3f(-300,250, 100); 
    glVertex3f(-300,250, 125); 
    glVertex3f(-300,200, 125); 
    // Plano ZX 1
    glVertex3f(200,200, 100);
    glVertex3f(-300,200, 100);
    glVertex3f(-300,200, 125);
    glVertex3f(200,200, 125);
    // Plano ZX 2
    glVertex3f(300,250, 100);
    glVertex3f(-200,250, 100);
    glVertex3f(-200,250, 125);
    glVertex3f(300,250, 125);

    glColor3f(0.4,0.4,0.4);

    // LADO DIREITO
    // Plano XY chão
    glVertex3f(300,200, 1); 
    glVertex3f(300,250, 1); 
    glVertex3f(200,250, 1); 
    glVertex3f(200,200, 1); 
    // Plano XY teto
    glVertex3f(300,200, 100);
    glVertex3f(300,250, 100);
    glVertex3f(200,250, 100);
    glVertex3f(200,200, 100);
    // Plano ZY 1
    glVertex3f(300,250, 1);
    glVertex3f(300,250, 100);
    glVertex3f(300,200, 100);
    glVertex3f(300,200, 1);
    // Plano ZY 2
    glVertex3f(200,250, 1);
    glVertex3f(200,250, 100);
    glVertex3f(200,200, 100);
    glVertex3f(200,200, 1);
    // Plano ZX 1
    glVertex3f(300,200, 1);
    glVertex3f(300,200, 100);
    glVertex3f(200,200, 100);
    glVertex3f(200,200, 1);
    // Plano ZX 2
    glVertex3f(300,250, 1);
    glVertex3f(300,250, 100);
    glVertex3f(200,250, 100);
    glVertex3f(200,250, 1);

    // LADO ESQUERDO
    // Plano XY chão
    glVertex3f(-300,200, 1); 
    glVertex3f(-300,250, 1); 
    glVertex3f(-200,250, 1); 
    glVertex3f(-200,200, 1); 
    // Plano XY teto
    glVertex3f(-300,200, 100);
    glVertex3f(-300,250, 100);
    glVertex3f(-200,250, 100);
    glVertex3f(-200,200, 100);
    // Plano ZY 1
    glVertex3f(-300,250, 1);
    glVertex3f(-300,250, 100);
    glVertex3f(-300,200, 100);
    glVertex3f(-300,200, 1);
    // Plano ZY 2
    glVertex3f(-200,250, 1);
    glVertex3f(-200,250, 100);
    glVertex3f(-200,200, 100);
    glVertex3f(-200,200, 1);
    // Plano ZX 1
    glVertex3f(-300,200, 1);
    glVertex3f(-300,200, 100);
    glVertex3f(-200,200, 100);
    glVertex3f(-200,200, 1);
    // Plano ZX 2
    glVertex3f(-300,250, 1);
    glVertex3f(-300,250, 100);
    glVertex3f(-200,250, 100);
    glVertex3f(-200,250, 1);
    glEnd();
    glEndList();
}

// Drawing routine.
void drawScene(void)
{
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // glTranslatef(0.0, -10.0, -30.0);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    // CALÇADAS
    glNewList(base, GL_COMPILE);
    glColor3f(0.0, 1.0, 0.0);
    glLineWidth(3.0);
    glBegin(GL_POLYGON);
    glVertex3f(-300.0, -1000.0, 0.0);
    glVertex3f(300, -1000.0, 0.0);
    glVertex3f(300.0, 1000.0, 0.0);
    glVertex3f(-300, 1000.0, 0.0);
    glVertex3f(-300.0, -1000.0, 0.0);

    glEnd();
    glEndList();

    // RUA
    glNewList(base+1, GL_COMPILE);
    glColor3f(0.66, 0.66, 0.66);
    glPushMatrix();
    glBegin(GL_POLYGON);
    glVertex3f(-200.0, -1000.0, 1.0);
    glVertex3f(200, -1000.0, 1.0);
    glVertex3f(200.0, 1000.0, 1.0);
    glVertex3f(-200, 1000.0, 1.0);
    glVertex3f(-200.0, -1000.0, 1.0);
    glEnd();
    // Faixas
    int i = 0;
    glColor3f(1,1,1);

    glBegin(GL_QUADS);
    for (i = 0; i < 14; ++i)
    {
        glVertex3f(10, -1000+i*150, 1.1);
        glVertex3f(-10.0, -1000+i*150, 1.1);
        glVertex3f(-10, -1000+i*150+100, 1.1);
        glVertex3f(10.0, -1000+i*150+100, 1.1);
    }
    glEnd();    
    glPopMatrix();
    glEndList();

    
    
    //Passarela
    desenhaPassarela(); 

    // Rotação da tela
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);
    // Desenho das listas
    glCallList(base);
    glCallList(base+1);
    glCallList(base+2);
    
    glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
    if (isAnimate)
    {
        t += 0.01;
        if (t >= 1.0) isAnimate = 0;
        
        glutPostRedisplay();
        glutTimerFunc(animationPeriod, animate, 1);
    }
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport (0, 0, w, h);                               // Especifica as dimensões da Viewport
    glMatrixMode(GL_PROJECTION);                                    // Inicializa o sistema de coordenadas
    glLoadIdentity();                                               // Substitui a matriz atual pela matriz identidade

    glOrtho(-500.0, 500.0, -500.0, 500.0, -1000.0, 1000.0);                       // Estabelece a janela de seleção (left, right, bottom, top)

    glMatrixMode(GL_MODELVIEW);                                     // Especifica qual é a matriz atual
    glLoadIdentity();                                               // Substitui a matriz atual pela matriz identidade
}


// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case ' ':
            if (isAnimate) isAnimate = 0;
            else
            {
                isAnimate = 1;
                animate(1);
            }
            return;
            break;
        case 127:
            if (isAnimate) isAnimate = 0;
            t = 0.0;
            break;
        case 'x':
            Xangle += 5.0;
            if (Xangle > 360.0) Xangle -= 360.0;
            break;
        case 'X':
            Xangle -= 5.0;
            if (Xangle < 0.0) Xangle += 360.0;
            break;
        case 'y':
            Yangle += 5.0;
            if (Yangle > 360.0) Yangle -= 360.0;
            break;
        case 'Y':
            Yangle -= 5.0;
            if (Yangle < 0.0) Yangle += 360.0;
            break;
        case 'z':
            Zangle += 5.0;
            if (Zangle > 360.0) Zangle -= 360.0;
            break;
        case 'Z':
            Zangle -= 5.0;
            if (Zangle < 0.0) Zangle += 360.0;
            break;
        default:
            return;
            break;
    }
    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    printf("Interaction:");
    printf("Press space to toggle between animation on and off.");
    printf("Press delete to reset.");
    printf("Press the x, X, y, Y, z, Z keys to rotate the scene.");
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);

    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100); 
    glutCreateWindow ("floweringPlant.cpp"); 
    glutDisplayFunc(drawScene); 
    glutReshapeFunc(resize);  
    glutKeyboardFunc(keyInput);
    

    
    setup(); 
    
    glutMainLoop(); 
}

