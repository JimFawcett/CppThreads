// ThreadPool.cpp

#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include "ThreadPool.h"

//----< test stub >--------------------------------------------------

#ifdef TEST_THREADPOOL

#include <iostream>
using namespace std::chrono_literals;
using namespace Utilities;
using Utils = StringHelper;

int main()
{
  Show::attach(&std::cout);
  Show::start();
  DebugLog::attach(&std::cout);
  DebugLog::start();

  Utils::Title("Testing ThreadPool");

  ThreadPool<8> trpl;

  ThreadPool<8>::CallObj co = [&trpl]() ->bool { 
    Show::write(
      "\n  hello from thread " + 
      Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id())
    ); 
    return true;
  };

  for (size_t i = 0; i < 20; ++i)
    trpl.workItem(co);

  ThreadPool<8>::CallObj exit = []() ->bool { return false; };
  trpl.workItem(exit);
  trpl.wait();

  //std::cout << "\n\n";
}

#endif
