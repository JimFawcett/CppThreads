///////////////////////////////////////////////////////////////////////
// QueuedWorkItems.cpp - child thread processes enqueued work items  //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
///////////////////////////////////////////////////////////////////////
/*
 * A single child thread processes work items equeued by main thread
 */

#include <string>
#include <iostream>
#include <conio.h>
#include "QueuedWorkItems.h"
#include "../Utilities/Utilities.h"

//----< demonstrate ProcessWorkItem class >----------------------------

using Util = Utilities::StringHelper;
using WorkResult = std::string;

int main()
{
  Util::Title("Enqueued Work Items");

  std::cout << "\n  main thread id = " << std::this_thread::get_id();

  ProcessWorkItem<WorkResult> processor;
  processor.start();

  // define 1st work item

  WorkItem<WorkResult> wi1 = []() {
    std::cout << "\n  working on thread " << std::this_thread::get_id();
    return "Hello from wi1";
  };

  processor.doWork(&wi1);

  // define 2nd work item

  WorkItem<WorkResult> wi2 = []()
  {
    std::cout << "\n  working on thread " << std::this_thread::get_id();
    size_t sum = 0;
    for (size_t i = 0; i < 100000; ++i)
      sum += i;
    return "wi2 result = " + Utilities::Converter<size_t>::toString(sum);
  };

  processor.doWork(&wi2);

  // the following calls to result() block until results are enqueued

  std::cout << "\n  " << processor.result();
  std::cout << "\n  " << processor.result();
  processor.doWork(nullptr);
  
  // wait for child thread to complete

  processor.wait();
  std::cout << "\n\n";
}