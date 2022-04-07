#include <wrd.hpp>
#include <iostream>
using namespace wrd;

struct helloWorld {
    void say() {
        std::cout << "hello world!\n";
    }

    int add(int a, int b) {
        return a + b;
    }
};

extern "C" void wrd_bridge_cpp_entrypoint(bicontainable* tray) {
    tray->add("helloWorld", tcppBridge<helloWorld>::def()
        ->func("say", &helloWorld::say)
        ->func("add", &helloWorld::add));
}
