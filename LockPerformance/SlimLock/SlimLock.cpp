// SlimLock.cpp

#include "SlimLock.h"
#include <thread>
#include <mutex>
#include <functional>
#include <string>
#include <iostream>
#include "../Utilities/Utilities.h"

void testAlone()
{
  SlimLock lck;
  std::string shared = "";
  std::function<void(size_t)> threadProc = [&](size_t id) {
    for (size_t i = 1; i < 20; ++i)
    {
      lck.lock();
      shared += Utilities::Converter<size_t>::toString(id);
      lck.unlock();
      Sleep(0);  // yield to allow other thread to run
    }
  };
  std::thread t1(threadProc, 1);
  std::thread t2(threadProc, 2);
  t1.join();
  t2.join();
  std::cout << "\n  shared = " << shared.c_str() << "\n";
}

void testWithLock_Guard()
{
  SlimLock lck;
  std::string shared = "";
  std::function<void(size_t)> threadProc = [&](size_t id) {
    for (size_t i = 1; i < 20; ++i)
    {
      {
        std::lock_guard<SlimLock> grd(lck);
        shared += Utilities::Converter<size_t>::toString(id);
      }
      Sleep(0);  // yield to allow other thread to run
    }
  };
  std::thread t1(threadProc, 1);
  std::thread t2(threadProc, 2);
  t1.join();
  t2.join();
  std::cout << "\n  shared = " << shared.c_str();
}

using namespace Utilities;
using Utils = StringHelper;

int main()
{
  Utils::Title("Testing SlimLock");
  putline();

  Utils::title("testing bare lock");
  testAlone();
  //putline();

  Utils::title("testing wrapped in lock_guard");
  testWithLock_Guard();
  std::cout << "\n\n";
}