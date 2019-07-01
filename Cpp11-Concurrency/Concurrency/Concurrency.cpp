///////////////////////////////////////////////////////////////
// Concurrency.cpp - illustration of concurrent activities   //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <Windows.h>

void Title(const std::string& title)
{
  std::cout << "\n  " << title.c_str();
  std::cout << "\n " << std::string(title.size() + 2, '=').c_str();
  std::cout.flush();
}

void threadProc(std::string& shared, std::string id, size_t sleepTime)
{
  static std::mutex mtx;
  for(int i=0; i<10; ++i)
  {
    mtx.lock();
    shared += id;
    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
  }
}

int main()
{
  Title("Concurrency");

  std::string shared;
  try {
    std::thread t1(threadProc, std::ref(shared), "1", 100);
    std::thread t2(threadProc, std::ref(shared), "2", 200);
    std::thread t3(threadProc, std::ref(shared), "3", 100);

    // get native handles and wait on them
    HANDLE handle[3];
    handle[0] = t1.native_handle();
    handle[1] = t2.native_handle();
    handle[2] = t3.native_handle();
    ::WaitForMultipleObjects(3, handle, true, INFINITE);

    std::cout << "\n  " << shared.c_str() << "\n\n";

    // You must either join or detach each thread
    // otherwise abort is called when program exits scope

    if (t1.joinable())
      t1.detach();
    if (t2.joinable())
      t2.detach();
    if (t3.joinable())
      t3.detach();
  }
  catch (std::exception& exc)
  {
    std::cout << "\n  " << exc.what() << "\n\n";
  }
  return 0;
}