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
  printf(" %s[%i].data = { %f, %f, %f, %f, %f, %f, %f, %f }\n", #x, i, \
  x[i].data[0], x[i].data[1], x[i].data[2], x[i].data[3], \
  x[i].data[4], x[i].data[5], x[i].data[6], x[i].data[7]); \
}

#define printVect(x) \
  printf(" %s.data = { %f, %f, %f, %f, %f, %f, %f, %f }\n", #x, \
  x.data[0], x.data[1], x.data[2], x.data[3], \
  x.data[4], x.data[5], x.data[6], x.data[7]); 

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
} fpoint;

typedef struct
{
  double xj;
  double yi;
} fpoint_ji;

typedef struct
{
  double xi;
  double yj;
} fpoint_ij;

typedef struct
{
  union
  {
    ALIGN16 __m256d data;
    fpoint values[2];
    fpoint_ji values_ji[2];
    fpoint_ij values_ij[2];
  };
} avx_fpoint2;

#endif