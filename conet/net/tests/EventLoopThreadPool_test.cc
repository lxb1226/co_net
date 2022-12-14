#include "conet/net/EventLoopThreadPool.h"
#include "conet/net/TcpServer.h"
#include "conet/net/EventLoop.h"
#include "conet/net/InetAddress.h"

using namespace conet;
using namespace conet::net;

class TestServer {
public:
    TestServer(EventLoop* loop, const InetAddress& listenAddr, int numThreads) :
        loop_(loop), 
        server_(loop, listenAddr, "TestServer"),
        numThreads_(numThreads) {
        server_.setConnectionCallback(
            std::bind(&TestServer::onConnection, this, std::placeholders::_1));
        server_.setMessageCallback(
            std::bind(&TestServer::onMessage, this, 
            std::placeholders::_1,      // conn
            std::placeholders::_2,      // data
            std::placeholders::_3));    // len
        server_.setThreadNum(numThreads);
    }
    void start() {
        server_.start();
    }
private:
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
                    Buffer* buf, Timestamp receiveTime) {
        std::string msg(buf->retrieveAllAsString());
        std::cout << "onMessage(): received " << msg.size() 
            << " bytes from connection " << conn->getName() 
            << "at " << receiveTime.toString() << std::endl;
    }   

private:
    EventLoop* loop_;
    TcpServer server_;
    int numThreads_;
};

int main() {
    std::cout << "main() : pid = " << ::getpid() << std::endl;
    InetAddress listenAddr(8888);
    EventLoop loop;
    TestServer server(&loop, listenAddr, 4);
    server.start();
    loop.loop();
}