#include <stdio.h>
#include "Triangle.h"

Triangle::Triangle()
{
   v[0].set(0, 0, 0);
   v[1].set(0, 0, 0);
   v[2].set(0, 0, 0);

   tex  = NULL;
   mtrl = NULL;
   fragment_p = NULL;  // points at fragments (after rasterization stage)
}


Triangle::Triangle(Vertex * v0, Vertex * v1, Vertex * v2)
{
   v[0] = (*v0);
   v[1] = (*v1);
   v[2] = (*v2);
/*
   v[0].r = v0->r;
   v[0].g = v0->g;
   v[0].b = v0->b;
   v[0].a = v0->a;
   v[1].r = v1->r;
   v[1].g = v1->g;
   v[1].b = v1->b;
   v[1].a = v1->a;
   v[2].r = v2->r;
   v[2].g = v2->g;
   v[2].b = v2->b;
   v[2].a = v2->a;

   v[0].s = v0->s;
   v[0].t = v0->t;
   v[1].s = v1->s;
   v[1].t = v1->t;
   v[2].t = v2->s;
   v[2].g = v2->t;
*/
   tex  = NULL;
   mtrl = NULL;
   fragment_p = NULL;
}


Triangle::~Triangle()
{
   Fragment *fptr;

   while ( fragment_p )
   {
      fptr = fragment_p->nextFragment;

      delete fragment_p; /* delete the Fragment object */

      fragment_p = fptr;
   }
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
