#pragma once
// Timer.h

#include <chrono>

/*
*  T is expected to be either:
*    std::chrono::system_clock or std::chrono::high_resolution_clock
*/
template <typename T>
class Timer
{
public:
  using time_point = std::chrono::time_point<T>;
  using duration = std::chrono::duration<double>;

  void start();
  duration stop();
private:
  time_point _start;
  time_point _end;
};

template <typename T>
void Timer<T>::start()
{
  _start = T::now();
}

template <typename T>
typename Timer<T>::duration Timer<T>::stop()
{
  _end = T::now();
  return _end - _start;
}
