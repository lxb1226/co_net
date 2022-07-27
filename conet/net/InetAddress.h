// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

// Taken from Muduo and modified
#ifndef INETADDRESS_H
#define INETADDRESS_H

/**
 * @brief 网际地址sockaddr_in封装
 */
#include "Endian.h"
#include "SocketsOps.h"
#include "conet/base/copyable.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <string>

namespace conet {
namespace net {

class InetAddress : conet::copyable {
public:
  // 仅仅指定port，不指定ip，则ip为INADDR_ANY
  explicit InetAddress(uint16_t port = 0);
  InetAddress(const std::string &ip, uint16_t port);
  explicit InetAddress(const struct sockaddr_in &addr);

  std::string getIpStr() const;
  std::string getPortStr() const;
  uint16_t getPort() const {
    return sockets::networkToHost16(getPortNetEndian());
  }
  std::string getIpPortStr() const;
  sa_family_t getFamily() const { return addrin_.sin_family; }

  const struct sockaddr_in &getSockAddrin() const { return addrin_; }
  const struct sockaddr &getSockAddr() const {
    return *sockets::sockaddr_cast(&addrin_);
  }
  void setSockAddrin(const struct sockaddr_in &addrin) { addrin_ = addrin; }
  uint32_t getIpNetEndian() const { return addrin_.sin_addr.s_addr; }
  uint16_t getPortNetEndian() const { return addrin_.sin_port; }

  // static bool hostNameToIp(); // TODO
private:
  struct sockaddr_in addrin_;
};

} // namespace net

} // namespace conet

#endif // INETADDRESS_H