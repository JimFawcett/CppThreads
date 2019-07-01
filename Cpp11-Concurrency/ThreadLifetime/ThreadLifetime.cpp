///////////////////////////////////////////////////////////////
// ThreadLifetime.cpp - to join() or not to join()           //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <thread>
#include <iostream>
#include <Windows.h>

void Hello(const std::string& msg)
{
  for(int i=0; i<5; ++i)
    std::cout << "\n  " << msg.c_str();
}
int main()
{
  std::cout << "\n  Demonstrating thread lifetime issues";
  std::cout << "\n ======================================\n";

  try {
    // Never runs - no problem when t1 leaves scope
    std::thread t1;

    // not joined or detached - Debug assert when leaving scope even if finished
    std::thread t2(Hello, "hello from t2");
    
    // joined - no problem when leaving scope
    std::thread t3(Hello, "hello from t3");
    t3.join();

    ////wait on handle, not joined or detached - Debug assert when leaving scope
    //std::thread t4(Hello, "hello from t4");
    //HANDLE hT4 = t4.native_handle();  // Debug assert when leaving scope
    //WaitForSingleObject(hT4, INFINITE);

    // detached - no problem when leaving scope
    std::thread t5(Hello, "hello from t5");
    HANDLE hT5 = t5.native_handle(); 
    WaitForSingleObject(hT5, INFINITE);
    t5.detach();
  }
  catch (std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
  }
  std::cout << "\n\n";
  return 0;
}