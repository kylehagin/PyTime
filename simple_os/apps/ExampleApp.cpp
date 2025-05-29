#include "App.h"
#include <iostream>

class ExampleApp : public App {
public:
    void Run() override {
        std::cout << "Hello from ExampleApp!\n";
    }
};

extern "C" App* create_app() {
    return new ExampleApp();
}
