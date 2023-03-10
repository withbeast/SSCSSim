cmake_minimum_required(VERSION 3.24)
project(SimpleCPUSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(SimpleCPUSim main.cpp src/network.hpp src/simulator.hpp src/model.hpp src/netgen.hpp src/spikegen.hpp src/macro.h src/timer.hpp src/util.hpp)
