//
// Created by heyjude on 2022/7/27.
//

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