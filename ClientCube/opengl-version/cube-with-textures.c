/*
   Use the x/X, y/Y and z/Z keys to move the camera.
   Use the 1/!, 2/@ and 3/# keys to rotate the cube.

   Be sure to look at all the sides of the cube.
*/
#include <windows.h>
#include <stdio.h>
#include "..\gl\glut.h"

/* Prototypes for the GLUT callback functions */
void display( void );
void keyboard( unsigned char, int, int );

/* global variables */
double xViewer =  3.0,
       yViewer =  3.0,
       zViewer = 10.0;

GLfloat theta[] = {0.0,0.0,0.0};

#define SIZE 256

GLuint texture1;  // wood,ppm image
GLuint texture2;  // Yoda.ppm image
GLuint texture3;  // pascal image

void initializeTextures()
{
   FILE* fp;
   char buf[30];
   int width, height, rgbDepth;
   int i, j, c;

   // Declare arrays to hold the texture pixel values
   GLubyte    woodImage[SIZE * SIZE * 3];
   GLubyte    yodaImage[SIZE * SIZE * 3];
   GLubyte  pascalImage[SIZE][SIZE][3];   // a procedural texture

   // First Texture
   // Read a "raw" texture image from a file
   fp = fopen("wood.ppm", "rb");
   if (!fp)
   {
      printf("Unable to open file: %s\n", "wood.ppm");
      exit(1);
   }
   fscanf(fp, "%s", buf);                /* read image format string "P6" */
   fscanf(fp, "%s%s%s%s", buf, buf, buf, buf);  /* read IrfanView comment */
   fscanf(fp, "%d%d", &width, &height);  /* read image dimensions */
   fscanf(fp, "%d", &rgbDepth);           /* read image rgb depth (which we don't use) */
   while (fgetc(fp) != '\n') ;           /* eat line feeds */
   for (i = 0; i < height; i++)          /* read pixel data from file */
   {  /* read one row of pixels at a time,
         store them from the bottom row of the data buffer
         up towards the top row */
      if (fread(woodImage+((height-i-1)*3*width), 3 * width, 1, fp) != 1)
      {
         fprintf(stderr, "ERROR! Could not load texture %s\n", "wood.ppm");
         fflush(stderr);
         exit(-1);
      }
   }
   fclose(fp);
   // Get a texture ID and create a texture object
   glGenTextures(1, &texture1);
   glBindTexture(GL_TEXTURE_2D, texture1);
   // Set some texture properties in this texture object
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // Specifiy this texture object's pixel image data
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIZE, SIZE, 0,
                GL_RGB, GL_UNSIGNED_BYTE, woodImage);


   // Second Texture
   // Read a JPEG texture image from a file
   fp = fopen("Yoda.ppm", "rb");
   if (!fp)
   {
      printf("Unable to open file: %s\n", "Yoda.ppm");
      exit(1);
   }
   fscanf(fp, "%s", buf);                /* read image format string "P6" */
   fscanf(fp, "%s%s%s%s", buf, buf, buf, buf);  /* read IrfanView comment */
   fscanf(fp, "%d%d", &width, &height);  /* read image dimensions */
   fscanf(fp, "%d", &rgbDepth);           /* read image rgb depth (which we don't use) */
   while (fgetc(fp) != '\n') ;           /* eat line feeds */
   for (i = 0; i < height; i++)          /* read pixel data from file */
   {  /* read one row of pixels at a time,
         store them from the bottom row of the data buffer
         up towards the top row */
      if (fread(yodaImage+((height-i-1)*3*width), 3 * width, 1, fp) != 1)
      {
         fprintf(stderr, "ERROR! Could not load texture %s\n", "Yoda.ppm");
         fflush(stderr);
         exit(-1);
      }
   }
   fclose(fp);
   // Get a texture ID and create a texture object
   glGenTextures(1, &texture2);
   glBindTexture(GL_TEXTURE_2D, texture2);
   // Set some texture properties in this texture object
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // Specifiy this texture object's pixel image data
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIZE, SIZE, 0,
                GL_RGB, GL_UNSIGNED_BYTE, yodaImage);


   // Third Texture
   // Create a "procedural" texture image, pixel by pixel
   for (i = 0; i < SIZE; i++)
   {
      for (j = 0; j < SIZE; j++)
      {
         c = (i|j)%255;  // Pascal's Triangle like image
         pascalImage[i][j][0] = (GLubyte) c;
         pascalImage[i][j][1] = (GLubyte) c;
         pascalImage[i][j][2] = (GLubyte) c;
      }
   }
   // Get a texture ID and create a texture object
   glGenTextures(1, &texture3);
   glBindTexture(GL_TEXTURE_2D, texture3);
   // Set some texture properties in this texture object
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // Specifiy this texture object's pixel image data
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIZE, SIZE, 0,
                GL_RGB, GL_UNSIGNED_BYTE, pascalImage);

}//initializeTextures()


int main(int argc, char** argv)
{
   /* Standard GLUT initialization */
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(200, 200);
   glutCreateWindow(argv[0]);
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);

   /* Initialize OpenGL state */
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHT0);
   glShadeModel(GL_SMOOTH);
   glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */

   /* initialize all of the textures */
   initializeTextures();

   glutMainLoop(); /* Enter GLUT event loop */
   return 0;       /* we never get here */
}


