# C++11基于协程和reactor的高性能Linux服务器框架

## 特征

- 实现了协程，以及协程调度模块，且在其之上实现了高性能Linux服务器框架
- 实现了基于多线程的reactor两种模型的高性能Linux服务器框架
- 实现了现代版的C++11接口，利用C++11新的特性
- 在协程模式中能够用同步的方式表现出异步的特性，简化编码的逻辑
- 封装了spdlog库，更加方便高效地利用高性能日志
- 封装了mysql C api，使得mysql使用更加简单
- 实现了可自动扩容、任务盗取的线程池
- 利用最小堆和timerfd实现了定时器以及实现了基于时间轮的定时器
- hook了系统底层和socket相关的API、socket io相关的API等

## 构建

```shell
sudo apt install g++ cmake make libboost-all-dev mysql-server libmysqlclient-dev libcurl4-openssl-dev libbenchmark-dev
git clone https://github.com/lxb1226/co_net.git
cd co_net

mkdir build && cmake ..
make
make install

```

## 示例

### 协程echo server

EchoServer.h

```cpp
#include "conet/base/Log.h"
#include "conet/net/CoTcpServer.h"

using namespace conet;
using namespace conet::net;

class EchoServer {
public:
  EchoServer(Scheduler *sched, const InetAddress &listenAddr);
  void start();

private:
  void connHandler(const CoTcpConnection::ptr &conn);

private:
  CoTcpServer server_;
};

```

EchoServer.cpp

```cpp
#include "EchoServer.h"

#include "conet/net/RingBuffer.h"

using namespace std::placeholders;

EchoServer::EchoServer(Scheduler *sched, const InetAddress &listenAddr)
    : server_(sched, listenAddr, "EchoServer") {
  server_.setConnectionHandler(std::bind(&EchoServer::connHandler, this, _1));
}

void EchoServer::start() { server_.start(); }

void EchoServer::connHandler(const CoTcpConnection::ptr &conn) {
  RingBuffer::ptr buffer = std::make_shared<RingBuffer>();
  DEBUG("conn process");
  while (conn->recv(buffer) > 0) {
    DEBUG("process process");
    conn->send(buffer);
  }
}
```

main.cpp

```c++
#include "EchoServer.h"
#include "conet/base/Log.h"
#include "conet/net/InetAddress.h"
#include "conet/net/Scheduler.h"

using namespace conet;
using namespace conet::net;

int main() {
  LOGGER();
  LOG_LEVEL_DEBUG;

  Scheduler sched;

  InetAddress listenAddr(8888);
  EchoServer server(&sched, listenAddr);
  DEBUG("echo server start");
  server.start();

  sched.wait();
}
```

## TODO:
