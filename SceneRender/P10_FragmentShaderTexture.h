#ifndef P10_FRAGMENTSHADERTEXTURE_H
#define P10_FRAGMENTSHADERTEXTURE_H

#include <immintrin.h>
#include "PipelineStage.h"

class P10_FragmentShaderTexture : public PipelineStage
{
  public:
  P10_FragmentShaderTexture();
  void processTriangles(Context *context);
};

#endif
