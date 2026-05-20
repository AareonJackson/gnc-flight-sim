cmake_minimum_required(VERSION 3.10)

project(FlightSimulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(FlightSimulator src/main.cpp
        include/vehicle.h
        src/vehicle.cpp)

target_include_directories(FlightSimulator PRIVATE include)

target_link_libraries(FlightSimulator PRIVATE SFML::Graphics SFML::Window SFML::System)