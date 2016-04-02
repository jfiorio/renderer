/*
   k number of triangles in the triangle fan at the top and bottom
   n number of triangle strips between the top and bottom
   x world x-coordinate of the center of the sphere
   y world y-coordinate of the center of the sphere
   z world z-coordinate of the center of the sphere
   r radius of the sphere
*/
#include <stdio.h>
#include <math.h>
#include "SceneLib.h"

void sphere(int k, int n, double x, double y, double z, double r,  Material* mtrl, Scene *scene)
{
   #define PI 3.1415926535897932384626433832795

    /* create sphere's geometry */
   Vertex v[n+1][k];

   // an array of vertices to be used to create triangles
   for (int j = 0; j < k; j++)
   {
      double c = cos(j*(2.0*PI)/k);
      double s = sin(j*(2.0*PI)/k);
      for (int i = 0; i <= n; i++)
      {
         v[i][j] = Vertex(x + r*sin((i+1)*PI/(n+2))*c,
                          y + r*cos((i+1)*PI/(n+2)),
                          z + r*sin((i+1)*PI/(n+2))*s);
         // give the vertex a normal vector
         (v[i][j]).n = Vector(sin((i+1)*PI/(n+2))*c,
                              cos((i+1)*PI/(n+2)),
                              sin((i+1)*PI/(n+2))*s);
         // give the vertex a color
         v[i][j].setColor(fabs(sin((i+1)*PI/(n+2))*c),
                          fabs(cos((i+1)*PI/(n+2))),
                          fabs(sin((i+1)*PI/(n+2))*s));
      }
   }

   // create the triangle fan at the top
   Triangle *t;
   Vertex topCenter = Vertex(0.0+x, r+y, 0.0+z);
   topCenter.n = Vector(0.0, 1.0, 0.0);
   for (int j = 0; j < k-1; j++)
   {
      t = new Triangle( &topCenter, &(v[0][j+1]), &(v[0][j]) );
      t->mtrl = mtrl;
      scene->addTriangle( t );
   }
   t = new Triangle( &topCenter, &(v[0][0]), &(v[0][k-1]) );
   t->mtrl = mtrl;
   scene->addTriangle( t );

   // create all the triangles along the cylinder wall
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < k-1; j++)
      {
         t = new Triangle( &(v[i][j]), &(v[i][j+1]), &(v[i+1][j+1]) );
         t->mtrl = mtrl;
         scene->addTriangle( t );
         t = new Triangle( &(v[i+1][j+1]), &(v[i+1][j]), &(v[i][j]) );
         t->mtrl = mtrl;
         scene->addTriangle( t );
      }
      t = new Triangle( &(v[i][k-1]), &(v[i][0]), &(v[i+1][0]) );
      t->mtrl = mtrl;
      scene->addTriangle( t );
      t = new Triangle( &(v[i+1][0]), &(v[i+1][k-1]), &(v[i][k-1]) );
      t->mtrl = mtrl;
      scene->addTriangle( t );
   }

   // create the triangle fan at the bottom
   Vertex bottomCenter = Vertex(0.0+x, -r+y, 0.0+z);
   bottomCenter.n = Vector(0.0, -1.0, 0.0);
   for (int j = 0; j < k-1; j++)
   {
      t = new Triangle(&bottomCenter, &(v[n][j]), &(v[n][j+1]) );
      t->mtrl = mtrl;
      scene->addTriangle( t );
   }
   t = new Triangle( &bottomCenter, &(v[n][k-1]), &(v[n][0]) );
   t->mtrl = mtrl;
   scene->addTriangle( t );

   return;
}
