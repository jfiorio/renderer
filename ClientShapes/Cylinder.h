/*
   k number of triangles in the triangle fan at the top and bottom
   n number of triangle strips between the top and bottom
   x world x-coordinate of the center of the cylinder
   y world y-coordinate of the center of the cylinder
   z world z-coordinate of the center of the cylinder
   r radius of the cylinder
   h height of the cylinder
*/
#include <stdio.h>
#include <math.h>
#include "SceneLib.h"

void cylinder(int k, int n, double x, double y, double z, double r, double h,  Material* mtrl, Scene *scene)
{
   #define PI 3.1415926535897932384626433832795

    /* create geometry for the cylinder */
   Vertex v[n+1][k];

   // an array of vertices to be used to create triangles
   for (int j = 0; j < k; j++)
   {
      double c = cos(j*(2.0*PI)/k);
      double s = sin(j*(2.0*PI)/k);
      for (int i = 0; i <= n; i++)
      {
         v[i][j] = Vertex( x + r*c, y + h-i*((2.0*h)/n), z + r*s );
         // give the vertex a normal vector
         (v[i][j]).n = Vector(c, 0.0, s);
         // give the vertex a color
         v[i][j].setColor( fabs(c), (double)i/(double)n, fabs(s) );
      }
   }

   // create the triangle fan at the top
   Triangle *t;
   Vertex topCenter = Vertex(0.0+x, h+y, 0.0+z);
   topCenter.n = Vector(0.0, 1.0, 0.0);
   topCenter.setColor(0.0, 0.0, 0.0);
   for (int j = 0; j < k-1; j++)
   {
      t = new Triangle( &topCenter, &(v[0][j+1]), &(v[0][j]) );
      t->mtrl = mtrl;
      // modify the normal in these vertices
      (t->v[1]).n = Vector(0.0, 1.0, 0.0);
      (t->v[2]).n = Vector(0.0, 1.0, 0.0);
      scene->addTriangle( t );
   }
   t = new Triangle( &topCenter, &(v[0][0]), &(v[0][k-1]) );
   (t->v[1]).n = Vector(0.0, 1.0, 0.0);
   (t->v[2]).n = Vector(0.0, 1.0, 0.0);
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
   Vertex bottomCenter = Vertex(0.0+x, -h+y, 0.0+z);
   bottomCenter.n = Vector(0.0, -1.0, 0.0);
   bottomCenter.setColor(0.0, 0.0, 0.0);
   for (int j = 0; j < k-1; j++)
   {
      t = new Triangle(&bottomCenter, &(v[n][j]), &(v[n][j+1]) );
      t->mtrl = mtrl;
      // modify the normal in these vertices
      (t->v[1]).n = Vector(0.0, -1.0, 0.0);
      (t->v[2]).n = Vector(0.0, -1.0, 0.0);
      scene->addTriangle( t );
   }
   t = new Triangle( &bottomCenter, &(v[n][k-1]), &(v[n][0]) );
   (t->v[1]).n = Vector(0.0, -1.0, 0.0);
   (t->v[2]).n = Vector(0.0, -1.0, 0.0);
   t->mtrl = mtrl;
   scene->addTriangle( t );

   return;
}
