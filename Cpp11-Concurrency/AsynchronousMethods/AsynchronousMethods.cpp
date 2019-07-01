///////////////////////////////////////////////////////////////////////
// AsynchronousMethods.cpp - pass member function to child thread    //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
///////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <thread>

class SayMessage
{
public:
  SayMessage(const std::string& msg = "") : _addendum(msg) {}
  void say(const std::string& msg)
  {
    std::cout << "\n  " << (msg + " - " + _addendum).c_str();
  }
private:
  std::string _addendum;
};

int main()
{
  std::cout << "\n  Running non-static method asynchronously";
  std::cout << "\n ==========================================";
  SayMessage s("an addendum");
  std::thread t1(&SayMessage::say, &s, "my message");
  t1.join();
  std::cout << "\n\n";
}