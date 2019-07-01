///////////////////////////////////////////////////////////////
// Future.cpp - asynchronous call                            //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <future>
#include <mutex>
#include <iostream>

size_t say(const std::string& msg, int millisec)
{
  static std::mutex mtx;  // same mutex shared by all callers
  size_t count = 0;
  for(int i=0; i<25; ++i)
  {
    mtx.lock();
    std::cout << "\n  " << msg.c_str();
    ++count;
    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
  }
  return count;
}

int main()
{
  std::cout << "\n  Demonstrating async with future";
  std::cout << "\n =================================\n";

  // futures give us a thread-safe way to retrieve 
  // computed values from a child thread

  auto fut = std::async(say, "hello from async", 1);
  size_t mainCount = say("hello from main", 0);

  size_t childCount = fut.get();  // blocking call

  std::cout << std::endl;
  std::cout << "\n  child thread wrote message " << childCount << " times";
  std::cout << "\n   main thread wrote message " << mainCount << " times";

  std::cout << "\n  type of future is " << typeid(fut).name();

  // doesn't assert with no wait, but background doesn't complete
  std::cout << "\n\n";
}