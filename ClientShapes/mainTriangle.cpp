/*
   Use the x/X, y/Y and z/Z keys to move the camera.
   Use the 1/!, 2/@ and 3/# keys to rotate the model.
*/
#include <stdio.h>
#include "SceneLib.h"
#include "GLUTCallbacks.h"

/*** Define global variables ***/
Scene *scene; /* Pointer to the Scene object that we will build and then render. */


int main(int argc, char **argv)
{
   /* Initialize GLUT */
   glutInit(&argc, argv);
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
   glutInitWindowSize(500, 500);
   glutCreateWindow(argv[0]);
   glutInitWindowPosition(200,100);

   /* Set all the GLUT callback functions */
   glutDisplayFunc(display);      /* in GLUTCallbacks.cpp */
   //glutIdleFunc(redisplay);     /* use an idle function to compute the frame rate */
   glutIdleFunc(NULL);            /* turn off idle function when debugging */
   glutReshapeFunc(reshape);      /* in GLUTCallbacks.cpp */
   glutKeyboardFunc(keyboard);    /* in GLUTCallbacks.cpp */

   /* Create the Scene object that we shall render */
   scene = new Scene();

   /* add the geometry to the Scene */
   Vertex v1 = Vertex(3.0, 0.0, 0.0);
   Vertex v2 = Vertex(0.0, 3.0, 0.0);
   Vertex v3 = Vertex(0.0, 0.0, 3.0);

   v1.setColor(1.0, 0.0, 0.0);
   v2.setColor(0.0, 1.0, 0.0);
   v3.setColor(0.0, 0.0, 1.0);
   Triangle *t1 = new Triangle(&v1, &v2, &v3);
   scene->addTriangle( t1 );

   /* give the Scene a model transformation */
   scene->model2Identity(); /* this will be changed by the windowing system */

   /* give the Scene a view transformation */
   scene->view2Identity();  /* this will be changed by the windowing system */

   /* give the scene a projection transformation */
   scene->proj2Identity();  /* this will be changed by the windowing system */

   /* set the Scene's viewport dimension */
   scene->vp_width  = 500;  /* this will be changed by the windowing system */
   scene->vp_height = 500;  /* this will be changed by the windowing system */

   glutMainLoop(); /* Enter GLUT's main loop */

   return 1;
}
