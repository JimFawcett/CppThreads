///////////////////////////////////////////////////////////////
// CallOnce.cpp - a singleton call                           //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <mutex>
#include <iostream>

void one()
{
  std::cout << "\n  this is one()";
}

void two()
{
  std::cout << "\n  this is two()";
}
void calls()
{
  static std::once_flag beenCalled;
  std::call_once(beenCalled, one);
  two();
}

int main()
{
  for(int i=0; i<5; ++i)
    calls();
  std::cout << "\n\n";
}