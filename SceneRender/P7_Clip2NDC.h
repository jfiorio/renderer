#ifndef P7_CLIP2NDC_H
#define P7_CLIP2NDC_H

#include "PipelineStage.h"

class P7_Clip2NDC : public PipelineStage
{
  private:
  public:
  P7_Clip2NDC();
  void processTriangles(Context *context);
};

#endif