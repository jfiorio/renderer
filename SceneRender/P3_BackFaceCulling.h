#ifndef P3_BACKFACECULLING_H
#define P3_BACKFACECULLING_H

#include "PipelineStage.h"

class P3_BackFaceCulling : public PipelineStage
{
  private:
  public:
  P3_BackFaceCulling();
  void processTriangles(Context *context);
};

bool triangleIsFrontFacing(bool frontFacingIsCCW, Triangle *t);
bool triangleIsCCW(Triangle *t);

#endif