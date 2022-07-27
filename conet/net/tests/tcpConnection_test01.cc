#include "conet/net/TcpServer.h"
#include "conet/net/EventLoop.h"
#include "conet/net/InetAddress.h"
#include <iostream>

using namespace conet;
using namespace conet::net;

void onConnection(const TcpConnectionPtr& conn) {
    if(conn->isConnected()) {
        std::cout << "onConnection(): new connection [" 
            << conn->getName() <<  "] from " 
            << conn->getPeerAddress().getIpPortStr() << std::endl;
    } else {
        std::cout << "onConnection() : connection " 
            << conn->getName() << " is down" << std::endl;
    }   
}

void onMessage(const TcpConnectionPtr& conn, 
                Buffer& buf, 
                Timestamp recieveTime) {
    std::cout << "onMessage(): received " << buf.getReadableBytes() 
        << " bytes from connection " << conn->getName() 
        << " at " << recieveTime.toFormatTime() <<std::endl;
    std::cout << "onMessage() : " << buf.retrieveAllAsString() << std::endl;
}   

int main() {
    std::cout << "main() : pid = " << ::getpid() << std::endl;
    InetAddress listenAddr(8888);
    EventLoop loop;
    TcpServer server(&loop, listenAddr, "TcpServer");
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();
}