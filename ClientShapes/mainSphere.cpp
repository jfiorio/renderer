/*
   Use the x/X, y/Y and z/Z keys to move the camera.
   Use the 1/!, 2/@ and 3/# keys to rotate the model.
*/
#include <stdio.h>
#include <math.h>
#include "SceneLib.h"
#include "GLUTCallbacks.h"

/*** Define global variables ***/
Scene *scene; /* Pointer to the Scene object that we will build and then render. */


int main(int argc, char **argv)
{
   int k; // number of triangles in the triangle fan at the top and bottom
   int n; // number of triangle strips between the top and bottom

   if (argc > 2)
   {
      k = atoi( argv[1] );
      n = atoi( argv[2] );
   }
   else
   {
      printf("Usage: %s <k> <n>", argv[0]);
      return 0;
   }

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

    /* create geometry for the Scene */
   Vertex v[n+1][k];

   #define PI 3.1415926535897932384626433832795

   // an array of vertices to be used to create triangles
   for (int j = 0; j < k; j++)
   {
      double c = cos(j*(2.0*PI)/k);
      double s = sin(j*(2.0*PI)/k);
      for (int i = 0; i <= n; i++)
      {
         v[i][j] = Vertex(sin((i+1)*PI/(n+2))*c, cos((i+1)*PI/(n+2)), sin((i+1)*PI/(n+2))*s);
      }
   }

   // give the vertices some color
   for (int i = 0; i <= n; i++)
   {
      for (int j = 0; j < k; j++)
      {
         v[i][j].setColor( fabs(v[i][j].x), fabs(v[i][j].y), fabs(v[i][j].z) );
      }
   }

   // create the triangle fan at the top
   Vertex topCenter = Vertex(0.0, 1.0, 0.0);
   for (int j = 0; j < k-1; j++)
   {
      scene->addTriangle( new Triangle( &topCenter, &(v[0][j+1]), &(v[0][j]) ) );
   }
   scene->addTriangle( new Triangle( &topCenter, &(v[0][0]), &(v[0][k-1]) ) );

   // create all the triangles along the cylinder wall
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < k-1; j++)
      {
         scene->addTriangle( new Triangle( &(v[i][j]), &(v[i][j+1]), &(v[i+1][j+1]) ) );
         scene->addTriangle( new Triangle( &(v[i+1][j+1]), &(v[i+1][j]), &(v[i][j]) ) );
      }
      scene->addTriangle( new Triangle( &(v[i][k-1]), &(v[i][0]), &(v[i+1][0]) ) );
      scene->addTriangle( new Triangle( &(v[i+1][0]), &(v[i+1][k-1]), &(v[i][k-1]) ) );
   }

   // create the triangle fan at the bottom
   Vertex bottomCenter = Vertex(0.0, -1.0, 0.0);
   for (int j = 0; j < k-1; j++)
   {
      scene->addTriangle( new Triangle(&bottomCenter, &(v[n][j]), &(v[n][j+1]) ) );
   }
   scene->addTriangle( new Triangle( &bottomCenter, &(v[n][k-1]), &(v[n][0]) ) );


   /* Give the Scene a model transformation */
   scene->model2Identity();
   scene->modelScale(3.0, 3.0, 3.0);

   /* Give the Scene a view transformation */
   scene->view2Identity();  /* this will be changed by the windowing system */

   /* Give the scene a projection transformation */
   scene->proj2Identity();  /* this will be changed by the windowing system */

   /* Set the Scene's viewport dimension */
   scene->vp_width  = 500;  /* this will be changed by the windowing system */
   scene->vp_height = 500;  /* this will be changed by the windowing system */

   glutMainLoop(); /* Enter GLUT's main loop */

   return 1;
}
