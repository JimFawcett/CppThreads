#pragma once
///////////////////////////////////////////////////////////////////////
// AsyncFunction.h - runs function asynchronously then runs callback //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
///////////////////////////////////////////////////////////////////////
/*
* This package provides two function wrappers that run synchronous
* functions asynchronously and then run supplied callback functions.
*/

#include <thread>

///////////////////////////////////////////////////////////////////////
// asyncRun function 
// - runs any callable object asynchronously
// - then runs callback callable object on same child thread
// - In a typical application, fun may be a function defined
//   in a library while callback is defined by the application

template <typename CallObj, typename CallBackObj>
std::thread asyncRun(CallObj fun, CallBackObj callback)
{
  std::function<void(CallObj co, CallBackObj cb)> runnable = [](CallObj fun, CallBackObj callback)
  {
    fun();
    callback();
  };
  std::thread t(runnable, fun, callback);
  return t;
}

///////////////////////////////////////////////////////////////////////
// asyncRunWithResult
// - runs any callable object fun with return value asynchronously
// - callback takes fun's return as an argument and runs

template <typename CallObj, typename CallBackObj>
std::thread asyncRunWithReturn(CallObj fun, CallBackObj callback)
{
  std::function<void(CallObj co, CallBackObj cb)> runnable = [](CallObj fun, CallBackObj callback)
  {
    callback(fun());
  };
  std::thread t(runnable, fun, callback);
  return t;
}

///////////////////////////////////////////////////////////////////////
// use nullFunction as callback to run functions asynchronously 
// with no callback

inline void nullFunction() {}

 
