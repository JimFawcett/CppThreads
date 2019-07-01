///////////////////////////////////////////////////////////////////////
// Notifier.cpp - Enable child thread to notify parent thread        //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Notifier.h"
#include "../Utilities/Utilities.h"

//----< function to run asynchronously >-------------------------------

void compute(INotifier<size_t>* pNotifier)
{
  for (size_t i = 0; i < 100; ++i)
    if (i % 10 == 0)
      pNotifier->invokeEventProcessing(i);
}

///////////////////////////////////////////////////////////////////////
// Demonstration of notifier

using Utils = Utilities::StringHelper;

int main()
{
  Utils::Title("Demonstrating Notification from Child Thread");

  class NotifyObject : public INotify<size_t>
  {
    virtual void doNotify(size_t reportedValue)
    {
      std::cout << "\n  notified that value is " << reportedValue;
    }
  } nObj;

  using Fptr = void(*)(INotifier<size_t>*);  // matches compute function
    
  Notifier<Fptr, size_t> notifier;
  notifier.reg(&nObj);
  std::thread t = notifier.asyncRun(compute);

  // do other useful work here

  t.join();
  std::cout << "\n\n";
}