#include "conet/base/Coroutine.h"
#include "conet/base/Log.h"
#include <algorithm>
#include <thread>
#include <vector>

using namespace conet;

void runInCo() {
  INFO("RUN IN coroutine begin");
  Coroutine::Yield();
  INFO("RUN IN coroutine end");
  Coroutine::Yield();
}

void testCo() {
  INFO("main begin -1");
  {
    INFO("main begin");
    Coroutine::ptr co = std::make_shared<Coroutine>(runInCo);
    co->resume();
    INFO("main after resume");
    co->resume();
    INFO("main after end");
    co->resume();
  }
  INFO("main after end2");
}

int main() {
  LOGGER();
  LOG_LEVEL_DEBUG;

  testCo();

  return 0;
}