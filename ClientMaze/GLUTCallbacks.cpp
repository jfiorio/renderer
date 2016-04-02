/*


*/
#include "GLUTcallbacks.h"

extern Scene *scene;     /* defined in mainMaze.cpp */
extern bool opengl_test; /* defined in mainMaze.cpp */

extern double eye_fov;   /* defined in mainMaze.cpp */
extern double eye_pos[3];
extern double eye_theta;
extern double eye_phi;



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
   scene->projPerspective( eye_fov,     /* field of view in degree */
                           w/(double)h, /* varying aspect ratio */
                           0.1,         /* near plane */
                           700 );       /* far plane */

   /* set the view matrix in the Scene object */
   scene->view2Identity();
   scene->viewRotate(-eye_phi,   1.0, 0.0, 0.0);  // about x-axis
   scene->viewRotate(-eye_theta, 0.0, 0.0, 1.0);  // about z-axis
   /* translate to get the eye origin in the center of the coordinate system */
   scene->viewTranslate( -eye_pos[0], -eye_pos[1], -eye_pos[2] );

   /* set the model matrix in the Scene object */
   scene->model2Identity();

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
     gluPerspective( eye_fov,     /* field of view in degree */
                     w/(double)h, /* varying aspect ratio */
                     0.1,         /* near plane */
                     700 );       /* far plane */
     printOpenGLMatrix(GL_PROJECTION_MATRIX);
     glPopMatrix();

     /* let OpenGL compute a modelview matrix (for comparison) */
     glMatrixMode(GL_MODELVIEW);
     glPushMatrix();
     glLoadIdentity();
     glRotatef(-eye_phi,   1, 0, 0);  // about x-axis
     glRotatef(-eye_theta, 0, 0, 1);  // about z-axis
     /* translate to get the eye origin in the center of the coordinate system */
     glTranslatef(-eye_pos[0], -eye_pos[1], -eye_pos[2]);
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
      case 'w':
         moveForward();
         redisplay();
         break;
      case 'a':
         moveLeft();
         redisplay();
         break;
      case 'd':
         moveRight();
         redisplay();
         break;
      case 'z':
         moveBack();
         redisplay();
         break;
      case 'u':
         moveUp();
         redisplay();
         break;
      case 'n':
         moveDown();
         redisplay();
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
   return;
}
