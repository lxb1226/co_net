#include "conet/net/Scheduler.h"
#include "conet/net/TimerId.h"
#include "conet/base/Log.h"

#include <unistd.h>
#include <iostream>


using namespace conet;
using namespace conet::net;

// todo : timer 实验

void test() {
    std::cout << "test timer" << std::endl;
}



int main() {
    LOGGER(); LOG_LEVEL_DEBUG;
    Scheduler sched;
    sched.startAsync();
    TimerId timerid = sched.runEvery(2, test, "timerTest");
    sleep(7);
    sched.cancel(timerid);
    sched.stop();
    sched.wait();
    return 0;
}