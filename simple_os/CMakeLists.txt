cmake_minimum_required(VERSION 3.10)
project(simple_os CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SimpleOSApp INTERFACE)
target_include_directories(SimpleOSApp INTERFACE include)

add_executable(simple_os
    src/main.cpp
    src/AppManager.cpp
)

target_link_libraries(simple_os dl)

add_library(example_app SHARED apps/ExampleApp.cpp)
target_link_libraries(example_app SimpleOSApp)
