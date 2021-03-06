///////////////////////////////////////////////////////////////////////
// CppThreadWithCallback.cpp - Using callbacks with C++11 Threads    //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

#include <thread>
#include <mutex>
#include <iostream>
#include <sstream>
#include <string>

template<typename T>
std::string toString(T t)
{
  std::ostringstream out;
  out << t;
  return out.str();
}
template<typename CB>
void threadProc(CB callback, size_t count)
{
  static std::mutex mtx;
  for (size_t i = 0; i < 5; ++i)
  {
    std::string msg;
    {
      std::lock_guard<std::mutex> lck(mtx);
      msg = "\n  child thread #" + toString(count) + " working";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << msg;
  }
  callback();
}

void callbackFunction()
{
  std::cout << "\n  another callback";
}
template<typename T>
void joinAll(T first)
{
  if(first.joinable())
    first.join();
}
template<typename T, typename... Args>
void joinAll(T first, Args... args)
{
  if(first.joinable())
    first.join(); 
  joinAll(std::move(args...));
}

int main()
{
  std::cout << "\n  Demonstrating C++11 Thread with Callback";
  std::cout << "\n ==========================================";

  auto callback = []() { std::cout << "\n  calling back"; };

  std::thread t1(threadProc<decltype(callback)>, callback, 1);
  std::thread t2(threadProc<decltype(callbackFunction)>, callbackFunction, 2);

  joinAll(std::move(t1), std::move(t2));

  // using joinAll(...);
  // instead of:
  // t1.join();
  // t2.join();

  std::cout << "\n\n";
  return 0;
}

