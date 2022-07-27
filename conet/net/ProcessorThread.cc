#include "ProcessorThread.h"
#include "Processor.h"
using namespace conet;
using namespace conet::net;

ProcessThread::ProcessThread(Scheduler *scheduler) : scheduler_(scheduler) {}

ProcessThread::~ProcessThread() { thread_.join(); }

Processor *ProcessThread::startProcess() {
  assert(!thread_.joinable());
  thread_ = std::thread{&ProcessThread::threadFunc, this};
  Processor *proc = nullptr;
  // 等待proc创建完毕
  // TODO:类似于muduo里面的做法
  {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return proc_ != nullptr; });
    proc = proc_;
  }
  return proc;
}

void ProcessThread::join() { thread_.join(); }

void ProcessThread::threadFunc() {
  Processor proc(scheduler_);
  {
    std::lock_guard<std::mutex> lock(mutex_);
    proc_ = &proc;
    cv_.notify_one();
  }
  proc.run();

  std::lock_guard<std::mutex> lock(mutex_);
  proc_ = nullptr;
}
