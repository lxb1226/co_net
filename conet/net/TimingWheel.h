#ifndef TIMINGWHEEL_H
#define TIMINGWHEEL_H

// todo

#include "Timer.h"
#include <chrono>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "ITimerQueue.h"
#include "Processor.h"
#include "TimerId.h"

namespace burger {
    namespace net {

        constexpr int TVR_BITS = 8;            // 第一个轮占的位数
        constexpr int TVR_SIZE = 1 << TVR_BITS;// 第一个轮的长度
        constexpr int TVN_BITS = 6;            // 第n个轮占的位数
        constexpr int TVN_SIZE = 1 << TVN_BITS;// 第n个轮的长度
        constexpr int HIERARCHY_NUM = 4;       // 一共有5个层级 1一个主 4个从
        constexpr int MAX_SLOT = 256 + HIERARCHY_NUM * 64;
        // 掩码:取模或整除用
        constexpr int TVR_MASK = TVR_SIZE - 1;
        constexpr int TVN_MASK = TVN_SIZE - 1;
        constexpr int OFFSET(int n) {
            return TVR_SIZE + n * TVN_SIZE;
        }

        constexpr uint32_t INDEX(uint64_t v, int n) {
            return ((v >> (TVR_SIZE + n * TVN_BITS)) & TVN_MASK);
        }


        class Processor;
        class TimerId;
        class TimerWheel {
        public:
            TimerWheel(uint16_t interval, uint64_t currTime) : lastTime_(currTime), currTime_(0),
                                                               interval_(interval), remainder_(0), timerFd_(burger::net::detail::createTimerfd()) {
            }

            TimerId addTimer(Coroutine::ptr co, Timestamp when, double interval = 0);

            TimerId addTimer(TimerCallback cb, const std::string &name, Timestamp when, double interval = 0);

        private:
            bool insert(TimerPtr timer);
            void tick();

            using TimersArray = std::array<std::list<TimerPtr>, MAX_SLOT>;
            using FreeNode = std::deque<std::list<TimerPtr>>;
            using TimersMap = std::unordered_map<uint64_t, std::list<TimerPtr>>;
            TimersArray timerNodes{}; // 维护5层时间轮的数组
            FreeNode freeTimerNodes{};// 已经删除的定时器结点队列
            TimersMap allTimers{};    // 保存所有的结点
            uint64_t nextId_{0};      // 下一个可用的句柄
            uint64_t lastTime_;       // 上一次的时间毫秒
            uint64_t currTime_;       // 当前的tick
            uint16_t interval_;       // 每个时间点的毫秒时间
            uint16_t remainder_;      // 剩余的毫秒

            Processor *proc_;
            const int timerFd_;
            int32_t cascadeTime(int32_t nOff, int32_t nIndex);
        };

        using TimeWheelPtr = std::shared_ptr<TimerWheel>;

        inline uint64_t getNowTimestamp() {
            using namespace std::chrono;
            auto now = system_clock::now().time_since_epoch();
            return static_cast<uint64_t>(duration_cast<milliseconds>(now).count());
        }

    }// namespace net

}// namespace burger


#endif// TIMINGWHEEL_H