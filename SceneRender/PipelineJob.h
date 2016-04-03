/** 
   A pipeline job consists of 
   - a list of "tasks" to be performed (in sequence)
   - a "context" in which those tasks are to be performed
   
   Each task corresponds to a pipeline stage. A job's context 
   comprises the input structure that is sent through each stage,
   and includes a list of triangles that those stages are to 
   manipulate.
   
   The purpose of this organization is to allow triangles in 
   the scene to be subdivided among several individual "jobs".
   Alternatively, for a single render, we could send each triangle 
   in the scene through each stage of the pipeline, in a single 
   context. However, this restricts things to being done in a 
   serial manner: with no division of the workload, only a single
   thread/core is able to process each triangle, for each stage of
   the pipeline.
   
   By subdividing the workload into individual jobs, each job can 
   be sent through the pipeline and processed concurrently, in its 
   own thread. 
 **/

#ifndef PIPELINEJOB_H
#define PIPELINEJOB_H

#include <list>
using namespace std;

#include "Job.h"
#include "PipelineStage.h"

class PipelineJob : public Job
{
  private:
  list<PipelineStage*> stages;
  PipelineStage *stage; // current stage
  
  public:
  Context *context;
  PipelineJob() {};
  PipelineJob(list<PipelineStage*> stages);
  void process();
  PipelineStage *getStage();
};

#endif
