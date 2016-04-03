#include "P2_World2View.h"

P2_World2View::P2_World2View() : PipelineStage()
{
  // This stage reads and modifies Triangle 
  // vertex positions.
  attributes |= (STAGE_ATTRIB_VERTEX_POSITION & STAGE_ATTRIB_RW);
}

void P2_World2View::processTriangles(Context *context)
{
   // Walk the list of Triangle objects.
   // For each Vertex in each Triangle object,
   // use the Scene's view matrix to transform
   // the Vertex object to view coordinates.
   Scene *scene = context->scene;
   for (ListNode<Triangle> *ptr = context->triangles->head; ptr; ptr = ptr->next)
   {
      Matrix m = scene->view;
      ptr->v[0] = m.times( ptr->v[0] );
      ptr->v[1] = m.times( ptr->v[1] );
      ptr->v[2] = m.times( ptr->v[2] );
   }
   return;
}
