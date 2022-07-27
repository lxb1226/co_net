#include "TimerId.h"


using namespace conet;
using namespace conet::net;

TimerId::TimerId() :
    seq_(0) {
}

TimerId::TimerId(std::shared_ptr<Timer> timer, uint64_t seq) :
    timer_(timer),
    seq_(seq) {
}




