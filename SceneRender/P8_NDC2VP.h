#ifndef P8_NDC2VP_H
#define P8_NDC2VP_H

#include "PipelineStage.h"

class P8_NDC2VP : public PipelineStage
{
  private:
  public:
  P8_NDC2VP();
  void processTriangles(Context *context);
};

#endif