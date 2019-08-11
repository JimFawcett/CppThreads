#pragma once
/////////////////////////////////////////////////////////////////////
// ThreadPool.h - Enqueue work items to run on one of a collection //
//                of threads, created at startup.                  //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016       //
/////////////////////////////////////////////////////////////////////
/*
* Threadpool is a template class, parameterized on the number of
* threads to start.  Each instance with the same parameter value
* shares a mutex used to synchronize access to data shared between
* threads.
*
* Each thread deQs workItems from a blocking queue of callable objects,
* executes the callable object, and goes back to the queue for another.
* The workItems must take no arguments and return a bool.  The value
* of the bool should always be true unless the user wants to shut down
* the threadpool.  If a job needs input parameters, they are simply
* included in the callable object's capture list (lambda) or as member
* data (functor).
*
* The thread pool can be shut down by enqueuing a work item that returns
* false.  Each thread executes the work item and if false encqueus the 
* same workItem and then exits.  This means that each thread will terminate.
*/
/*
 * ToDo:
 * - Provide a longRunningOperation option that starts another thread
 *   and terminates when long running operation completes.
 */
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"

using Show = StaticLogger<0>;
using DebugLog = StaticLogger<1>;

template <size_t numThreads>
class ThreadPool
{
public:
  using CallObj = std::function<bool()>;

  ThreadPool();
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  void workItem(CallObj& co);
  void wait();
  std::mutex& mutex();
  ~ThreadPool();
private:
  BlockingQueue<CallObj> Q_;
  std::vector<std::thread> threads_;
  std::mutex mtx_;    // lock shared by all threads in pool
  std::function<void()> threadProc_;
};

template<size_t numThreads>
std::mutex& ThreadPool<numThreads>::mutex() { return mtx_; }

//----< define threadProc and start threads >------------------------

template <size_t numThreads>
ThreadPool<numThreads>::ThreadPool()
{
  threadProc_ = [this]()  // all threads use this to acquire their callable objects.
  {
    while (true)
    {
      CallObj co = Q_.deQ();
      if (!co())
      {
        Q_.enQ(co);
        break;
      }
    }
    DebugLog::write(
      "\n  thread " +
      Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id()) +
      " terminating"
      );
  };
  for (size_t i = 0; i < numThreads; ++i)
  {
    std::thread t(threadProc_);
    DebugLog::write(
      "\n--starting threadpool thread " +
      Utilities::Converter<std::thread::id>::toString(t.get_id())
      );
    threads_.push_back(std::move(t));
  }
}

template <size_t numThreads>
void ThreadPool<numThreads>::workItem(CallObj& co)
{
  Q_.enQ(co);
  DebugLog::write(
    "\n--threadpool queue size = " +
    Utilities::Converter<size_t>::toString(Q_.size())
  );
}

template <size_t numThreads>
void ThreadPool<numThreads>::wait()
{
  DebugLog::write(
    "\n  entering wait with queue size = " +
    Utilities::Converter<size_t>::toString(Q_.size())
    );
  for (auto& thrd : threads_)
    thrd.join();
  Q_.clear();
  DebugLog::write(
    "\n--leaving wait with queue size = " +
    Utilities::Converter<size_t>::toString(Q_.size())
    );
}

template<size_t numThreads>
ThreadPool<numThreads>::~ThreadPool()
{
  for (auto& thrd : threads_)
  {
    if (thrd.joinable())
      thrd.join();
  }
}
