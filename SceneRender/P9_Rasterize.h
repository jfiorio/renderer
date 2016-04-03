#ifndef P9_RASTERIZE_H
#define P9_RASTERIZE_H

#include <immintrin.h>
#include "PipelineStage.h"

#ifndef ALIGN16
#define ALIGN16 __attribute__ ((aligned(32)))
#endif

#define printVectL(x) \
for (int i=0; i<3; i++) \
{ \
  printf(" %s[%i].data = { %f, %f, %f, %f }\n", #x, i, \
  x[i].data[0], x[i].data[1], x[i].data[2], x[i].data[3]); \
}

#define printVect(x) \
  printf(" %s.data = { %f, %f, %f, %f }\n", #x, \
  x.data[0], x.data[1], x.data[2], x.data[3]); 

class P9_Rasterize : public PipelineStage
{
  private:
  public:
  P9_Rasterize();
  void processTriangles(Context *context);
};

typedef struct
{
  double x;
  double y;
} dpoint;

typedef struct
{
  double xj;
  double yi;
} dpoint_ji;

typedef struct
{
  double xi;
  double yj;
} dpoint_ij;

typedef struct
{
  union
  {
    ALIGN16 __m256d data;
    dpoint values[2];
    dpoint_ji values_ji[2];
    dpoint_ij values_ij[2];
  };
} avx_fpoint2;

#endif