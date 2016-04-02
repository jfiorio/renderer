/*
                   y
                   |
                   |
                   | v5
                 3 +-----------------+ v6=(3,3,0)
                  /|                /|
                /  |              /  |
              /    |            /    |
            /      |          /      |
        v4 +-----------------+ v3    |
           |       |         |       |                    y
           |       |         |       |                    |
           |       |         |       | v7                 |
           |    v8 +---------|-------+------> x           |
           |      /          |      /3                    |
           |    /            |    /                       +-----> x
           |  /              |  /                        /
           |/                |/                         /
         3 +-----------------+                         /
         / v1=(0,0,3)        v2=(3,0,3)              z
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

    /* Add the geometry to the Scene */
   Vertex v1 = Vertex(0.0, 0.0, 3.0);
   Vertex v2 = Vertex(3.0, 0.0, 3.0);
   Vertex v3 = Vertex(3.0, 3.0, 3.0);
   Vertex v4 = Vertex(0.0, 3.0, 3.0);
   Vertex v5 = Vertex(0.0, 3.0, 0.0);
   Vertex v6 = Vertex(3.0, 3.0, 0.0);
   Vertex v7 = Vertex(3.0, 0.0, 0.0);
   Vertex v8 = Vertex(0.0, 0.0, 0.0);
   Vertex v9 = Vertex(1.5, 0.0, 1.5);  // center of the bottom face

   Vertex v11 = Vertex(4.0, 0.0, 1.0);
   Vertex v12 = Vertex(5.0, 0.0, 1.0);
   Vertex v13 = Vertex(5.0, 1.0, 1.0);
   Vertex v14 = Vertex(4.0, 1.0, 1.0);
   Vertex v15 = Vertex(4.0, 1.0, 0.0);
   Vertex v16 = Vertex(5.0, 1.0, 0.0);
   Vertex v17 = Vertex(5.0, 0.0, 0.0);
   Vertex v18 = Vertex(4.0, 0.0, 0.0);
   Vertex v19 = Vertex(4.5, 0.0, 0.5);  // center of the bottom face


   v1.setColor(1.0, 0.0, 0.0);
   v2.setColor(0.0, 1.0, 0.0);
   v3.setColor(0.0, 0.0, 1.0);
   Triangle *t1 = new Triangle(&v1, &v2, &v3);
   scene->addTriangle( t1 );  // front face

   v3.setColor(0.0, 0.0, 1.0);
   v4.setColor(0.0, 1.0, 0.0);
   v1.setColor(1.0, 0.0, 0.0);
   Triangle *t2 = new Triangle(&v3, &v4, &v1);
   scene->addTriangle( t2 );   // front face

   v4.setColor(1.0, 1.0, 0.0);
   v3.setColor(1.0, 1.0, 0.0);
   v6.setColor(1.0, 0.0, 0.0);
   Triangle *t3 = new Triangle(&v4, &v3, &v6);
   scene->addTriangle( t3 );   // top face

   v5.setColor(1.0, 1.0, 0.0);
   v6.setColor(1.0, 1.0, 0.0);
   v4.setColor(1.0, 1.0, 0.0);
   Triangle *t4 = new Triangle(&v6, &v5, &v4);
   scene->addTriangle( t4 );   // top face

   v2.setColor(0.0, 1.0, 1.0);
   v7.setColor(0.0, 1.0, 1.0);
   v6.setColor(0.0, 1.0, 1.0);
   Triangle *t5 = new Triangle(&v2, &v7, &v6);
   scene->addTriangle( t5 );   // right face

   v6.setColor(1.0, 1.0, 1.0);
   v3.setColor(0.0, 1.0, 1.0);
   v2.setColor(0.0, 1.0, 1.0);
   Triangle *t6 = new Triangle(&v6, &v3, &v2);
   scene->addTriangle( t6 );   // right face

   v8.setColor(1.0, 0.0, 0.0);
   v1.setColor(0.0, 1.0, 0.0);
   v4.setColor(0.0, 0.0, 1.0);
   Triangle *t7 = new Triangle(&v8, &v1, &v4);
   scene->addTriangle( t7 );   // left face

   v4.setColor(0.0, 0.0, 1.0);
   v5.setColor(0.0, 1.0, 0.0);
   v8.setColor(1.0, 0.0, 0.0);
   Triangle *t8 = new Triangle(&v4, &v5, &v8);
   scene->addTriangle( t8 );   // left face

   v8.setColor(1.0, 0.0, 0.0);
   v1.setColor(0.0, 1.0, 0.0);
   v4.setColor(0.0, 0.0, 1.0);
   Triangle *t9 = new Triangle(&v7, &v8, &v6);
   scene->addTriangle( t9 );   // back face

   v8.setColor(1.0, 0.0, 0.0);
   v1.setColor(0.0, 1.0, 0.0);
   v4.setColor(0.0, 0.0, 1.0);
   Triangle *t10 = new Triangle(&v6, &v5, &v8);
   scene->addTriangle( t10 );   // back face

   v8.setColor(1.0, 0.0, 0.0);
   v1.setColor(1.0, 0.0, 0.0);
   v4.setColor(1.0, 0.0, 0.0);
   Triangle *t11 = new Triangle(&v9, &v1, &v2);
   scene->addTriangle( t11 );   // bottom face

   v8.setColor(0.0, 1.0, 0.0);
   v1.setColor(0.0, 1.0, 0.0);
   v4.setColor(0.0, 1.0, 0.0);
   Triangle *t12 = new Triangle(&v9, &v2, &v7);
   scene->addTriangle( t12 );   // bottom face

   v8.setColor(0.0, 0.0, 1.0);
   v1.setColor(0.0, 0.0, 1.0);
   v4.setColor(0.0, 0.0, 1.0);
   Triangle *t13 = new Triangle(&v9, &v7, &v8);
   scene->addTriangle( t13 );   // bottom face

   v8.setColor(1.0, 1.0, 1.0);
   v1.setColor(1.0, 1.0, 1.0);
   v4.setColor(1.0, 1.0, 1.0);
   Triangle *t14 = new Triangle(&v9, &v8, &v1);
   scene->addTriangle( t14 );   // bottom face



   v11.setColor(1.0, 0.0, 0.0);
   v12.setColor(0.0, 1.0, 0.0);
   v13.setColor(0.0, 0.0, 1.0);
   t1 = new Triangle(&v11, &v12, &v13);
   scene->addTriangle( t1 );  // front face

   v13.setColor(0.0, 0.0, 1.0);
   v14.setColor(0.0, 1.0, 0.0);
   v11.setColor(1.0, 0.0, 0.0);
   t2 = new Triangle(&v13, &v14, &v11);
   scene->addTriangle( t2 );   // front face

   v14.setColor(1.0, 1.0, 0.0);
   v13.setColor(1.0, 1.0, 0.0);
   v16.setColor(1.0, 0.0, 0.0);
   t3 = new Triangle(&v14, &v13, &v16);
   scene->addTriangle( t3 );   // top face

   v15.setColor(1.0, 1.0, 0.0);
   v16.setColor(1.0, 1.0, 0.0);
   v14.setColor(1.0, 1.0, 0.0);
   t4 = new Triangle(&v16, &v15, &v14);
   scene->addTriangle( t4 );   // top face

   v12.setColor(0.0, 1.0, 1.0);
   v17.setColor(0.0, 1.0, 1.0);
   v16.setColor(0.0, 1.0, 1.0);
   t5 = new Triangle(&v12, &v17, &v16);
   scene->addTriangle( t5 );   // right face

   v16.setColor(1.0, 1.0, 1.0);
   v13.setColor(0.0, 1.0, 1.0);
   v12.setColor(0.0, 1.0, 1.0);
   t6 = new Triangle(&v16, &v13, &v12);
   scene->addTriangle( t6 );   // right face

   v18.setColor(1.0, 0.0, 0.0);
   v11.setColor(0.0, 1.0, 0.0);
   v14.setColor(0.0, 0.0, 1.0);
   t7 = new Triangle(&v18, &v11, &v14);
   scene->addTriangle( t7 );   // left face

   v14.setColor(0.0, 0.0, 1.0);
   v15.setColor(0.0, 1.0, 0.0);
   v18.setColor(1.0, 0.0, 0.0);
   t8 = new Triangle(&v14, &v15, &v18);
   scene->addTriangle( t8 );   // left face

   v18.setColor(1.0, 0.0, 0.0);
   v11.setColor(0.0, 1.0, 0.0);
   v14.setColor(0.0, 0.0, 1.0);
   t9 = new Triangle(&v17, &v18, &v16);
   scene->addTriangle( t9 );   // back face

   v18.setColor(1.0, 0.0, 0.0);
   v11.setColor(0.0, 1.0, 0.0);
   v14.setColor(0.0, 0.0, 1.0);
   t10 = new Triangle(&v16, &v15, &v18);
   scene->addTriangle( t10 );   // back face

   v18.setColor(1.0, 0.0, 0.0);
   v11.setColor(1.0, 0.0, 0.0);
   v14.setColor(1.0, 0.0, 0.0);
   t11 = new Triangle(&v19, &v11, &v12);
   scene->addTriangle( t11 );   // bottom face

   v18.setColor(0.0, 1.0, 0.0);
   v11.setColor(0.0, 1.0, 0.0);
   v14.setColor(0.0, 1.0, 0.0);
   t12 = new Triangle(&v19, &v12, &v17);
   scene->addTriangle( t12 );   // bottom face

   v18.setColor(0.0, 0.0, 1.0);
   v11.setColor(0.0, 0.0, 1.0);
   v14.setColor(0.0, 0.0, 1.0);
   t13 = new Triangle(&v19, &v17, &v18);
   scene->addTriangle( t13 );   // bottom face

   v18.setColor(1.0, 1.0, 1.0);
   v11.setColor(1.0, 1.0, 1.0);
   v14.setColor(1.0, 1.0, 1.0);
   t14 = new Triangle(&v19, &v18, &v11);
   scene->addTriangle( t14 );   // bottom face



   /* Give the Scene a model transformation */
   scene->model2Identity(); /* this will be changed by the windowing system */

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
