#include "AppManager.h"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>

void AppManager::LoadApps(const std::string& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() != ".so")
            continue;
        void* handle = dlopen(entry.path().c_str(), RTLD_NOW);
        if (!handle) {
            std::cerr << "Failed to load " << entry.path() << "\n";
            continue;
        }
        auto create = reinterpret_cast<CreateAppFunc*>(dlsym(handle, "create_app"));
        if (!create) {
            std::cerr << "No create_app in " << entry.path() << "\n";
            dlclose(handle);
            continue;
        }
        apps.emplace_back(create());
    }
}

void AppManager::RunAll() {
    for (auto& app : apps) {
        app->Run();
    }
}
