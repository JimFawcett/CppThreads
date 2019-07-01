///////////////////////////////////////////////////////////////
// Atomics.cpp - atomic operations on integral types         //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <atomic>
#include <mutex>
#include <thread>
#include <iostream>
#include <random>

std::mutex mtx;

void up(std::atomic<int>& i)
{
  srand(time(nullptr));

  for(int j=0; j<20; ++j)
  {
    std::this_thread::sleep_for(std::chrono::microseconds(rand() % 100));
    ++i;  // atomic operation
    std::lock_guard<std::mutex> l(mtx);                     // guard for I/O integrity
    std::cout << "\n  t - value of atomic is " << i.load(); // could use i since guarded
  }
}

int main()
{
  std::cout << "\n  Demonstrating atomic<T>";
  std::cout << "\n =========================\n";

  std::atomic<int> i = 0;
  if (i.is_lock_free())
    std::cout << "\n  atomic<int> is lock free";
  else
    std::cout << "\n  atomic<int> is not lock free";
  std::cout << "\n";

  std::thread t(up, std::ref(i));  // shared count

  srand(time(nullptr));

  for(int j=0; j<8; ++j)
  {
    std::this_thread::sleep_for(std::chrono::microseconds(rand()%1400));
    --i;
    std::lock_guard<std::mutex> l(mtx);
    std::cout << "\n  m - value of atomic is " << i.load();
  }
  t.join();
  std::cout << "\n  value of atomic is " << i;
  std::cout << "\n\n";
}