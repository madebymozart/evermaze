/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __THREAD_UTILS_HXX__
#define __THREAD_UTILS_HXX__

/**
 * Includes
 */
#include "globals.hxx"

class ThreadUtils {
 public:
  /**
   * Runs a function on an async thread using the AsyncTaskPool.
   *
   * @param func ~ Function to run.
   * @param name ~ Name of method run for logging.
   *
   * @note Using this method will put the functions in a queue, which blocked until the function ahead of it
   * is finished. If you need multiple functions to be run at the same time on different threads although, the
   * cost of created a thread is high.
   */
  static void runOnAsyncThread(const function<void()>& func, const string& name = "");

  /**
   * Runs a function on an arbitrary thread. Allows multiple tasks to be run in parallel
   *
   * @param func ~ Function that will be run on another thread
   */
  static void runOnArbitraryThread(const function<void()>& func);

 private:
  /**
   * __DISALLOW_IMPLICIT_CONSTRUCTORS__
   */
  __DISALLOW_IMPLICIT_CONSTRUCTORS__(ThreadUtils)
};

#endif  // __THREAD_UTILS_HXX__
