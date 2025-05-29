#pragma once

#include "App.h"
#include <vector>
#include <memory>
#include <string>

class AppManager {
public:
    void LoadApps(const std::string& directory);
    void RunAll();

private:
    std::vector<std::unique_ptr<App>> apps;
};
