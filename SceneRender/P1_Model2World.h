#ifndef P1_MODEL2WORLD_H
#define P1_MODEL2WORLD_H

#include "PipelineStage.h"

class P1_Model2World : public PipelineStage
{
  private:
  public:
  P1_Model2World();
  void processTriangles(Context *context);
};

#endif
