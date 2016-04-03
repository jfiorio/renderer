#include <stdio.h>
#include "SceneLib.h"

void P10_FragmentShader(Scene* scene)
{
   // Walk the list of Triangle objects and for each traingle,
   // walk its list of Fragment objects. For each fragment, use
   // the barycentric coordinates in the fragment to interpolate
   // color data from the triangle's three vertices to the fragment.

   for (TriangleListNode *ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      // For this triangle, get the color information
      // from each of the three vertices.
      double r0 = ptr->t->v[0].r;
      double g0 = ptr->t->v[0].g;
      double b0 = ptr->t->v[0].b;
      double a0 = ptr->t->v[0].a;
      double r1 = ptr->t->v[1].r;
      double g1 = ptr->t->v[1].g;
      double b1 = ptr->t->v[1].b;
      double a1 = ptr->t->v[1].a;
      double r2 = ptr->t->v[2].r;
      double g2 = ptr->t->v[2].g;
      double b2 = ptr->t->v[2].b;
      double a2 = ptr->t->v[2].a;

      // Walk this triangle's list of fragments.
      for (Fragment *fptr = ptr->t->fragment_p; fptr; fptr = fptr->nextFragment)
      {
         // get this fragment's barycentric coordinates
         double alpha = fptr->alpha;
         double beta  = fptr->beta;
         double gamma = fptr->gamma;

         //fprintf(stderr, "alpha = %f, beta = %f, gamma = %f\n", alpha,beta,gamma);

         // interpolate color data from the triangle's vertices to this fragment
         fptr->r = alpha*r0 + beta*r1 + gamma*r2;
         fptr->g = alpha*g0 + beta*g1 + gamma*g2;
         fptr->b = alpha*b0 + beta*b1 + gamma*b2;
         fptr->a = alpha*a0 + beta*a1 + gamma*a2;

         //fprintf(stderr, "r = %f, g = %f, b = %f\n", fptr->r,fptr->g,fptr->b);
      }
   }
   return;
}
