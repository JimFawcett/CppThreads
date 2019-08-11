/////////////////////////////////////////////////////////////////////
// Task.cpp - Enqueue work item to run on a threadpool thread      //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016       //
/////////////////////////////////////////////////////////////////////

#include "Task.h"
#include "../Utilities/Utilities.h"
#include "../Logger/Logger.h"
#include <sstream>
#include <chrono>

using DebugLog = StaticLogger<1>;
using Show = StaticLogger<0>;

//----< test stub >--------------------------------------------------

#ifdef TEST_TASK

#include <iostream>

using namespace std::chrono_literals;
using namespace Utilities;
using Utils = StringHelper;

int main()
{
  Utils::Title("Testing Task Class");
  Show::attach(&std::cout);
  Show::start();
  DebugLog::attach(&std::cout);
  //DebugLog::start();

  Task<8> tsk;

  Task<8>::CallObj co = [&tsk]() ->bool { 
    Show::write(
      "\n  hello from thread " + 
      Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id())
    );
    std::this_thread::sleep_for(5ms);
    return true;
  };

  for (size_t i = 0; i < 20; ++i)
  {
    tsk.workItem(co);
  }
  Task<8>::CallObj exit = []() ->bool { return false; };
  tsk.workItem(exit);
  tsk.wait();
}

#endif
