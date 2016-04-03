#include "Pipeline.h"

Pipeline::Pipeline()
{

}

void Pipeline::appendStage(PipelineStage *stage)
{
  stages.push_back(stage);
}

void Pipeline::process(Context *context)
{
  for (list<PipelineStage*>::iterator i = stages.begin(); i != stages.end(); i++)
  {
    PipelineStage *stage = *i;
    stage->processTriangles(context);
  }
}
