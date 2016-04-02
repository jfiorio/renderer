#include <stdio.h>
#include "SceneLib.h"
#include "Constants.h"
#include "MazeParser.h"
#include "GLUTCallbacks.h"
#include "GLUTMouseCallbacks.h"


/*** Define six global variables ***/
Scene *scene; /* Pointer to the Scene object that we will build and then render. */

/* store the eye's position and parameters */
double eye_fov;    /* field of view of the eye */
double eye_pos[3]; /* the position of the eye */
double eye_theta;  /* the theta angle of the eye (the longitude).
                      it is positive in a counterclockwise direction when looking down -z */
double eye_phi;    /* the phi angle of the eye (the latitude).
                      i define 0 as the north pole. */


int main(int argc, char **argv)
{
   /* Initialize GLUT */
   glutInit(&argc, argv);
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
   glutInitWindowSize(window_width, window_height);
   glutCreateWindow("MazeViewer");
   glutInitWindowPosition(100,50);

   /* Set all the GLUT callback functions */
   glutDisplayFunc(display);      /* in GLUTCallbacks.cpp */
   //glutIdleFunc(redisplay);     /* use an idle function to compute frame rate */
   glutIdleFunc(NULL);            /* turn off idle function when debugging */
   glutReshapeFunc(reshape);      /* in GLUTCallbacks.cpp */
   glutKeyboardFunc(keyboard);    /* in GLUTCallbacks.cpp */
   glutEntryFunc(mouseEntry);     /* in GLUTMouseCallbacks.cpp */  /* doesn't work */
   glutMotionFunc(mouseMotion);   /* in GLUTMouseCallbacks.cpp */
   glutPassiveMotionFunc(mouseMotion);

   /* set the eye's initial location */
   eye_pos[0] = 0.0;
   eye_pos[1] = 0.0;
   eye_pos[2] = 5.0;

   /* initialize the eye's parameters */
   eye_theta = 180.0;
   eye_phi = 90.0;
   eye_fov = 45.0;

   /* Create the Scene object that we shall render */
   MazeParser mp;
   scene = mp.loadScene((char*)"maze.txt");

   initializeTexturesOpenGL(scene); /* in SceneRenderOpenGL.cpp */

   glutMainLoop();                  /* Enter GLUT's main loop */

   return 1;
}
