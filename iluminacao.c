// Felipe 1600001
// Jéssica 1490001
// Rodrigo 1488635

#include <stdio.h>

# ifdef __APPLE__ 
// # include< OpenGL/gl.h>
#include <GLUT/glut.h>
# else
// # include<GL/gl.h>
# include<GL/glut.h>
# endif


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "image.h"
 
#define PI 3.14159265358979324
 

 //textura
#define COORD_TEXTURA_PLANO 0.1
#define COR_DO_PLANO 0.52,0.52,0.78,1.0
#define TEXTURA_DO_PLANO "water3_1.rgb"
GLfloat ctp[4][2]={
    {-COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO},
    {+COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO},
    {+COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO},
    {-COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO}
};
GLuint  textura_plano;
static int shoulder = 0, elbow = 0, elbow2 = 0;

float cor_carro1[3];
int pos_carro1[2];
float cor_carro2[3];
int pos_carro2[2];
float cor_carro3[3];
int pos_carro3[2];
int idx_base = 0;
 // Globals.
 static float t = 0.0; // Animation parameter.
 static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
 static int isAnimate = 0; // Animated?
 static int animationPeriod = 1; // Time interval between frames.
 static unsigned int base; // Display lists base index.
 
// Luz local
float lightPos[] = {0.0, 50.0, 50.0, 1.0}; // Spotlight position.
static float spotAngle = 10.0; // Spotlight cone half-angle.
float spotDirection[] = {0.0, -1.0, 100.0}; // Spotlight direction.
static float spotExponent = 200.0; // Spotlight exponent = attenuation factor.

float angle, fAspect;
void carregar_texturas(void){
    IMAGE *img;
    GLenum gluerr;

    /* textura do plano */
    glGenTextures(1, &textura_plano);
    glBindTexture(GL_TEXTURE_2D, textura_plano);

    if(!(img=ImageLoad(TEXTURA_DO_PLANO))) {
        fprintf(stderr,"Error reading a texture.\n");
        exit(-1);
    }

    gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 
           img->sizeX, img->sizeY, 
           GL_RGB, GL_UNSIGNED_BYTE, 
           (GLvoid *)(img->data));
    if(gluerr){
        fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
        exit(-1);
    }

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

}

// Initialization routine.
void setup(void)
{  
     
    glEnable(GL_DEPTH_TEST); // Enable depth testing.

    base = glGenLists(8);
    glListBase(base);
    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //ILUMINAÇÃO GLOBAL 

    float luzAmbiente[4]={0.2,0.2,0.2,1.0};
    float luzDifusa[4]={0.7,0.7,0.7,1.0};		 // "cor"
    float luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
    float posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

    // Capacidade de brilho do material
    float especularidade[4]={1.0,1.0,1.0,1.0};
    int especMaterial = 60;

    // Especifica que a cor de fundo da janela sera azul
    glClearColor(0.2, 0.75, 0.9, 1.0);

    // Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);

    // Define o reflexo do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentraÃ§Ã£o do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    // Define os parâmetros da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    // //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    // // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);

    angle=90;
    carregar_texturas();
    srand(time(NULL));
    cor_carro1[0] = (rand()%10)/10.0;
    cor_carro1[1] = (rand()%10)/10.0;
    cor_carro1[2] = (rand()%10)/10.0;
    pos_carro1[0] = 100;
    pos_carro1[1] = -900;

    cor_carro2[0] = (rand()%10)/10.0;
    cor_carro2[1] = (rand()%10)/10.0;
    cor_carro2[2] = (rand()%10)/10.0;
    pos_carro2[0] = -100;
    pos_carro2[1] = 900;
    cor_carro3[0] = (rand()%10)/10.0;
    cor_carro3[1] = (rand()%10)/10.0;
    cor_carro3[2] = (rand()%10)/10.0;
    pos_carro3[0] = -100;
    pos_carro3[1] = 500;

}

