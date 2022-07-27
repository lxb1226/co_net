#include "conet/base/Exception.h"
#include <iostream>

class Bar {
public:
    void test() {
        throw conet::Exception("oops");
    }
};

void foo() {
    Bar b;
    b.test();
}

int main() {
    try {
        foo();
    } catch (const conet::Exception& ex) {
        std::cout << "reasons: " << std::string(ex.what()) << std::endl;
        std::cout << "stack trace: " << std::string(ex.stackTrace()) << std::endl;
    }
}