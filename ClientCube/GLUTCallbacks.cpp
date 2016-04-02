/*
   Use the x/X, y/Y and z/Z keys to move the camera.
   Use the 1/!, 2/@ and 3/# keys to rotate the model.
*/
#include "GLUTcallbacks.h"

extern Scene *scene;
extern bool opengl_test;

extern double xViewer,
              yViewer,
              zViewer;
extern GLfloat theta[];



static bool useOpenGL = false;  /* Determines OpenGL or software rendering mode */
static Timer timer1;            /* Used to compute rendering speed in frames per second */

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
   scene->viewLookAt(xViewer, yViewer, zViewer, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   /* set the model matrix in the Scene object */
   scene->model2Identity();
   scene->modelRotate(theta[0], 1.0, 0.0, 0.0);
   scene->modelRotate(theta[1], 0.0, 1.0, 0.0);
   scene->modelRotate(theta[2], 0.0, 0.0, 1.0);

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
     gluPerspective( 17,          /* field of view in degree */
                     w/(double)h, /* varying aspect ratio */
                     1.0,         /* near plane */
                     20.0 );      /* far plane */
     printOpenGLMatrix(GL_PROJECTION_MATRIX);
     glPopMatrix();

     /* let OpenGL compute a modelview matrix (for comparison) */
     glMatrixMode(GL_MODELVIEW);
     glPushMatrix();
     glLoadIdentity();
     gluLookAt(xViewer, yViewer, zViewer, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
     glRotate(theta[0], 1.0, 0.0, 0.0);
     glRotate(theta[1], 0.0, 1.0, 0.0);
     glRotate(theta[2], 0.0, 0.0, 1.0);
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
      default:
         break;
   }
   printf("xViewer = %f, yViewer = %f, zViewer = %f\n", xViewer, yViewer, zViewer);
   printf("theta[0] = %f, theta[1] = %f, theta[2] = %f\n", theta[0], theta[1], theta[2]);
   glutPostRedisplay();
   return;
}
