#include <stdio.h>
#include <stdlib.h>
#include <math.h>
# ifdef __APPLE__
# include<OpenGL/gl.h>
# include<GLUT/glut.h>
# else
# include<GL/gl.h>
# include<GL/glut.h>
# endif


#define COORD_TEXTURA_PLANO 1.0
#define COR_DO_PLANO 0.52,0.52,0.78,1.0
#define TEXTURA_DO_PLANO "montanhas.rgb"

typedef struct {
  unsigned short imagic;
  unsigned short type;
  unsigned short dim;
  unsigned short sizeX, sizeY, sizeZ;
  char name[128];
   char *data;
} IMAGE;

typedef struct {
  unsigned short imagic;
  unsigned short type;
  unsigned short dim;
  unsigned short sizeX, sizeY, sizeZ;
  unsigned long min, max;
  unsigned long wasteBytes;
  char name[80];
  unsigned long colorMap;
  FILE *file;
  unsigned char *tmp[5];
  unsigned long rleEnd;
  unsigned long *rowStart;
  unsigned long *rowSize;
} Image;

IMAGE *ImageLoad(char *);

#define COORD_TEXTURA_PLANO 1.0
#define COR_DO_PLANO 0.6,0.6,0.6,1.0
#define TEXTURA_DO_PLANO "montanhas.rgb"
#define IMAGIC      0x01da
#define IMAGIC_SWAP 0xda01

#define SWAP_SHORT_BYTES(x) ((((x) & 0xff) << 8) | (((x) & 0xff00) >> 8))
#define SWAP_LONG_BYTES(x) (((((x) & 0xff) << 24) | (((x) & 0xff00) << 8)) | \
((((x) & 0xff0000) >> 8) | (((x) & 0xff000000) >> 24)))



GLuint  textura_plano;
static int shoulder = 0, elbow = 0, elbow2 = 0;
  

static Image *ImageOpen(char *fileName) {
  Image *image;
  unsigned long *rowStart, *rowSize, ulTmp;
  int x, i;

  image = (Image *)malloc(sizeof(Image));
  if (image == NULL) {
    fprintf(stderr, "Out of memory!\n");
    exit(-1);
  }
  if ((image->file = fopen(fileName, "rb")) == NULL) {
    perror(fileName);
    exit(-1);
  }
  /*
  * Read the image header
  */
  fread(image, 1, 12, image->file);
  /*
  * Check byte order
  */
  if (image->imagic == IMAGIC_SWAP) {
    image->type = SWAP_SHORT_BYTES(image->type);
    image->dim = SWAP_SHORT_BYTES(image->dim);
    image->sizeX = SWAP_SHORT_BYTES(image->sizeX);
    image->sizeY = SWAP_SHORT_BYTES(image->sizeY);
    image->sizeZ = SWAP_SHORT_BYTES(image->sizeZ);
  }

  for (i = 0; i <= image->sizeZ; i++) {
    image->tmp[i] = (unsigned char *)malloc(image->sizeX * 256);
    if (image->tmp[i] == NULL) {
      fprintf(stderr, "Out of memory!\n");
      exit(-1);
    }
  }

  if ((image->type & 0xFF00) == 0x0100) { /* RLE image */
    x = image->sizeY * image->sizeZ * sizeof(long);
    image->rowStart = (unsigned long *)malloc(x);
    image->rowSize = (unsigned long *)malloc(x);
    if (image->rowStart == NULL || image->rowSize == NULL) {
      fprintf(stderr, "Out of memory!\n");
      exit(-1);
    }
    image->rleEnd = 512 + (2 * x);
    fseek(image->file, 512, SEEK_SET);
    fread(image->rowStart, 1, x, image->file);
    fread(image->rowSize, 1, x, image->file);
    if (image->imagic == IMAGIC_SWAP) {
      x /= sizeof(long);
      rowStart = image->rowStart;
      rowSize = image->rowSize;
      while (x--) {
        ulTmp = *rowStart;
        *rowStart++ = SWAP_LONG_BYTES(ulTmp);
        ulTmp = *rowSize;
        *rowSize++ = SWAP_LONG_BYTES(ulTmp);
      }
    }
  }
  return image;
}

static void ImageClose(Image *image) {
  int i;

  fclose(image->file);
  for (i = 0; i <= image->sizeZ; i++)
    free(image->tmp[i]);
  free(image);
}

static void ImageGetRow(Image *image, unsigned char *buf, int y, int z) {
  unsigned char *iPtr, *oPtr, pixel;
  int count;

  if ((image->type & 0xFF00) == 0x0100) {  /* RLE image */
    fseek(image->file, image->rowStart[y + z*image->sizeY], SEEK_SET);
    fread(image->tmp[0], 1, (unsigned int)image->rowSize[y + z*image->sizeY],
      image->file);

    iPtr = image->tmp[0];
    oPtr = buf;
    while (1) {
      pixel = *iPtr++;
      count = (int)(pixel & 0x7F);
      if (!count)
        return;
      if (pixel & 0x80) {
        while (count--) {
          *oPtr++ = *iPtr++;
        }
      }
      else {
        pixel = *iPtr++;
        while (count--) {
          *oPtr++ = pixel;
        }
      }
    }
  }
  else { /* verbatim image */
    fseek(image->file, 512 + (y*image->sizeX) + (z*image->sizeX*image->sizeY),
      SEEK_SET);
    fread(buf, 1, image->sizeX, image->file);
  }
}

