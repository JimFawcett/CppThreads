#pragma once
/////////////////////////////////////////////////////////////////////
// Task.h - Enqueue work item to run on a threadpool thread        //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016       //
/////////////////////////////////////////////////////////////////////
/*
 * Task<numThreads> class runs a callable object asynchronously.  It does 
 * that using a static ThreadPool member.
 *
 * Task is a template class with parameter the number of threads to 
 * start in the ThreadPool instance.  Siince the ThreadPool is static
 * all Tasks with the same template parameter value share the same set
 * of threadpool threads.
 *
 * Task<numThreads> takes its callable objects by value, so they are
 * copied or moved into the queue.  That means that you don't have to
 * worry about the source object going out of scope or changing its
 * parameters before the threadpool executes it.  If you really want
 * to pass the callable object by reference you can do that with
 * std::ref(co).  But do that carefully.
 */
/*
 * ToDo:
 * - Provide a way to run a long-running task on a separate thread
 *   that exits as soon as the task completes.
 */
#include <functional>
#include <memory>
#include "../ThreadPool/ThreadPool.h"

template<size_t numThreads = 8>
class Task
{
public:
  using CallObj = std::function<bool()>;

  Task() {}
  Task(const Task&) = delete;
  Task& operator=(const Task&) = delete;

  void workItem(CallObj co);
  void wait();
  std::mutex& mutex();
  static ThreadPool<numThreads>& threadPool() { return trpl_; }
private:
  static ThreadPool<numThreads> trpl_;
};

template<size_t numThreads>
ThreadPool<numThreads> Task<numThreads>::trpl_;  // declare static member

template<size_t numThreads>
std::mutex& Task<numThreads>::mutex() { return trpl_.mutex(); }

template<size_t numThreads>
void Task<numThreads>::workItem(CallObj co)
{
  trpl_.workItem(co);
}

template<size_t numThreads>
void Task<numThreads>::wait()
{
  trpl_.wait();
}

