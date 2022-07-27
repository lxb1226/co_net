// #include "gtest/gtest.h"
#include "conet/net/Socket.h"

#include <iostream>

using namespace conet;
using namespace conet::net;

// TEST(Socket, getTcpInfoString) {
//     Socket(11);
// }

int main() {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    Socket sk(fd);
    std::cout << sk.getTcpInfoString() << std::endl;
}