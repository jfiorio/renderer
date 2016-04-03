#ifndef SCENERENDER_H
#define SCENERENDER_H

#include <windows.h>

#include "Context.h"
#include "Pipeline.h"

#include "P1_Model2World.h"
#include "P2_World2View.h"
#include "P3_BackFaceCulling.h"
#include "P4_Lighting.h"
#include "P5_View2Clip.h"
#include "P6_Clipping.h"
#include "P7_Clip2NDC.h"
#include "P8_NDC2VP.h"
#include "P9_Rasterize.h"
#include "P10_FragmentShaderTexture.h"
#include "P11_Fragments2FB.h"

class SceneRender
{
  private:
  Pipeline *pipeline;
  Context *context;

  public:
  SceneRender();
  void renderScene(Scene* scene, FrameBuffer* fb);
};

//void P11_FragmentOperations(Scene *s);
//void P12_Antialiasing(Scene *s);

#endif /* SCENERENDER_H */
