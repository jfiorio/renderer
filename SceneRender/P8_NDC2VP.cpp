#include "P8_NDC2VP.h"

P8_NDC2VP::P8_NDC2VP() : PipelineStage()
{
  // This stage reads and modifies Triangle 
  // vertex positions.
  attributes |= (STAGE_ATTRIB_VERTEX_POSITION & STAGE_ATTRIB_RW);
}

void P8_NDC2VP::processTriangles(Context *context)
{
   // Walk the list of Triangle objects and transform
   // the coordinates of each vertex, which are now in
   // "Normalized Device Coordinates", to "pixel
   // coordinates" in the viewport. This is the last
   // transformation before we rasterize the triangles.
   
   Scene *scene = context->scene;
   int vpWidth  = scene->vp_width;
   int vpHeight = scene->vp_height;

   for (ListNode<Triangle> *ptr = context->triangles->head; ptr; ptr = ptr->next)
   {
      // The "normalized device coordinates" are all between -1 and 1.
      // The horizontal viewport coordinates should be between 0 and vpWidth.
      // The vertical viewport coordinates should be between 0 and vpHeight.
      // So x_vp =  (vpWidth/2.0)*(x_ndc + 1.0)
      //    y_vp = (vpHeight/2.0)*(y_ndc + 1.0)
      ptr->v[0].x = (vpWidth/2.0)  * (ptr->v[0].x + 1.0);
      ptr->v[0].y = (vpHeight/2.0) * (ptr->v[0].y + 1.0);
      ptr->v[1].x = (vpWidth/2.0)  * (ptr->v[1].x + 1.0);
      ptr->v[1].y = (vpHeight/2.0) * (ptr->v[1].y + 1.0);
      ptr->v[2].x = (vpWidth/2.0)  * (ptr->v[2].x + 1.0);
      ptr->v[2].y = (vpHeight/2.0) * (ptr->v[2].y + 1.0);

      // What about the z-coordinates????
   }
   return;
}
