#ifndef P6_CLIPPING_H
#define P6_CLIPPING_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "PipelineStage.h"

class P6_Clipping : public PipelineStage
{
  private:
  public:
  P6_Clipping();
  void processTriangles(Context *context);
};

void interpolateNewVertex(Vertex* v_outside, Vertex* v_inside, Vertex* v_new, int eqn_number);
void interpolateNewTriangle(Vertex* v0_in, Vertex* v1_out, Vertex* v2_out, Triangle* tp, Context* context, int);
void interpolateNewTriangles(Vertex* v0_out, Vertex* v1_in, Vertex* v2_in, Triangle* tp, Context* context, int);

#endif