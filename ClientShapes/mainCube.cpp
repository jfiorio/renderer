/*
                   y
                   |
                   |
                   | v[5]
                 3 +-----------------+ v[6]=(3,3,0)
                  /|                /|
                /  |              /  |
              /    |            /    |
            /      |          /      |
      v[8] +-----------------+ v[7]  |
           |       |         |       |                    y
           |       |         |       |                    |
           |       |         |       | v[2]               |
           |  v[1] +---------|-------+------> x           |
           |      /          |      /3                    |
           |    /            |    /                       +-----> x
           |  /              |  /                        /
           |/                |/                         /
         3 +-----------------+                         /
         / v[4]=(0,0,3)      v[3]=(3,0,3)            z
       /
     /
    z


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

   /* add geometry to the Scene */
   Vertex v[9];

   v[0] = Vertex(1.5, 0.0, 1.5);  // center of the bottom face
   v[1] = Vertex(0.0, 0.0, 0.0);  // four vertices around the bottom face
   v[2] = Vertex(3.0, 0.0, 0.0);
   v[3] = Vertex(3.0, 0.0, 3.0);
   v[4] = Vertex(0.0, 0.0, 3.0);
   v[5] = Vertex(0.0, 3.0, 0.0);  // four vertices around the top face
   v[6] = Vertex(3.0, 3.0, 0.0);
   v[7] = Vertex(3.0, 3.0, 3.0);
   v[8] = Vertex(0.0, 3.0, 3.0);


   v[0].setColor(1.0, 0.0, 0.0);
   v[1].setColor(1.0, 0.0, 0.0);
   v[2].setColor(1.0, 0.0, 0.0);
   Triangle *t1 = new Triangle(v+0, v+1, v+2);
   scene->addTriangle( t1 );   // bottom face

   v[0].setColor(0.0, 1.0, 0.0);
   v[2].setColor(0.0, 1.0, 0.0);
   v[3].setColor(0.0, 1.0, 0.0);
   Triangle *t2 = new Triangle(v+0, v+2, v+3);
   scene->addTriangle( t2 );   // bottom face

   v[0].setColor(0.0, 0.0, 1.0);
   v[3].setColor(0.0, 0.0, 1.0);
   v[4].setColor(0.0, 0.0, 1.0);
   Triangle *t3 = new Triangle(v+0, v+3, v+4);
   scene->addTriangle( t3 );   // bottom face

   v[0].setColor(1.0, 1.0, 1.0);
   v[4].setColor(1.0, 1.0, 1.0);
   v[1].setColor(1.0, 1.0, 1.0);
   Triangle *t4 = new Triangle(v+0, v+4, v+1);
   scene->addTriangle( t4 );   // bottom face

   v[4].setColor(1.0, 0.0, 0.0);
   v[3].setColor(0.0, 1.0, 0.0);
   v[7].setColor(0.0, 0.0, 1.0);
   Triangle *t5 = new Triangle(v+4, v+3, v+7);
   scene->addTriangle( t5 );  // front face

   v[7].setColor(0.0, 0.0, 1.0);
   v[8].setColor(0.0, 1.0, 0.0);
   v[4].setColor(1.0, 0.0, 0.0);
   Triangle *t6 = new Triangle(v+7, v+8, v+4);
   scene->addTriangle( t6 );   // front face

   v[3].setColor(0.0, 1.0, 1.0);
   v[2].setColor(0.0, 1.0, 1.0);
   v[6].setColor(0.0, 1.0, 1.0);
   Triangle *t7 = new Triangle(v+3, v+2, v+6);
   scene->addTriangle( t7 );   // right face

   v[6].setColor(1.0, 1.0, 1.0);
   v[7].setColor(0.0, 1.0, 1.0);
   v[3].setColor(0.0, 1.0, 1.0);
   Triangle *t8 = new Triangle(v+6, v+7, v+3);
   scene->addTriangle( t8 );   // right face

   v[2].setColor(1.0, 0.0, 0.0);
   v[1].setColor(0.0, 1.0, 0.0);
   v[5].setColor(0.0, 0.0, 1.0);
   Triangle *t9 = new Triangle(v+2, v+1, v+5);
   scene->addTriangle( t9 );   // back face

   v[5].setColor(1.0, 0.0, 0.0);
   v[6].setColor(0.0, 1.0, 0.0);
   v[2].setColor(0.0, 0.0, 1.0);
   Triangle *t10 = new Triangle(v+5, v+6, v+2);
   scene->addTriangle( t10 );   // back face

   v[1].setColor(1.0, 0.0, 0.0);
   v[4].setColor(0.0, 1.0, 0.0);
   v[8].setColor(0.0, 0.0, 1.0);
   Triangle *t11 = new Triangle(v+1, v+4, v+8);
   scene->addTriangle( t11 );   // left face

   v[8].setColor(0.0, 0.0, 1.0);
   v[5].setColor(0.0, 1.0, 0.0);
   v[1].setColor(1.0, 0.0, 0.0);
   Triangle *t12 = new Triangle(v+8, v+5, v+1);
   scene->addTriangle( t12 );   // left face

   v[8].setColor(1.0, 1.0, 0.0);
   v[7].setColor(1.0, 1.0, 0.0);
   v[6].setColor(1.0, 0.0, 0.0);
   Triangle *t13 = new Triangle(v+8, v+7, v+6);
   scene->addTriangle( t13 );   // top face

   v[6].setColor(1.0, 1.0, 0.0);
   v[5].setColor(1.0, 1.0, 0.0);
   v[8].setColor(1.0, 1.0, 0.0);
   Triangle *t14 = new Triangle(v+6, v+5, v+8);
   scene->addTriangle( t14 );   // top face


   /* give the Scene a model transformation */
   scene->model2Identity();

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
