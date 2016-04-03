#ifndef PIPELINE_H
#define PIPELINE_H

#include <list>
using namespace std;

#include "PipelineStage.h"

class Pipeline
{
  protected:
  list<PipelineStage*> stages;
  
  public:
  Pipeline();
  void appendStage(PipelineStage *stage);
  virtual void process(Context *context);
};

#endif