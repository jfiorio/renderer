#include "P11_Fragments2FB.h"

P11_Fragments2FB::P11_Fragments2FB() : PipelineStage()
{
  attributes |= (STAGE_ATTRIB_READ_FRAGMENT_LIST /*| STAGE_ATTRIB_READ_FRAGMENT_COLOR */);
}

void P11_Fragments2FB::processTriangles(Context *context)
{
  // Walk the list of Triangle objects and for each triangle,
  // walk its list of Fragment objects. For each fragment, use
  // the fragment's z-coordinate to determine if the fragment
  // should be written to the viewport within the frambuffer.

  FrameBuffer *fb = context->fb;
  for (ListNode<Triangle> *ptr = context->triangles->head; ptr; ptr = ptr->next)
  {
    for (ListNode<Fragment> *fptr = ptr->fragments.head; fptr; fptr = fptr->next)
    {
      // get this frament's z-coordinate
      float z = fptr->zCoord;
      // get this fragment's x and y index into the viewport
      int x = fptr->xIndex;
      int y = fptr->yIndex;
      // get the z-buffer value currently at that pixel of the viewport
      float zBuff = fb->getDepthVP(x, y);
      // if the current fragment is in front of what is in the framebuffer,
      // then overwrite the data in the framebuffer
      if (z < zBuff)
      {
        //unsigned char c[3];
        //c[0] = (fptr->r); // convert from float to unsigned char
        //c[1] = (fptr->g);
        //c[2] = (fptr->b); /* not yet using the alpha channel */
        fb->setPixelVP(x, y, fptr->pixel, z);
//      fb->setPixelFB(x, y, c, z);
      }
    }
  }
  return;
}
