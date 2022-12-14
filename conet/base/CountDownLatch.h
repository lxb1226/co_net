#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include <boost/noncopyable.hpp>
#include <mutex>
#include <condition_variable>

namespace conet {

class CountDownLatch : boost::noncopyable {
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
    int getCount() const;

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};
} // namespace conet

#endif // COUNTDOWNLATCH_H