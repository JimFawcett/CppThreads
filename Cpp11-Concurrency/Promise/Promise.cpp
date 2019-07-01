///////////////////////////////////////////////////////////////
// Promise.cpp - getting answers from thread                 //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <functional>
#include <future>
#include <thread>
#include <string>
#include <iostream>

void getAnswer(std::promise<std::string>& p)
{
  std::cout << "\n  thread is thinking";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  p.set_value("The answer is 42");
}
void getAnotherAnswer(std::promise<std::string>& p)
{
  std::cout << "\n  thread is thinking";
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  p.set_value("The answer still is 42");
}

int main()
{
  std::cout << "\n  Demonstrate use of Promise - essentially what async does";
  std::cout << "\n ==========================================================\n";

  std::promise<std::string> p1;
  std::future<std::string> f1(p1.get_future());
  std::cout << "\n  main is waiting for meaning of the Universe";
  std::thread t1(getAnswer, std::ref(p1));
  std::cout << "\n  " << f1.get() << "\n";
  t1.detach();

  std::promise<std::string> p2;
  std::future<std::string> f2 = p2.get_future();
  std::cout << "\n  main is waiting for a better answer";
  std::thread t2(getAnotherAnswer, std::ref(p2));
  std::cout << "\n  " << f2.get();
  t2.detach();
  std::cout << "\n\n";
}