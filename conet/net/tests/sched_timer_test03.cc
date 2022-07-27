#include "conet/net/Scheduler.h"
#include "conet/net/TimerId.h"
#include "conet/base/Log.h"
#include "conet/base/Util.h"

#include <unistd.h>
#include <iostream>
#include <functional>

using namespace conet;
using namespace conet::net;

// todo : timer 实验03

int cnt = 0;
Scheduler* g_sched;

void print(const std::string& msg) {
    std::cout << "msg " << msg << " : " << Timestamp::now().toFormatTime() << std::endl;
    if(++cnt == 3) {
        g_sched->stop();
    }
}

void stopSched() {
    g_sched->stop();
}

int main() {
    LOGGER(); LOG_LEVEL_TRACE;

    Scheduler sched;
    g_sched = &sched;
    sched.startAsync();
    std::cout << "main" << std::endl;

    sched.runEvery(1, std::bind(print, "every1"));

    sched.wait();
    
    return 0;
}