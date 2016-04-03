#ifndef P4_LIGHTING_H
#define P4_LIGHTING_H

#include <math.h>
#include "PipelineStage.h"

class P4_Lighting : public PipelineStage
{
  private:
  public:
  P4_Lighting();
  void processTriangles(Context *context);
};

void doSmoothLighting(Scene *scene, List<Triangle> *triangles);
void doFlatLighting(Scene *scene, List<Triangle> *triangles);

#endif