cmake_minimum_required(VERSION 3.10)

project(FlightSimulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(FlightSimulator src/main.cpp
        include/vehicle.h
        src/vehicle.cpp
        include/pidcontroller.h
        src/pidcontroller.cpp
        include/simulationwidget.h
        src/simulationwidget.cpp
)

target_include_directories(FlightSimulator PRIVATE include)

target_link_libraries(FlightSimulator PRIVATE Qt6::Widgets)