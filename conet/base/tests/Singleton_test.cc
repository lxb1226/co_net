#include "conet/base/Singleton.h"
#include <boost/noncopyable.hpp>
#include <string>
#include <thread>
#include <iostream>

class Test :boost::noncopyable {
public:
    Test() { 
        std::cout << "Test tid = " << std::this_thread::get_id() << "  Address = "
            <<  static_cast<const void *>(this) << std::endl;
    }
    
    ~Test() {
        std::cout << "~Test tid = " << std::this_thread::get_id() << "  Address = "
        <<  static_cast<const void *>(this) << "  name = " << name_ <<std::endl;
    }
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }

private:
    std::string name_;
};

void ThreadFunc() {
    std::cout << "tid = " << std::this_thread::get_id()
        << " Address = " << static_cast<const void *>(&conet::Singleton<Test>::Instance())
        << " name = " << conet::Singleton<Test>::Instance().getName() << std::endl;
    conet::Singleton<Test>::Instance().setName("only one, changed");
}

int main() {
    conet::Singleton<Test>::Instance().setName("only one");
    std::thread t1(ThreadFunc);
    t1.join();
    std::cout << "tid = " << std::this_thread::get_id()
        << " Address = " << static_cast<const void *>(&conet::Singleton<Test>::Instance())
        << " name = " << conet::Singleton<Test>::Instance().getName() << std::endl;
}