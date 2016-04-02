#include <stdio.h>
#include "SceneLib.h"

void P9_Rasterize(Scene* scene)
{
   // Walk the list of Triangle objects and rasterize each triangle.

   // This rasterization algorithm is from
   //   "Fundamentals of Computer Graphics", 2nd Edition,
   //    by Peter Shirley, pages 63-66.

   for (TriangleListNode *ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      double x0 = ptr->t->v[0].x;
      double y0 = ptr->t->v[0].y;
      double x1 = ptr->t->v[1].x;
      double y1 = ptr->t->v[1].y;
      double x2 = ptr->t->v[2].x;
      double y2 = ptr->t->v[2].y;

      // find the greatest integer less than x0, x1, and x2
      int xMin = (int)x0;
      if (x1 < xMin) xMin = (int)x1;
      if (x2 < xMin) xMin = (int)x2;

      // find the greatest integer less than y0, y1, and y2
      int yMin = (int)y0;
      if (y1 < yMin) yMin = (int)y1;
      if (y2 < yMin) yMin = (int)y2;

      // find the least integer greater than x0, x1, and x2
      int xMax = (int)(x0 + 1);
      if (x1 > xMax) xMax = (int)(x1 + 1);
      if (x2 > xMax) xMax = (int)(x2 + 1);

      // find the least integer greater than y0, y1, and y2
      int yMax = (int)(y0 + 1);
      if (y1 > yMax) yMax = (int)(y1 + 1);
      if (y2 > yMax) yMax = (int)(y2 + 1);

      //fprintf(stderr, "xMin=%d, xMax=%d, yMin=%d, yMax=%d\n", xMin,xMax,yMin,yMax);

      #define f01(x,y) ( (y0-y1)*(x) + (x1-x0)*(y) + x0*y1 - x1*y0 )
      #define f12(x,y) ( (y1-y2)*(x) + (x2-x1)*(y) + x1*y2 - x2*y1 )
      #define f20(x,y) ( (y2-y0)*(x) + (x0-x2)*(y) + x2*y0 - x0*y2 )

      for (int y = yMax; y > yMin; y--) // top to bottom
      {
         for (int x = xMin; x < xMax; x++)  /// left to right
         {
            double alpha = f12(x,y) / f12(x0,y0);
            double beta  = f20(x,y) / f20(x1,y1);
            double gamma = f01(x,y) / f01(x2,y2);

            //fprintf(stderr, "alpha = %f, beta = %f, gamma = %f\n", alpha,beta,gamma);

            if (alpha >= 0 && beta >= 0 && gamma >= 0)
            {  // create a new fragment
               Fragment *f = new Fragment();
               f->xIndex = x;
               f->yIndex = y;
               f->zCoord = alpha*(ptr->t->v[0].z) + beta*(ptr->t->v[1].z) + gamma*(ptr->t->v[2].z);
               f->alpha = alpha;
               f->beta = beta;
               f->gamma = gamma;
               f->r = 0;
               f->g = 0;
               f->b = 0;
               f->a = 0;
               // add this fragment to this triangle's list of fragments
               f->nextFragment = ptr->t->fragment_p;
               ptr->t->fragment_p = f;

               //fprintf(stderr, "x=%d,y=%d,alpha=%f,beta=%f,gamma=%f\n", x,y,alpha,beta,gamma);
            }
         }
      }
   }
   return;
}
