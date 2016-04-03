/*
   For a good explanation of orientation, forward and backward facing polygons,
   and back face culling, see Chapter 9 of
      "Computer Graphics Through OpenGL: From Theory to Experiments "
       by Sumanta Guha, Chapman and Hall/CRC, 2010.
  That chapter is available at
       http://www.sumantaguha.com/files/materials/chapter9.pdf
*/
#include "P3_BackFaceCulling.h"

P3_BackFaceCulling::P3_BackFaceCulling() : PipelineStage()
{
  // This stage reads vertex positions
  // and modifies the arrangement of triangles
  attributes |= (STAGE_ATTRIB_READ_VERTEX_POSITION | STAGE_ATTRIB_WRITE_TRIANGLE_LIST);
}

void P3_BackFaceCulling::processTriangles(Context *context)
{
   // Walk the list of Triangle objects.
   // Determine if a triangle is forward or
   // backward facing. Delete any triangle
   // that is backward facing.

   Scene *scene = context->scene;
   if (scene->doBackFaceCulling)
   {
      Pool<Triangle> *trianglePool = context->trianglePool;
      List<Triangle> *triangles = context->triangles;
      ListNode<Triangle> *prev = &triangles->sentinel;
      
      for (ListNode<Triangle> *ptr = triangles->head; ptr; ptr = ptr->next)
      {
        if (!triangleIsFrontFacing(scene->frontFacingIsCCW, ptr))
        {
          triangles->remove(prev, ptr);
          trianglePool->reclaimNode(ptr);
          ptr = prev;
        }
        prev = ptr;
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

/*
  quicker loop? for future?
while (ptr && !triangleIsFrontFacing(scene->frontFacingIsCCW, ptr))
{
  triangles->removeHead();
  trianglePool->reclaimNode(ptr);
  ptr = ptr->next;
}
if (!ptr) { return; }
prev = ptr;
ptr = ptr->next;
while (ptr)
{
  triangles->remove(prev, ptr);
  trianglePool->reclaimNode(ptr);
  prev = ptr;
  ptr = ptr->next;
}
*/