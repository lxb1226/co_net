//
// Created by heyjude on 2022/7/27.
//

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