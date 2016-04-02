/*
   Use the x/X, y/Y, z/Z, ,/< and ./> keys to move the camera.
   Use the 1/!, 2/@ and 3/# keys to rotate the model.
*/
#include "GLUTcallbacks.h"
#include <math.h>
extern Scene *scene;


static double xViewer =  0.0,   // location of viewer (in world coordinates)
              yViewer =  0.0,
              zViewer =  10.0;
static double theta1 = 0.0;     // angle1 for look-at vector (using spherical coordinates)
static double theta2 = 0.0;     // angle2 for look-at vector (using spherical coordinates)
static double thetaM[] = {0.0,0.0,0.0};  // angles for model transformation

static bool useOpenGL = false;   /* Determines OpenGL or software rendering mode */
static Timer timer1;             /* Used to compute rendering speed in frames per second */

/* GLUT callback function */
void display(void)
{
   double time;
   char name[128]; /* used for GLUT window title */

   /* get viewport dimensions */
   int w = scene->vp_width;
   int h = scene->vp_height;

   timer1.startTimer();

   /* set the projection matrix in the Scene object*/
   scene->proj2Identity();
   scene->projPerspective( 45,          /* field of view in degree */
                           w/(double)h, /* varying aspect ratio */
                           1.0,         /* near plane */
                           50.0 );      /* far plane */

   /* set the view matrix in the Scene object */
   scene->view2Identity();
   scene->viewRotate(-theta2, 1.0, 0.0, 0.0);
   scene->viewRotate(-theta1, 0.0, 1.0, 0.0);
   scene->viewTranslate(-xViewer, -yViewer, -zViewer);

   /* set the model matrix in the Scene object */
   scene->model2Identity();
   scene->modelRotate(thetaM[0], 1.0, 0.0, 0.0);
   scene->modelRotate(thetaM[1], 0.0, 1.0, 0.0);
   scene->modelRotate(thetaM[2], 0.0, 0.0, 1.0);

#if DEBUG3  /* display the matrices stored in the Scene object */
   fprintf(stderr, "\n");
   scene->projection.print();
   scene->view.print();
   scene->model.print();

   #if DEBUG3=2
     /* let OpenGL compute a projection matrix (for comparison) */
     glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     glLoadIdentity();
     gluPerspective( 45,          /* field of view in degree */
                     w/(double)h, /* varying aspect ratio */
                     1.0,         /* near plane */
                     50.0 );      /* far plane */
     printOpenGLMatrix(GL_PROJECTION_MATRIX);
     glPopMatrix();

     /* let OpenGL compute a modelview matrix (for comparison) */
     glMatrixMode(GL_MODELVIEW);
     glPushMatrix();
     glLoadIdentity();
     glRotated(-theta2, 1.0, 0.0, 0.0);          // view part
     glRotated(-theta1, 0.0, 1.0, 0.0);
     glTranslated(-xViewer, -yViewer, -zViewer);
     glRotated(thetaM[0], 1.0, 0.0, 0.0);        // model part
     glRotated(thetaM[1], 0.0, 1.0, 0.0);
     glRotated(thetaM[2], 0.0, 0.0, 1.0);
     printOpenGLMatrix(GL_MODELVIEW_MATRIX);
     // here is the "model view" matrix diretly from the Scene
     ((scene->view).times(scene->model)).print();
     glPopMatrix();
   #endif /* DEBUG3=2 */
#endif /* DEBUG3 */

   /*** render the scene ***/
   if (useOpenGL)
   {
      renderSceneOpenGL(scene);
   }
   else /* software rendering mode */
   {
      FrameBuffer fb(w, h); /* This is the object that the
                               software renderer will render
                               the Scene object into. */

      // initialize the framebuffer (set a background color)
      fb.clearFB(220, 220, 220);

      renderScene(scene, &fb);

      /* now transfer the pixel data from our framebuffer to OpenGL's frame buffer */
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glDrawPixels( w, h, GL_RGB, GL_UNSIGNED_BYTE, fb.pixel_buffer );
      glFlush();
      glFinish(); /* wait while everything gets rendered */
      checkErrorsOpenGL("OpenGL errors in software rendering of scene!");
#if 0
      /* The rendered image is now in the viewport
         within the framebuffer. We need to transfer
         the pixel data from the framebuffer into
         a contiguous buffer so that we can then
         transfer the data to OpenGL's frame buffer.
      */
      unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * w * h * 3);
      if (!data)
      {
         fprintf(stderr, "ERROR! Unable to allocate memory for data buffer\n");
         fflush(stderr);
         exit(-1);
      }

      for (int i = 0; i < h; i++)
      {
         for(int j = 0; j < w; j++)
         {
            unsigned char *p = NULL;
            fb.getPixelVP(i,j, &p);
            *(data + i*w*3 + j*3 + 0) = p[0];
            *(data + i*w*3 + j*3 + 1) = p[1];
            *(data + i*w*3 + j*3 + 2) = p[2];
         }
      }
      /* now transfer the pixel data to OpenGL's frame buffer */
      //glPixelTransfer();
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      //glWindowPos2i(0, 0);
      glDrawPixels( w, h, GL_RGB, GL_UNSIGNED_BYTE, data );
      glFlush();
      glFinish(); /* wait while everything gets rendered */
      checkErrorsOpenGL("OpenGL errors in software rendering of scene!");
      free(data);
#endif
   }

   timer1.stopTimer();
   time = timer1.getTime();
   sprintf(name, "VP w=%d, h=%d: %s: %.4lf s, %.2lf fps", w, h,
                                                          useOpenGL ? "GL" : "soft",
                                                          time, 1.0/time);
   glutSetWindowTitle(name);

   glutSwapBuffers();
   return;
}


