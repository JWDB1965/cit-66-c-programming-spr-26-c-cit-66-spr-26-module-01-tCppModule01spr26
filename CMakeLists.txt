cmake_minimum_required(VERSION 3.15)
project(WeatherDashboard CXX)

# Enforce Modern C++ (C++20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the packages installed by vcpkg
find_package(crow CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)

add_executable(dashboard main.cpp
        main.cpp)

# Link Crow and libcurl to our executable
target_link_libraries(dashboard PRIVATE Crow::Crow CURL::libcurl)
