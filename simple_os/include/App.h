#pragma once

class App {
public:
    virtual ~App() = default;
    virtual void Run() = 0;
};

using CreateAppFunc = App*();
