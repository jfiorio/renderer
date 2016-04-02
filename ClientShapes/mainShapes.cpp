/*
   Use the x/X, y/Y and z/Z keys to move the camera.
   Use the 1/!, 2/@ and 3/# keys to rotate the model.
*/
#include <stdio.h>
#include <math.h>
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Triangle.h"
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
   glutInitWindowPosition(400,100); /* doesn't work? */

   /* Set all the GLUT callback functions */
   glutDisplayFunc(display);      /* in GLUTCallbacks.cpp */
   //glutIdleFunc(redisplay);     /* use an idle function to compute the frame rate */
   glutIdleFunc(NULL);            /* turn off idle function when debugging */
   glutReshapeFunc(reshape);      /* in GLUTCallbacks.cpp */
   glutKeyboardFunc(keyboard);    /* in GLUTCallbacks.cpp */
   glutSpecialFunc(keySpecial);   /* in GLUTCallbacks.cpp */

   /* Create the Scene object that we shall render */
   scene = new Scene();
   scene->doLighting = true;
   scene->doSmoothLighting = false; // use flat shading

   // create a Light object for the scene
   Light *light = new Light();
   light->pos[0] = 0.0;
   light->pos[1] = 0.0;
   light->pos[2] = 10.0;
   light->pos[3] = 1.0;
   light->ambient[0] = 1.0;
   light->ambient[1] = 1.0;
   light->ambient[2] = 1.0;
   light->ambient[3] = 1.0;
   light->diffuse[0] = 1.0;
   light->diffuse[1] = 1.0;
   light->diffuse[2] = 1.0;
   light->diffuse[3] = 1.0;
   light->specular[0] = 1.0;
   light->specular[1] = 1.0;
   light->specular[2] = 1.0;
   light->specular[3] = 1.0;
   // add the light to the scene
   scene->light = light;

   // create a Material object to use with one model (brass)
   Material *mtrl1 = new Material();
   mtrl1->ambient[0] = 0.33;
   mtrl1->ambient[1] = 0.22;
   mtrl1->ambient[2] = 0.03;
   mtrl1->ambient[3] = 1.00;
   mtrl1->diffuse[0] = 0.78;
   mtrl1->diffuse[1] = 0.57;
   mtrl1->diffuse[2] = 0.11;
   mtrl1->diffuse[3] = 1.00;
   mtrl1->specular[0] = 0.99;
   mtrl1->specular[1] = 0.91;
   mtrl1->specular[2] = 0.81;
   mtrl1->specular[3] = 1.00;
   mtrl1->s = 27.8;

   // create a Material object to use with the other model (chrome)
   Material *mtrl2 = new Material();
   mtrl2->ambient[0] = 0.25;
   mtrl2->ambient[1] = 0.25;
   mtrl2->ambient[2] = 0.25;
   mtrl2->ambient[3] = 1.00;
   mtrl2->diffuse[0] = 0.4;
   mtrl2->diffuse[1] = 0.4;
   mtrl2->diffuse[2] = 0.4;
   mtrl2->diffuse[3] = 1.00;
   mtrl2->specular[0] = 0.775;
   mtrl2->specular[1] = 0.775;
   mtrl2->specular[2] = 0.775;
   mtrl2->specular[3] = 1.00;
   mtrl2->s = 76.8;

/*
   // a triangle, in the xy-plane, with its center exactly at the origin
   Vertex v0 = Vertex(-1.0, -1.0, 0.0);
   Vertex v1 = Vertex( 2.0, -1.0, 0.0);
   Vertex v2 = Vertex(-1.0,  2.0, 0.0);
   v0.setColor(1.0, 0.0, 0.0);
   v1.setColor(0.0, 1.0, 0.0);
   v2.setColor(0.0, 0.0, 1.0);
   v0.n = Vector(0.0, 0.0, 1.0);
   v1.n = Vector(0.0, 0.0, 1.0);
   v2.n = Vector(0.0, 0.0, 1.0);
   Triangle *t = new Triangle(&v0, &v1, &v2);
   t->mtrl = mtrl1;
   scene->addTriangle( t );
*/

    /* add geometry to the Scene */
    //triangle(-1.0, -1.0, 0.0, 3.0, mtrl1, scene);
    sphere(k, n, 0.0, 0.0, 0.0, 1.0, mtrl2, scene);
    cylinder(k, n, 3.0, 0.0, 0.0, 1.0, 4.0, mtrl1, scene);

   /* Give the Scene a model transformation */
   //scene->model2Identity();

   /* Give the Scene a view transformation */
   //scene->view2Identity();  /* this will be changed by the windowing system */

   /* Give the scene a projection transformation */
   //scene->proj2Identity();  /* this will be changed by the windowing system */

   /* Set the Scene's viewport dimension */
   scene->vp_width  = 500;  /* this will be changed by the windowing system */
   scene->vp_height = 500;  /* this will be changed by the windowing system */

   glutMainLoop(); /* Enter GLUT's main loop */

   return 1;
}
