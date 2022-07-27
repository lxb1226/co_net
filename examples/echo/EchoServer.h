//
// Created by heyjude on 2022/7/27.
//

#ifndef CO_NET_ECHOSERVER_H
#define CO_NET_ECHOSERVER_H

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

#endif // CO_NET_ECHOSERVER_H
