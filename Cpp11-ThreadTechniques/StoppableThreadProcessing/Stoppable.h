#pragma once
///////////////////////////////////////////////////////////////////////
// Stoppable.h - run function asynchronously, passing a stop signal  //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
///////////////////////////////////////////////////////////////////////

#include <thread>
#include <atomic>
#include <functional>

///////////////////////////////////////////////////////////////////////
// Stoppable class runs callable object asynchronously
// - Callable Object must accept an atomic<bool> stop signal

template <typename CallObj>
class Stoppable
{
public:
  std::thread asyncRun(CallObj& co);
  void stop();
private:
  std::atomic<bool> doStop = false;
};
//----< run callable object on child thread >--------------------------

template <typename CallObj>
std::thread Stoppable<CallObj>::asyncRun(CallObj& co)
{
  // lambda acquires callable object co and 
  // member field doStop by reference

  std::function<void()> f = [&]()
  {
    co(doStop);
  };
  std::thread t(f);
  return t;
}
//----< signal stop by changing value of doStop to true >--------------

template <typename CallObj>
void Stoppable<CallObj>::stop()
{
  doStop.store(true);  // this is an atomic operation
}