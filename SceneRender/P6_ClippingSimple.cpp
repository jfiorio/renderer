#include <math.h>
#include "SceneLib.h"

void P6_Clipping(Scene* scene)
{
   // Walk the list of Triangle objects and
   // clip the triangles so that all of their
   // vertices are within the clipping volume.

   // A simple clipping algorithm can just
   // delete from the Scene's list of Triangles
   // any Triangle that has a vertex that sticks
   // out of the clipping volume.

   // A better clipping algorithm will replace
   // any Triangle that sticks out of the clipping
   // volume with a set of Triangles that triangulate
   // the polygon that results when the sticking out
   // Triangle is clipped.

   TriangleListNode *ptr;  // use this pointer to walk the scene's list of triangles
   TriangleListNode *previous_ptr;

   previous_ptr = &(scene->head_node);

   for (ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      bool deleteFlag = false;
      for (int i = 0; i < 3; i++)  // test all three vertices
      {
         double w = fabs( ptr->t->v[i].w );
         if ( fabs(ptr->t->v[i].x) > w
           || fabs(ptr->t->v[i].y) > w
           || fabs(ptr->t->v[i].z) > w )
         {
            deleteFlag = true;
         }
      }
      if ( deleteFlag )
      {// unlink this triangle
         previous_ptr->next = ptr->next;
         delete ptr->t;   // delete the Triangle object
         delete ptr;      // delete the TriangleListNode object
         ptr = previous_ptr;
      }
      previous_ptr = ptr;
   }// continue with the other triangles

   return;
}
