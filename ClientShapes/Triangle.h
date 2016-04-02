/*
   x, world x-coordinate of the "origin" for the triangle
   y, world y-coordinate of the "origin" for the triangle
   z, world z-coordinate of the "origin" for the triangle
   h, the side length of the triangle is h*sqrt(2)
*/
#include <stdio.h>
#include "SceneLib.h"

void triangle(double x, double y, double z, double h, Material* mtrl, Scene *scene)
{
   Vertex v1 = Vertex(x + h,   y + 0.0, z + 0.0);
   Vertex v2 = Vertex(x + 0.0, y + h,   z + 0.0);
   Vertex v3 = Vertex(x + 0.0, y + 0.0, z + h);

   v1.setColor(1.0, 0.0, 0.0);
   v2.setColor(0.0, 1.0, 0.0);
   v3.setColor(0.0, 0.0, 1.0);
   Triangle *t = new Triangle(&v1, &v2, &v3);
   t->mtrl = mtrl;
   scene->addTriangle( t );

   return;
}
