#pragma once
// SlimLock.h

#include <windows.h>
#include <process.h>
#pragma warning (disable : 4800)

class SlimLock
{
public:
  SlimLock();
  void lock();
  void unlock();
  bool trylock();
  ~SlimLock();
private:
  CRITICAL_SECTION cs;
};

inline SlimLock::SlimLock() { InitializeCriticalSection(&cs); }
inline void SlimLock::lock() { EnterCriticalSection(&cs); }
inline void SlimLock::unlock() { LeaveCriticalSection(&cs); }
inline bool SlimLock::trylock() 
{ 
  return static_cast<bool>(TryEnterCriticalSection(&cs)); 
}
inline SlimLock::~SlimLock() { DeleteCriticalSection(&cs); }