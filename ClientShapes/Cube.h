/*
                   y
                   |
                   |
                   | v[5]
                 h +-----------------+ v[6]=(h,h,0)
                  /|                /|
                /  |              /  |
              /    |            /    |
            /      |          /      |
      v[8] +-----------------+ v[7]  |
           |       |         |       |                    y
           |       |         |       |                    |
           |       |         |       | v[2]               |
           |  v[1] +---------|-------+------> x           |
           |      /          |      /h                    |
           |    /            |    /                       +-----> x
           |  /              |  /                        /
           |/                |/                         /
         h +-----------------+                         /
         / v[4]=(0,0,h)      v[3]=(h,0,h)            z
       /
     /
    z

   x world x-coordinate of the "origin" (v[1]) of the cube
   y world y-coordinate of the "origin" (v[1]) of the cube
   z world z-coordinate of the "origin" (v[1]) of the cube
   h height of the cube
*/
#include <stdio.h>
#include "SceneLib.h"

void cube(double x, double y, double z, double h, Material* mtrl, Scene *scene)
{

   /* create geometry for the cube */
   Vertex v[9];

   v[0] = Vertex(h/2+x, 0.0+y, h/2+z);  // center of the bottom face
   v[1] = Vertex(0.0+x, 0.0+y, 0.0+z);  // four vertices around the bottom face
   v[2] = Vertex(  h+x, 0.0+y, 0.0+z);
   v[3] = Vertex(  h+x, 0.0+y,   h+z);
   v[4] = Vertex(0.0+x, 0.0+y,   h+z);
   v[5] = Vertex(0.0+x,   h+y, 0.0+z);  // four vertices around the top face
   v[6] = Vertex(  h+x,   h+y, 0.0+z);
   v[7] = Vertex(  h+x,   h+y,   h+z);
   v[8] = Vertex(0.0+x,   h+y,   h+z);


   v[0].setColor(1.0, 0.0, 0.0);
   v[1].setColor(1.0, 0.0, 0.0);
   v[2].setColor(1.0, 0.0, 0.0);
   v[0].n = Vector(0.0, -1.0, 0.0);
   v[1].n = Vector(0.0, -1.0, 0.0);
   v[2].n = Vector(0.0, -1.0, 0.0);
   Triangle *t1 = new Triangle(v+0, v+1, v+2);
   t1->mtrl = mtrl;
   scene->addTriangle( t1 );   // bottom face

   v[0].setColor(0.0, 1.0, 0.0);
   v[2].setColor(0.0, 1.0, 0.0);
   v[3].setColor(0.0, 1.0, 0.0);
   v[0].n = Vector(0.0, -1.0, 0.0);
   v[1].n = Vector(0.0, -1.0, 0.0);
   v[2].n = Vector(0.0, -1.0, 0.0);
   Triangle *t2 = new Triangle(v+0, v+2, v+3);
   t2->mtrl = mtrl;
   scene->addTriangle( t2 );   // bottom face

   v[0].setColor(0.0, 0.0, 1.0);
   v[3].setColor(0.0, 0.0, 1.0);
   v[4].setColor(0.0, 0.0, 1.0);
   v[0].n = Vector(0.0, -1.0, 0.0);
   v[1].n = Vector(0.0, -1.0, 0.0);
   v[2].n = Vector(0.0, -1.0, 0.0);
   Triangle *t3 = new Triangle(v+0, v+3, v+4);
   t3->mtrl = mtrl;
   scene->addTriangle( t3 );   // bottom face

   v[0].setColor(1.0, 1.0, 1.0);
   v[4].setColor(1.0, 1.0, 1.0);
   v[1].setColor(1.0, 1.0, 1.0);
   v[0].n = Vector(0.0, -1.0, 0.0);
   v[1].n = Vector(0.0, -1.0, 0.0);
   v[2].n = Vector(0.0, -1.0, 0.0);
   Triangle *t4 = new Triangle(v+0, v+4, v+1);
   t4->mtrl = mtrl;
   scene->addTriangle( t4 );   // bottom face

   v[4].setColor(1.0, 0.0, 0.0);
   v[3].setColor(0.0, 1.0, 0.0);
   v[7].setColor(0.0, 0.0, 1.0);
   v[0].n = Vector(0.0, 0.0, 1.0);
   v[1].n = Vector(0.0, 0.0, 1.0);
   v[2].n = Vector(0.0, 0.0, 1.0);
   Triangle *t5 = new Triangle(v+4, v+3, v+7);
   t5->mtrl = mtrl;
   scene->addTriangle( t5 );  // front face

   v[7].setColor(0.0, 0.0, 1.0);
   v[8].setColor(0.0, 1.0, 0.0);
   v[4].setColor(1.0, 0.0, 0.0);
   v[0].n = Vector(0.0, 0.0, 1.0);
   v[1].n = Vector(0.0, 0.0, 1.0);
   v[2].n = Vector(0.0, 0.0, 1.0);
   Triangle *t6 = new Triangle(v+7, v+8, v+4);
   t6->mtrl = mtrl;
   scene->addTriangle( t6 );   // front face

   v[3].setColor(0.0, 1.0, 1.0);
   v[2].setColor(0.0, 1.0, 1.0);
   v[6].setColor(0.0, 1.0, 1.0);
   v[0].n = Vector(1.0, 0.0, 0.0);
   v[1].n = Vector(1.0, 0.0, 0.0);
   v[2].n = Vector(1.0, 0.0, 0.0);
   Triangle *t7 = new Triangle(v+3, v+2, v+6);
   t7->mtrl = mtrl;
   scene->addTriangle( t7 );   // right face

   v[6].setColor(1.0, 1.0, 1.0);
   v[7].setColor(0.0, 1.0, 1.0);
   v[3].setColor(0.0, 1.0, 1.0);
   v[0].n = Vector(1.0, 0.0, 0.0);
   v[1].n = Vector(1.0, 0.0, 0.0);
   v[2].n = Vector(1.0, 0.0, 0.0);
   Triangle *t8 = new Triangle(v+6, v+7, v+3);
   t8->mtrl = mtrl;
   scene->addTriangle( t8 );   // right face

   v[2].setColor(1.0, 0.0, 0.0);
   v[1].setColor(0.0, 1.0, 0.0);
   v[5].setColor(0.0, 0.0, 1.0);
   v[0].n = Vector(0.0, 0.0, -1.0);
   v[1].n = Vector(0.0, 0.0, -1.0);
   v[2].n = Vector(0.0, 0.0, -1.0);
   Triangle *t9 = new Triangle(v+2, v+1, v+5);
   t9->mtrl = mtrl;
   scene->addTriangle( t9 );   // back face

   v[5].setColor(1.0, 0.0, 0.0);
   v[6].setColor(0.0, 1.0, 0.0);
   v[2].setColor(0.0, 0.0, 1.0);
   v[0].n = Vector(0.0, 0.0, -1.0);
   v[1].n = Vector(0.0, 0.0, -1.0);
   v[2].n = Vector(0.0, 0.0, -1.0);
   Triangle *t10 = new Triangle(v+5, v+6, v+2);
   t10->mtrl = mtrl;
   scene->addTriangle( t10 );   // back face

   v[1].setColor(1.0, 0.0, 0.0);
   v[4].setColor(0.0, 1.0, 0.0);
   v[8].setColor(0.0, 0.0, 1.0);
   v[0].n = Vector(-1.0, 0.0, 0.0);
   v[1].n = Vector(-1.0, 0.0, 0.0);
   v[2].n = Vector(-1.0, 0.0, 0.0);
   Triangle *t11 = new Triangle(v+1, v+4, v+8);
   t11->mtrl = mtrl;
   scene->addTriangle( t11 );   // left face

   v[8].setColor(0.0, 0.0, 1.0);
   v[5].setColor(0.0, 1.0, 0.0);
   v[1].setColor(1.0, 0.0, 0.0);
   v[0].n = Vector(-1.0, 0.0, 0.0);
   v[1].n = Vector(-1.0, 0.0, 0.0);
   v[2].n = Vector(-1.0, 0.0, 0.0);
   Triangle *t12 = new Triangle(v+8, v+5, v+1);
   t12->mtrl = mtrl;
   scene->addTriangle( t12 );   // left face

   v[8].setColor(1.0, 1.0, 0.0);
   v[7].setColor(1.0, 1.0, 0.0);
   v[6].setColor(1.0, 0.0, 0.0);
   v[0].n = Vector(0.0, 1.0, 0.0);
   v[1].n = Vector(0.0, 1.0, 0.0);
   v[2].n = Vector(0.0, 1.0, 0.0);
   Triangle *t13 = new Triangle(v+8, v+7, v+6);
   t13->mtrl = mtrl;
   scene->addTriangle( t13 );   // top face

   v[6].setColor(1.0, 1.0, 0.0);
   v[5].setColor(1.0, 1.0, 0.0);
   v[8].setColor(1.0, 1.0, 0.0);
   v[0].n = Vector(0.0, 1.0, 0.0);
   v[1].n = Vector(0.0, 1.0, 0.0);
   v[2].n = Vector(0.0, 1.0, 0.0);
   Triangle *t14 = new Triangle(v+6, v+5, v+8);
   t14->mtrl = mtrl;
   scene->addTriangle( t14 );   // top face

   return;
}
