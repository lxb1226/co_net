#ifndef COTCPSERVER_H
#define COTCPSERVER_H

#include "Callbacks.h"
#include "CoTcpConnection.h"
#include "InetAddress.h"
#include "conet/base/Atomic.h"
#include <boost/noncopyable.hpp>
#include <fcntl.h>
#include <functional>
#include <map>
#include <memory>
#include <sys/stat.h>

namespace conet {
namespace net {
class InetAddress;
class Scheduler;
class Socket;

class CoTcpServer : boost::noncopyable {
public:
  using ConnectionHandler = std::function<void(const CoTcpConnection::ptr &)>;

  CoTcpServer(Scheduler *sched, const InetAddress &listenAddr,
              const std::string &name = "CoTcpServer", bool reuseport = true);

  ~CoTcpServer();
  void setThreadNum(size_t threadNum);
  void start();
  void setConnectionHandler(const ConnectionHandler &handler);
  Scheduler *getScheduler() { return sched_; }

  // void setConnEstablishCallback(const CoConnEstablishCallback& cb) {
  // connEstablishCallback_ = cb; }
private:
  void startAccept();

private:
  Scheduler *sched_;
  InetAddress listenAddr_;
  std::unique_ptr<Socket> listenSock_;
  ConnectionHandler connHandler_;
  AtomicInt32 started_;
  const std::string hostIpPort_;
  const std::string hostName_;
  int nextConnId_;
  int idleFd_; // 占位fd,用于fd满的情况，避免一直电平触发
  // CoConnEstablishCallback connEstablishCallback_;   // no need this
};

} // namespace net

} // namespace conet

#endif // COTCPSERVER_H