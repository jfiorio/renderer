#include <stdio.h>
#include "SceneLib.h"
#include "FrameBuffer.h"

void P11_Fragments2FB(Scene* scene, FrameBuffer* fb)
{
   // Walk the list of Triangle objects and for each traingle,
   // walk its list of Fragment objects. For each fragment, use
   // the fragment's z-coordinate to determine if the fragment
   // should be written to the viewport within the frambuffer.

   for (TriangleListNode *ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      //int fragmentCounter = 0;
      for (Fragment *fptr = ptr->t->fragment_p; fptr; fptr = fptr->nextFragment)
      {
         //fragmentCounter++;
         // get this frament's z-coordinate
         double z = fptr->zCoord;
         // get this fragment's x and y index into the viewport
         int x = fptr->xIndex;
         int y = fptr->yIndex;
         // get the z-buffer value currently at that pixel of the viewport
         double zBuff = fb->getDepthVP(x, y);
         // if the current fragment is in front of what is in the framebuffer,
         // then overwrite the data in the framebuffer
         if (z < zBuff)
         {
            unsigned char c[3];
            /***** WHY DO THE NEXT THREE LINES COMPILE??? *******/
            c[0] = fptr->r;
            c[1] = fptr->g;  // this is wrong
            c[2] = fptr->b;
            c[0] = (unsigned char)(255*(fptr->r)); // convert from double to unsigned char
            c[1] = (unsigned char)(255*(fptr->g));
            c[2] = (unsigned char)(255*(fptr->b)); /* not yet using the alpha channel */
            fb->setPixelVP(x, y, c, z);
//          fb->setPixelFB(x, y, c, z);
         }
      }
      //fprintf(stderr, "fragmentCounter = %d\n", fragmentCounter);fflush(stderr);
   }
   return;
}
