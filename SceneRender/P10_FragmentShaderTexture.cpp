#include <stdio.h>
#include "SceneLib.h"

void P10_FragmentShaderTexture(Scene* scene)
{
   // Walk the list of Triangle objects and for each traingle,
   // walk its list of Fragment objects. For each fragment, use
   // the barycentric coordinates in the fragment to interpolate
   // color data or texture data from the triangle's three vertices
   //to the fragment.

   for (TriangleListNode *ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      if (NULL == ptr->t->tex)  // if this triangle dows not have a texture
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
      else  // do the texture case
      {
         // For this triangle, get the texture coordinates
         // from each of the three vertices.
         double s0 = ptr->t->v[0].s;
         double t0 = ptr->t->v[0].t;
         double s1 = ptr->t->v[1].s;
         double t1 = ptr->t->v[1].t;
         double s2 = ptr->t->v[2].s;
         double t2 = ptr->t->v[2].t;

         // Walk this triangle's list of fragments.
         for (Fragment *fptr = ptr->t->fragment_p; fptr; fptr = fptr->nextFragment)
         {
            // get this fragment's barycentric coordinates
            double alpha = fptr->alpha;
            double beta  = fptr->beta;
            double gamma = fptr->gamma;

            //fprintf(stderr, "alpha = %f, beta = %f, gamma = %f\n", alpha,beta,gamma);

            // interpolate texture coordinates, for this fragment, from the triangle's vertices
            double s = alpha*s0 + beta*s1 + gamma*s2;
            double t = alpha*t0 + beta*t1 + gamma*t2;

            s = s - (int)s;  // wrap the texture in both the s
            t = t - (int)t;  // and t coordinate directions

            //fprintf(stderr, "s = %f, t = %f\n", fptr->s,fptr->t);

            // convert (interpolated) texture coordinates into pixel indices
            int i = t * ptr->t->tex->height;  // row index
            int j = s * ptr->t->tex->width;   // column index

            // copy color data from the texture to this fragment
            fptr->r = *((ptr->t->tex->data)+(i*3*(ptr->t->tex->width))+(3*j)+0) / 255.0;
            fptr->g = *((ptr->t->tex->data)+(i*3*(ptr->t->tex->width))+(3*j)+1) / 255.0;
            fptr->b = *((ptr->t->tex->data)+(i*3*(ptr->t->tex->width))+(3*j)+2) / 255.0;
            fptr->a = 1.0;
            //fprintf(stderr, "r = %f, g = %f, b = %f\n", fptr->r,fptr->g,fptr->b);
         }
      }
   }
   return;
}