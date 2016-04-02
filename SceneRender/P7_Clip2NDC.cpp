#include "SceneLib.h"

void P7_Clip2NDC(Scene* scene)
{
   // Do the perpective division step of the pipeline.

   // Walk the list of Triangle objects and
   // divide each of the coordinates in a
   // vertex by its w coordinate, i.e.,
   // (x, y, z, w) -> (x/w, y/w, z/w, 1).

   for (TriangleListNode *ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      ptr->t->v[0] = (ptr->t->v[0]).times( 1.0/(ptr->t->v[0].w) );
      ptr->t->v[1] = (ptr->t->v[1]).times( 1.0/(ptr->t->v[1].w) );
      ptr->t->v[2] = (ptr->t->v[2]).times( 1.0/(ptr->t->v[2].w) );
   }
   return;
}
