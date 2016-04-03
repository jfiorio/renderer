#include "P1_Model2World.h"

P1_Model2World::P1_Model2World() : PipelineStage()
{
  // This stage reads and modifies Triangle 
  // vertex positions.
  attributes |= (STAGE_ATTRIB_VERTEX_POSITION & STAGE_ATTRIB_RW);
}

void P1_Model2World::processTriangles(Context *context)
{
  // Walk the list of Triangle objects.
  // For each Vertex in each Triangle object,
  // use the Scene's model matrix to transform
  // the Vertex object to model coordinates.
  Scene *scene = context->scene;
  for (ListNode<Triangle> *ptr = context->triangles->head; ptr; ptr = ptr->next)
  {
     Matrix m = scene->model;
     ptr->v[0] = m.times( ptr->v[0] );
     ptr->v[1] = m.times( ptr->v[1] );
     ptr->v[2] = m.times( ptr->v[2] );
  }
  return;
}
