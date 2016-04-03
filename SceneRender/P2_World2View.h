#ifndef P2_WORLD2VIEW_H
#define P2_WORLD2VIEW_H

#include "PipelineStage.h"

class P2_World2View : public PipelineStage
{
  private:
  public:
  P2_World2View();
  void processTriangles(Context *context);
};

#endif
