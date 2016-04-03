#ifndef P5_VIEW2CLIP_H
#define P5_VIEW2CLIP_H

#include "PipelineStage.h"

class P5_View2Clip : public PipelineStage
{
  private:
  public:
  P5_View2Clip();
  void processTriangles(Context *context);
};

#endif