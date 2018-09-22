// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "thread_utils.hxx"
#include "macros.hxx"

void ThreadUtils::runOnAsyncThread(const function<void()>& func, const string& name) {
  // Using cocos2d Async Task Pool to run the function on another thread. This pool uses a queue for task
  // which means only one will be done at a time. Use this mainly for one of tasks.
  CCASSERT(func not_eq nullptr, "ThreadUtils ~ Thread cannot be created with null function");

  // Creating function to run in async pool
  const auto function = bind(
      [&](const string& id) {
        if (!id.empty()) CCLOG("ThreadUtils ~ AsyncTask complete: %s", id.c_str());
      },
      name);

  // Run in async pool
  AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_OTHER, function, nullptr,
                                        [=]() { func(); });
}

void ThreadUtils::runOnArbitraryThread(const function<void()>& func) {
  // Creating an arbitrary thread to execute the function and not interrupt the main thread. Note that
  // creating threads is usually expensive
  CCASSERT(func not_eq nullptr, "Thread cannot be created with null function");
  std::thread t1([=]() { func(); });
  t1.detach();
}
