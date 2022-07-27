#include "conet/net/EventLoop.h"
#include <iostream>
/**
 * 主线程创建两个EventLoop对象
 * 负面测试
 */

using namespace conet;
using namespace conet::net;


int main() {
    EventLoop loop1;
    EventLoop loop2;
    return 0;
}