void desenhaPassarela(){
    glNewList(base+idx_base, GL_COMPILE);
    idx_base++;
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
 
void desenhaPoste(){
    glNewList(base+idx_base, GL_COMPILE);
    idx_base++;
    glColor3f(0.66, 0.66, 0.66);
    glPushMatrix();
    glBegin(GL_QUADS);
    //BASE
        // Plano XY 1 
        glVertex3f(220, -100, 1);
        glVertex3f(220, -110, 1);
        glVertex3f(230, -110, 1);
        glVertex3f(230, -100, 1);
        // Plano XY 2
        glVertex3f(220, -100, 100);
        glVertex3f(220, -110, 100);
        glVertex3f(230, -110, 100);
        glVertex3f(230, -100, 100);
        // Plano YZ 1
        glVertex3f(220, -100, 1);
        glVertex3f(220, -110, 1);
        glVertex3f(220, -110, 100);
        glVertex3f(220, -100, 100);
        // Plano YZ 2
        glVertex3f(230, -100, 1);
        glVertex3f(230, -110, 1);
        glVertex3f(230, -110, 100);
        glVertex3f(230, -100, 100);
        // Plano XZ 1
        glVertex3f(220, -100, 1);
        glVertex3f(230, -100, 1);
        glVertex3f(230, -100, 100);
        glVertex3f(220, -100, 100);
        // Plano XZ 2
        glVertex3f(220, -110, 1);
        glVertex3f(230, -110, 1);
        glVertex3f(230, -110, 100);
        glVertex3f(220, -110, 100);

    //TOPO
        // Plano XY 1 
        glVertex3f(230, -100, 100);
        glVertex3f(230, -110, 100);
        glVertex3f(150, -110, 100);
        glVertex3f(150, -100, 100);
        // Plano XY 2
        glVertex3f(230, -100, 110);
        glVertex3f(230, -110, 110);
        glVertex3f(150, -110, 110);
        glVertex3f(150, -100, 110);
        // Plano YZ 1
        glVertex3f(230, -100, 100);
        glVertex3f(230, -110, 100);
        glVertex3f(230, -110, 110);
        glVertex3f(230, -100, 110);
        // Plano YZ 2
        glVertex3f(150, -100, 110);
        glVertex3f(150, -110, 110);
        glVertex3f(150, -110, 100);
        glVertex3f(150, -100, 100);
        // Plano XZ 1
        glVertex3f(230, -100, 100);
        glVertex3f(150, -100, 100);
        glVertex3f(150, -100, 110);
        glVertex3f(230, -100, 110);
        // Plano XZ 2
        glVertex3f(230, -110, 100);
        glVertex3f(150, -110, 100);
        glVertex3f(150, -110, 110);
        glVertex3f(230, -110, 110);

        glEnd();    
        glPopMatrix();
        glEndList();

        // glPushMatrix();

        //LUZ LOCAL
    //     glTranslatef(151, -105, 100); // Move the spotlight.

    //     glEnable(GL_LIGHTING);
    //     glEnable(GL_LIGHT1);
    //     // glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    //     // glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotAngle);
    //     // glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
    //     // glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, spotExponent);

    // float luzAmbiente[4]={0.2,0.2,0.2,1.0};
    // float luzDifusa[4]={0.7,0.7,0.7,1.0};        // "cor"
    // float luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
    // float posicaoLuz[4]={150.0, -105.0, 100.0, 1.0};

    // glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa );
    // glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular );
    // glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz );

        // glPopMatrix();
 
}
// direcao = 0, vai pra cima, qualquer outro, vai pra baixo
void desenhaCarro(int x, int y, float* cor, int direcao)
{
    int z = 20;
    glNewList(base+idx_base, GL_COMPILE);
    idx_base++;
    glColor4f(cor[0], cor[1], cor[2], 1);
    glPushMatrix();
    if(!direcao){
        glBegin(GL_QUADS);

            // PLANO XY PARTE DE BAIXO
            glVertex3f(x-25, y-100, z);
            glVertex3f(x-25, y+100, z);
            glVertex3f(x+25, y+100, z);
            glVertex3f(x+25, y-100, z);
            // PLANO XY CAPO
            glVertex3f(x-25, y+20, z+25);
            glVertex3f(x-25, y+100, z+15);
            glVertex3f(x+25, y+100, z+15);
            glVertex3f(x+25, y+20, z+25);
            // PLANO XY PORTA MALA
            glVertex3f(x-25, y-100, z+15);
            glVertex3f(x-25, y-50, z+25);
            glVertex3f(x+25, y-50, z+25);
            glVertex3f(x+25, y-100, z+15);
            // PLANO XZ PARACHOQUE DIANTEIRO
            glVertex3f(x-25, y+100, z);
            glVertex3f(x-25, y+100, z+15);
            glVertex3f(x+25, y+100, z+15);
            glVertex3f(x+25, y+100, z);
            // PLANO XZ PARACHOQUE TRASEIRO
            glVertex3f(x-25, y-100, z);
            glVertex3f(x-25, y-100, z+15);
            glVertex3f(x+25, y-100, z+15);
            glVertex3f(x+25, y-100, z);

            // PLANO XY CABINE
            glVertex3f(x-25, y-50, z+50);
            glVertex3f(x-25, y, z+50);
            glVertex3f(x+25, y, z+50);
            glVertex3f(x+25, y-50, z+50);
            glColor4f(cor[0], cor[1], cor[2], 0.5);

            // PLANO XZ VIDRO DIANTEIRO
            glVertex3f(x-25, y+20, z+25);
            glVertex3f(x-25, y, z+50);
            glVertex3f(x+25, y, z+50);
            glVertex3f(x+25, y+20, z+25);
            // PLANO XZ VIDRO TRASEIRO
            glVertex3f(x-25, y-50, z+25);
            glVertex3f(x-25, y-50, z+50);
            glVertex3f(x+25, y-50, z+50);
            glVertex3f(x+25, y-50, z+25);
            // PLANO YZ VIDRO LATERAL DIREITO
            glVertex3f(x-25, y-50, z+25);
            glVertex3f(x-25, y-50, z+50);
            glVertex3f(x-25, y, z+50);
            glVertex3f(x-25, y+20, z+25);
            // PLANO YZ VIDRO LATERAL DIREITO
            glVertex3f(x+25, y-50, z+25);
            glVertex3f(x+25, y-50, z+50);
            glVertex3f(x+25, y, z+50);
            glVertex3f(x+25, y+20, z+25);
            glColor4f(cor[0], cor[1], cor[2], 1);
        glEnd();    
        glBegin(GL_POLYGON);
            // PLANO YZ LATERAL DIREITO
            glVertex3f(x-25, y+100, z);
            glVertex3f(x-25, y+100, z+15);
            glVertex3f(x-25, y+20, z+25);
            glVertex3f(x-25, y-50, z+25);
            glVertex3f(x-25, y-100, z+15);
            glVertex3f(x-25, y-100, z);
            glVertex3f(x-25, y+100, z);
            // PLANO YZ LATERAL ESQUERDO
            glVertex3f(x+25, y+100, z);
            glVertex3f(x+25, y+100, z+15);
            glVertex3f(x+25, y+20, z+25);
            glVertex3f(x+25, y-50, z+25);
            glVertex3f(x+25, y-100, z+15);
            glVertex3f(x+25, y-100, z);
            glVertex3f(x+25, y+100, z);


        glEnd();
    }else{
            glBegin(GL_QUADS);

        // PLANO XY PARTE DE BAIXO
        glVertex3f(x+25, y+100, z);
        glVertex3f(x+25, y-100, z);
        glVertex3f(x-25, y-100, z);
        glVertex3f(x-25, y+100, z);
        // PLANO XY CAPO
        glVertex3f(x+25, y-20, z+25);
        glVertex3f(x+25, y-100, z+15);
        glVertex3f(x-25, y-100, z+15);
        glVertex3f(x-25, y-20, z+25);
        // PLANO XY PORTA MALA
        glVertex3f(x+25, y+100, z+15);
        glVertex3f(x+25, y+50, z+25);
        glVertex3f(x-25, y+50, z+25);
        glVertex3f(x-25, y+100, z+15);
        // PLANO XZ PARACHOQUE DIANTEIRO
        glVertex3f(x+25, y-100, z);
        glVertex3f(x+25, y-100, z+15);
        glVertex3f(x-25, y-100, z+15);
        glVertex3f(x-25, y-100, z);
        // PLANO XZ PARACHOQUE TRASEIRO
        glVertex3f(x+25, y+100, z);
        glVertex3f(x+25, y+100, z+15);
        glVertex3f(x-25, y+100, z+15);
        glVertex3f(x-25, y+100, z);

        // PLANO XY CABINE
        glVertex3f(x+25, y+50, z+50);
        glVertex3f(x+25, y, z+50);
        glVertex3f(x-25, y, z+50);
        glVertex3f(x-25, y+50, z+50);
        glColor4f(cor[0], cor[1], cor[2], 0.5);

        // PLANO XZ VIDRO DIANTEIRO
        glVertex3f(x+25, y-20, z+25);
        glVertex3f(x+25, y, z+50);
        glVertex3f(x-25, y, z+50);
        glVertex3f(x-25, y-20, z+25);
        // PLANO XZ VIDRO TRASEIRO
        glVertex3f(x+25, y+50, z+25);
        glVertex3f(x+25, y+50, z+50);
        glVertex3f(x-25, y+50, z+50);
        glVertex3f(x-25, y+50, z+25);
        // PLANO YZ VIDRO LATERAL DIREITO
        glVertex3f(x+25, y+50, z+25);
        glVertex3f(x+25, y+50, z+50);
        glVertex3f(x+25, y, z+50);
        glVertex3f(x+25, y-20, z+25);
        // PLANO YZ VIDRO LATERAL DIREITO
        glVertex3f(x-25, y+50, z+25);
        glVertex3f(x-25, y+50, z+50);
        glVertex3f(x-25, y, z+50);
        glVertex3f(x-25, y-20, z+25);
        glColor4f(cor[0], cor[1], cor[2], 1);
    glEnd();    
    glBegin(GL_POLYGON);
        // PLANO YZ LATERAL DIREITO
        glVertex3f(x+25, y-100, z);
        glVertex3f(x+25, y-100, z+15);
        glVertex3f(x+25, y-20, z+25);
        glVertex3f(x+25, y+50, z+25);
        glVertex3f(x+25, y+100, z+15);
        glVertex3f(x+25, y+100, z);
        glVertex3f(x+25, y-100, z);
        // PLANO YZ LATERAL ESQUERDO
        glVertex3f(x-25, y-100, z);
        glVertex3f(x-25, y-100, z+15);
        glVertex3f(x-25, y-20, z+25);
        glVertex3f(x-25, y+50, z+25);
        glVertex3f(x-25, y+100, z+15);
        glVertex3f(x-25, y+100, z);
        glVertex3f(x-25, y-100, z);


    glEnd();
    }


    glPopMatrix();
    glEndList();


}

// Drawing routine.
void drawScene(void)
{

    idx_base = 0;
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    // glTranslatef(0.0, -10.0, -30.0);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    // CALÇADAS
    glNewList(base+idx_base, GL_COMPILE);
    idx_base++;
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
    glNewList(base+idx_base, GL_COMPILE);
    idx_base++;
    glColor3f(0.66, 0.66, 0.66);
    glPushMatrix();
    //textura
    glColor4f(0.4,0.4,0.4,1);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glBindTexture(GL_TEXTURE_2D,textura_plano);
    glEnable(GL_TEXTURE_2D);
     
    glBegin(GL_POLYGON);
    glTexCoord2fv(ctp[3]); glVertex3f(-200.0, -1000.0, 1.0);
    glTexCoord2fv(ctp[2]); glVertex3f(200, -1000.0, 1.0);
    glTexCoord2fv(ctp[2]); glVertex3f(200.0, 1000.0, 1.0);
    glTexCoord2fv(ctp[3]); glVertex3f(-200, 1000.0, 1.0);
    glTexCoord2fv(ctp[3]); glVertex3f(-200.0, -1000.0, 1.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    // Faixas
    int i = 0;
    glColor4f(1,1,1,1);

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

     // Poste
    desenhaPoste();
     
    //Passarela
    desenhaPassarela(); 


    // Carro
    desenhaCarro(pos_carro1[0], pos_carro1[1], cor_carro1, 0);
    desenhaCarro(pos_carro2[0], pos_carro2[1], cor_carro2, 1);
    desenhaCarro(pos_carro3[0], pos_carro3[1], cor_carro3, 1);


    // Rotação da tela
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);
    // Desenho das listas
    int u;
    for(u =0; u < idx_base; u++)
        glCallList(base+u);

    glutSwapBuffers();
}
 
// Timer function.
void animate(int value)
{
    if (isAnimate)
    {
        pos_carro1[1]+= 10;
        if (pos_carro1[1] > 1000) pos_carro1[1] = -1000;
        pos_carro2[1]-= 10;
        if (pos_carro2[1] < -1000) pos_carro2[1] = 1000;
        pos_carro3[1]-= 10;
        if (pos_carro3[1] < -1000) pos_carro3[1] = 1000;

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
    glutCreateWindow ("rodovia.cpp"); 
    glutDisplayFunc(drawScene); 
    glutReshapeFunc(resize);  
    glutKeyboardFunc(keyInput);

    setup(); 

    glutMainLoop(); 
}
