/*
   For a good explanation of orientation, forward and backward facing polygons,
   and back face culling, see Chapter 9 of
      "Computer Graphics Through OpenGL: From Theory to Experiments "
       by Sumanta Guha, Chapman and Hall/CRC, 2010.
  That chapter is available at
       http://www.sumantaguha.com/files/materials/chapter9.pdf
*/
#include <stdio.h>
#include "SceneLib.h"


bool triangleIsFrontFacing(bool frontFacingIsCCW, Triangle *t);
bool triangleIsCCW(Triangle *t);


void P3_BackFaceCulling(Scene* scene)
{
   // Walk the list of Triangle objects.
   // Determine if a triangle is forward or
   // backward facing. Delete any triangle
   // that is backward facing.

   if (scene->doBackFaceCulling)
   {
      TriangleListNode *ptr;  // use this pointer to walk the scene's list of triangles
      TriangleListNode *previous_ptr;

      previous_ptr = &(scene->head_node);

      for (ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
      {
         if ( !triangleIsFrontFacing(scene->frontFacingIsCCW, ptr->t) )
         {// unlink this triangle
            previous_ptr->next = ptr->next;
            delete ptr->t;   // delete the Triangle object
            delete ptr;      // delete the TriangleListNode object
            if (ptr == scene->tail_ptr) scene->tail_ptr = previous_ptr;
            ptr = previous_ptr;
         }
         previous_ptr = ptr;
      }
   }
   return;
}


bool triangleIsFrontFacing(bool frontFacingIsCCW, Triangle *t)
{
   bool result = triangleIsCCW(t);
   if ( !frontFacingIsCCW )
      result = !result;
   return result;
}


bool triangleIsCCW(Triangle *t)
{
   // We only need to know the z component of the cross product
   // of the two "edge vectors", v1-v0 and v2-v0. This works
   // because we are in view coordinates, with the camera at
   // the origin looking down the negative z-axis.
   double x0 = t->v[0].x;
   double x1 = t->v[1].x;
   double x2 = t->v[2].x;
   double y0 = t->v[0].y;
   double y1 = t->v[1].y;
   double y2 = t->v[2].y;
   double zValueOfCrossProduct = (x1-x0)*(y2-y0) - (y1-y0)*(x2-x0);

   //fprintf(stderr, "zValueOfCrossProduct = % .6f\n", zValueOfCrossProduct);
   //fflush(stderr);

   return (zValueOfCrossProduct >= 0);
}

/*
bool triangleIsCCW(Triangle *t)
{
   // Here is another way to calculate the exact same thing.
   // Compute the signed area of the triangle projected into the xy-plane.
   double x0 = t->v[0].x;
   double x1 = t->v[1].x;
   double x2 = t->v[2].x;
   double y0 = t->v[0].y;
   double y1 = t->v[1].y;
   double y2 = t->v[2].y;
   double determinant = (x1-x0)*(y2-y0) - (y1-y0)*(x2-x0);

   //fprintf(stderr, "determinant = % .6f\n", determinant);
   //fflush(stderr);

   return (determinant > 0);
}
*/


/*
bool triangleIsCCW(Triangle *t)
{
   double x0 = t->v[0].x;
   double x1 = t->v[1].x;
   double x2 = t->v[2].x;
   double y0 = t->v[0].y;
   double y1 = t->v[1].y;
   double y2 = t->v[2].y;
   double z0 = t->v[0].z;
   double z1 = t->v[1].z;
   double z2 = t->v[2].z;
   double determinant = x0*(y1*z2-y2*z1)-x1*(y0*z2-y2*z0)+x2*(y0*z1-y1*z0);

   //fprintf(stderr, "determinant = % .6f\n", determinant);
   //fflush(stderr);

   return (determinant > 0);
}
*/
