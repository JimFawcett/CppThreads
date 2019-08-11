// Timer.cpp

#ifdef TEST_TIMER

#include "Timer.h"
#include "../Utilities/Utilities.h"
#include <windows.h>
#include <iostream>

using Clock = std::chrono::high_resolution_clock;
using namespace Utilities;
using Utils = StringHelper;

int main()
{
  Utils::Title("Testing Timer class");
  putline();

  Timer<Clock> timer;
  timer.start();
  Sleep(100);
  std::chrono::duration<double> elapsedTime = timer.stop();
  timer.start();
  std::cout << "\n  elapsed time for Sleep(100) = ";
  std::cout << elapsedTime.count() << " seconds";
  elapsedTime = timer.stop();
  std::cout << "\n  elapsed time for previous cout = ";
  std::cout << elapsedTime.count() << " seconds";
  std::cout << "\n\n";
}

#endif
