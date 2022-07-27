#include "conet/net/Scheduler.h"
#include "conet/base/Log.h"
#include "conet/base/Util.h"

using namespace conet; 
using namespace conet::net;

int main() {
    // todo : sigleton error
    // auto sched = Singleton<Scheduler>::Instance();
    LOGGER(); LOG_LEVEL_TRACE;
    auto sched = util::make_unique<Scheduler>();
    sched->addTask([]() {
        DEBUG("before sleep");
        sleep(5);
        DEBUG("after sleep");
    }, "sleep func");
    sched->start();
    return 0;
}