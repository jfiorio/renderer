#include <stdio.h>
#include "Triangle.h"

Triangle::Triangle()
{
   v[0].set(0, 0, 0);
   v[1].set(0, 0, 0);
   v[2].set(0, 0, 0);

   tex  = NULL;
   mtrl = NULL;
}


Triangle::Triangle(Vertex * v0, Vertex * v1, Vertex * v2)
{
   v[0] = (*v0);
   v[1] = (*v1);
   v[2] = (*v2);

   tex  = NULL;
   mtrl = NULL;
}


Triangle::~Triangle()
{
  // !
}


void Triangle::setTexture(Texture * t)
{
   tex = t;
   return;
}


/* for debugging */
void Triangle::print(int fields)
{
   fprintf(stderr,"Triangle:\n");
   v[0].print(fields);
   v[1].print(fields);
   v[2].print(fields);
   return;
}
