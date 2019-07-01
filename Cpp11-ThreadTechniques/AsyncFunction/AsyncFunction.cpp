/////////////////////////////////////////////////////////////////////////
// AsyncFunction.cpp - runs function asynchronously then runs callback //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
/////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "AsyncFunction.h"
#include "../Utilities/Utilities.h"

//----< test function for asyncRun >-------------------------------------
void hello()
{
  std::cout << "\n  Hello OOD!";
  std::cout << "\n  in hello function thread id = " << std::this_thread::get_id();
}
//----< test callback >--------------------------------------------------

std::string goodbye()
{
  std::cout << "\n  in goodbye function thread id = " << std::this_thread::get_id();
  return "goodbye";
}

/////////////////////////////////////////////////////////////////////////
// Demonstrating asyncRun with and without callback and
// Demonstrating asyncRunWithResult

using Utils = Utilities::StringHelper;

int main()
{
  Utils::Title("Demonstrating Asynchronous Function Wrappers");

  Utilities::putline();
  Utils::title("asyncRun wrapper with no callback");

  std::cout << "\n  main thread id = " << std::this_thread::get_id();

  std::thread t1 = asyncRun(hello, nullFunction);
  t1.join();

  Utilities::putline();
  Utils::title("asyncRun wrapper with callback");

  std::cout << "\n  main thread id = " << std::this_thread::get_id();

  // define callback for asyncRun

  std::string msg = "this is a callback";
  std::function<void()> cb = [msg]()
  {
    std::cout << "\n  " << msg;
    std::cout << "\n  in callback thread id = " << std::this_thread::get_id();
  };

  std::thread t2 = asyncRun(hello, cb);
  
  // do useful work here while hello runs

  t2.join();   // blocks when there is noting more to do

  Utilities::putline();
  Utils::title("asyncRunWithResult wrapper with callback accepting result");

  std::cout << "\n  main thread id = " << std::this_thread::get_id();

  // define callback for asyncRunWithResult

  std::function<void(const std::string&)> cbwr = [](const std::string& msg)
  {
    std::cout << "\n  callback processing message \"" << msg << "\" returned from async function";
    std::cout << "\n  in callback thread id = " << std::this_thread::get_id();
  };

  std::thread t3 = asyncRunWithReturn(goodbye, cbwr);
  t3.join();

  std::cout << "\n\n";
}