void reshape(int w, int h)
{
   scene->vp_width  = w;  /* used to set the viewport */
   scene->vp_height = h;

   display();
   return;
}


void redisplay(void)
{
   glutPostRedisplay();
   return;
}


void keySpecial(int key, int x, int y)
{printf("get special key\n");
   switch (key)
   {
      case GLUT_KEY_LEFT:
         theta1 -= 2.0;
         if( theta1 < 0.0 ) theta1 += 360.0;
         break;
      case GLUT_KEY_RIGHT:
         theta1 += 2.0;
         if( theta1 > 360.0 ) theta1 -= 360.0;
         break;
      case GLUT_KEY_DOWN:
         theta2 -= 2.0;
         if( theta2 < -70.0 ) theta2 = -70.0; // clamp at -70 degrees
         break;
      case GLUT_KEY_UP:
         theta2 += 2.0;
         if( theta2 > 70.0 ) theta2 = 70.0; // clamp at 70 degrees
         break;
      default:
         break;
   }
   glutPostRedisplay();
   return;
}


void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 27:
      case 'q':
      case 'Q':
         /* quit the program */
         exit(0);
         break;
      case ' ':
         /* toggle between OpenGL and sofware rendering */
         useOpenGL = !useOpenGL;
         redisplay();
         break;
      case 'f':
         scene->frontFacingIsCCW = !(scene->frontFacingIsCCW);
         fprintf(stderr, "scene->frontFacingIsCCW = %s\n",
                          scene->frontFacingIsCCW ? "true" : "false");
         break;
      case 'c':
         scene->doBackFaceCulling = !(scene->doBackFaceCulling);
         fprintf(stderr, "scene->doBackFaceCulling = %s\n",
                          scene->doBackFaceCulling ? "true" : "false");
         break;
      case 'H':
         fprintf(stderr, "space - toggle between software and hardware renderers\n");
         fprintf(stderr, "p - toggle between perspective and parallel projections\n");
         fprintf(stderr, "c - turn on and off backface culling\n");
         fprintf(stderr, "f - toggle front face as CW or CCW\n");
         fprintf(stderr, "l - turn lighting calculation on and off\n");
         fprintf(stderr, "s - do smooth or flat lighting\n");
         fprintf(stderr, "O - ???? \n");
         fprintf(stderr, "o - cycle between 4 different flat lighting models\n");
         fprintf(stderr, "h - turn on and off the use of the \"half vector\"\n");
         fprintf(stderr, "H - display this help message\n");
         fprintf(stderr, "b - toggle bilinear interpolation of the textures\n");
         fprintf(stderr, "x/X - move viewer in the pos/neg x-axis direction\n");
         fprintf(stderr, "y/Y - move viewer in the pos/neg y-axis direction\n");
         fprintf(stderr, "z/Z - move viewer in the pos/neg z-axis direction\n");

         fprintf(stderr, ",/< - rotate viewer about x-axis in the neg/pos direction\n");
         fprintf(stderr, "./> - rotate viewer about y-axis in the neg/pos direction\n");

         fprintf(stderr, "1/! - rotate model about x-axis in the pos/neg direction\n");
         fprintf(stderr, "2/@ - rotate model about y-axis in the pos/neg direction\n");
         fprintf(stderr, "3/# - rotate model about z-axis in the pos/neg direction\n");

         fprintf(stderr, "\n");
         fprintf(stderr, "\n");
         fprintf(stderr, "\n");
         fprintf(stderr, "\n");
         fprintf(stderr, "\n");
         break;
      case 'l':
         scene->doLighting = !(scene->doLighting);
         fprintf(stderr, "scene->doLighting = %s\n",
                          scene->doLighting ? "true" : "false");
         break;
      case 's':
         scene->doSmoothLighting = !(scene->doSmoothLighting);
         fprintf(stderr, "scene->doSmoothLighting = %s\n",
                          scene->doSmoothLighting ? "true" : "false");
         break;
      case 'o':
         scene->doOpenglFlatLighting = (scene->doOpenglFlatLighting + 1) % 4;
         fprintf(stderr, "scene->doOpenglFlatLighting = %d\n",
                          scene->doOpenglFlatLighting);
         break;
      case 'O':
         scene->doOpenglFlatLighting = (scene->doOpenglFlatLighting - 1);
         if (scene->doOpenglFlatLighting < 0) scene->doOpenglFlatLighting = 3;
         fprintf(stderr, "scene->doOpenglFlatLighting = %d\n",
                          scene->doOpenglFlatLighting);
         break;
      case 'h':
         scene->useHalfVector = !(scene->useHalfVector);
         fprintf(stderr, "scene->useHalfVector = %s\n",
                          scene->useHalfVector ? "true" : "false");
         break;
      case 'x':
         xViewer += 0.5;
         break;
      case 'X':
         xViewer -= 0.5;
         break;
      case 'y':
         yViewer += 0.5;
         break;
      case 'Y':
         yViewer -= 0.5;
         break;
      case 'z':
         zViewer += 0.5;
         break;
      case 'Z':
         zViewer -= 0.5;
         break;

      case ',':  // unshifted '<'
         theta1 -= 2.0;
         if( theta1 < 0.0 ) theta1 += 360.0;
         break;
      case '.':  // unshifted '>'
         theta1 += 2.0;
         if( theta1 > 360.0 ) theta1 -= 360.0;
         break;
      case '<':
         theta2 -= 2.0;
         if( theta2 < -70.0 ) theta2 = -70.0; // clamp at -70 degrees
         break;
      case '>':
         theta2 += 2.0;
         if( theta2 > 70.0 ) theta2 = 70.0; // clamp at 70 degrees
         break;

      case '1':
         thetaM[0] += 2.0;
         if( thetaM[0] > 360.0 ) thetaM[0] -= 360.0;
         break;
      case '!':    // shift 1
         thetaM[0] -= 2.0;
         if( thetaM[0] < 0.0 ) thetaM[0] += 360.0;
         break;
      case '2':
         thetaM[1] += 2.0;
         if( thetaM[1] > 360.0 ) thetaM[1] -= 360.0;
         break;
      case '@':   // shift 2
         thetaM[1] -= 2.0;
         if( thetaM[1] < 0.0 ) thetaM[1] += 360.0;
         break;
      case '3':
         thetaM[2] += 2.0;
         if( thetaM[2] > 360.0 ) thetaM[2] -= 360.0;
         break;
      case '#':   // shift 3
         thetaM[2] -= 2.0;
         if( thetaM[2] < 0.0 ) thetaM[2] += 360.0;
         break;

      case 'b':
         /* toggle bilinear interpolation of the textures */
         scene->bilerp_flag = !(scene->bilerp_flag);
         switchTextureFilteringOpenGL(scene);
         if (scene->bilerp_flag)
         {
            fprintf(stderr,"glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)\n");
         }
         else
         {
            fprintf(stderr, "glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)\n");
         }
         fflush(stderr);
         redisplay();
         break;

      default:
         break;
   }
   glutPostRedisplay();
   return;
}
