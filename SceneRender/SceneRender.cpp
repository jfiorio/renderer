#include "SceneRender.h"

#include "PipelineMultithreaded.h"
#define THREAD_COUNT 4

SceneRender::SceneRender()
{
  context = new Context();
  pipeline = new PipelineMultithreaded(THREAD_COUNT);
  
  // Apply the model transformation
  pipeline->appendStage(new P1_Model2World());
  // Apply the view transformation
  pipeline->appendStage(new P2_World2View());
  // Delete back facing triangles
  pipeline->appendStage(new P3_BackFaceCulling());
  // Compute vertex shading
  pipeline->appendStage(new P4_Lighting());
  // Apply the projection transformation
  pipeline->appendStage(new P5_View2Clip());
  // Clip triangles to the standard volume
  pipeline->appendStage(new P6_Clipping());
  // Transform to normalized device coordinates
  pipeline->appendStage(new P7_Clip2NDC());
  // Transform normalized device coordinates to viewport coordinates
  pipeline->appendStage(new P8_NDC2VP());
  // Rasterize each triangle into fragments
  pipeline->appendStage(new P9_Rasterize());
  // Shade each fragment of each triangle
  pipeline->appendStage(new P10_FragmentShaderTexture());
  // Use the fragments to fill in the viewport
  pipeline->appendStage(new P11_Fragments2FB());
}

void SceneRender::renderScene(Scene *scene, FrameBuffer *fb)
{
  Scene *renderedScene = new Scene(scene);  // Duplicate the Scene object,
                                            // so that we can modify the copy.
  context->set(renderedScene, fb);                                          
  pipeline->process(context);
  
  delete renderedScene;  // Note: ~Scene() calls ~Triangle() wich calls ~Fragment()
}//renderScene()