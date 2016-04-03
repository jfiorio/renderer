#include "P7_Clip2NDC.h"

P7_Clip2NDC::P7_Clip2NDC() : PipelineStage()
{
  // This stage reads and modifies Triangle 
  // vertex positions.
  attributes |= (STAGE_ATTRIB_VERTEX_POSITION & STAGE_ATTRIB_RW);
}

void P7_Clip2NDC::processTriangles(Context *context)
{
   // Do the perpective division step of the pipeline.

   // Walk the list of Triangle objects and
   // divide each of the coordinates in a
   // vertex by its w coordinate, i.e.,
   // (x, y, z, w) -> (x/w, y/w, z/w, 1).

   // EDIT: retain the w coordinate for perspective
   // correct triangle interpolation at the fragment
   // shading stage
   for (ListNode<Triangle> *ptr = context->triangles->head; ptr; ptr = ptr->next)
   {
      float w0, w1, w2;
      w0 = ptr->v[0].w;
      w1 = ptr->v[1].w;
      w2 = ptr->v[2].w;

      ptr->v[0] = (ptr->v[0]).times( 1.0/(ptr->v[0].w) );
      ptr->v[1] = (ptr->v[1]).times( 1.0/(ptr->v[1].w) );
      ptr->v[2] = (ptr->v[2]).times( 1.0/(ptr->v[2].w) );
      
      ptr->v[0].w = w0;
      ptr->v[1].w = w1;
      ptr->v[2].w = w2;
   }
   return;
}
