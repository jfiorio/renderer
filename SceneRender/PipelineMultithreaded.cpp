#include "PipelineMultithreaded.h"

#include <stdio.h>

PipelineMultithreaded::PipelineMultithreaded(int threadCount)
{
  this->threadCount = threadCount;
  for (int i=0; i<threadCount-1; i++)
    threads.push_back(new Thread(0));
  threads.push_back(new Thread(1)); // last thread = main thread/dummy thread
}

PipelineMultithreaded::~PipelineMultithreaded()
{
  for (int i=0; i<threadCount; i++)
  {
    Thread *thread = threads.back();
    threads.pop_back();
    delete thread;
  }
}

void PipelineMultithreaded::process(Context *context)
{
  Context *contexts;
  PipelineJob jobs[threadCount];
  for (int i=0; i<threadCount; i++)
    jobs[i] = PipelineJob(stages);
  
  // Distribute workload according to the first stage
  // that reads the Scene data
  PipelineStage *stage = stages.front();
  distribute(stage, STAGE_ATTRIB_FIRST, context, contexts, jobs, threadCount);
    
  while (stage) // While pipeline stages remain
  {
    // [Re-]Schedule threads
    for (int i=0; i<threadCount; i++)
    {
      Thread *thread = threads[i];
      thread->appendJob(&jobs[i]);
      thread->processJobs();
      stage = jobs[i].getStage();
    }
    
    // Wait for all other threads to complete their part
    Thread::waitForCompletion(threads);
    
    // If a stage is returned, the pipeline has been interrupted there;
    // we've stopped because the workload must be reevaluated 
    if (stage)
      distribute(stage, STAGE_ATTRIB_WRITE, context, contexts, jobs, threadCount); 
  }
  
  /* 
     TODO: Reclaiming triangles and their fragments is out of place here.
           Normally this would be done when a scene is deleted.
           We must iterate through the main triangle list to return each existing triangle back
           to its respective sub-pool, but before this can be done, the triangle 'sub-lists' need 
           to be re-merged to reform the main list. Also, these sub-pools can only reclaim those 
           triangles -before- they are re-merged to reform the main triangle pool. Thus, in some
           cases we cannot fork or join a context all at once, particularly before or after its
           associated triangles (which are also the triangles of the associated scene) are returned.
           This means that, for now, our PipelineMultithreaded class is forced the responsibilty of 
           returning.
  */
  context = Context::join(contexts, threadCount, 4);
  for (ListNode<Triangle> *ptr = context->triangles->head; ptr;)
  {
    ListNode<Triangle> *next = ptr->next;
    if (ptr->fragmentPool) { 
      if (ptr->fragments.size) ptr->fragmentPool->reclaim(&ptr->fragments); 
    }
    ptr->trianglePool->reclaimNode(ptr);
    ptr = next;
  }
  context = Context::join(contexts, threadCount, 2);
}

/* 
   Accordingly, under the condition that the specified 'stage'  has the specified 'attribs', 
   distribute the workload specified by and resources supplied by the specified input 'context'
   among 'count' output sub'contexts'; then assign each of the 'count' specified 'jobs' a different 
   one of these subcontexts. If the specified 'attribs' do not imply that the specified stage will 
   ultimately be the first to process these jobs, then it is assumed that prior stages have already 
   done some work to manipulate the contexts returned by a prior call to this function, and thus 
   that 'contexts' acts as an input for those manipulated contexts; these contexts are then recombined 
   to form the altered 'context', prior to being redistributed evenly among the same 'contexts'
   (therefore, 'context' also acts as an output.) 
*/
   
void PipelineMultithreaded::distribute    (PipelineStage *stage, 
                                                    int attribs, 
                                              Context *&context, 
                                             Context *&contexts,
                                              PipelineJob *jobs, 
                                                   int jobCount)
{
  // First stage?
  if (attribs & STAGE_ATTRIB_FIRST)
    contexts = fork(stage, STAGE_ATTRIB_READ, context, jobCount);   
  else if (stage->getAttributes() & STAGE_ATTRIB_SERIAL) // Single-threaded?
  {
    context = Context::join(contexts, jobCount, 1);
    stage->processTriangles(context);
    contexts = fork(stage, attribs, context, jobCount, 1);
  }
  else
  {
    // Redistribute the workload according to next stage
    context = Context::join(contexts, jobCount);
    contexts = fork(stage, attribs, context, jobCount);
  }
  
  for (int i=0; i<jobCount; i++)
    jobs[i].context = &contexts[i];
}


Context *PipelineMultithreaded::fork(PipelineStage *stage, int attribs, Context *context, int jobCount, int flag)
{
  int stageAttribs = stage->getAttributes() & attribs;
  if (stageAttribs & STAGE_ATTRIB_TRIANGLE_LIST)
    return distributeTriangles(context, jobCount);
  else if (stageAttribs & STAGE_ATTRIB_FRAGMENT_LIST)
    return distributeFragments(context, jobCount);
  else return 0;
}

/* Evenly divide the workload/triangles in the scene into jobCount jobs */
Context *PipelineMultithreaded::distributeTriangles(Context *context, int jobCount, int flag)
{
  return Context::fork(context, jobCount, flag);
}

/* Evenly divide the workload based on cumulative fragment counts */
Context *PipelineMultithreaded::distributeFragments(Context *context, int jobCount, int flag)
{ 
  Context *contexts = Context::fork(context, jobCount, 2|flag);
  for (int i=0; i<jobCount; i++)
    contexts[i].triangles->reset();
    
  __attribute__ ((aligned(32))) __m128i min_t;
  __attribute__ ((aligned(32))) __m128s counts;
    
  counts.data = _mm_set_epi16(-1, -1, -1, -1, 0, 0, 0, 0); // -1 = max unsigned short
  for (ListNode<Triangle> *ptr = context->triangles->head; ptr;)
  {
    ListNode<Triangle> *next = ptr->next;
    min_t = _mm_minpos_epu16(counts.data);
    int min = min_t[0] >> 16;
    counts.values[min] += (ptr->fragments.size >> 5);
    contexts[min].triangles->add(ptr);
    ptr = next;
  }
  return contexts;
}