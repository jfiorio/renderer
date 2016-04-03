#ifndef P11_FRAGMENTS2FB_H
#define P11_FRAGMENTS2FB_H

#include <immintrin.h>
#include "PipelineStage.h"

class P11_Fragments2FB : public PipelineStage
{
  private:
  public:
  P11_Fragments2FB();
  void processTriangles(Context *context);
};

#endif
