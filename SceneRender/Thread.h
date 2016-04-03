#ifndef THREAD_RENDERER_H
#define THREAD_RENDERER_H

#include <windows.h>
#include <thread>
#include <vector>
using namespace std;

#include "Job.h"

class Thread
{
  private:
  thread plThread;
  bool running;
  HANDLE jobEvent;
  HANDLE completionEvent;
  vector<Job*> jobs;
  
  public:
  Thread(int type);
  bool isRunning();
  int type;
  
  void appendJob(Job *job);
  vector<Job*> *getJobs();
  //...
  void processJobs();
  void signalCompletion();
  void waitForJobs();
  void waitForCompletion();

  static void waitForCompletion(vector<Thread*> threads);
};

void processJobs(Thread *thread);

#endif