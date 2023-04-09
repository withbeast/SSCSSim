cmake_minimum_required(VERSION 3.15)
project(SSCSSim)

set(CMAKE_CXX_STANDARD 17)
aux_source_directory(./src LIB_SSCSSIM)

add_executable(Main main.cpp)
target_include_directories(Main PUBLIC ${LIB_SSCSSIM})
add_executable(Train train_mnist.cpp)
target_include_directories(Train PUBLIC ${LIB_SSCSSIM})
