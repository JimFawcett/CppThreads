#pragma once
///////////////////////////////////////////////////////////////////////
// Notifier.h - Enable child thread to notify parent thread          //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
///////////////////////////////////////////////////////////////////////

#include <vector>
#include <functional>
#include <thread>

///////////////////////////////////////////////////////////////////////
// Interface INotify used for application specific event handling

template <typename EventArgs>
struct INotify
{
  virtual void doNotify(EventArgs ea) = 0;
  virtual ~INotify(){}
};

///////////////////////////////////////////////////////////////////////
// Interface INotifier used by application processing function
// run on child thread in asyncRun

template <typename EventArgs>
struct INotifier
{
  virtual void invokeEventProcessing(EventArgs arg) = 0;
  virtual void reg(INotify<EventArgs>* pNotify) = 0;
  virtual ~INotifier() {}
};

///////////////////////////////////////////////////////////////////////
// Notifier class provides facilities to:
// - run application specific callable object ca in asyncRun(ca)
// - register event Handler objects that derive from INotify
// - invoke notifications when called from callable object ca

template <typename CallObj, typename EventArgs>
class Notifier : public INotifier<EventArgs>
{
public:
  void invokeEventProcessing(EventArgs ea);
  std::thread asyncRun(CallObj ca);
  void reg(INotify<EventArgs>* pNotify);
private:
  std::vector<INotify<EventArgs>*> _notified;
};

template <typename CallObj, typename EventArgs>
void Notifier<CallObj, EventArgs>::reg(INotify<EventArgs>* pINotify)
{
  _notified.push_back(pINotify);
}

template <typename CallObj, typename EventArgs>
void Notifier<CallObj, EventArgs>::invokeEventProcessing(EventArgs ea)
{
  for (auto pNotify : _notified)
    pNotify->doNotify(ea);
}

template <typename CallObj, typename EventArgs>
std::thread Notifier<CallObj, EventArgs>::asyncRun(CallObj co)
{
  std::function<void()> runner = [this, co]()
  {
    co(this);
  };
  std::thread t(runner);
  return t;
}