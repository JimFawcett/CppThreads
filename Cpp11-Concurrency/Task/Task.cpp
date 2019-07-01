///////////////////////////////////////////////////////////////
// Task.cpp - getting answers from thread running task       //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <future>
#include <thread>
#include <iostream>

std::string getAnAnswer() { return "42"; }

class anotherAnswer {
public:
  std::string getAnotherAnswer() { return "answer is still 42"; }
};

int main()
{
  std::cout << "\n  Demonstrating packaged_task";
  std::cout << "\n =============================\n";

  std::packaged_task<std::string()> task(getAnAnswer);
  std::future<std::string> fut = task.get_future();
  std::thread t(std::move(task));
  std::cout << "\n  The answer is: " << fut.get().c_str();
  t.detach();
  
  anotherAnswer aa;
  std::packaged_task<std::string()> task2(std::bind(&anotherAnswer::getAnotherAnswer, aa));
  std::future<std::string> f2 = task2.get_future();
  task2();
  std::cout << "\n  " << f2.get().c_str();

  std::cout << "\n\n";
  return 0;
}