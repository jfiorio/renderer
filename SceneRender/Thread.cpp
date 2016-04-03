#include "Thread.h"

Thread::Thread(int type)
{
  jobEvent = CreateEvent( 
      NULL,            // default security attributes
      TRUE,            // manual-reset event
      FALSE,           // initial state is nonsignaled
      NULL             // object name
      );
  completionEvent = CreateEvent( 
      NULL,            // default security attributes
      TRUE,            // manual-reset event
      FALSE,           // initial state is nonsignaled
      NULL             // object name
      ); 
  if (type == 0)      
    plThread = thread(&::processJobs, this);

  this->type = type;
  running = true;
}

void Thread::appendJob(Job *job)
{
  jobs.push_back(job);
}

vector<Job*> *Thread::getJobs()
{
  return &jobs;
}

void Thread::processJobs()
{
  if (type == 0)
    SetEvent(jobEvent);
  else if (type == 1)
  {
    running = true;
    ::processJobs(this);
  }
}

void Thread::signalCompletion()
{
  SetEvent(completionEvent);
}

void Thread::waitForJobs()
{
  if (type == 0)
  {
    WaitForSingleObject(jobEvent, INFINITE);  
    ResetEvent(jobEvent);
  }
  else if (type == 1)
    running = false;
}

void Thread::waitForCompletion()
{
  WaitForSingleObject(completionEvent, INFINITE);  
  ResetEvent(completionEvent);
}

void Thread::waitForCompletion(vector<Thread*> threads)
{
  HANDLE completionEvents[16];
  
  int threadCount = threads.size();
  for (int i=0; i<threadCount; i++)
  {
    Thread *thread = threads.back();
    threads.pop_back();
    completionEvents[i] = thread->completionEvent;
  }
  
  WaitForMultipleObjects(threadCount-1, completionEvents+1, TRUE, INFINITE);
  for (int i=0; i<threadCount; i++)
    ResetEvent(completionEvents[i]);
}

bool Thread::isRunning()
{
  return running;
}

void processJobs(Thread *thread)
{
  Job *job;
  vector<Job*> *jobs = thread->getJobs();
  
  while (thread->isRunning())
  {
    if (!jobs->empty())
    {
      job = jobs->back();
      jobs->pop_back();
      job->process();
      
      while(!jobs->empty())
      {
        job = jobs->back();
        jobs->pop_back();
        job->process();
      }
      thread->signalCompletion();
    }
    else
    {
      thread->waitForJobs();
    }
  }
}