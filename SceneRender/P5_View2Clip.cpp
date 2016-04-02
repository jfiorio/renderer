#include "SceneLib.h"

void P5_View2Clip(Scene* scene)
{
   // Walk the list of Triangle objects.
   // For each Vertex in each Triangle object,
   // use the Scene's projection matrix to transform
   // the Vertex object to clip coordinates.

   for (TriangleListNode *ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      Matrix m = scene->projection;
      ptr->t->v[0] = m.times( ptr->t->v[0] );
      ptr->t->v[1] = m.times( ptr->t->v[1] );
      ptr->t->v[2] = m.times( ptr->t->v[2] );
   }
   return;
}