static void ImageGetRawData(Image *image, char *data)
{
  int i, j, k;
  int remain;

  switch (image->sizeZ) {
  case 1:
    remain = image->sizeX % 4;
    break;
  case 2:
    remain = image->sizeX % 2;
    break;
  case 3:
    remain = (image->sizeX * 3) & 0x3;
    if (remain)
      remain = 4 - remain;
    break;
  case 4:
    remain = 0;
    break;
  }

  for (i = 0; i < image->sizeY; i++) {
    for (k = 0; k < image->sizeZ; k++)
      ImageGetRow(image, image->tmp[k + 1], i, k);
    for (j = 0; j < image->sizeX; j++)
      for (k = 1; k <= image->sizeZ; k++)
        *data++ = *(image->tmp[k] + j);
    data += remain;
  }
}

IMAGE *ImageLoad(char *fileName)
{
  Image *image;
  IMAGE *final;
  int sx;

  image = ImageOpen(fileName);

  final = (IMAGE *)malloc(sizeof(IMAGE));
  if (final == NULL) {
    fprintf(stderr, "Out of memory!\n");
    exit(-1);
  }
  final->imagic = image->imagic;
  final->type = image->type;
  final->dim = image->dim;
  final->sizeX = image->sizeX;
  final->sizeY = image->sizeY;
  final->sizeZ = image->sizeZ;

  /*
  * Round up so rows are long-word aligned
  */
  sx = ((image->sizeX) * (image->sizeZ) + 3) >> 2;

  final->data = ( char *)malloc(sx * image->sizeY * sizeof(unsigned int));

  if (final->data == NULL) {
    fprintf(stderr, "Out of memory!\n");
    exit(-1);
  }

  ImageGetRawData(image, final->data);
  //ImageGetRawData(image, (char *)final->data);  //para rodar no dev, trocar a linha acima
  ImageClose(image);
  return final;
}


GLfloat ctp[4][2]={
  {-COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO},
  {+COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO},
  {+COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO},
  {-COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO}
};

GLuint  textura_plano;



void display(void){

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    /* habilita/desabilita uso de texturas*/
  glEnable(GL_TEXTURE_2D);

  glColor4f(COR_DO_PLANO);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
  glBindTexture(GL_TEXTURE_2D,textura_plano);
   
  glBegin(GL_QUADS);
  glTexCoord2fv(ctp[0]);  glVertex3f(-5,-3.2,5);
  glTexCoord2fv(ctp[1]);  glVertex3f(5,-3.2,5);
  glTexCoord2fv(ctp[2]);  glVertex3f(5,-3.2,-5);
  glTexCoord2fv(ctp[3]);  glVertex3f(-5,-3.2,-5);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  glPushMatrix();


  glRotatef ((GLfloat) shoulder, 0.0, 1.0, 0.0);

  /* Chao*/
  glColor3f(0.0,0.0,1.0);
  glPushMatrix();
  glTranslatef (0.0, -3.4, 0.0);
  glScalef (10.0, 0.2, 10.0);
  glutSolidCube (1.0);
  glPopMatrix();

  /* armario */
  glColor3f(1.0,0.0,1.0);
  glPushMatrix();
  glScalef (4.0, 6.0, 2.0);
  glutSolidCube (1.0);
  glPopMatrix();

  /* porta 1*/

  glColor3f(1.0,1.0,0.0);
  glPushMatrix();
  glTranslatef (-2.0, 0.0, 1.1);
  glRotatef ((GLfloat) elbow, 0.0, 1.0, 0.0);
  glTranslatef (2.0, 0.0, -1.1);
  glTranslatef (-1.0, 0.0, 1.0);
  glScalef (2.0, 6.0, 0.2);
  glutSolidCube (1.0);
  glColor3f(1.0,0.0,0.0);
  glPushMatrix();    /* desenha macaneta 1*/
  glTranslatef (0.4, 0.0, 1.1);
  glScalef (0.2, 0.2, 0.2);
  glutSolidCube (1.0);
  glPopMatrix();
  glPopMatrix();

/* porta 2*/

  glColor3f(1.0,1.0,0.0);
  glPushMatrix();
  glTranslatef (2.0, 0.0, 1.1);
  glRotatef ((GLfloat) elbow2, 0.0, 1.0, 0.0);
  glTranslatef (-2.0, 0.0, -1.1);
  glTranslatef (1.0, 0.0, 1.0);
  glScalef (2.0, 6.0, 0.2);
  glutSolidCube (1.0);
  glColor3f(1.0,0.0,0.0);
  glPushMatrix();    /* desenha macaneta 2*/
  glTranslatef (-0.4, 0.0, 1.1);
  glScalef (0.2, 0.2, 0.2);
  glutSolidCube (1.0);
  glPopMatrix();
  glPopMatrix();

  /* origem volta para o sistema de coordenadas original */
  glPopMatrix();
  glutSwapBuffers();
}

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

void reshape (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef (0.0, 0.0, -15.0);
}

void keyboard (unsigned char key, int x, int y){
  switch (key) {
  case 's':
    shoulder = (shoulder + 5) % 360;
    glutPostRedisplay();
    break;
  case 'S':
    shoulder = (shoulder - 5) % 360;
    glutPostRedisplay();
    break;
  case 'e':
    elbow = (elbow + 5) % 360;
    glutPostRedisplay();
    break;
  case 'E':
    elbow = (elbow - 5) % 360;
    glutPostRedisplay();
    break;
  case 'p':
    elbow2 = (elbow2 + 5) % 360;
    glutPostRedisplay();
    break;
  case 'P':
    elbow2 = (elbow2 - 5) % 360;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}

void init(void){

  glClearColor (1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  carregar_texturas();
  //glEnable(GL_TEXTURE_2D);
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init ();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0; 
}