void display(void)
{
   /* set up the projection */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(17, 1.0, 1.0, 20.0);

   /* set up viewing */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(xViewer, yViewer, zViewer, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glRotatef(theta[0], 1.0, 0.0, 0.0);
   glRotatef(theta[1], 0.0, 1.0, 0.0);
   glRotatef(theta[2], 0.0, 0.0, 1.0);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* draw three "coordinate" axes. */
   glDisable(GL_LIGHTING);
   glBegin(GL_LINES);
     glColor3f(1.0, 0.0, 0.0); /* draw in red */
     glVertex3f(0.0, 0.0, 0.0);
     glVertex3f(3.0, 0.0, 0.0);
     glColor3f(0.0, 1.0, 0.0); /* draw in green */
     glVertex3f(0.0, 0.0, 0.0);
     glVertex3f(0.0, 3.0, 0.0);
     glColor3f(0.0, 0.0, 1.0); /* draw in blue */
     glVertex3f(0.0, 0.0, 0.0);
     glVertex3f(0.0, 0.0, 3.0);
   glEnd();
   glEnable(GL_LIGHTING);   /* enable lighting */

   /* draw a unit cube */
   /* front */
   glNormal3f(0.0, 0.0, 1.0);
   glEnable(GL_TEXTURE_2D);                // Enable texture mapping
   glBindTexture(GL_TEXTURE_2D, texture2); // Pick a texture object
   glBegin(GL_QUADS);
     // Send each vertex down the pipeline with a texture coordinater
     glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 1.0);
     glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.0, 1.0);
     glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
     glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1.0, 1.0);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   /* top */
   glNormal3f(0.0, 1.0, 0.0);
   glEnable(GL_TEXTURE_2D);                // Enable texture mapping
   glBindTexture(GL_TEXTURE_2D, texture3); // Pick a texture object
   glBegin(GL_QUADS);
     // Send each vertex down the pipeline with a texture coordinater
     glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 1.0, 0.0);
     glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
     glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
     glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1.0, 1.0);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   /* right */
   glNormal3f(1.0, 0.0, 0.0);
   glEnable(GL_TEXTURE_2D);                // Enable texture mapping
   glBindTexture(GL_TEXTURE_2D, texture1); // Pick a texture object
   glBegin(GL_QUADS);
     glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 0.0, 0.0);
     glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
     glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
     glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 0.0, 1.0);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   /* back */
   glNormal3f(0.0, 0.0, -1.0);
   glBegin(GL_QUADS);
     glVertex3f(0.0, 0.0, 0.0);
     glVertex3f(0.0, 1.0, 0.0);
     glVertex3f(1.0, 1.0, 0.0);
     glVertex3f(1.0, 0.0, 0.0);
   glEnd();

   /* bottom */
   glNormal3f(0.0, -1.0, 0.0);
   glPolygonMode(GL_FRONT, GL_LINE);
   glBegin(GL_TRIANGLE_FAN);
     glVertex3f(0.5, 0.0, 0.5);
     glVertex3f(0.5, 0.0, 1.0);
     glVertex3f(0.0, 0.0, 1.0);
     glVertex3f(0.0, 0.0, 0.5);
     glVertex3f(0.0, 0.0, 0.0);
     glVertex3f(0.5, 0.0, 0.0);
     glVertex3f(1.0, 0.0, 0.0);
     glVertex3f(1.0, 0.0, 0.5);
     glVertex3f(1.0, 0.0, 1.0);
     glVertex3f(0.5, 0.0, 0.5);
   glEnd();
   glPolygonMode(GL_FRONT, GL_FILL);

   /* left */
   glNormal3f(-1.0, 0.0, 0.0);
   glDisable(GL_LIGHTING);
   glBegin(GL_QUADS);
     // Send each vertex down the pipeline with a vertex color
     glColor3f(1.0, 0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
     glColor3f(0.0, 1.0, 0.0); glVertex3f(0.0, 0.0, 1.0);
     glColor3f(0.0, 0.0, 1.0); glVertex3f(0.0, 1.0, 1.0);
     glColor3f(1.0, 1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
   glEnd();
   glEnable(GL_LIGHTING);

   glFlush(); /* clear buffers */
}//display()


void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 'x':
         xViewer+=0.5;
         break;
      case 'X':
         xViewer-=0.5;
         break;
      case 'y':
         yViewer+=0.5;
         break;
      case 'Y':
         yViewer-=0.5;
         break;
      case 'z':
         zViewer+=0.5;
         break;
      case 'Z':
         zViewer-=0.5;
         break;
      case '1':
         theta[0] += 2.0;
         if( theta[0] > 360.0 ) theta[0] -= 360.0;
         break;
      case '!':    // shift 1
         theta[0] -= 2.0;
         if( theta[0] < 0.0 ) theta[0] += 360.0;
         break;
      case '2':
         theta[1] += 2.0;
         if( theta[1] > 360.0 ) theta[1] -= 360.0;
         break;
      case '@':   // shift 2
         theta[1] -= 2.0;
         if( theta[1] < 0.0 ) theta[1] += 360.0;
         break;
      case '3':
         theta[2] += 2.0;
         if( theta[2] > 360.0 ) theta[2] -= 360.0;
         break;
      case '#':   // shift 3
         theta[2] -= 2.0;
         if( theta[2] < 0.0 ) theta[2] += 360.0;
         break;
      default:
         break;
   }
   printf("xViewer = %f, yViewer = %f, zViewer = %f\n", xViewer, yViewer, zViewer);
   printf("theta[0] = %f, theta[1] = %f, theta[2] = %f\n", theta[0], theta[1], theta[2]);
   glutPostRedisplay();
}//keyboard()
