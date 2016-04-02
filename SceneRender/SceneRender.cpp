#include <stdio.h>
#include <stdlib.h>
#include "SceneRender.h"


#if DEBUG
void printMatrices(Scene *s);
void printVertices(Scene *s, int stageFlag, int stageNo);
#endif /* DEBUG */


void renderScene(Scene *scene, FrameBuffer *fb)
{
   Scene *scene2 = new Scene(scene);  // Duplicate the Scene object,
                                      // so that we can modify the copy.
/* NOTE: We could make it the client's responsibility
         to copy the scene. In the case of OpenGL,
         the client "duplicates" the scene when it
         passes all of the scene information into the
         pipeline, and so all of the information gets
         copied into the graphics card's memory.
*/

   #if DEBUG
     printMatrices(scene2);
   #endif

   #if DEBUG
      printVertices(scene2, 1, 0);
   #endif

   P1_Model2World(scene2); // apply the model transformation

   #if DEBUG
      printVertices(scene2, 2, 1);
   #endif

   P2_World2View(scene2);  // apply the view transformation

   #if DEBUG
      printVertices(scene2, 4, 2);
   #endif

   P3_BackFaceCulling(scene2); // delete back facing triangles

   #if DEBUG
      printVertices(scene2, 8, 3);
   #endif

   P4_Lighting(scene2);    // compute vertex shading

   #if DEBUG
      printVertices(scene2, 16, 4);
   #endif

   P5_View2Clip(scene2);   // apply the projection transformation

   #if DEBUG
      printVertices(scene2, 32, 5);
   #endif

   P6_Clipping(scene2);    // clip triangles to the standard volume

   #if DEBUG
      printVertices(scene2, 64, 6);
   #endif

   P7_Clip2NDC(scene2);    // transform to normalized device coordinates

   #if DEBUG
      printVertices(scene2, 128, 7);
   #endif

   //P8_Switch2OpenGL(scene2); // let OpenGL finish the pipeline

   P8_NDC2VP(scene2);    // transform normalized device coordinates to viewport coordinates

   #if DEBUG
      printVertices(scene2, 256, 8);
   #endif

   P9_Rasterize(scene2);   // rasterize each triangle into fragments

   #if DEBUG
      printVertices(scene2, 512, 9);
   #endif

   P10_FragmentShaderTexture(scene2);  // shade each fragment of each triangle

   #if DEBUG
      printVertices(scene2, 1024, 10);
   #endif

   P11_Fragments2FB(scene2, fb);    // use the fragments to fill in the viewport

   delete scene2;  // Note: ~Scene() calls ~Triangle() wich calls ~Fragment()

   return;
}//renderScene()


#if DEBUG
void printMatrices(Scene *scene)
{
   /* display the matrices stored in the Scene object */
   int printMatrices = 0;
   char * MatrixString = getenv("PrintMatrices");
   if (MatrixString != NULL)
   {
      printMatrices = atoi(MatrixString); // 1 => print model matrix
   }                                      // 2 => print view matrix
                                          // 4 => print projection matrix
   if (printMatrices & 1)
   {
      scene->model.print();
   }
   if (printMatrices & 2)
   {
      scene->view.print();
   }
   if (printMatrices & 4)
   {
      scene->projection.print();
   }
}//printMatrices()


void printVertices(Scene *scene, int stageFlag, int stageNo)
{
   int debugStages = 0;
   char * DebugString;
   if ( (DebugString = getenv("PipeLine0")) != NULL ) debugStages += 1;
   if ( (DebugString = getenv("PipeLine1")) != NULL ) debugStages += 2;
   if ( (DebugString = getenv("PipeLine2")) != NULL ) debugStages += 4;
   if ( (DebugString = getenv("PipeLine3")) != NULL ) debugStages += 8;
   if ( (DebugString = getenv("PipeLine4")) != NULL ) debugStages += 16;
   if ( (DebugString = getenv("PipeLine5")) != NULL ) debugStages += 32;
   if ( (DebugString = getenv("PipeLine6")) != NULL ) debugStages += 64;
   if ( (DebugString = getenv("PipeLine7")) != NULL ) debugStages += 128;
   if ( (DebugString = getenv("PipeLine8")) != NULL ) debugStages += 256;
   if ( (DebugString = getenv("PipeLine9")) != NULL ) debugStages += 512;
   if (debugStages == 0) debugStages = 128; // default to NDC

   int printFields = 1;  // default to printing out vertex data
   char * PrintString = getenv("PrintLevel");
   if (PrintString != NULL)
   {
      printFields = atoi(PrintString); // 1 => print vertex data
   }                                   // 2 => print color data
                                       // 4 => print texture coordinates
                                       // 8 => print normal vector
   if (stageFlag & debugStages)
   {
      fprintf(stderr, "================= Stage P%d begin ===========\n", stageNo);
      scene->print(printFields);
      fprintf(stderr, "================= Stage P%d end =============\n", stageNo);
   }
}
#endif /* DEBUG */
