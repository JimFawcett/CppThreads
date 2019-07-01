/////////////////////////////////////////////////////////////////////////
// Stoppable.cpp - run function asynchronously, passing a stop signal  //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
/////////////////////////////////////////////////////////////////////////

#include "Stoppable.h"
#include "../Utilities/Utilities.h"
#include <iostream>
#include <chrono>

using namespace std::literals;

//----< application defined function to run asynchronously >-------------

void compute(std::atomic<bool>& doStop)
{
  std::cout << "\n  compute() has started running";
  for (size_t i = 0; i < 100; ++i)
  {
    if (doStop.load())  // atomic operation
    {
      std::cout << "\n  compute() is stopping";
      break;
    }
    std::cout << "\n  still running";
    std::this_thread::sleep_for(5ms);
  }
}

//----< demonstrate Stoppable operation >--------------------------------

using Fptr = void(std::atomic<bool>&);
using Utils = Utilities::StringHelper;

int main()
{
  Utils::Title("Demonstrating Stoppable Thread");

  Stoppable<Fptr> stopper;
  std::thread t = stopper.asyncRun(compute);
  std::cout << "\n  main thread will now sleep for 50 milliseconds";
  std::this_thread::sleep_for(50ms);
  std::cout << "\n  main thread is about to issue stop request";
  stopper.stop();
  t.join();
  std::cout << "\n\n";
}