/////////////////////////////////////////////////////////////////////
// SimpleLocking - Performance of various locking mechanisms       //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016       //
/////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <process.h>
#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <queue>
#include "../SlimLock/SlimLock.h"
#include "../Utilities/Utilities.h"
#include "../Timer/Timer.h"

using Clock = std::chrono::high_resolution_clock;

void Display(const std::string& testType, size_t count, double elapsedTime)
{
  std::cout << std::left;
  std::cout << "\n  " << std::setw(25);
  std::cout << testType.c_str() << " locking ";
  std::cout << count << " times required ";
  std::cout << std::setw(11) << elapsedTime << " seconds";
  std::cout << std::right;
}

void testNoLock(size_t count)
{
  Timer<Clock> timer;
  timer.start();
  size_t i = 0;
  while (i < count)
  {
    ++i;
  }
  Timer<Clock>::duration  elapsedTime = timer.stop();
  Display("No lock", count, elapsedTime.count());
}

void testWin32CriticalSection(size_t count)
{
  CRITICAL_SECTION cs;
  InitializeCriticalSection(&cs);
  Timer<Clock> timer;
  timer.start();
  size_t i = 0;
  while (i < count)
  {
    EnterCriticalSection(&cs);
    ++i;
    LeaveCriticalSection(&cs);
  }
  Timer<Clock>::duration  elapsedTime = timer.stop();
  Display("Win32 critical section", count, elapsedTime.count());
  DeleteCriticalSection(&cs);
}

void testSlimLock(size_t count)
{
  SlimLock lck;
  Timer<Clock> timer;
  timer.start();
  size_t i = 0;
  while (i < count)
  {
    lck.lock();
    ++i;
    lck.unlock();
  }
  Timer<Clock>::duration  elapsedTime = timer.stop();
  Display("SlimLock", count, elapsedTime.count());
}

void testWin32Mutex(size_t count)
{
  HANDLE hMutex = CreateMutex(0, FALSE, 0);
  Timer<Clock> timer;
  timer.start();
  size_t i = 0;
  while (i < count)
  {
    WaitForSingleObject(hMutex, INFINITE);
    ++i;
    ReleaseMutex(hMutex);
  }
  Timer<Clock>::duration  elapsedTime = timer.stop();
  Display("Win32 mutex", count, elapsedTime.count());
  CloseHandle(hMutex);
}

void testCpp11Mutex(size_t count)
{
  std::mutex mtx;
  Timer<Clock> timer;
  timer.start();
  size_t i = 0;
  while (i < count)
  {
    mtx.lock();
    ++i;
    mtx.unlock();
  }
  Timer<Clock>::duration  elapsedTime = timer.stop();
  Display("C++11 std::mutex", count, elapsedTime.count());
}

void testCpp11Unique_Lock(size_t count)
{
  std::mutex mtx;
  std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
  Timer<Clock> timer;
  timer.start();
  size_t i = 0;
  while (i < count)
  {
    lck.lock();
    ++i;
    lck.unlock();
  }
  Timer<Clock>::duration  elapsedTime = timer.stop();
  Display("C++11 unique_lock", count, elapsedTime.count());
}

void testCpp11Lock_Guard(size_t count)
{
  std::mutex mtx;
  Timer<Clock> timer;
  timer.start();
  size_t i = 0;
  while (i < count)
  {
    {
      std::lock_guard<std::mutex> lck(mtx);
      ++i;
    }
  }
  Timer<Clock>::duration  elapsedTime = timer.stop();
  Display("C++11 std::lock_guard", count, elapsedTime.count());
}

void testLock_Guard_SlimLock(size_t count)
{
  SlimLock lck;
  Timer<Clock> timer;
  timer.start();
  size_t i = 0;
  while (i < count)
  {
    {
      std::lock_guard<SlimLock> grd(lck);
      ++i;
    }
  }
  Timer<Clock>::duration  elapsedTime = timer.stop();
  Display("SlimLock in lock_guard", count, elapsedTime.count());
}

