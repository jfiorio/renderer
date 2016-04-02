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
*/
#include <stdio.h>
#include "SceneLib.h"
#include "GLUTCallbacks.h"
#include "GLUTMouseCallbacks.h"

/*** Define global variables ***/
Scene *scene; /* Pointer to the Scene object that we will build and then render. */

/* store the eye's position and parameters */
double eye_fov;    /* field of view of the eye */
double eye_pos[3]; /* position of the eye */
double eye_theta;  /* theta angle of the eye (the longitude),
                      positive in a counterclockwise direction when looking down -z */
double eye_phi;    /* phi angle of the eye (the latitude), 0 is the north pole */

#define SIZE 256
GLubyte pascalImage[SIZE][SIZE][3]; /* a procedural texture */

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
   glutEntryFunc(mouseEntry);     /* in GLUTMouseCallbacks.cpp */  /* doesn't work */
   glutMotionFunc(mouseMotion);   /* in GLUTMouseCallbacks.cpp */
   glutPassiveMotionFunc(mouseMotion);

   /* set the eye's initial location */
   eye_pos[0] = 0.0;
   eye_pos[1] = 0.0;
   eye_pos[2] = 10.0;

   /* initialize the eye's parameters */
   eye_theta = 0.0;
   eye_phi = 90.0;
   eye_fov = 45.0;


   /* Create the Scene object that we shall render */
   scene = new Scene();

   /* First, add three textures to the Scene */
   // Create a "procedural" texture image, pixel by pixel
   for (int i = 0; i < SIZE; i++)
   {
      for (int j = 0; j < SIZE; j++)
      {
         int c = (i|j)%255;  // Pascal's Triangle like image
         pascalImage[i][j][0] = (GLubyte) c;
         pascalImage[i][j][1] = (GLubyte) c;
         pascalImage[i][j][2] = (GLubyte) c;
      }
   }

   Texture *texture1 = new Texture((char*)"Yoda.ppm");
   Texture *texture2 = new Texture((char*)"wood.ppm");
   Texture *texture3 = new Texture(SIZE, SIZE, (GLubyte*)pascalImage);
   scene->addTexture( texture1 );
   scene->addTexture( texture2 );
   scene->addTexture( texture3 );
   initializeTexturesOpenGL(scene); /* in SceneRenderOpenGL.cpp */


   /* Second, add the geometry to the Scene */
   Vertex v1 = Vertex(0.0, 0.0, 3.0);
   Vertex v2 = Vertex(3.0, 0.0, 3.0);
   Vertex v3 = Vertex(3.0, 3.0, 3.0);
   Vertex v4 = Vertex(0.0, 3.0, 3.0);
   Vertex v5 = Vertex(0.0, 3.0, 0.0);
   Vertex v6 = Vertex(3.0, 3.0, 0.0);
   Vertex v7 = Vertex(3.0, 0.0, 0.0);
   Vertex v8 = Vertex(0.0, 0.0, 0.0);
   Vertex v9 = Vertex(1.5, 0.0, 1.5);  // center of the bottom face

   v1.setCoords(0.0, 0.0);
   v2.setCoords(1.0, 0.0);
   v3.setCoords(1.0, 1.0);
   Triangle *t1 = new Triangle(&v1, &v2, &v3);
   t1->setTexture( texture1 );
   scene->addTriangle( t1 );  // front face

   v3.setCoords(1.0, 1.0);
   v4.setCoords(0.0, 1.0);
   v1.setCoords(0.0, 0.0);
   Triangle *t2 = new Triangle(&v3, &v4, &v1);
   t2->setTexture( texture1 );
   scene->addTriangle( t2 );   // front face

   v4.setCoords(0.0, 0.0);
   v3.setCoords(1.0, 0.0);
   v6.setCoords(1.0, 1.0);
   Triangle *t3 = new Triangle(&v4, &v3, &v6);
   t3->setTexture( texture2 );
   scene->addTriangle( t3 );   // top face

   v6.setCoords(1.0, 1.0);
   v5.setCoords(0.0, 1.0);
   v4.setCoords(0.0, 0.0);
   Triangle *t4 = new Triangle(&v6, &v5, &v4);
   t4->setTexture( texture2 );
   scene->addTriangle( t4 );   // top face

   v2.setCoords(0.0, 0.0);
   v7.setCoords(1.0, 0.0);
   v6.setCoords(1.0, 1.0);
   Triangle *t5 = new Triangle(&v2, &v7, &v6);
   t5->setTexture( texture3 );
   scene->addTriangle( t5 );   // right face

   v6.setCoords(1.0, 1.0);
   v3.setCoords(0.0, 1.0);
   v2.setCoords(0.0, 0.0);
   Triangle *t6 = new Triangle(&v6, &v3, &v2);
   t6->setTexture( texture3 );
   scene->addTriangle( t6 );   // right face

   v8.setColor(1.0, 0.0, 0.0);
   v1.setColor(0.0, 1.0, 0.0);
   v4.setColor(0.0, 0.0, 1.0);
   Triangle *t7 = new Triangle(&v8, &v1, &v4);
   t7->setTexture( NULL );
   scene->addTriangle( t7 );   // left face

   v4.setColor(0.0, 0.0, 1.0);
   v5.setColor(0.0, 1.0, 0.0);
   v8.setColor(1.0, 0.0, 0.0);
   Triangle *t8 = new Triangle(&v4, &v5, &v8);
   t8->setTexture( NULL );
   scene->addTriangle( t8 );   // left face

   v8.setColor(0.0, 0.0, 0.0);  // try commenting out this line
   Triangle *t9 = new Triangle(&v7, &v8, &v6);
   t9->setTexture( NULL );
   scene->addTriangle( t9 );   // back face

   v5.setColor(0.0, 0.0, 0.0);
   Triangle *t10 = new Triangle(&v6, &v5, &v8);
   t10->setTexture( NULL );
   scene->addTriangle( t10 );   // back face

   v1.setColor(0.0, 0.0, 0.0);
   Triangle *t11 = new Triangle(&v9, &v1, &v2);
   t11->setTexture( NULL );
   scene->addTriangle( t11 );   // bottom face

   Triangle *t12 = new Triangle(&v9, &v2, &v7);
   t12->setTexture( NULL );
   scene->addTriangle( t12 );   // bottom face

   Triangle *t13 = new Triangle(&v9, &v7, &v8);
   t13->setTexture( NULL );
   scene->addTriangle( t13 );   // bottom face

   Triangle *t14 = new Triangle(&v9, &v8, &v1);
   t14->setTexture( NULL );
   scene->addTriangle( t14 );   // bottom face

   /* Third, give the Scene a model transformation */
   scene->model2Identity(); /* this will be changed by the windowing system */

   /* Fourth, give the Scene a view transformation */
   scene->view2Identity();  /* this will be changed by the windowing system */

   /* Fifth, give the scene a projection transformation */
   scene->proj2Identity();  /* this will be changed by the windowing system */

   /* Sixth, set the Scene's viewport dimension */
   scene->vp_width  = 500;  /* this will be changed by the windowing system */
   scene->vp_height = 500;  /* this will be changed by the windowing system */

   glutMainLoop(); /* Enter GLUT's main loop */

   return 1;
}
