#include "conet/base/StringUtil.h"
#include <iostream>
using namespace conet;

void test01() {
    std::cout << "This is test01" << std::endl;
    std::string a = "11 222 333";
    std::vector<std::string> vec;
    StringUtil::split(a, vec);
    for(auto& v: vec) {
        std::cout << v << std::endl;
    }
}

void test02() {
    std::cout << "This is test02" << std::endl;
    std::string a = "  11 222 333   ";
    std::vector<std::string> vec;
    StringUtil::split(a, vec);
    for(auto& v: vec) {
        std::cout << v << std::endl;
    }
}

void test03() {
    std::cout << "This is test03" << std::endl;
    std::string a = "   ";
    std::vector<std::string> vec;
    StringUtil::split(a, vec);
    for(auto& v: vec) {
        std::cout << v << std::endl;
    }
}



int main() {
    test01();
}