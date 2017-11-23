// Felipe 1600001
// Jéssica 1490001
// Rodrigo 1488635



#include <stdlib.h>
#include <math.h>
#include <stdio.h>
// #include <GL/GLUT.h>
# ifdef __APPLE__
# include<OpenGL/gl.h>
# include<GLUT/glut.h>
# else
# include<GL/gl.h>
# include<GL/glut.h>
# endif

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

#define COORD_TEXTURA_PLANO 30.0
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
	*	Read the image header
	*/
	fread(image, 1, 12, image->file);
	/*
	*	Check byte order
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

void carregar_texturas(void) {
	IMAGE *img;
	GLenum gluerr;

	/* textura do plano */
	glGenTextures(1, &textura_plano);
	glBindTexture(GL_TEXTURE_2D, textura_plano);

	if (!(img = ImageLoad(TEXTURA_DO_PLANO))) {
		fprintf(stderr, "Error reading a texture.\n");
		exit(-1);
	}

	gluerr = gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
		img->sizeX, img->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE,
		(GLvoid *)(img->data));
	if (gluerr) {
		fprintf(stderr, "GLULib%s\n", gluErrorString(gluerr));
		exit(-1);
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}


void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	carregar_texturas();
	glOrtho(0, 500, 0, 500, -1, 1);
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//pista
	glEnable(GL_TEXTURE_2D);

	glColor4f(COR_DO_PLANO);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, textura_plano);

	// glBegin(GL_QUADS);
	//   glTexCoord2fv(ctp[0]);  glVertex3f(-0,0,0);
	//   glTexCoord2fv(ctp[1]);  glVertex3f(190,200,0);
	//   glTexCoord2fv(ctp[2]);  glVertex3f(300,200,0);
	//   glTexCoord2fv(ctp[3]);  glVertex3f(500,0,0);
	//   glEnd();


	//glColor3f(0.66, 0.66, 0.66);
	glBegin(GL_QUADS);
	glTexCoord2fv(ctp[1]); glVertex2i(0,0);  
	glTexCoord2fv(ctp[0]); glVertex2i(190,200);
	glTexCoord2fv(ctp[1]); glVertex2i(300,200); 
	glTexCoord2fv(ctp[0]); glVertex2i(500,0);
	glTexCoord2fv(ctp[1]); glVertex2i(0,0);  
	// glTexCoord2fv(ctp[1]); glVertex2i(500,0); 
	// glTexCoord2fv(ctp[3]); glVertex2i(300,200);
	// glTexCoord2fv(ctp[1]); glVertex2i(0,0); 
	glEnd();
	  glDisable(GL_TEXTURE_2D);

	//Céu
	glColor3f(0.68, 0.85, 0.9);
	glBegin(GL_QUADS);
	glVertex2i(0,200); glVertex2i(0,500);
	glVertex2i(500,500); glVertex2i(500,500);
	glVertex2i(500,200);glVertex2i(500,500);
	glVertex2i(0,200); glVertex2i(500,200);
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
}

void keyboard(unsigned char key, int x, int y) {
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
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TOPGEAR2017.cpp");
	setup();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

}