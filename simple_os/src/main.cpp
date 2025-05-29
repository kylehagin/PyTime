#include "AppManager.h"
#include <iostream>

int main() {
    AppManager mgr;
    mgr.LoadApps("apps");
    std::cout << "Loaded apps. Running...\n";
    mgr.RunAll();
    return 0;
}
