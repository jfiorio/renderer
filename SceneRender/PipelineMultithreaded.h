#ifndef PIPELINEMULTITHREADED_H
#define PIPELINEMULTITHREADED_H

#include <immintrin.h>

#include "Pipeline.h"
#include "PipelineJob.h"
#include "Thread.h"

class PipelineMultithreaded : public Pipeline
{
  private:
  vector<Thread*> threads;
  int threadCount; 
  
  public:
  PipelineMultithreaded(int threadCount);
  ~PipelineMultithreaded();
  void process(Context *context);
  
  static void distribute(PipelineStage *stage, int attribs, Context *&context, Context *&contexts, PipelineJob *jobs, int jobCount);
   
  static Context *fork(PipelineStage *stage, int attribs, Context *context, int jobCount, int flag=0);
  static Context *distributeTriangles(Context *context, int jobCount, int flag=0);
  static Context *distributeFragments(Context *context, int jobCount, int flag=0);
};

typedef struct
{
  union
  {
    __m128i data;
    unsigned short values[8];
  };
} __m128s;

#endif