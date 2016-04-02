#include "SceneLib.h"

void P1_Model2World(Scene* scene)
{
   // Walk the list of Triangle objects.
   // For each Vertex in each Triangle object,
   // use the Scene's model matrix to transform
   // the Vertex object to model coordinates.

   for (TriangleListNode *ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      Matrix m = scene->model;
      ptr->t->v[0] = m.times( ptr->t->v[0] );
      ptr->t->v[1] = m.times( ptr->t->v[1] );
      ptr->t->v[2] = m.times( ptr->t->v[2] );
   }
   return;
}
