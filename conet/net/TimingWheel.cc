//
// Created by heyjude on 2022/7/22.
//

#include "TimingWheel.h"

using namespace burger::net;

TimerId TimerWheel::addTimer(burger::Coroutine::ptr co, burger::Timestamp when, double interval) {
    Timer::ptr timer = std::make_shared<Timer>(co, proc_, when, interval);

    insert(timer);

    return {timer, timer->getSeq()};
}


TimerId TimerWheel::addTimer(TimerCallback cb, const std::string &name, burger::Timestamp when, double interval) {
    Timer::ptr timer = std::make_shared<Timer>(cb, name, proc_, when, interval);

    insert(timer);


    return {timer, timer->getSeq()};
}

bool TimerWheel::insert(TimerPtr timer) {

    uint64_t expireTime = timer->getExpiration().microSecondsSinceEpoch();

    // 计算应该在哪个slot上
    uint32_t slotIdx = 0;
    uint64_t delay = expireTime - currTime_;
    if (delay < TVR_SIZE) {
        // 第一层的索引为低8位的值
        slotIdx = expireTime & TVR_MASK;
    } else if (delay < (1 << (TVR_BITS + TVN_BITS))) {
        slotIdx = OFFSET(0) + INDEX(expireTime, 0);
    } else if (delay < (1 << (TVR_BITS + 2 * TVN_BITS))) {
        slotIdx = OFFSET(1) + INDEX(expireTime, 1);
    } else if (delay < (1 << (TVR_BITS + 3 * TVN_BITS))) {

        slotIdx = OFFSET(2) + INDEX(expireTime, 2);
    } else if (delay < 0) {
        slotIdx = currTime_ & TVR_MASK;
    } else {
        slotIdx = OFFSET(3) + INDEX(expireTime, 3);
    }

    auto head = timerNodes[slotIdx];
    head.push_back(timer);

    return false;
}
void TimerWheel::tick() {
    uint64_t nowTime = getNowTimestamp();
    while (currTime_ < nowTime) {
        int32_t nIdx = currTime_ & TVR_MASK;
        // 当前时间低8位如果为0，则把外层时间轮上的时间结点向内层转移
        if (!nIdx && !cascadeTime(OFFSET(0), INDEX(currTime_, 0)) && !cascadeTime(OFFSET(1), INDEX(currTime_, 1)) && !cascadeTime(OFFSET(2), INDEX(currTime_, 2))) {

            cascadeTime(OFFSET(3), INDEX(currTime_, 3));
        }

        auto nodeLists = timerNodes[nIdx];
        while (!nodeLists.empty()) {
            auto timer = nodeLists.front();
            nodeLists.pop_front();

            assert(timer->getProcessor() != nullptr);
            if (timer->getCo() != nullptr) {
                timer->getProcessor()->addTask(timer->getCo());
            } else {
                timer->getProcessor()->addPendingTask(timer->getCb(), timer->getName());
            }

            if (timer->getInterval() > 0) {
                Timestamp newTs = Timestamp::now() + timer->getInterval();
                timer->setExpiration(newTs);
                if (timer->getCo() != nullptr) {
                    timer->setCo(timer->getProcessor()->resetAndGetCo(timer->getCo()->getCallback(), "repeat"));
                } else {
                    timer->setCo(std::make_shared<Coroutine>(timer->getCb()));
                }
                insert(timer);
            }
        }
    }

    Timestamp ts = Timestamp::now();
    detail::resetTimerfd(timerFd_, ts);
}


int32_t TimerWheel::cascadeTime(int32_t nOff, int32_t nIndex) {
    int32_t slotIdx = nOff + nIndex;

    auto list = timerNodes[slotIdx];
    while (!list.empty()) {
        auto node = list.front();
        list.pop_back();
        insert(node);
    }

    return nIndex;
}