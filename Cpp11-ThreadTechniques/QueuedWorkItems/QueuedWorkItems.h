#pragma once
///////////////////////////////////////////////////////////////////////
// QueuedWorkItems.h - child thread processes enqueued work items    //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
///////////////////////////////////////////////////////////////////////
/*
* A single child thread processes work items equeued by main thread
*/

#include <thread>
#include <functional>
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"

template<typename Result>
using WorkItem = std::function<Result()>;

///////////////////////////////////////////////////////////////////////
// class to process work items
// - each work item is processed sequentially on a single child thread

template<typename Result>
class ProcessWorkItem
{
public:
  void start();
  void doWork(WorkItem<Result>* pWi);
  Result result();
  void wait();
  ~ProcessWorkItem();
private:
  std::thread* _pThread;
  BlockingQueue<WorkItem<Result>*> _workItemQueue;
  BlockingQueue<Result> _resultsQueue;
};
//----< wait for child thread to terminate >---------------------------

template<typename Result>
void ProcessWorkItem<Result>::wait()
{
  _pThread->join();
}
//----< enqueue work item >--------------------------------------------

template<typename Result>
void ProcessWorkItem<Result>::doWork(WorkItem<Result>* pWi)
{
  _workItemQueue.enQ(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result>
Result ProcessWorkItem<Result>::result()
{
  return _resultsQueue.deQ();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result>
void ProcessWorkItem<Result>::start()
{
  std::function<void()> threadProc =
    [&]() {
    while (true)
    {
      WorkItem<Result>* pWi = _workItemQueue.deQ();
      if (pWi == nullptr)
      {
        std::cout << "\n  shutting down work item processing";
        return;
      }
      Result result = (*pWi)();
      _resultsQueue.enQ(result);
    }
  };
  _pThread = new std::thread(threadProc);
}
//----< clean up heap >------------------------------------------------

template<typename Result>
ProcessWorkItem<Result>::~ProcessWorkItem()
{
  delete _pThread;
}


