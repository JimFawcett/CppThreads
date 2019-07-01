///////////////////////////////////////////////////////////////
// ThreadParameters.cpp - passing parameters to threads      //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <thread>
#include <mutex>
#include <iostream>

void Title(const std::string& title)
{
  std::cout << "\n  " << title.c_str();
  std::cout << "\n " << std::string(title.size() + 2, '=').c_str();
  std::cout.flush();
}

void Hello(size_t id)
{
  static std::mutex mtx;
  for(int i=0; i<5; ++i)
  {
    mtx.lock();
    std::cout << "\n  hello world from thread " << id;
    mtx.unlock();
  }
}

int main()
{
  Title("Demonstrating Thread Parameters");

  std::thread t1(Hello, 1);
  std::thread t2(Hello, 2);
  std::thread t3(Hello, 3);
  t1.join();
  t2.join();
  t3.join();
  std::cout << "\n\n";
}