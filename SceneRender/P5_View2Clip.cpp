#include "P5_View2Clip.h"
 
P5_View2Clip::P5_View2Clip() : PipelineStage()
{
  // This stage reads and modifies Triangle 
  // vertex positions.
  attributes |= (STAGE_ATTRIB_VERTEX_POSITION & STAGE_ATTRIB_RW);
}

void P5_View2Clip::processTriangles(Context *context)
{
  // Walk the list of Triangle objects.
  // For each Vertex in each Triangle object,
  // use the Scene's projection matrix to transform
  // the Vertex object to clip coordinates.
  
  Scene *scene = context->scene;
  for (ListNode<Triangle> *ptr = context->triangles->head; ptr; ptr = ptr->next)
  {
    Matrix m = scene->projection;
    ptr->v[0] = m.times( ptr->v[0] );
    ptr->v[1] = m.times( ptr->v[1] );
    ptr->v[2] = m.times( ptr->v[2] );
  }
  return;
}