class TestWin32
{
  static size_t mainCount;
  static size_t maxCount;
  static size_t waitCount;
  static CRITICAL_SECTION cs;
  static CONDITION_VARIABLE cv;
  static std::queue<int> Q;
  /*
  * threadProc must be static to be used with
  * _beginthread(...)
  */
  static void _cdecl threadProc(void* pInfo)
  {
    waitCount = 0;
    while (mainCount < maxCount)
    {
      EnterCriticalSection(&cs);
      while (Q.size() == 0)
      {
        ++waitCount;
        // wait for main thread to enqueue
        SleepConditionVariableCS(&cv, &cs, INFINITE);
      }
      Q.pop();
      LeaveCriticalSection(&cs);
    }
    std::cout << "\n  leaving threadProc: ";
    std::cout << "waitCount = " << waitCount;
  }

public:
  TestWin32()
  {
    InitializeConditionVariable(&cv);
    InitializeCriticalSection(&cs);
    mainCount = 0;
  }
  static void testWin32ConditionVariable(size_t count)
  {
    HANDLE hThread = (HANDLE)_beginthread(threadProc, 0, NULL);
    SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
    Timer<Clock> timer;
    timer.start();
    maxCount = count;
    while (mainCount < maxCount)
    {
      EnterCriticalSection(&cs);
      Q.push(++mainCount);
      WakeConditionVariable(&cv);
      LeaveCriticalSection(&cs);
      Sleep(0); //yield so child thread runs
    }
    WaitForSingleObject(hThread, INFINITE);
    Timer<Clock>::duration  elapsedTime = timer.stop();
    Display("Win32 condition variable", count, elapsedTime.count());
  }
};

size_t TestWin32::mainCount;
size_t TestWin32::maxCount;
size_t TestWin32::waitCount;
CRITICAL_SECTION TestWin32::cs;
CONDITION_VARIABLE TestWin32::cv;
std::queue<int> TestWin32::Q;

class TestCpp11
{
  size_t mainCount;
  size_t maxCount;
  size_t waitCount;
  std::mutex mtx;
  std::condition_variable cv;
  std::queue<size_t> Q;

  void threadProc()
  {
    waitCount = 0;
    std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
    while (mainCount < maxCount)
    {
      lck.lock();
      while (Q.size() == 0)
      {
        ++waitCount;
        // wait for main thread to enqueue
        cv.wait(lck, [this]() { return Q.size() > 0; });
      }
      Q.pop();
      lck.unlock();
    }
    std::cout << "\n  leaving threadProc: ";
    std::cout << "waitCount = " << waitCount;
  }

public:
  TestCpp11()
  {
    mainCount = 0;
  }
  void TestCpp11ConditionVariable(size_t count)
  {
    maxCount = count;
    std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
    std::thread t(&TestCpp11::threadProc, this);
    SetThreadPriority(t.native_handle(), THREAD_PRIORITY_HIGHEST);
    Timer<Clock> timer;
    timer.start();
    while (mainCount < maxCount)
    {
      lck.lock();
      Q.push(++mainCount);
      lck.unlock();
      cv.notify_one();
      Sleep(0); //yield so child thread runs
    }
    t.join();
    Timer<Clock>::duration  elapsedTime = timer.stop();
    Display("C++11 condition variable", count, elapsedTime.count());
  }
};

using namespace Utilities;
using Utils = StringHelper;

int main()
{
  Utils::Title("Testing Locking Performance");
  putline();

  Utils::title("Testing simple locks");
  testNoLock(100000);
  testWin32CriticalSection(100000);
  testSlimLock(100000);
  testWin32Mutex(100000);
  testCpp11Mutex(100000);
  testCpp11Unique_Lock(100000);
  testCpp11Lock_Guard(100000);
  testLock_Guard_SlimLock(100000);
  putline();

  Utils::title("Testing condition variables");
  TestWin32 test;
  test.testWin32ConditionVariable(100000);
  TestCpp11 testcpp;
  testcpp.TestCpp11ConditionVariable(100000);
  std::cout << "\n\n";
}