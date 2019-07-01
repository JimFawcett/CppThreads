/////////////////////////////////////////////////////////////////
// LockGuard.cpp - ensures unlocking in exception environment  //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013   //
/////////////////////////////////////////////////////////////////

#include <mutex>
#include <thread>
#include <iostream>

void test(std::mutex& mtx)
{
  for(int i=0; i<5; ++i)
  {
    {
      std::lock_guard<std::mutex> guard(mtx);  // guards integrity of I/O
      std::cout << "\n  in " << __FUNCTION__;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
int main()
{
  std::cout << "\n  Demonstrating lock_guard";
  std::cout << "\n ==========================\n";

  std::mutex mtx;  // need to use shared mutex

  std::thread t(test, ref(mtx));  // thread ctor copies args
  for(int i=0; i<5; ++i)
  {
    {
      std::lock_guard<std::mutex> guard(mtx);  // guards integrity of I/O
      std::cout << "\n  in " << __FUNCTION__;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
  t.join();
  std::cout << "\n\n";
}