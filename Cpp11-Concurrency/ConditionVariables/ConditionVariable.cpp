///////////////////////////////////////////////////////////////
// ConditionVariable.cpp - Blocking Queue                    //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <iostream>
#include <sstream>

template <typename T>
class BlockingQueue {
public:
  BlockingQueue() {}
  T deQ();
  void enQ(const T& t);
  size_t size();
private:
  BlockingQueue(const BlockingQueue<T>&);
  BlockingQueue<T>& operator=(const BlockingQueue<T>&);
  std::queue<T> q_;
  std::mutex mtx_;
  std::condition_variable cv_;
};

template<typename T>
T BlockingQueue<T>::deQ()
{
  std::unique_lock<std::mutex> l(mtx_);
  if(q_.size() > 0)
  {
    T temp = q_.front();
    q_.pop();
    return temp;
  }
  // loop needed due to spurious returns from wait
  while (q_.size() == 0)
    cv_.wait(l, [this] () { return q_.size() > 0; });
  T temp = q_.front();
  q_.pop();
  return temp;
}

template<typename T>
void BlockingQueue<T>::enQ(const T& t)
{
  {
    std::lock_guard<std::mutex> l(mtx_);
    q_.push(t);
  }
  cv_.notify_one();
}

template<typename T>
size_t BlockingQueue<T>::size()
{
  std::lock_guard<std::mutex> l(mtx_);
  return q_.size();
}

std::mutex ioLock;

void test(BlockingQueue<std::string>* pQ)
{
  std::string msg;
  do
  {
    msg = pQ->deQ();
    {
      std::lock_guard<std::mutex> l(ioLock);
      std::cout << "\n  thread deQed " << msg.c_str();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  } while(msg != "quit");
}

int main()
{
  BlockingQueue<std::string> q;
  std::thread t(test, &q);
  for(int i=0; i<25; ++i)
  {
    std::ostringstream temp;
    temp << i;
    std::string msg = std::string("msg#") + temp.str();
    {
      std::lock_guard<std::mutex> l(ioLock);
      std::cout << "\n   main enQing " << msg.c_str();
    }
    q.enQ(msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  t.join();
}